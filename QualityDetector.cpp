//---------------------------------------------------------------------------

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------

#include "QualityDetector.h"
#include <DateUtils.hpp>
#include <SysUtils.hpp>
#include "modbus-tcp.h"  // 包含TCP相关的头文件

#pragma link "modbus_omf.lib"
#pragma link "ws2_32.lib"

QualityDetector::QualityDetector()
    : modbusContext(nullptr), isConnected(false), pollTimer(nullptr),
      onDetectionComplete(nullptr), onStatusChanged(nullptr), onError(nullptr) {

    pollTimer = new TTimer(nullptr);
    pollTimer->Enabled = false;
    pollTimer->OnTimer = OnPollTimer;
}

QualityDetector::~QualityDetector() {
    Disconnect();
    if (pollTimer) {
        delete pollTimer;
        pollTimer = nullptr;
    }
}

bool QualityDetector::Connect(const AnsiString& ip, int port, int deviceId) {
    try {
        // 断开现有连接
        Disconnect();

        // 创建TCP连接上下文
        modbusContext = modbus_new_tcp(ip.c_str(), port);
        if (!modbusContext) {
            if (onError) onError(-1, "Failed to create Modbus TCP context");
            return false;
        }

        // 设置设备ID (slave地址)
        if (modbus_set_slave(modbusContext, deviceId) == -1) {
            if (onError) onError(-2, AnsiString("Failed to set slave ID: ") + modbus_strerror(errno));
            modbus_free(modbusContext);
            modbusContext = nullptr;
            return false;
        }

        // 设置响应超时时间 (3秒)
        if (modbus_set_response_timeout(modbusContext, 3, 0) == -1) {
            if (onError) onError(-3, AnsiString("Failed to set response timeout: ") + modbus_strerror(errno));
            modbus_free(modbusContext);
            modbusContext = nullptr;
            return false;
        }

        // 设置字节超时时间
        if (modbus_set_byte_timeout(modbusContext, 1, 0) == -1) {
            if (onError) onError(-4, AnsiString("Failed to set byte timeout: ") + modbus_strerror(errno));
            modbus_free(modbusContext);
            modbusContext = nullptr;
            return false;
        }

        // 建立连接
        if (modbus_connect(modbusContext) == -1) {
            if (onError) onError(-5, AnsiString("Connection failed: ") + modbus_strerror(errno));
            modbus_free(modbusContext);
            modbusContext = nullptr;
            return false;
        }

        this->ipAddress = ip;
        this->port = port;
        this->deviceId = deviceId;
        isConnected = true;

        // 检查设备状态
        if (!CheckDeviceStatus()) {
            if (onError) onError(-6, "Device status check failed");
            Disconnect();
            return false;
        }

        return true;
    }
    catch (const Exception& e) {
        if (onError) onError(-99, AnsiString("Connection exception: ") + e.Message);
        if (modbusContext) {
            modbus_free(modbusContext);
            modbusContext = nullptr;
        }
        return false;
    }
}

void QualityDetector::Disconnect() {
    StopPolling();

    if (modbusContext) {
        modbus_close(modbusContext);
        modbus_free(modbusContext);
        modbusContext = nullptr;
    }

    isConnected = false;
}

bool QualityDetector::StartDetection() {
    if (!isConnected) return false;

    bool result = WriteRegister(DETECTION_CONTROL_ADDR, CMD_START_DETECTION);
    if (result && onStatusChanged) {
        onStatusChanged(STATUS_COLLECTING);
    }
    return result;
}

bool QualityDetector::StopDetection() {
    if (!isConnected) return false;

    return WriteRegister(DETECTION_CONTROL_ADDR, CMD_STOP_DETECTION);
}

bool QualityDetector::StartReferenceCollection() {
    if (!isConnected) return false;

    return WriteRegister(REFERENCE_CONTROL_ADDR, CMD_START_REFERENCE);
}

int QualityDetector::GetDetectionStatus() {
    uint16_t status;
    if (isConnected && ReadRegister(DETECTION_CONTROL_ADDR, status)) {
        return static_cast<int>(status);
    }
    return -1;
}

int QualityDetector::GetReferenceStatus() {
    uint16_t status;
    if (isConnected && ReadRegister(REFERENCE_CONTROL_ADDR, status)) {
        return static_cast<int>(status);
    }
    return -1;
}

int QualityDetector::GetModbusExceptionCode() {
    uint16_t code;
    if (isConnected && ReadRegister(EXCEPTION_ADDR, code)) {
        return static_cast<int>(code);
    }
    return -1;
}

int QualityDetector::GetSerialNumber() {
    uint16_t serialNumber;
    if (isConnected && ReadRegister(SERIAL_NUMBER_ADDR, serialNumber)) {
        return static_cast<int>(serialNumber);
    }
    return -1;
}

bool QualityDetector::ReadDetectionResults(DetectionResult& result) {
    if (!isConnected) return false;

    try {
        // 读取流水号
        result.serialNumber = GetSerialNumber();

        // 读取异常码
        result.exceptionCode = this->GetModbusExceptionCode();

        // 读取检测结果
        std::vector<uint16_t> rawValues;
        int resultCount = DETECTION_RESULT_END_ADDR - DETECTION_RESULT_START_ADDR + 1;

        if (!ReadRegisters(DETECTION_RESULT_START_ADDR, resultCount, rawValues)) {
            result.isValid = false;
            return false;
        }

        // 转换检测结果（假设前4个地址分别对应糖度、酸度、水分、硬度）
        result.values.clear();
        if (rawValues.size() >= 1) result.values[SUGAR_CONTENT] = ConvertToActualValue(rawValues[0]);
        if (rawValues.size() >= 2) result.values[ACIDITY] = ConvertToActualValue(rawValues[1]);
        if (rawValues.size() >= 3) result.values[MOISTURE] = ConvertToActualValue(rawValues[2]);
        if (rawValues.size() >= 4) result.values[HARDNESS] = ConvertToActualValue(rawValues[3]);

        // 设置时间戳
        GetSystemTime(&result.timestamp);

        result.isValid = (result.exceptionCode == 0);
        return true;
    }
    catch (...) {
        result.isValid = false;
        return false;
    }
}

bool QualityDetector::ReadDetectionResult(int address, double& value) {
    if (!isConnected) return false;

    uint16_t rawValue;
    if (ReadRegister(address, rawValue)) {
        value = ConvertToActualValue(rawValue);
        return true;
    }
    return false;
}

std::vector<double> QualityDetector::ReadAllDetectionResults() {
    std::vector<double> results;

    if (!isConnected) return results;

    std::vector<uint16_t> rawValues;
    int resultCount = DETECTION_RESULT_END_ADDR - DETECTION_RESULT_START_ADDR + 1;

    if (ReadRegisters(DETECTION_RESULT_START_ADDR, resultCount, rawValues)) {
        for (uint16_t rawValue : rawValues) {
            results.push_back(ConvertToActualValue(rawValue));
        }
    }

    return results;
}

bool QualityDetector::CheckDeviceStatus() {
    if (!isConnected) return false;

    int referenceStatus = GetReferenceStatus();
    return (referenceStatus == STATUS_SUCCESS || referenceStatus == STATUS_COLLECTING);
}

bool QualityDetector::IsDeviceReady() {
    int referenceStatus = GetReferenceStatus();
    return (referenceStatus == STATUS_SUCCESS);
}

void QualityDetector::StartPolling(int interval) {
    if (interval < 50) interval = 50; // 最小50ms间隔，防止性能问题

    pollTimer->Interval = interval;
    pollTimer->Enabled = true;
}

void QualityDetector::StopPolling() {
    if (pollTimer) {
        pollTimer->Enabled = false;
    }
}

void QualityDetector::SetOnDetectionComplete(void (__fastcall *callback)(DetectionResult)) {
    onDetectionComplete = callback;
}

void QualityDetector::SetOnStatusChanged(void (__fastcall *callback)(int)) {
    onStatusChanged = callback;
}

void QualityDetector::SetOnError(void (__fastcall *callback)(int, AnsiString)) {
    onError = callback;
}

double QualityDetector::ConvertToActualValue(int rawValue) {
    return static_cast<double>(rawValue) / 100.0;
}

AnsiString QualityDetector::GetStatusText(int status) {
    switch (status) {
        case STATUS_COLLECTING: return "采集中";
        case STATUS_SUCCESS: return "采集成功";
        case STATUS_FAILED: return "采集失败";
        default: return "未知状态";
    }
}

AnsiString QualityDetector::GetExceptionText(int exceptionCode) {
    switch (exceptionCode) {
        case 0: return "无异常";
        case 1: return "传感器异常";
        case 2: return "光源异常";
        case 3: return "温度异常";
        case 4: return "校准异常";
        default: return AnsiString("异常代码: ") + IntToStr(exceptionCode);
    }
}

// 私有方法实现

bool QualityDetector::WriteRegister(int address, int value) {
    if (!isConnected || !modbusContext) return false;

    try {
        // 确保value在uint16_t范围内
        if (value < 0 || value > 65535) {
            if (onError) onError(-10, "Value out of range for uint16_t");
            return false;
        }

        int result = modbus_write_register(modbusContext, address, static_cast<const uint16_t>(value));
        if (result == -1) {
            HandleError("WriteRegister");
            return false;
        }
        return true;
    }
    catch (...) {
        HandleError("WriteRegister Exception");
        return false;
    }
}

bool QualityDetector::ReadRegister(int address, uint16_t& value) {
    if (!isConnected || !modbusContext) return false;

    try {
        int result = modbus_read_registers(modbusContext, address, 1, &value);
        if (result == -1) {
            HandleError("ReadRegister");
            return false;
        }
        return true;
    }
    catch (...) {
        HandleError("ReadRegister Exception");
        return false;
    }
}

bool QualityDetector::ReadRegisters(int startAddress, int count, std::vector<uint16_t>& values) {
    if (!isConnected || !modbusContext) return false;

    try {
        // 检查读取数量是否在允许范围内
        if (count <= 0 || count > MODBUS_MAX_READ_REGISTERS) {
            if (onError) onError(-11, "Register count out of range");
            return false;
        }

        values.resize(count);
        int result = modbus_read_registers(modbusContext, startAddress, count, values.data());
        if (result == -1) {
            HandleError("ReadRegisters");
            return false;
        }

        // 检查实际读取的数量
        if (result != count) {
            if (onError) onError(-12, "Partial read - expected " + IntToStr(count) + ", got " + IntToStr(result));
            values.resize(result);  // 调整vector大小
        }

        return true;
    }
    catch (...) {
        HandleError("ReadRegisters Exception");
        return false;
    }
}

void __fastcall QualityDetector::OnPollTimer(TObject* Sender) {
    if (!isConnected) return;

    static int lastDetectionStatus = -1;

    try {
        // 检查检测状态
        int detectionStatus = GetDetectionStatus();

        // 如果检测完成，读取结果
        if (detectionStatus == STATUS_SUCCESS &&
            lastDetectionStatus != STATUS_SUCCESS &&
            onDetectionComplete) {

            DetectionResult result;
            if (ReadDetectionResults(result)) {
                onDetectionComplete(result);
            }
        }

        // 通知状态变化
        if (detectionStatus != lastDetectionStatus && onStatusChanged) {
            onStatusChanged(detectionStatus);
            lastDetectionStatus = detectionStatus;
        }
    }
    catch (...) {
        HandleError("Poll Timer Exception");
    }
}

void QualityDetector::HandleError(const AnsiString& operation) {
    if (!modbusContext) {
        if (onError) onError(-20, operation + ": No modbus context");
        return;
    }

    if (onError) {
        AnsiString errorMsg = AnsiString(operation) + " failed: " + modbus_strerror(errno);
        onError(errno, errorMsg);
    }
}

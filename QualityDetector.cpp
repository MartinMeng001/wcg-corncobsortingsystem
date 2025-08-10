//---------------------------------------------------------------------------

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------

#include "QualityDetector.h"
#include <DateUtils.hpp>
#include <SysUtils.hpp>

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
        // �Ͽ���������
        Disconnect();

        // ����TCP����������
        modbusContext = modbus_new_tcp(ip.c_str(), port);
        if (!modbusContext) {
            if (onError) onError(-1, "Failed to create Modbus context");
            return false;
        }

        // �����豸ID
        if (modbus_set_slave(modbusContext, deviceId) == -1) {
            if (onError) onError(-2, "Failed to set device ID");
            modbus_free(modbusContext);
            modbusContext = nullptr;
            return false;
        }

        // �������ӳ�ʱ��3�룩
        modbus_set_response_timeout(modbusContext, 3, 0);

        // ��������
        if (modbus_connect(modbusContext) == -1) {
            if (onError) onError(-3, AnsiString("Connection failed: ") + modbus_strerror(errno));
            modbus_free(modbusContext);
            modbusContext = nullptr;
            return false;
        }

        this->ipAddress = ip;
        this->port = port;
        this->deviceId = deviceId;
        isConnected = true;

        // ����豸״̬
        if (!CheckDeviceStatus()) {
            if (onError) onError(-4, "Device status check failed");
            Disconnect();
            return false;
        }

        return true;
    }
    catch (const Exception& e) {
        if (onError) onError(-99, AnsiString("Connection exception: ") + e.Message);
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
    int status = -1;
    if (isConnected && ReadRegister(DETECTION_CONTROL_ADDR, status)) {
        return status;
    }
    return -1;
}

int QualityDetector::GetReferenceStatus() {
    int status = -1;
    if (isConnected && ReadRegister(REFERENCE_CONTROL_ADDR, status)) {
        return status;
    }
    return -1;
}

int QualityDetector::GetExceptionCode() {
    int code = -1;
    if (isConnected && ReadRegister(EXCEPTION_ADDR, code)) {
        return code;
    }
    return -1;
}

int QualityDetector::GetSerialNumber() {
    int serialNumber = -1;
    if (isConnected && ReadRegister(SERIAL_NUMBER_ADDR, serialNumber)) {
        return serialNumber;
    }
    return -1;
}

bool QualityDetector::ReadDetectionResults(DetectionResult& result) {
    if (!isConnected) return false;

    try {
        // ��ȡ��ˮ��
        result.serialNumber = GetSerialNumber();

        // ��ȡ�쳣��
        result.exceptionCode = GetExceptionCode();

        // ��ȡ�����
        std::vector<int> rawValues;
        int resultCount = DETECTION_RESULT_END_ADDR - DETECTION_RESULT_START_ADDR + 1;

        if (!ReadRegisters(DETECTION_RESULT_START_ADDR, resultCount, rawValues)) {
            result.isValid = false;
            return false;
        }

        // ת�������������ǰ4����ַ�ֱ��Ӧ�Ƕȡ���ȡ�ˮ�֡�Ӳ�ȣ�
        result.values.clear();
        if (rawValues.size() >= 1) result.values[SUGAR_CONTENT] = ConvertToActualValue(rawValues[0]);
        if (rawValues.size() >= 2) result.values[ACIDITY] = ConvertToActualValue(rawValues[1]);
        if (rawValues.size() >= 3) result.values[MOISTURE] = ConvertToActualValue(rawValues[2]);
        if (rawValues.size() >= 4) result.values[HARDNESS] = ConvertToActualValue(rawValues[3]);

        // ����ʱ���
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

    int rawValue;
    if (ReadRegister(address, rawValue)) {
        value = ConvertToActualValue(rawValue);
        return true;
    }
    return false;
}

std::vector<double> QualityDetector::ReadAllDetectionResults() {
    std::vector<double> results;

    if (!isConnected) return results;

    std::vector<int> rawValues;
    int resultCount = DETECTION_RESULT_END_ADDR - DETECTION_RESULT_START_ADDR + 1;

    if (ReadRegisters(DETECTION_RESULT_START_ADDR, resultCount, rawValues)) {
        for (int rawValue : rawValues) {
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
    if (interval < 50) interval = 50; // ��С50ms���������Э��Ҫ��

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
        case STATUS_COLLECTING: return "�ɼ���";
        case STATUS_SUCCESS: return "�ɼ��ɹ�";
        case STATUS_FAILED: return "�ɼ�ʧ��";
        default: return "δ֪״̬";
    }
}

AnsiString QualityDetector::GetExceptionText(int exceptionCode) {
    switch (exceptionCode) {
        case 0: return "���쳣";
        case 1: return "�������쳣";
        case 2: return "��Դ�쳣";
        case 3: return "�¶��쳣";
        case 4: return "У׼�쳣";
        default: return AnsiString("�쳣����: ") + IntToStr(exceptionCode);
    }
}

// ˽�з���ʵ��

bool QualityDetector::WriteRegister(int address, int value) {
    if (!isConnected || !modbusContext) return false;

    try {
        int result = modbus_write_register(modbusContext, address, value);
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

bool QualityDetector::ReadRegister(int address, int& value) {
    if (!isConnected || !modbusContext) return false;

    try {
        uint16_t regValue;
        int result = modbus_read_holding_registers(modbusContext, address, 1, &regValue);
        if (result == -1) {
            HandleError("ReadRegister");
            return false;
        }
        value = static_cast<int>(regValue);
        return true;
    }
    catch (...) {
        HandleError("ReadRegister Exception");
        return false;
    }
}

bool QualityDetector::ReadRegisters(int startAddress, int count, std::vector<int>& values) {
    if (!isConnected || !modbusContext) return false;

    try {
        std::vector<uint16_t> regValues(count);
        int result = modbus_read_holding_registers(modbusContext, startAddress, count, regValues.data());
        if (result == -1) {
            HandleError("ReadRegisters");
            return false;
        }

        values.clear();
        values.reserve(count);
        for (uint16_t regValue : regValues) {
            values.push_back(static_cast<int>(regValue));
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

    // �����״̬
    int detectionStatus = GetDetectionStatus();

    // ��������ɣ���ȡ���
    if (detectionStatus == STATUS_SUCCESS && onDetectionComplete) {
        DetectionResult result;
        if (ReadDetectionResults(result)) {
            onDetectionComplete(result);
        }
    }

    // ֪ͨ״̬�仯
    if (onStatusChanged) {
        onStatusChanged(detectionStatus);
    }
}

void QualityDetector::HandleError(const AnsiString& operation) {
    if (onError) {
        AnsiString errorMsg = AnsiString(operation) + " failed: " + modbus_strerror(errno);
        onError(errno, errorMsg);
    }
}

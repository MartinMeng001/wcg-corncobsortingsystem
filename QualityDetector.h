#ifndef QualityDetectorH
#define QualityDetectorH

#include <vcl.h>
#include <modbus.h>
#include <modbus-tcp.h>  // 添加TCP特定的头文件
#include <vector>
#include <map>

// 寄存器地址定义
#define DETECTION_CONTROL_ADDR      102  // 检测控制位
#define REFERENCE_CONTROL_ADDR      103  // 参考控制位
#define EXCEPTION_ADDR              109  // 异常位
#define SERIAL_NUMBER_ADDR          110  // 流水号
#define DETECTION_RESULT_START_ADDR 111  // 检测结果起始地址
#define DETECTION_RESULT_END_ADDR   129  // 检测结果结束地址

// 控制命令定义
#define CMD_START_DETECTION         1    // 开始检测
#define CMD_STOP_DETECTION          0    // 停止检测
#define CMD_START_REFERENCE         1    // 开始采集参考

// 状态定义
#define STATUS_COLLECTING           1    // 采集中
#define STATUS_SUCCESS             2    // 采集成功
#define STATUS_FAILED              3    // 采集失败

// 检测结果类型
enum DetectionType {
    SUGAR_CONTENT = 0,     // 糖度
    ACIDITY = 1,           // 酸度
    MOISTURE = 2,          // 水分
    HARDNESS = 3,          // 硬度
    // 可根据实际需求添加更多检测项目
};

// 检测结果结构
struct DetectionResult {
    int serialNumber;      // 流水号
    std::map<DetectionType, double> values;  // 检测值
    int exceptionCode;     // 异常码
    SYSTEMTIME timestamp;  // 时间戳
    bool isValid;         // 结果有效性
};

class QualityDetector {
private:
    modbus_t* modbusContext;
    AnsiString ipAddress;
    int port;
    int deviceId;
    bool isConnected;
    TTimer* pollTimer;

    // 回调函数指针
    void (__fastcall *onDetectionComplete)(DetectionResult result);
    void (__fastcall *onStatusChanged)(int status);
    void (__fastcall *onError)(int errorCode, AnsiString errorMsg);

public:
    QualityDetector();
    ~QualityDetector();

    // 连接管理
    bool Connect(const AnsiString& ip, int port = 502, int deviceId = 1);
    void Disconnect();
    bool IsConnected() const { return isConnected; }

    // 基本控制操作
    bool StartDetection();
    bool StopDetection();
    bool StartReferenceCollection();

    // 状态查询
    int GetDetectionStatus();
    int GetReferenceStatus();
    int GetModbusExceptionCode();
    int GetSerialNumber();

    // 检测结果读取
    bool ReadDetectionResults(DetectionResult& result);
    bool ReadDetectionResult(int address, double& value);
    std::vector<double> ReadAllDetectionResults();

    // 设备状态检测
    bool CheckDeviceStatus();
    bool IsDeviceReady();

    // 轮询控制
    void StartPolling(int interval = 100); // 默认100ms间隔
    void StopPolling();

    // 回调设置
    void SetOnDetectionComplete(void (__fastcall *callback)(DetectionResult));
    void SetOnStatusChanged(void (__fastcall *callback)(int));
    void SetOnError(void (__fastcall *callback)(int, AnsiString));

    // 工具函数
    static double ConvertToActualValue(int rawValue);
    static AnsiString GetStatusText(int status);
    static AnsiString GetExceptionText(int exceptionCode);

private:
    // 内部辅助函数
    bool WriteRegister(int address, int value);
    bool ReadRegister(int address, uint16_t& value);  // 修正：使用uint16_t
    bool ReadRegisters(int startAddress, int count, std::vector<uint16_t>& values);  // 修正：使用uint16_t
    void __fastcall OnPollTimer(TObject* Sender);
    void HandleError(const AnsiString& operation);
};

#endif

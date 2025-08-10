//---------------------------------------------------------------------------

#ifndef TQualityDetectionPresenterH
#define TQualityDetectionPresenterH
//---------------------------------------------------------------------------

#include "TBasePresenter.h"
#include <vector>
#include <string>
#include <modbus.h>

// Modbus地址定义
const int ADDR_DETECTION_CONTROL = 102;  // 检测控制位
const int ADDR_REFERENCE_CONTROL = 103;  // 参比控制位
const int ADDR_ERROR_CODE = 109;         // 异常位
const int ADDR_SERIAL_NUMBER = 110;      // 流水号
const int ADDR_RESULTS_START = 111;      // 检测结果起始地址
const int ADDR_RESULTS_END = 129;        // 检测结果结束地址
const int RESULTS_COUNT = 19;            // 检测结果数量

class TQualityDetectionPresenter : public TBasePresenter
{
private:
    modbus_t* m_ModbusContext;
    std::string m_LastError;
    bool m_bConnected;

    // 内部方法
    bool WriteRegister(int address, int value);
    int ReadRegister(int address);
    std::vector<uint16_t> ReadRegisters(int startAddress, int count);
    void SetLastError(const std::string& error);

public:
    TQualityDetectionPresenter();
    virtual ~TQualityDetectionPresenter();

    // 连接管理
    bool Connect(const char* ip, int port);
    void Disconnect();
    bool IsConnected() const { return m_bConnected; }

    // 参比操作
    bool StartReference();
    bool StopReference();
    int GetReferenceStatus();

    // 检测操作
    bool StartDetection();
    bool StopDetection();
    int GetDetectionStatus();

    // 数据读取
    std::vector<float> ReadDetectionResults();
    int GetSerialNumber();
    int GetErrorCode();

    // 错误处理
    std::string GetLastError() const { return m_LastError; }
};

//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------

#ifndef TQualityDetectionPresenterH
#define TQualityDetectionPresenterH
//---------------------------------------------------------------------------

#include "TBasePresenter.h"
#include <vector>
#include <string>
#include <modbus.h>

// Modbus��ַ����
const int ADDR_DETECTION_CONTROL = 102;  // ������λ
const int ADDR_REFERENCE_CONTROL = 103;  // �αȿ���λ
const int ADDR_ERROR_CODE = 109;         // �쳣λ
const int ADDR_SERIAL_NUMBER = 110;      // ��ˮ��
const int ADDR_RESULTS_START = 111;      // �������ʼ��ַ
const int ADDR_RESULTS_END = 129;        // �����������ַ
const int RESULTS_COUNT = 19;            // ���������

class TQualityDetectionPresenter : public TBasePresenter
{
private:
    modbus_t* m_ModbusContext;
    std::string m_LastError;
    bool m_bConnected;

    // �ڲ�����
    bool WriteRegister(int address, int value);
    int ReadRegister(int address);
    std::vector<uint16_t> ReadRegisters(int startAddress, int count);
    void SetLastError(const std::string& error);

public:
    TQualityDetectionPresenter();
    virtual ~TQualityDetectionPresenter();

    // ���ӹ���
    bool Connect(const char* ip, int port);
    void Disconnect();
    bool IsConnected() const { return m_bConnected; }

    // �αȲ���
    bool StartReference();
    bool StopReference();
    int GetReferenceStatus();

    // ������
    bool StartDetection();
    bool StopDetection();
    int GetDetectionStatus();

    // ���ݶ�ȡ
    std::vector<float> ReadDetectionResults();
    int GetSerialNumber();
    int GetErrorCode();

    // ������
    std::string GetLastError() const { return m_LastError; }
};

//---------------------------------------------------------------------------
#endif

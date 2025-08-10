#ifndef QualityDetectorH
#define QualityDetectorH

#include <vcl.h>
#include <modbus.h>
#include <modbus-tcp.h>  // ���TCP�ض���ͷ�ļ�
#include <vector>
#include <map>

// �Ĵ�����ַ����
#define DETECTION_CONTROL_ADDR      102  // ������λ
#define REFERENCE_CONTROL_ADDR      103  // �ο�����λ
#define EXCEPTION_ADDR              109  // �쳣λ
#define SERIAL_NUMBER_ADDR          110  // ��ˮ��
#define DETECTION_RESULT_START_ADDR 111  // �������ʼ��ַ
#define DETECTION_RESULT_END_ADDR   129  // �����������ַ

// ���������
#define CMD_START_DETECTION         1    // ��ʼ���
#define CMD_STOP_DETECTION          0    // ֹͣ���
#define CMD_START_REFERENCE         1    // ��ʼ�ɼ��ο�

// ״̬����
#define STATUS_COLLECTING           1    // �ɼ���
#define STATUS_SUCCESS             2    // �ɼ��ɹ�
#define STATUS_FAILED              3    // �ɼ�ʧ��

// ���������
enum DetectionType {
    SUGAR_CONTENT = 0,     // �Ƕ�
    ACIDITY = 1,           // ���
    MOISTURE = 2,          // ˮ��
    HARDNESS = 3,          // Ӳ��
    // �ɸ���ʵ��������Ӹ�������Ŀ
};

// ������ṹ
struct DetectionResult {
    int serialNumber;      // ��ˮ��
    std::map<DetectionType, double> values;  // ���ֵ
    int exceptionCode;     // �쳣��
    SYSTEMTIME timestamp;  // ʱ���
    bool isValid;         // �����Ч��
};

class QualityDetector {
private:
    modbus_t* modbusContext;
    AnsiString ipAddress;
    int port;
    int deviceId;
    bool isConnected;
    TTimer* pollTimer;

    // �ص�����ָ��
    void (__fastcall *onDetectionComplete)(DetectionResult result);
    void (__fastcall *onStatusChanged)(int status);
    void (__fastcall *onError)(int errorCode, AnsiString errorMsg);

public:
    QualityDetector();
    ~QualityDetector();

    // ���ӹ���
    bool Connect(const AnsiString& ip, int port = 502, int deviceId = 1);
    void Disconnect();
    bool IsConnected() const { return isConnected; }

    // �������Ʋ���
    bool StartDetection();
    bool StopDetection();
    bool StartReferenceCollection();

    // ״̬��ѯ
    int GetDetectionStatus();
    int GetReferenceStatus();
    int GetModbusExceptionCode();
    int GetSerialNumber();

    // �������ȡ
    bool ReadDetectionResults(DetectionResult& result);
    bool ReadDetectionResult(int address, double& value);
    std::vector<double> ReadAllDetectionResults();

    // �豸״̬���
    bool CheckDeviceStatus();
    bool IsDeviceReady();

    // ��ѯ����
    void StartPolling(int interval = 100); // Ĭ��100ms���
    void StopPolling();

    // �ص�����
    void SetOnDetectionComplete(void (__fastcall *callback)(DetectionResult));
    void SetOnStatusChanged(void (__fastcall *callback)(int));
    void SetOnError(void (__fastcall *callback)(int, AnsiString));

    // ���ߺ���
    static double ConvertToActualValue(int rawValue);
    static AnsiString GetStatusText(int status);
    static AnsiString GetExceptionText(int exceptionCode);

private:
    // �ڲ���������
    bool WriteRegister(int address, int value);
    bool ReadRegister(int address, uint16_t& value);  // ������ʹ��uint16_t
    bool ReadRegisters(int startAddress, int count, std::vector<uint16_t>& values);  // ������ʹ��uint16_t
    void __fastcall OnPollTimer(TObject* Sender);
    void HandleError(const AnsiString& operation);
};

#endif

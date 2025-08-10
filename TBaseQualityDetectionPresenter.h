//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#ifndef TQualityDetectionPresenterH
#define TQualityDetectionPresenterH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Themes.hpp>
#include <memory>
#include <vector>

//---------------------------------------------------------------------------
// ����Presenter������
class TBaseQualityDetectionPresenter
{
public:
    virtual ~TBaseQualityDetectionPresenter() = default;

    // ���ֹ��� - ����ؼ���λ�á���С�������
    virtual void __fastcall ApplyLayout(TForm* AForm) = 0;

    // �ֲ���ʽ - ����ؼ�����ۡ����塢��ɫ��
    virtual void __fastcall ApplyLocalStyles(TForm* AForm) = 0;

    // ����Ӧ�� - �������������VCL��ʽ
    virtual void __fastcall ApplyTheme(TForm* AForm) = 0;

    // ������ʽӦ�� - ͳһ���
    virtual void __fastcall ApplyFullStyle(TForm* AForm)
    {
        ApplyTheme(AForm);      // ��Ӧ������
        ApplyLayout(AForm);     // Ȼ�󲼾�
        ApplyLocalStyles(AForm); // ���Ӧ�þֲ���ʽ
    }

    // ҵ���߼��ӿڣ�����ԭ�й��ܣ�
    virtual bool Connect(const char* ip, int port) = 0;
    virtual void Disconnect() = 0;
    virtual bool StartReference() = 0;
    virtual bool StopReference() = 0;
    virtual bool StartDetection() = 0;
    virtual bool StopDetection() = 0;
    virtual std::vector<float> ReadDetectionResults() = 0;
    virtual int GetReferenceStatus() = 0;
    virtual int GetDetectionStatus() = 0;
    virtual int GetSerialNumber() = 0;
    virtual int GetErrorCode() = 0;
    virtual String GetLastError() = 0;
};



//---------------------------------------------------------------------------
#endif

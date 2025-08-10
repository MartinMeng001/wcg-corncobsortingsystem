//---------------------------------------------------------------------------

#ifndef ModernQualityDetectionPresenterH
#define ModernQualityDetectionPresenterH
//---------------------------------------------------------------------------
#include "TBaseQualityDetectionPresenter.h"
// ��ʽ�Ͳ������ýṹ��
struct TModernStyle
{
	TColor BackgroundColor = TColor(0x00F0F0F0);
    TColor PanelColor = TColor(0x00FFFFFF);
    TColor GroupBoxColor = TColor(0x00FFFFFF);
    TColor PrimaryColor = TColor(0x000078D7); // ��ɫ
    TColor SuccessColor = TColor(0x0028A745); // ��ɫ
    TColor ErrorColor = TColor(0x00DC3545);   // ��ɫ
    TColor TextColor = TColor(0x00333333);
    TColor BorderColor = TColor(0x00CCCCCC);
    AnsiString FontName = "Segoe UI";
    int FontSize = 9;
    int TitleFontSize = 10;
};

struct TModernLayout
{
    int PanelMargin = 8;
    int GroupBoxSpacing = 8;
    int MinButtonWidth = 120;
    int ButtonHeight = 28;
    int EditHeight = 24;
    int GridMinHeight = 150;
};
//---------------------------------------------------------------------------
// �ִ����Presenterʵ��
class TModernQualityDetectionPresenter : public TBaseQualityDetectionPresenter
{
private:
    // ����״̬
    bool m_bConnected;
    String m_LastError;

    // Modbus���Ӳ�����ģ�⣩
    String m_IP;
    int m_Port;

    // �豸״̬����
    int m_ReferenceStatus;
    int m_DetectionStatus;
    int m_SerialNumber;
    int m_ErrorCode;

    // �������ò���
    struct LayoutConfig {
        int PanelMargin = 8;
        int GroupBoxSpacing = 10;
        int ControlSpacing = 6;
        int ButtonHeight = 28;
        int EditHeight = 23;
        int LabelHeight = 17;
        int MinButtonWidth = 80;
        int GridMinHeight = 200;
    } m_Layout;

    // ��ʽ���ò���
    struct StyleConfig {
        TColor BackgroundColor = TColor(0x00F0F0F0);
        TColor PanelColor = clWhite;
        TColor GroupBoxColor = clWhite;
        TColor SuccessColor = TColor(0x00008000);  // ����ɫ
        TColor ErrorColor = TColor(0x000000FF);    // ��ɫ
        TColor WarningColor = TColor(0x000080FF);  // ��ɫ
        TColor ProcessingColor = TColor(0x00FF8000); // ��ɫ
        String FontName = "Microsoft YaHei UI";
        int FontSize = 9;
        int TitleFontSize = 10;
    } m_Style;

public:
    TModernQualityDetectionPresenter();
    virtual ~TModernQualityDetectionPresenter();

    // UI��ʽ����ӿ�ʵ��
    virtual void __fastcall ApplyLayout(TForm* AForm) override;
    virtual void __fastcall ApplyLocalStyles(TForm* AForm) override;
    virtual void __fastcall ApplyTheme(TForm* AForm) override;

    // ҵ���߼��ӿ�ʵ��
    virtual bool Connect(const char* ip, int port) override;
    virtual void Disconnect() override;
    virtual bool StartReference() override;
    virtual bool StopReference() override;
    virtual bool StartDetection() override;
    virtual bool StopDetection() override;
    virtual std::vector<float> ReadDetectionResults() override;
    virtual int GetReferenceStatus() override;
    virtual int GetDetectionStatus() override;
    virtual int GetSerialNumber() override;
    virtual int GetErrorCode() override;
    virtual String GetLastError() override;

private:
    // ��������
    void ApplyPanelLayout(TForm* AForm);
    void ApplyGroupBoxLayout(TForm* AForm);
    void ApplyControlLayout(TForm* AForm);
    void ApplyGridLayout(TForm* AForm);

    void ApplyFontStyles(TForm* AForm);
    void ApplyColorStyles(TForm* AForm);
    void ApplyControlStyles(TForm* AForm);

    template<typename T>
    T* FindControl(TForm* AForm, const String& Name);
};

// Ϊ�������ݣ��ṩ���ͱ���
using TQualityDetectionPresenter = TModernQualityDetectionPresenter;
#endif

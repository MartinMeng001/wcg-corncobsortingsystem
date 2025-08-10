//---------------------------------------------------------------------------

#ifndef ModernQualityDetectionPresenterH
#define ModernQualityDetectionPresenterH
//---------------------------------------------------------------------------
#include "TBaseQualityDetectionPresenter.h"
// 样式和布局配置结构体
struct TModernStyle
{
	TColor BackgroundColor = TColor(0x00F0F0F0);
    TColor PanelColor = TColor(0x00FFFFFF);
    TColor GroupBoxColor = TColor(0x00FFFFFF);
    TColor PrimaryColor = TColor(0x000078D7); // 蓝色
    TColor SuccessColor = TColor(0x0028A745); // 绿色
    TColor ErrorColor = TColor(0x00DC3545);   // 红色
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
// 现代风格Presenter实现
class TModernQualityDetectionPresenter : public TBaseQualityDetectionPresenter
{
private:
    // 连接状态
    bool m_bConnected;
    String m_LastError;

    // Modbus连接参数（模拟）
    String m_IP;
    int m_Port;

    // 设备状态缓存
    int m_ReferenceStatus;
    int m_DetectionStatus;
    int m_SerialNumber;
    int m_ErrorCode;

    // 布局配置参数
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

    // 样式配置参数
    struct StyleConfig {
        TColor BackgroundColor = TColor(0x00F0F0F0);
        TColor PanelColor = clWhite;
        TColor GroupBoxColor = clWhite;
        TColor SuccessColor = TColor(0x00008000);  // 深绿色
        TColor ErrorColor = TColor(0x000000FF);    // 红色
        TColor WarningColor = TColor(0x000080FF);  // 橙色
        TColor ProcessingColor = TColor(0x00FF8000); // 蓝色
        String FontName = "Microsoft YaHei UI";
        int FontSize = 9;
        int TitleFontSize = 10;
    } m_Style;

public:
    TModernQualityDetectionPresenter();
    virtual ~TModernQualityDetectionPresenter();

    // UI样式管理接口实现
    virtual void __fastcall ApplyLayout(TForm* AForm) override;
    virtual void __fastcall ApplyLocalStyles(TForm* AForm) override;
    virtual void __fastcall ApplyTheme(TForm* AForm) override;

    // 业务逻辑接口实现
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
    // 辅助方法
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

// 为了向后兼容，提供类型别名
using TQualityDetectionPresenter = TModernQualityDetectionPresenter;
#endif

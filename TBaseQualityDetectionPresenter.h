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
// 基础Presenter抽象类
class TBaseQualityDetectionPresenter
{
public:
    virtual ~TBaseQualityDetectionPresenter() = default;

    // 布局管理 - 负责控件的位置、大小、对齐等
    virtual void __fastcall ApplyLayout(TForm* AForm) = 0;

    // 局部样式 - 负责控件的外观、字体、颜色等
    virtual void __fastcall ApplyLocalStyles(TForm* AForm) = 0;

    // 主题应用 - 负责整体主题和VCL样式
    virtual void __fastcall ApplyTheme(TForm* AForm) = 0;

    // 完整样式应用 - 统一入口
    virtual void __fastcall ApplyFullStyle(TForm* AForm)
    {
        ApplyTheme(AForm);      // 先应用主题
        ApplyLayout(AForm);     // 然后布局
        ApplyLocalStyles(AForm); // 最后应用局部样式
    }

    // 业务逻辑接口（保持原有功能）
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

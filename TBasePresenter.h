//---------------------------------------------------------------------------

#ifndef TBasePresenterH
#define TBasePresenterH
//---------------------------------------------------------------------------

#include <Vcl.Forms.hpp>

class TBasePresenter
{
public:
    // 应用完整样式：先布局后局部样式
    virtual void __fastcall ApplyStyle(TForm* AForm) final
    {
        // 1. 先进行布局设置
        ApplyLayout(AForm);

        // 2. 再应用局部样式
        ApplyLocalStyles(AForm);

        // 3. 最后应用主题
        ApplyTheme(AForm);
    }

protected:
    // 布局管理 - 负责控件的位置、大小、对齐等
    virtual void __fastcall ApplyLayout(TForm* AForm) = 0;

    // 局部样式 - 负责控件的外观、字体、颜色等
    virtual void __fastcall ApplyLocalStyles(TForm* AForm) = 0;

    // 主题应用 - 负责整体主题和VCL样式
    virtual void __fastcall ApplyTheme(TForm* AForm) = 0;

public:
    // 事件处理接口
    virtual void __fastcall OnButtonClick() = 0;

    // 布局相关工具方法
    virtual void __fastcall ResizeControls(TForm* AForm)
    {
        // 默认实现：当窗体大小改变时重新应用布局
        ApplyLayout(AForm);
    }

    // 样式刷新方法
    virtual void __fastcall RefreshStyles(TForm* AForm)
    {
        ApplyLocalStyles(AForm);
    }
};

#endif

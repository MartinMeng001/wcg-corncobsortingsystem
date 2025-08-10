//---------------------------------------------------------------------------

#ifndef MainFramePresenterH
#define MainFramePresenterH
//---------------------------------------------------------------------------
#include "TBasePresenter.h"

class TMainFramePresenter : public TBasePresenter
{
private:
    // 布局配置结构
    struct LayoutConfig {
        int Margin;
        int Padding;
        int ButtonHeight;
        int ButtonWidth;
        int LabelHeight;
        int ControlSpacing;
    };

    // 样式配置结构
    struct StyleConfig {
        int FontSize;
        String ButtonCaption;
        String LabelText;
        TColor BackgroundColor;
    };

    LayoutConfig m_LayoutConfig;
    StyleConfig m_StyleConfig;

    // 初始化配置
    void __fastcall InitializeConfigs();

    // 布局辅助方法
    void __fastcall LayoutButton(TForm* AForm, TButton* Button);
    void __fastcall LayoutLabel(TForm* AForm, TLabel* Label);
    void __fastcall CalculateControlPositions(TForm* AForm);

protected:
    // 实现基类的布局接口
    void __fastcall ApplyLayout(TForm* AForm) override;

    // 实现基类的样式接口
    void __fastcall ApplyLocalStyles(TForm* AForm) override;

    // 实现基类的主题接口
    void __fastcall ApplyTheme(TForm* AForm) override;

public:
    // 构造函数
	TMainFramePresenter();

	// 事件处理
    void __fastcall OnButtonClick() override;

    // 现代风格特有的方法
    void __fastcall SetLayoutMargin(int Margin);
    void __fastcall SetControlSpacing(int Spacing);
    void __fastcall EnableResponsiveLayout(bool Enable);
};
#endif

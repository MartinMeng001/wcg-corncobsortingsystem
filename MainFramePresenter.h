//---------------------------------------------------------------------------

#ifndef MainFramePresenterH
#define MainFramePresenterH
//---------------------------------------------------------------------------
#include "TBasePresenter.h"

class TMainFramePresenter : public TBasePresenter
{
private:
    // �������ýṹ
    struct LayoutConfig {
        int Margin;
        int Padding;
        int ButtonHeight;
        int ButtonWidth;
        int LabelHeight;
        int ControlSpacing;
    };

    // ��ʽ���ýṹ
    struct StyleConfig {
        int FontSize;
        String ButtonCaption;
        String LabelText;
        TColor BackgroundColor;
    };

    LayoutConfig m_LayoutConfig;
    StyleConfig m_StyleConfig;

    // ��ʼ������
    void __fastcall InitializeConfigs();

    // ���ָ�������
    void __fastcall LayoutButton(TForm* AForm, TButton* Button);
    void __fastcall LayoutLabel(TForm* AForm, TLabel* Label);
    void __fastcall CalculateControlPositions(TForm* AForm);

protected:
    // ʵ�ֻ���Ĳ��ֽӿ�
    void __fastcall ApplyLayout(TForm* AForm) override;

    // ʵ�ֻ������ʽ�ӿ�
    void __fastcall ApplyLocalStyles(TForm* AForm) override;

    // ʵ�ֻ��������ӿ�
    void __fastcall ApplyTheme(TForm* AForm) override;

public:
    // ���캯��
	TMainFramePresenter();

	// �¼�����
    void __fastcall OnButtonClick() override;

    // �ִ�������еķ���
    void __fastcall SetLayoutMargin(int Margin);
    void __fastcall SetControlSpacing(int Spacing);
    void __fastcall EnableResponsiveLayout(bool Enable);
};
#endif

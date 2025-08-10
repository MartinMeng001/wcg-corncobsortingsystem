//---------------------------------------------------------------------------

#ifndef TBasePresenterH
#define TBasePresenterH
//---------------------------------------------------------------------------

#include <Vcl.Forms.hpp>

class TBasePresenter
{
public:
    // Ӧ��������ʽ���Ȳ��ֺ�ֲ���ʽ
    virtual void __fastcall ApplyStyle(TForm* AForm) final
    {
        // 1. �Ƚ��в�������
        ApplyLayout(AForm);

        // 2. ��Ӧ�þֲ���ʽ
        ApplyLocalStyles(AForm);

        // 3. ���Ӧ������
        ApplyTheme(AForm);
    }

protected:
    // ���ֹ��� - ����ؼ���λ�á���С�������
    virtual void __fastcall ApplyLayout(TForm* AForm) = 0;

    // �ֲ���ʽ - ����ؼ�����ۡ����塢��ɫ��
    virtual void __fastcall ApplyLocalStyles(TForm* AForm) = 0;

    // ����Ӧ�� - �������������VCL��ʽ
    virtual void __fastcall ApplyTheme(TForm* AForm) = 0;

public:
    // �¼�����ӿ�
    virtual void __fastcall OnButtonClick() = 0;

    // ������ع��߷���
    virtual void __fastcall ResizeControls(TForm* AForm)
    {
        // Ĭ��ʵ�֣��������С�ı�ʱ����Ӧ�ò���
        ApplyLayout(AForm);
    }

    // ��ʽˢ�·���
    virtual void __fastcall RefreshStyles(TForm* AForm)
    {
        ApplyLocalStyles(AForm);
    }
};

#endif

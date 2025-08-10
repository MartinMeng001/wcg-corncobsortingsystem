//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <System.SysUtils.hpp>
#include "MainFramePresenter.h"
#include "MainFrame.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner), m_TestForm(nullptr)
{
	m_Presenter = std::make_unique<TMainFramePresenter>();
}
__fastcall TForm1::~TForm1()
{
	// *** ����������ȷ����ȷ����FormTest ***
	CloseTestForm();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
    // һ��Ӧ�ã��Ȳ��֣�����ʽ���������
	m_Presenter->ApplyStyle(this);

	// ��ѡ���Զ��岼�ֲ���
	TMainFramePresenter* modernPresenter =
		static_cast<TMainFramePresenter*>(m_Presenter.get());
	if (modernPresenter) {
		modernPresenter->SetLayoutMargin(25);
		modernPresenter->SetControlSpacing(20);

		// ����Ӧ�ò��֣�ֻӰ�첼�֣����ظ�Ӧ����ʽ��
		modernPresenter->ResizeControls(this);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormResize(TObject *Sender)
{
    if (m_Presenter) {
		// ֻ����Ӧ�ò��֣����ظ�������ʽ
		m_Presenter->ResizeControls(this);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	if (m_Presenter) {
		m_Presenter->OnButtonClick();
	}
    try {
        ShowTestForm();
    }
    catch (const Exception& e) {
        ShowMessage("�򿪲���ҳ��ʧ�ܣ�" + e.Message);
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// *** ����FormTestʵ�� ***
void TForm1::CreateTestForm()
{
    if (m_TestForm == nullptr) {
        m_TestForm = new TForm2(this);  // ��MainFormΪOwner

        // ����FormTest�Ļ�������
        m_TestForm->Caption = "����Ʒ�ʼ��ϵͳ - ���Խ���";
        m_TestForm->Position = poScreenCenter;
        m_TestForm->WindowState = wsNormal;
        m_TestForm->Width = 1000;
        m_TestForm->Height = 700;

        // ������С�ߴ�����
        m_TestForm->Constraints->MinWidth = 800;
        m_TestForm->Constraints->MinHeight = 600;
    }
}

//---------------------------------------------------------------------------
// *** ��ʾFormTest ***
void TForm1::ShowTestForm()
{
    CreateTestForm();

    if (m_TestForm) {
        // ��鴰���Ƿ��Ѿ���ʾ
        if (!m_TestForm->Visible) {
            m_TestForm->Show();  // ʹ��Show()������ShowModal()������ͬʱ������������
        } else {
            // ����Ѿ���ʾ���������ǰ̨
            m_TestForm->BringToFront();
            if (m_TestForm->WindowState == wsMinimized) {
                m_TestForm->WindowState = wsNormal;
            }
        }
    }
}

//---------------------------------------------------------------------------
// *** �ر�FormTest ***
void TForm1::CloseTestForm()
{
    if (m_TestForm) {
        m_TestForm->Close();
        delete m_TestForm;
        m_TestForm = nullptr;
    }
}

//---------------------------------------------------------------------------
// *** ���������沼�� ***
//void TForm1::SetupMainLayout()
//{
    // ��̬���������������ؼ�
//    if (ButtonOpenTest) {
//		ButtonOpenTest->Caption = "�򿪼����Խ���";
//		ButtonOpenTest->Width = 150;
//		ButtonOpenTest->Height = 35;
//		ButtonOpenTest->Left = (ClientWidth - ButtonOpenTest->Width) / 2;
//		ButtonOpenTest->Top = 100;
//	}
//
//	if (Button1) {
//		Button1->Caption = "ϵͳ����";
//		Button1->Width = 150;
//		Button1->Height = 35;
//		Button1->Left = (ClientWidth - Button1->Width) / 2;
//		Button1->Top = 150;
//	}
//
//	if (Label1) {
//		Label1->Caption = "����о�ּ�ϵͳ";
//		Label1->Font->Size = 16;
//		Label1->Font->Style = TFontStyles() << fsBold;
//		Label1->Left = (ClientWidth - Label1->Width) / 2;
//		Label1->Top = 50;
//	}
//}
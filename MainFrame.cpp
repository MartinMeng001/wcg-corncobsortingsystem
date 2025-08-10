//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <System.SysUtils.hpp>
#include "TModernPresenter.h"
#include "MainFrame.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	m_Presenter = std::make_unique<TModernPresenter>();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
    // һ��Ӧ�ã��Ȳ��֣�����ʽ���������
	m_Presenter->ApplyStyle(this);

	// ��ѡ���Զ��岼�ֲ���
	TModernPresenter* modernPresenter =
		static_cast<TModernPresenter*>(m_Presenter.get());
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
}
//---------------------------------------------------------------------------

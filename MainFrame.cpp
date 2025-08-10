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
    // 一键应用：先布局，后样式，最后主题
	m_Presenter->ApplyStyle(this);

	// 可选：自定义布局参数
	TModernPresenter* modernPresenter =
		static_cast<TModernPresenter*>(m_Presenter.get());
	if (modernPresenter) {
		modernPresenter->SetLayoutMargin(25);
		modernPresenter->SetControlSpacing(20);

		// 重新应用布局（只影响布局，不重复应用样式）
		modernPresenter->ResizeControls(this);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormResize(TObject *Sender)
{
    if (m_Presenter) {
		// 只重新应用布局，不重复设置样式
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

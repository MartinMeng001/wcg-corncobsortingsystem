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
	// *** 析构函数：确保正确清理FormTest ***
	CloseTestForm();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
    // 一键应用：先布局，后样式，最后主题
	m_Presenter->ApplyStyle(this);

	// 可选：自定义布局参数
	TMainFramePresenter* modernPresenter =
		static_cast<TMainFramePresenter*>(m_Presenter.get());
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
    try {
        ShowTestForm();
    }
    catch (const Exception& e) {
        ShowMessage("打开测试页面失败：" + e.Message);
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// *** 创建FormTest实例 ***
void TForm1::CreateTestForm()
{
    if (m_TestForm == nullptr) {
        m_TestForm = new TForm2(this);  // 以MainForm为Owner

        // 设置FormTest的基本属性
        m_TestForm->Caption = "内在品质检测系统 - 测试界面";
        m_TestForm->Position = poScreenCenter;
        m_TestForm->WindowState = wsNormal;
        m_TestForm->Width = 1000;
        m_TestForm->Height = 700;

        // 设置最小尺寸限制
        m_TestForm->Constraints->MinWidth = 800;
        m_TestForm->Constraints->MinHeight = 600;
    }
}

//---------------------------------------------------------------------------
// *** 显示FormTest ***
void TForm1::ShowTestForm()
{
    CreateTestForm();

    if (m_TestForm) {
        // 检查窗体是否已经显示
        if (!m_TestForm->Visible) {
            m_TestForm->Show();  // 使用Show()而不是ShowModal()，允许同时操作两个窗体
        } else {
            // 如果已经显示，则将其带到前台
            m_TestForm->BringToFront();
            if (m_TestForm->WindowState == wsMinimized) {
                m_TestForm->WindowState = wsNormal;
            }
        }
    }
}

//---------------------------------------------------------------------------
// *** 关闭FormTest ***
void TForm1::CloseTestForm()
{
    if (m_TestForm) {
        m_TestForm->Close();
        delete m_TestForm;
        m_TestForm = nullptr;
    }
}

//---------------------------------------------------------------------------
// *** 设置主界面布局 ***
//void TForm1::SetupMainLayout()
//{
    // 动态创建或调整主界面控件
//    if (ButtonOpenTest) {
//		ButtonOpenTest->Caption = "打开检测测试界面";
//		ButtonOpenTest->Width = 150;
//		ButtonOpenTest->Height = 35;
//		ButtonOpenTest->Left = (ClientWidth - ButtonOpenTest->Width) / 2;
//		ButtonOpenTest->Top = 100;
//	}
//
//	if (Button1) {
//		Button1->Caption = "系统设置";
//		Button1->Width = 150;
//		Button1->Height = 35;
//		Button1->Left = (ClientWidth - Button1->Width) / 2;
//		Button1->Top = 150;
//	}
//
//	if (Label1) {
//		Label1->Caption = "玉米芯分拣系统";
//		Label1->Font->Size = 16;
//		Label1->Font->Style = TFontStyles() << fsBold;
//		Label1->Left = (ClientWidth - Label1->Width) / 2;
//		Label1->Top = 50;
//	}
//}
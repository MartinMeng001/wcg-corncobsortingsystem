//---------------------------------------------------------------------------

#ifndef MainFrameH
#define MainFrameH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "TBasePresenter.h"
#include "TestQualityDetecotor.h"
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TButton *Button1;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
	std::unique_ptr<TBasePresenter> m_Presenter;
	TForm2* m_TestForm;  // *** 添加FormTest实例指针 ***

	// *** 添加窗体管理方法 ***
    void CreateTestForm();
    void ShowTestForm();
	void CloseTestForm();
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
	__fastcall ~TForm1();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif

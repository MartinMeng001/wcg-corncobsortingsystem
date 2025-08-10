//---------------------------------------------------------------------------

#ifndef TestQualityDetecotorH
#define TestQualityDetecotorH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include "TBaseQualityDetectionPresenter.h"
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
	TPanel *PanelTop;
	TPanel *PanelCenter;
	TPanel *PanelBottom;
	TGroupBox *GroupBoxConnection;
	TLabel *LabelIP;
	TLabel *LabelPort;
	TEdit *EditIP;
	TEdit *EditPort;
	TButton *ButtonConnect;
	TButton *ButtonDisconnect;
	TLabel *LabelConnectionStatus;
	TGroupBox *GroupBoxReference;
	TButton *ButtonStartReference;
	TButton *ButtonStopReference;
	TLabel *LabelReferenceStatus;
	TGroupBox *GroupBoxDetection;
	TButton *ButtonStartDetection;
	TButton *ButtonStopDetection;
	TLabel *LabelDetectionStatus;
	TLabel *LabelSerialNumber;
	TEdit *EditSerialNumber;
	TGroupBox *GroupBoxResults;
	TStringGrid *GridResults;
	TButton *ButtonReadResults;
	TButton *ButtonClearResults;
	TGroupBox *GroupBoxStatus;
	TMemo *MemoLog;
	TLabel *LabelErrorCode;
	TEdit *EditErrorCode;
	TTimer *TimerPolling;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall ButtonConnectClick(TObject *Sender);
	void __fastcall ButtonDisconnectClick(TObject *Sender);
	void __fastcall ButtonStartReferenceClick(TObject *Sender);
	void __fastcall ButtonStopReferenceClick(TObject *Sender);
	void __fastcall ButtonStartDetectionClick(TObject *Sender);
	void __fastcall ButtonStopDetectionClick(TObject *Sender);
	void __fastcall ButtonReadResultsClick(TObject *Sender);
	void __fastcall ButtonClearResultsClick(TObject *Sender);
	void __fastcall TimerPollingTimer(TObject *Sender);
private:	// User declarations
	std::unique_ptr<TBaseQualityDetectionPresenter> m_Presenter;
	bool m_bConnected;
	bool m_bPollingActive;

	void InitializeComponents();
	void UpdateConnectionStatus(bool connected);
	void UpdateReferenceStatus(int status);
	void UpdateDetectionStatus(int status);
	void UpdateResults(const std::vector<float>& results);
	void AddLogMessage(const String& message);
	void SetControlsEnabled(bool enabled);
public:		// User declarations
	__fastcall TForm2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif

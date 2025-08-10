//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TestQualityDetecotor.h"
#include "ModernQualityDetectionPresenter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner), m_bConnected(false), m_bPollingActive(false)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormCreate(TObject *Sender)
{
    // �����ִ�����ҵ���߼�������
    m_Presenter = std::make_unique<TModernQualityDetectionPresenter>();

    // *** �ؼ���Ӧ��������UI��ʽ ***
    m_Presenter->ApplyFullStyle(this);

    // ��ʼ�����������������Presenter�����֣�������Լ򻯣�
    InitializeComponents();

    // ����Ĭ�����Ӳ���
    EditIP->Text = "192.168.1.100";
    EditPort->Text = "502";

    // ��ʼ���ؼ�״̬
    UpdateConnectionStatus(false);
    SetControlsEnabled(false);

    // ���ö�ʱ��
    TimerPolling->Interval = 100; // 100ms��ѯ���
    TimerPolling->Enabled = false;

	AddLogMessage("ϵͳ��ʼ�����");
}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormDestroy(TObject *Sender)
{
    if (m_bConnected) {
		m_Presenter->Disconnect();
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormResize(TObject *Sender)
{
    // *** �����С�ı�ʱ����Ӧ�ò��� ***
    if (m_Presenter) {
        m_Presenter->ApplyLayout(this);
	}
}
void TForm2::InitializeComponents()
{
    // Presenter�Ѵ���󲿷ֲ��֣�����ֻ��Ҫ����������صĳ�ʼ��
    if (GridResults) {
        // ���ñ��������������������Presenter����
        GridResults->ColCount = 3;
        GridResults->RowCount = 20; // ֧�����19�������
        GridResults->FixedCols = 0;
        GridResults->FixedRows = 1;

        // ���ñ�����
        GridResults->Cells[0][0] = "��ַ";
        GridResults->Cells[1][0] = "����";
        GridResults->Cells[2][0] = "��ֵ";

        // Ԥ�賣�ü����Ŀ
        GridResults->Cells[0][1] = "111";
        GridResults->Cells[1][1] = "�Ƕ�";
        GridResults->Cells[2][1] = "0.00";

        GridResults->Cells[0][2] = "112";
        GridResults->Cells[1][2] = "���";
        GridResults->Cells[2][2] = "0.00";

        // ������Ӹ�������Ŀ
        GridResults->Cells[0][3] = "113";
        GridResults->Cells[1][3] = "ˮ��";
        GridResults->Cells[2][3] = "0.00";

        GridResults->Cells[0][4] = "114";
        GridResults->Cells[1][4] = "������";
        GridResults->Cells[2][4] = "0.00";
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm2::ButtonConnectClick(TObject *Sender)
{
	try {
		String ip = EditIP->Text;
		int port = EditPort->Text.ToInt();

		AddLogMessage("�������ӵ� " + ip + ":" + IntToStr(port));

		bool result = m_Presenter->Connect(AnsiString(ip).c_str(), port);

		if (result) {
			m_bConnected = true;
			UpdateConnectionStatus(true);
			SetControlsEnabled(true);
			TimerPolling->Enabled = true;
			m_bPollingActive = true;
			AddLogMessage("���ӳɹ�");
		} else {
			AddLogMessage("����ʧ�ܣ�" + m_Presenter->GetLastError());
		}
	}
	catch (const Exception& e) {
		AddLogMessage("�����쳣��" + e.Message);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm2::ButtonDisconnectClick(TObject *Sender)
{
	try {
		TimerPolling->Enabled = false;
		m_bPollingActive = false;

		m_Presenter->Disconnect();
		m_bConnected = false;

		UpdateConnectionStatus(false);
		SetControlsEnabled(false);
		AddLogMessage("�ѶϿ�����");
	}
	catch (const Exception& e) {
		AddLogMessage("�Ͽ������쳣��" + e.Message);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm2::ButtonStartReferenceClick(TObject *Sender)
{
	try {
		bool result = m_Presenter->StartReference();
		if (result) {
			AddLogMessage("��ʼ�αȲɼ�");
		} else {
			AddLogMessage("�����α�ʧ�ܣ�" + m_Presenter->GetLastError());
		}
	}
	catch (const Exception& e) {
		AddLogMessage("�αȲɼ��쳣��" + e.Message);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm2::ButtonStopReferenceClick(TObject *Sender)
{
	try {
		bool result = m_Presenter->StopReference();
		if (result) {
			AddLogMessage("ֹͣ�αȲɼ�");
		} else {
			AddLogMessage("ֹͣ�α�ʧ�ܣ�" + m_Presenter->GetLastError());
		}
	}
	catch (const Exception& e) {
		AddLogMessage("ֹͣ�α��쳣��" + e.Message);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm2::ButtonStartDetectionClick(TObject *Sender)
{
	try {
		bool result = m_Presenter->StartDetection();
		if (result) {
			AddLogMessage("��ʼ���");
		} else {
			AddLogMessage("�������ʧ�ܣ�" + m_Presenter->GetLastError());
		}
	}
	catch (const Exception& e) {
		AddLogMessage("����쳣��" + e.Message);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm2::ButtonStopDetectionClick(TObject *Sender)
{
	try {
		bool result = m_Presenter->StopDetection();
		if (result) {
			AddLogMessage("ֹͣ���");
		} else {
			AddLogMessage("ֹͣ���ʧ�ܣ�" + m_Presenter->GetLastError());
		}
	}
	catch (const Exception& e) {
		AddLogMessage("ֹͣ����쳣��" + e.Message);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm2::ButtonReadResultsClick(TObject *Sender)
{
	try {
		std::vector<float> results = m_Presenter->ReadDetectionResults();
		UpdateResults(results);
		AddLogMessage("��ȡ��������");
	}
	catch (const Exception& e) {
		AddLogMessage("��ȡ����쳣��" + e.Message);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm2::ButtonClearResultsClick(TObject *Sender)
{
	// ��ս����ʾ
	for (int i = 1; i < GridResults->RowCount; i++) {
		GridResults->Cells[2][i] = "0.00";
	}
	EditSerialNumber->Text = "";
	EditErrorCode->Text = "";
	AddLogMessage("����ռ����");
}
//---------------------------------------------------------------------------
void __fastcall TForm2::TimerPollingTimer(TObject *Sender)
{
	if (!m_bConnected || !m_bPollingActive) {
		return;
	}

	try {
		// ��ѯ�α�״̬
		int refStatus = m_Presenter->GetReferenceStatus();
		UpdateReferenceStatus(refStatus);

		// ��ѯ���״̬
		int detStatus = m_Presenter->GetDetectionStatus();
		UpdateDetectionStatus(detStatus);

		// ��ȡ��ˮ��
		int serialNum = m_Presenter->GetSerialNumber();
		EditSerialNumber->Text = IntToStr(serialNum);

		// ��ȡ�쳣��
		int errorCode = m_Presenter->GetErrorCode();
		EditErrorCode->Text = IntToStr(errorCode);

		// ��������ɣ��Զ���ȡ���
		if (detStatus == 2) { // �ɼ��ɹ�
			std::vector<float> results = m_Presenter->ReadDetectionResults();
			UpdateResults(results);
		}
	}
	catch (const Exception& e) {
		// ��Ĭ������ѯ�쳣��������濨��
		static int errorCount = 0;
		errorCount++;
		if (errorCount % 50 == 1) { // ÿ5���¼һ�δ���
			AddLogMessage("��ѯ�쳣��" + e.Message);
		}
	}
}
//---------------------------------------------------------------------------

void TForm2::UpdateConnectionStatus(bool connected)
{
    if (LabelConnectionStatus) {
        if (connected) {
            LabelConnectionStatus->Caption = "������";
            LabelConnectionStatus->Font->Color = TColor(0x00008000); // ʹ��Presenter�ĳɹ�ɫ
            ButtonConnect->Enabled = false;
            ButtonDisconnect->Enabled = true;
        } else {
            LabelConnectionStatus->Caption = "δ����";
            LabelConnectionStatus->Font->Color = TColor(0x000000FF); // ʹ��Presenter�Ĵ���ɫ
            ButtonConnect->Enabled = true;
            ButtonDisconnect->Enabled = false;
        }
	}
}

//---------------------------------------------------------------------------

void TForm2::UpdateReferenceStatus(int status)
{
    if (!LabelReferenceStatus) return;

    String statusText;
    TColor statusColor = clBlack;

    switch (status) {
        case 1:
            statusText = "�ɼ���";
            statusColor = TColor(0x00FF8000); // ��ɫ - ������
            break;
        case 2:
            statusText = "�ɼ��ɹ�";
            statusColor = TColor(0x00008000); // ��ɫ - �ɹ�
            break;
        case 3:
            statusText = "�ɼ�ʧ��";
            statusColor = TColor(0x000000FF); // ��ɫ - ʧ��
            break;
        default:
            statusText = "����";
            statusColor = clGray;
            break;
    }

    LabelReferenceStatus->Caption = statusText;
	LabelReferenceStatus->Font->Color = statusColor;
}

//---------------------------------------------------------------------------

void TForm2::UpdateDetectionStatus(int status)
{
    if (!LabelDetectionStatus) return;

    String statusText;
    TColor statusColor = clBlack;

    switch (status) {
        case 1:
            statusText = "�����";
            statusColor = TColor(0x00FF8000); // ��ɫ - ������
            break;
        case 2:
            statusText = "������";
            statusColor = TColor(0x00008000); // ��ɫ - �ɹ�
            break;
        case 3:
            statusText = "���ʧ��";
            statusColor = TColor(0x000000FF); // ��ɫ - ʧ��
            break;
        default:
            statusText = "����";
            statusColor = clGray;
            break;
    }

    LabelDetectionStatus->Caption = statusText;
	LabelDetectionStatus->Font->Color = statusColor;
}

//---------------------------------------------------------------------------

void TForm2::UpdateResults(const std::vector<float>& results)
{
	// ���¼���������
	for (size_t i = 0; i < results.size() && i < 19; i++) {
		int row = i + 1;
		if (row < GridResults->RowCount) {
			GridResults->Cells[2][row] = FloatToStrF(results[i], ffFixed, 0, 2);
		}
	}
}

//---------------------------------------------------------------------------

void TForm2::AddLogMessage(const String& message)
{
	TDateTime now = Now();
	String timeStr = FormatDateTime("hh:nn:ss", now);
	String logLine = "[" + timeStr + "] " + message;

	MemoLog->Lines->Add(logLine);

	// ������־�����������ڴ����
	if (MemoLog->Lines->Count > 1000) {
		MemoLog->Lines->Delete(0);
	}

	// �Զ�������������Ϣ
	MemoLog->SelStart = MemoLog->GetTextLen();
	MemoLog->SelLength = 0;
	SendMessage(MemoLog->Handle, EM_SCROLLCARET, 0, 0);
}
//---------------------------------------------------------------------------

void TForm2::SetControlsEnabled(bool enabled)
{
	ButtonStartReference->Enabled = enabled;
	ButtonStopReference->Enabled = enabled;
	ButtonStartDetection->Enabled = enabled;
	ButtonStopDetection->Enabled = enabled;
	ButtonReadResults->Enabled = enabled;
}
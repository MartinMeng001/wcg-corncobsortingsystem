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
    // 创建现代风格的业务逻辑处理器
    m_Presenter = std::make_unique<TModernQualityDetectionPresenter>();

    // *** 关键：应用完整的UI样式 ***
    m_Presenter->ApplyFullStyle(this);

    // 初始化界面组件（现在由Presenter管理布局，这个可以简化）
    InitializeComponents();

    // 设置默认连接参数
    EditIP->Text = "192.168.1.100";
    EditPort->Text = "502";

    // 初始化控件状态
    UpdateConnectionStatus(false);
    SetControlsEnabled(false);

    // 配置定时器
    TimerPolling->Interval = 100; // 100ms轮询间隔
    TimerPolling->Enabled = false;

	AddLogMessage("系统初始化完成");
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
    // *** 窗体大小改变时重新应用布局 ***
    if (m_Presenter) {
        m_Presenter->ApplyLayout(this);
	}
}
void TForm2::InitializeComponents()
{
    // Presenter已处理大部分布局，这里只需要设置数据相关的初始化
    if (GridResults) {
        // 设置表格列数和行数（布局由Presenter处理）
        GridResults->ColCount = 3;
        GridResults->RowCount = 20; // 支持最多19个检测结果
        GridResults->FixedCols = 0;
        GridResults->FixedRows = 1;

        // 设置表格标题
        GridResults->Cells[0][0] = "地址";
        GridResults->Cells[1][0] = "名称";
        GridResults->Cells[2][0] = "数值";

        // 预设常用检测项目
        GridResults->Cells[0][1] = "111";
        GridResults->Cells[1][1] = "糖度";
        GridResults->Cells[2][1] = "0.00";

        GridResults->Cells[0][2] = "112";
        GridResults->Cells[1][2] = "酸度";
        GridResults->Cells[2][2] = "0.00";

        // 可以添加更多检测项目
        GridResults->Cells[0][3] = "113";
        GridResults->Cells[1][3] = "水分";
        GridResults->Cells[2][3] = "0.00";

        GridResults->Cells[0][4] = "114";
        GridResults->Cells[1][4] = "蛋白质";
        GridResults->Cells[2][4] = "0.00";
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm2::ButtonConnectClick(TObject *Sender)
{
	try {
		String ip = EditIP->Text;
		int port = EditPort->Text.ToInt();

		AddLogMessage("正在连接到 " + ip + ":" + IntToStr(port));

		bool result = m_Presenter->Connect(AnsiString(ip).c_str(), port);

		if (result) {
			m_bConnected = true;
			UpdateConnectionStatus(true);
			SetControlsEnabled(true);
			TimerPolling->Enabled = true;
			m_bPollingActive = true;
			AddLogMessage("连接成功");
		} else {
			AddLogMessage("连接失败：" + m_Presenter->GetLastError());
		}
	}
	catch (const Exception& e) {
		AddLogMessage("连接异常：" + e.Message);
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
		AddLogMessage("已断开连接");
	}
	catch (const Exception& e) {
		AddLogMessage("断开连接异常：" + e.Message);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm2::ButtonStartReferenceClick(TObject *Sender)
{
	try {
		bool result = m_Presenter->StartReference();
		if (result) {
			AddLogMessage("开始参比采集");
		} else {
			AddLogMessage("启动参比失败：" + m_Presenter->GetLastError());
		}
	}
	catch (const Exception& e) {
		AddLogMessage("参比采集异常：" + e.Message);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm2::ButtonStopReferenceClick(TObject *Sender)
{
	try {
		bool result = m_Presenter->StopReference();
		if (result) {
			AddLogMessage("停止参比采集");
		} else {
			AddLogMessage("停止参比失败：" + m_Presenter->GetLastError());
		}
	}
	catch (const Exception& e) {
		AddLogMessage("停止参比异常：" + e.Message);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm2::ButtonStartDetectionClick(TObject *Sender)
{
	try {
		bool result = m_Presenter->StartDetection();
		if (result) {
			AddLogMessage("开始检测");
		} else {
			AddLogMessage("启动检测失败：" + m_Presenter->GetLastError());
		}
	}
	catch (const Exception& e) {
		AddLogMessage("检测异常：" + e.Message);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm2::ButtonStopDetectionClick(TObject *Sender)
{
	try {
		bool result = m_Presenter->StopDetection();
		if (result) {
			AddLogMessage("停止检测");
		} else {
			AddLogMessage("停止检测失败：" + m_Presenter->GetLastError());
		}
	}
	catch (const Exception& e) {
		AddLogMessage("停止检测异常：" + e.Message);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm2::ButtonReadResultsClick(TObject *Sender)
{
	try {
		std::vector<float> results = m_Presenter->ReadDetectionResults();
		UpdateResults(results);
		AddLogMessage("读取检测结果完成");
	}
	catch (const Exception& e) {
		AddLogMessage("读取结果异常：" + e.Message);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm2::ButtonClearResultsClick(TObject *Sender)
{
	// 清空结果显示
	for (int i = 1; i < GridResults->RowCount; i++) {
		GridResults->Cells[2][i] = "0.00";
	}
	EditSerialNumber->Text = "";
	EditErrorCode->Text = "";
	AddLogMessage("已清空检测结果");
}
//---------------------------------------------------------------------------
void __fastcall TForm2::TimerPollingTimer(TObject *Sender)
{
	if (!m_bConnected || !m_bPollingActive) {
		return;
	}

	try {
		// 轮询参比状态
		int refStatus = m_Presenter->GetReferenceStatus();
		UpdateReferenceStatus(refStatus);

		// 轮询检测状态
		int detStatus = m_Presenter->GetDetectionStatus();
		UpdateDetectionStatus(detStatus);

		// 读取流水号
		int serialNum = m_Presenter->GetSerialNumber();
		EditSerialNumber->Text = IntToStr(serialNum);

		// 读取异常码
		int errorCode = m_Presenter->GetErrorCode();
		EditErrorCode->Text = IntToStr(errorCode);

		// 如果检测完成，自动读取结果
		if (detStatus == 2) { // 采集成功
			std::vector<float> results = m_Presenter->ReadDetectionResults();
			UpdateResults(results);
		}
	}
	catch (const Exception& e) {
		// 静默处理轮询异常，避免界面卡顿
		static int errorCount = 0;
		errorCount++;
		if (errorCount % 50 == 1) { // 每5秒记录一次错误
			AddLogMessage("轮询异常：" + e.Message);
		}
	}
}
//---------------------------------------------------------------------------

void TForm2::UpdateConnectionStatus(bool connected)
{
    if (LabelConnectionStatus) {
        if (connected) {
            LabelConnectionStatus->Caption = "已连接";
            LabelConnectionStatus->Font->Color = TColor(0x00008000); // 使用Presenter的成功色
            ButtonConnect->Enabled = false;
            ButtonDisconnect->Enabled = true;
        } else {
            LabelConnectionStatus->Caption = "未连接";
            LabelConnectionStatus->Font->Color = TColor(0x000000FF); // 使用Presenter的错误色
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
            statusText = "采集中";
            statusColor = TColor(0x00FF8000); // 蓝色 - 处理中
            break;
        case 2:
            statusText = "采集成功";
            statusColor = TColor(0x00008000); // 绿色 - 成功
            break;
        case 3:
            statusText = "采集失败";
            statusColor = TColor(0x000000FF); // 红色 - 失败
            break;
        default:
            statusText = "就绪";
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
            statusText = "检测中";
            statusColor = TColor(0x00FF8000); // 蓝色 - 处理中
            break;
        case 2:
            statusText = "检测完成";
            statusColor = TColor(0x00008000); // 绿色 - 成功
            break;
        case 3:
            statusText = "检测失败";
            statusColor = TColor(0x000000FF); // 红色 - 失败
            break;
        default:
            statusText = "就绪";
            statusColor = clGray;
            break;
    }

    LabelDetectionStatus->Caption = statusText;
	LabelDetectionStatus->Font->Color = statusColor;
}

//---------------------------------------------------------------------------

void TForm2::UpdateResults(const std::vector<float>& results)
{
	// 更新检测结果到表格
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

	// 限制日志行数，避免内存溢出
	if (MemoLog->Lines->Count > 1000) {
		MemoLog->Lines->Delete(0);
	}

	// 自动滚动到最新消息
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
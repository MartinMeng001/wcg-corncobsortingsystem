//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ModernQualityDetectionPresenter.h"
#include <Vcl.Themes.hpp>
#include <algorithm>

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
// TModernQualityDetectionPresenter 实现
//---------------------------------------------------------------------------

TModernQualityDetectionPresenter::TModernQualityDetectionPresenter()
    : TBaseQualityDetectionPresenter(), // 调用基类的构造函数
      m_bConnected(false),
      m_ReferenceStatus(0),
      m_DetectionStatus(0),
      m_SerialNumber(0),
      m_ErrorCode(0)
{
}

TModernQualityDetectionPresenter::~TModernQualityDetectionPresenter()
{
}

//---------------------------------------------------------------------------
// 主题应用 - 负责整体主题和VCL样式
//---------------------------------------------------------------------------
void __fastcall TModernQualityDetectionPresenter::ApplyTheme(TForm* AForm)
{
    if (!AForm) return;

    try {
        // 应用现代VCL主题
//        if (TStyleManager::StyleNames->IndexOf("Windows10") >= 0) {
//			TStyleManager::SetStyle("Windows10");
//		} else if (TStyleManager::StyleNames->IndexOf("Cobalt") >= 0) {
//			TStyleManager::SetStyle("Cobalt");
//		} else if (TStyleManager::StyleNames->IndexOf("Aqua Light Slate") >= 0) {
//			TStyleManager::SetStyle("Aqua Light Slate");
//		}
		TStyleManager::SetStyle("Windows10 Dark");

        // 设置窗体基础属性
        AForm->Color = m_Style.BackgroundColor;
        AForm->Font->Name = m_Style.FontName;
        AForm->Font->Size = m_Style.FontSize;
        AForm->Font->Style = TFontStyles();

        // 确保窗体支持主题
        AForm->DoubleBuffered = true;
        //AForm->ParentBackground = false;

    } catch (const Exception& e) {
        // 主题应用失败时回退到默认样式
        AForm->Color = clBtnFace;
    }
}

//---------------------------------------------------------------------------
// 布局管理 - 负责控件的位置、大小、对齐等
//---------------------------------------------------------------------------
void __fastcall TModernQualityDetectionPresenter::ApplyLayout(TForm* AForm)
{
    if (!AForm) return;

    // 按层级顺序应用布局
    ApplyPanelLayout(AForm);     // 主面板布局
    ApplyGroupBoxLayout(AForm);  // 功能组布局
    ApplyControlLayout(AForm);   // 控件布局
    ApplyGridLayout(AForm);      // 表格布局
}

void TModernQualityDetectionPresenter::ApplyPanelLayout(TForm* AForm)
{
    // 顶部面板布局
    if (auto panelTop = FindControl<TPanel>(AForm, "PanelTop")) {
        panelTop->Align = alTop;
        panelTop->Height = 120; // 为连接和参比功能预留空间
        panelTop->AlignWithMargins = true;
        panelTop->Margins->SetBounds(m_Layout.PanelMargin, m_Layout.PanelMargin,
                                   m_Layout.PanelMargin, m_Layout.PanelMargin/2);
    }

    // 底部面板布局
    if (auto panelBottom = FindControl<TPanel>(AForm, "PanelBottom")) {
        panelBottom->Align = alBottom;
        panelBottom->Height = 180; // 为状态信息和日志预留空间
        panelBottom->AlignWithMargins = true;
        panelBottom->Margins->SetBounds(m_Layout.PanelMargin, m_Layout.PanelMargin/2,
                                      m_Layout.PanelMargin, m_Layout.PanelMargin);
    }

    // 中心面板布局（用于检测控制和结果显示）
    if (auto panelCenter = FindControl<TPanel>(AForm, "PanelCenter")) {
        panelCenter->Align = alClient;
        panelCenter->AlignWithMargins = true;
        panelCenter->Margins->SetBounds(m_Layout.PanelMargin, 0,
                                      m_Layout.PanelMargin, 0);
    }
}

void TModernQualityDetectionPresenter::ApplyGroupBoxLayout(TForm* AForm)
{
    // 连接设置组 - 位于顶部面板左侧
    if (auto groupConn = FindControl<TGroupBox>(AForm, "GroupBoxConnection")) {
        groupConn->Left = m_Layout.GroupBoxSpacing;
        groupConn->Top = m_Layout.GroupBoxSpacing;
        groupConn->Width = 280;
        groupConn->Height = 100;
    }

    // 参比控制组 - 位于顶部面板右侧
    if (auto groupRef = FindControl<TGroupBox>(AForm, "GroupBoxReference")) {
        groupRef->Left = 300;
        groupRef->Top = m_Layout.GroupBoxSpacing;
        groupRef->Width = 200;
        groupRef->Height = 100;
    }

    // 检测控制组 - 位于中心面板顶部
    if (auto groupDet = FindControl<TGroupBox>(AForm, "GroupBoxDetection")) {
        groupDet->Left = m_Layout.GroupBoxSpacing;
        groupDet->Top = m_Layout.GroupBoxSpacing;
        groupDet->Width = 250;
        groupDet->Height = 120;
    }

    // 检测结果组 - 位于中心面板，占据主要空间
    if (auto groupResults = FindControl<TGroupBox>(AForm, "GroupBoxResults")) {
        groupResults->Left = 270;
        groupResults->Top = m_Layout.GroupBoxSpacing;
        groupResults->Anchors = TAnchors() << akLeft << akTop << akRight << akBottom;
        groupResults->Width = AForm->ClientWidth - 270 - m_Layout.GroupBoxSpacing;
        groupResults->Height = 200; // 将在ApplyGridLayout中进一步调整
    }

    // 状态信息组 - 位于底部面板
    if (auto groupStatus = FindControl<TGroupBox>(AForm, "GroupBoxStatus")) {
        groupStatus->Align = alClient;
        groupStatus->AlignWithMargins = true;
        groupStatus->Margins->SetBounds(m_Layout.GroupBoxSpacing, m_Layout.GroupBoxSpacing,
                                      m_Layout.GroupBoxSpacing, m_Layout.GroupBoxSpacing);
    }
}

void TModernQualityDetectionPresenter::ApplyControlLayout(TForm* AForm)
{
    // 连接设置组内的控件布局
    if (auto groupConn = FindControl<TGroupBox>(AForm, "GroupBoxConnection")) {
        int baseLeft = 12;
        int baseTop = 20;
        int lineHeight = 26;

        // IP地址行
        if (auto label = FindControl<TLabel>(AForm, "LabelIP")) {
            label->Left = baseLeft;
            label->Top = baseTop;
            label->Width = 40;
        }
        if (auto edit = FindControl<TEdit>(AForm, "EditIP")) {
            edit->Left = baseLeft + 45;
            edit->Top = baseTop - 2;
            edit->Width = 100;
            edit->Height = m_Layout.EditHeight;
        }

        // 端口行
        if (auto label = FindControl<TLabel>(AForm, "LabelPort")) {
            label->Left = baseLeft + 155;
            label->Top = baseTop;
            label->Width = 30;
        }
        if (auto edit = FindControl<TEdit>(AForm, "EditPort")) {
            edit->Left = baseLeft + 190;
            edit->Top = baseTop - 2;
            edit->Width = 60;
            edit->Height = m_Layout.EditHeight;
        }

        // 连接按钮行
        if (auto btn = FindControl<TButton>(AForm, "ButtonConnect")) {
            btn->Left = baseLeft;
            btn->Top = baseTop + lineHeight;
            btn->Width = m_Layout.MinButtonWidth;
            btn->Height = m_Layout.ButtonHeight;
        }
        if (auto btn = FindControl<TButton>(AForm, "ButtonDisconnect")) {
            btn->Left = baseLeft + m_Layout.MinButtonWidth + 8;
            btn->Top = baseTop + lineHeight;
            btn->Width = m_Layout.MinButtonWidth;
            btn->Height = m_Layout.ButtonHeight;
        }

        // 连接状态标签
        if (auto label = FindControl<TLabel>(AForm, "LabelConnectionStatus")) {
            label->Left = baseLeft + 170;
            label->Top = baseTop + lineHeight + 6;
            label->Width = 80;
        }
    }

    // 参比控制组内的控件布局
    if (auto groupRef = FindControl<TGroupBox>(AForm, "GroupBoxReference")) {
        int baseLeft = 12;
        int baseTop = 20;

        if (auto btn = FindControl<TButton>(AForm, "ButtonStartReference")) {
            btn->Left = baseLeft;
            btn->Top = baseTop;
            btn->Width = m_Layout.MinButtonWidth;
            btn->Height = m_Layout.ButtonHeight;
        }
        if (auto btn = FindControl<TButton>(AForm, "ButtonStopReference")) {
            btn->Left = baseLeft + m_Layout.MinButtonWidth + 8;
            btn->Top = baseTop;
            btn->Width = m_Layout.MinButtonWidth;
            btn->Height = m_Layout.ButtonHeight;
        }
        if (auto label = FindControl<TLabel>(AForm, "LabelReferenceStatus")) {
            label->Left = baseLeft;
            label->Top = baseTop + m_Layout.ButtonHeight + 8;
            label->Width = 160;
        }
    }

    // 检测控制组内的控件布局
    if (auto groupDet = FindControl<TGroupBox>(AForm, "GroupBoxDetection")) {
        int baseLeft = 12;
        int baseTop = 20;

        if (auto btn = FindControl<TButton>(AForm, "ButtonStartDetection")) {
            btn->Left = baseLeft;
            btn->Top = baseTop;
            btn->Width = m_Layout.MinButtonWidth;
            btn->Height = m_Layout.ButtonHeight;
        }
        if (auto btn = FindControl<TButton>(AForm, "ButtonStopDetection")) {
            btn->Left = baseLeft + m_Layout.MinButtonWidth + 8;
            btn->Top = baseTop;
            btn->Width = m_Layout.MinButtonWidth;
            btn->Height = m_Layout.ButtonHeight;
        }
        if (auto label = FindControl<TLabel>(AForm, "LabelDetectionStatus")) {
            label->Left = baseLeft;
            label->Top = baseTop + m_Layout.ButtonHeight + 8;
            label->Width = 160;
        }

        // 流水号显示
        if (auto label = FindControl<TLabel>(AForm, "LabelSerialNumber")) {
            label->Left = baseLeft;
            label->Top = baseTop + m_Layout.ButtonHeight + 32;
            label->Width = 50;
        }
        if (auto edit = FindControl<TEdit>(AForm, "EditSerialNumber")) {
            edit->Left = baseLeft + 55;
            edit->Top = baseTop + m_Layout.ButtonHeight + 29;
            edit->Width = 80;
            edit->Height = m_Layout.EditHeight;
        }
    }
}

void TModernQualityDetectionPresenter::ApplyGridLayout(TForm* AForm)
{
    if (auto groupResults = FindControl<TGroupBox>(AForm, "GroupBoxResults")) {
        // 结果操作按钮
        if (auto btn = FindControl<TButton>(AForm, "ButtonReadResults")) {
            btn->Left = 12;
            btn->Top = 20;
            btn->Width = m_Layout.MinButtonWidth;
            btn->Height = m_Layout.ButtonHeight;
        }
        if (auto btn = FindControl<TButton>(AForm, "ButtonClearResults")) {
            btn->Left = 12 + m_Layout.MinButtonWidth + 8;
            btn->Top = 20;
            btn->Width = m_Layout.MinButtonWidth;
            btn->Height = m_Layout.ButtonHeight;
        }

        // 结果表格
        if (auto grid = FindControl<TStringGrid>(AForm, "GridResults")) {
            grid->Left = 12;
            grid->Top = 20 + m_Layout.ButtonHeight + 8;
            grid->Width = groupResults->Width - 24;
            grid->Height = std::max(m_Layout.GridMinHeight,
                                  groupResults->Height - (20 + m_Layout.ButtonHeight + 8) - 12);
            grid->Anchors = TAnchors() << akLeft << akTop << akRight << akBottom;
        }
    }
}

//---------------------------------------------------------------------------
// 局部样式 - 负责控件的外观、字体、颜色等
//---------------------------------------------------------------------------
void __fastcall TModernQualityDetectionPresenter::ApplyLocalStyles(TForm* AForm)
{
    if (!AForm) return;

    ApplyFontStyles(AForm);    // 字体样式
    ApplyColorStyles(AForm);   // 颜色样式
    ApplyControlStyles(AForm); // 控件特定样式
}

void TModernQualityDetectionPresenter::ApplyFontStyles(TForm* AForm)
{
    // 为所有GroupBox设置标题字体
    for (int i = 0; i < AForm->ComponentCount; i++) {
        if (auto groupBox = dynamic_cast<TGroupBox*>(AForm->Components[i])) {
            groupBox->Font->Name = m_Style.FontName;
            groupBox->Font->Size = m_Style.TitleFontSize;
            groupBox->Font->Style = TFontStyles() << fsBold;
        }

        // 为所有标签设置字体
        else if (auto label = dynamic_cast<TLabel*>(AForm->Components[i])) {
            label->Font->Name = m_Style.FontName;
            label->Font->Size = m_Style.FontSize;
            label->Font->Style = TFontStyles();
        }

        // 为所有按钮设置字体
        else if (auto button = dynamic_cast<TButton*>(AForm->Components[i])) {
            button->Font->Name = m_Style.FontName;
            button->Font->Size = m_Style.FontSize;
            button->Font->Style = TFontStyles();
        }

        // 为所有编辑框设置字体
        else if (auto edit = dynamic_cast<TEdit*>(AForm->Components[i])) {
            edit->Font->Name = m_Style.FontName;
            edit->Font->Size = m_Style.FontSize;
        }
    }
}

void TModernQualityDetectionPresenter::ApplyColorStyles(TForm* AForm)
{
    // 设置面板颜色
    for (int i = 0; i < AForm->ComponentCount; i++) {
        if (auto panel = dynamic_cast<TPanel*>(AForm->Components[i])) {
            panel->Color = m_Style.PanelColor;
            panel->ParentBackground = false;
            panel->BevelOuter = bvNone;
            panel->BorderStyle = bsSingle;
        }

        else if (auto groupBox = dynamic_cast<TGroupBox*>(AForm->Components[i])) {
            groupBox->Color = m_Style.GroupBoxColor;
            groupBox->ParentBackground = false;
        }
    }

    // 设置状态相关控件的初始颜色
    if (auto label = FindControl<TLabel>(AForm, "LabelConnectionStatus")) {
        label->Font->Color = m_Style.ErrorColor;
    }
    if (auto label = FindControl<TLabel>(AForm, "LabelReferenceStatus")) {
        label->Font->Color = clGray;
    }
    if (auto label = FindControl<TLabel>(AForm, "LabelDetectionStatus")) {
        label->Font->Color = clGray;
    }
}

void TModernQualityDetectionPresenter::ApplyControlStyles(TForm* AForm)
{
    // 设置表格样式
    if (auto grid = FindControl<TStringGrid>(AForm, "GridResults")) {
        grid->Color = clWhite;
        grid->FixedColor = TColor(0x00F0F0F0);
        grid->GridLineWidth = 1;
        grid->DefaultDrawing = true;
        grid->Font->Name = m_Style.FontName;
        grid->Font->Size = m_Style.FontSize;

        // 设置列宽比例而非固定值
        if (grid->ColCount >= 3) {
            int totalWidth = grid->ClientWidth - 2;
            grid->ColWidths[0] = totalWidth * 20 / 100; // 地址 20%
            grid->ColWidths[1] = totalWidth * 40 / 100; // 名称 40%
            grid->ColWidths[2] = totalWidth * 40 / 100; // 数值 40%
        }
    }

    // 设置日志控件样式
    if (auto memo = FindControl<TMemo>(AForm, "MemoLog")) {
        memo->Color = clWhite;
        memo->Font->Name = "Consolas"; // 使用等宽字体便于阅读日志
        memo->Font->Size = 8;
        memo->ReadOnly = true;
        memo->ScrollBars = ssVertical;
        memo->WordWrap = false;
    }

    // 设置只读编辑框样式
    if (auto edit = FindControl<TEdit>(AForm, "EditSerialNumber")) {
        edit->ReadOnly = true;
        edit->Color = TColor(0x00F0F0F0);
    }
    if (auto edit = FindControl<TEdit>(AForm, "EditErrorCode")) {
        edit->ReadOnly = true;
        edit->Color = TColor(0x00F0F0F0);
    }
}

//---------------------------------------------------------------------------
// 辅助方法：查找控件的模板函数
//---------------------------------------------------------------------------
template<typename T>
T* TModernQualityDetectionPresenter::FindControl(TForm* AForm, const String& Name)
{
    if (!AForm) return nullptr;

    TComponent* comp = AForm->FindComponent(Name);
    return dynamic_cast<T*>(comp);
}

//---------------------------------------------------------------------------
// 业务逻辑实现（简化版本，用于演示）
//---------------------------------------------------------------------------

bool TModernQualityDetectionPresenter::Connect(const char* ip, int port)
{
    try {
        m_IP = ip;
        m_Port = port;

        // 这里应该是实际的Modbus TCP连接逻辑
        // 目前使用模拟实现
        if (String(ip).IsEmpty() || port <= 0) {
            m_LastError = "无效的IP地址或端口";
            return false;
        }

        m_bConnected = true;
        m_LastError = "";
        return true;
    }
    catch (...) {
        m_LastError = "连接过程中发生异常";
        return false;
    }
}

void TModernQualityDetectionPresenter::Disconnect()
{
    m_bConnected = false;
    m_LastError = "";
}

bool TModernQualityDetectionPresenter::StartReference()
{
    if (!m_bConnected) {
        m_LastError = "设备未连接";
        return false;
    }

    // 模拟写入Modbus地址103（参比控制位）
    m_ReferenceStatus = 1; // 采集中
    return true;
}

bool TModernQualityDetectionPresenter::StopReference()
{
    if (!m_bConnected) {
        m_LastError = "设备未连接";
        return false;
    }

    m_ReferenceStatus = 0;
    return true;
}

bool TModernQualityDetectionPresenter::StartDetection()
{
    if (!m_bConnected) {
        m_LastError = "设备未连接";
        return false;
    }

    // 模拟写入Modbus地址102（检测控制位）
    m_DetectionStatus = 1; // 采集中
    return true;
}

bool TModernQualityDetectionPresenter::StopDetection()
{
    if (!m_bConnected) {
        m_LastError = "设备未连接";
        return false;
    }

    m_DetectionStatus = 0;
    return true;
}

std::vector<float> TModernQualityDetectionPresenter::ReadDetectionResults()
{
    std::vector<float> results;

    if (!m_bConnected) {
        return results;
    }

    // 模拟读取Modbus地址111-129的检测结果
    // 这里返回模拟数据
    results.push_back(12.35f); // 糖度
    results.push_back(0.68f);  // 酸度

    return results;
}

int TModernQualityDetectionPresenter::GetReferenceStatus()
{
    if (!m_bConnected) return 0;

    // 模拟状态变化
    static int counter = 0;
    counter++;
    if (m_ReferenceStatus == 1 && counter > 50) { // 模拟5秒后完成
        m_ReferenceStatus = 2; // 采集成功
        counter = 0;
    }

    return m_ReferenceStatus;
}

int TModernQualityDetectionPresenter::GetDetectionStatus()
{
    if (!m_bConnected) return 0;

    // 模拟状态变化
    static int counter = 0;
    counter++;
    if (m_DetectionStatus == 1 && counter > 30) { // 模拟3秒后完成
        m_DetectionStatus = 2; // 采集成功
        counter = 0;
    }

    return m_DetectionStatus;
}

int TModernQualityDetectionPresenter::GetSerialNumber()
{
    if (!m_bConnected) return 0;
    return m_SerialNumber++;
}

int TModernQualityDetectionPresenter::GetErrorCode()
{
    return m_ErrorCode;
}

String TModernQualityDetectionPresenter::GetLastError()
{
    return m_LastError;
}

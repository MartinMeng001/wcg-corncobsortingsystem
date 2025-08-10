//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ModernQualityDetectionPresenter.h"
#include <Vcl.Themes.hpp>
#include <algorithm>

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
// TModernQualityDetectionPresenter ʵ��
//---------------------------------------------------------------------------

TModernQualityDetectionPresenter::TModernQualityDetectionPresenter()
    : TBaseQualityDetectionPresenter(), // ���û���Ĺ��캯��
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
// ����Ӧ�� - �������������VCL��ʽ
//---------------------------------------------------------------------------
void __fastcall TModernQualityDetectionPresenter::ApplyTheme(TForm* AForm)
{
    if (!AForm) return;

    try {
        // Ӧ���ִ�VCL����
//        if (TStyleManager::StyleNames->IndexOf("Windows10") >= 0) {
//			TStyleManager::SetStyle("Windows10");
//		} else if (TStyleManager::StyleNames->IndexOf("Cobalt") >= 0) {
//			TStyleManager::SetStyle("Cobalt");
//		} else if (TStyleManager::StyleNames->IndexOf("Aqua Light Slate") >= 0) {
//			TStyleManager::SetStyle("Aqua Light Slate");
//		}
		TStyleManager::SetStyle("Windows10 Dark");

        // ���ô����������
        AForm->Color = m_Style.BackgroundColor;
        AForm->Font->Name = m_Style.FontName;
        AForm->Font->Size = m_Style.FontSize;
        AForm->Font->Style = TFontStyles();

        // ȷ������֧������
        AForm->DoubleBuffered = true;
        //AForm->ParentBackground = false;

    } catch (const Exception& e) {
        // ����Ӧ��ʧ��ʱ���˵�Ĭ����ʽ
        AForm->Color = clBtnFace;
    }
}

//---------------------------------------------------------------------------
// ���ֹ��� - ����ؼ���λ�á���С�������
//---------------------------------------------------------------------------
void __fastcall TModernQualityDetectionPresenter::ApplyLayout(TForm* AForm)
{
    if (!AForm) return;

    // ���㼶˳��Ӧ�ò���
    ApplyPanelLayout(AForm);     // ����岼��
    ApplyGroupBoxLayout(AForm);  // �����鲼��
    ApplyControlLayout(AForm);   // �ؼ�����
    ApplyGridLayout(AForm);      // ��񲼾�
}

void TModernQualityDetectionPresenter::ApplyPanelLayout(TForm* AForm)
{
    // ������岼��
    if (auto panelTop = FindControl<TPanel>(AForm, "PanelTop")) {
        panelTop->Align = alTop;
        panelTop->Height = 120; // Ϊ���ӺͲαȹ���Ԥ���ռ�
        panelTop->AlignWithMargins = true;
        panelTop->Margins->SetBounds(m_Layout.PanelMargin, m_Layout.PanelMargin,
                                   m_Layout.PanelMargin, m_Layout.PanelMargin/2);
    }

    // �ײ���岼��
    if (auto panelBottom = FindControl<TPanel>(AForm, "PanelBottom")) {
        panelBottom->Align = alBottom;
        panelBottom->Height = 180; // Ϊ״̬��Ϣ����־Ԥ���ռ�
        panelBottom->AlignWithMargins = true;
        panelBottom->Margins->SetBounds(m_Layout.PanelMargin, m_Layout.PanelMargin/2,
                                      m_Layout.PanelMargin, m_Layout.PanelMargin);
    }

    // ������岼�֣����ڼ����ƺͽ����ʾ��
    if (auto panelCenter = FindControl<TPanel>(AForm, "PanelCenter")) {
        panelCenter->Align = alClient;
        panelCenter->AlignWithMargins = true;
        panelCenter->Margins->SetBounds(m_Layout.PanelMargin, 0,
                                      m_Layout.PanelMargin, 0);
    }
}

void TModernQualityDetectionPresenter::ApplyGroupBoxLayout(TForm* AForm)
{
    // ���������� - λ�ڶ���������
    if (auto groupConn = FindControl<TGroupBox>(AForm, "GroupBoxConnection")) {
        groupConn->Left = m_Layout.GroupBoxSpacing;
        groupConn->Top = m_Layout.GroupBoxSpacing;
        groupConn->Width = 280;
        groupConn->Height = 100;
    }

    // �αȿ����� - λ�ڶ�������Ҳ�
    if (auto groupRef = FindControl<TGroupBox>(AForm, "GroupBoxReference")) {
        groupRef->Left = 300;
        groupRef->Top = m_Layout.GroupBoxSpacing;
        groupRef->Width = 200;
        groupRef->Height = 100;
    }

    // �������� - λ��������嶥��
    if (auto groupDet = FindControl<TGroupBox>(AForm, "GroupBoxDetection")) {
        groupDet->Left = m_Layout.GroupBoxSpacing;
        groupDet->Top = m_Layout.GroupBoxSpacing;
        groupDet->Width = 250;
        groupDet->Height = 120;
    }

    // ������� - λ��������壬ռ����Ҫ�ռ�
    if (auto groupResults = FindControl<TGroupBox>(AForm, "GroupBoxResults")) {
        groupResults->Left = 270;
        groupResults->Top = m_Layout.GroupBoxSpacing;
        groupResults->Anchors = TAnchors() << akLeft << akTop << akRight << akBottom;
        groupResults->Width = AForm->ClientWidth - 270 - m_Layout.GroupBoxSpacing;
        groupResults->Height = 200; // ����ApplyGridLayout�н�һ������
    }

    // ״̬��Ϣ�� - λ�ڵײ����
    if (auto groupStatus = FindControl<TGroupBox>(AForm, "GroupBoxStatus")) {
        groupStatus->Align = alClient;
        groupStatus->AlignWithMargins = true;
        groupStatus->Margins->SetBounds(m_Layout.GroupBoxSpacing, m_Layout.GroupBoxSpacing,
                                      m_Layout.GroupBoxSpacing, m_Layout.GroupBoxSpacing);
    }
}

void TModernQualityDetectionPresenter::ApplyControlLayout(TForm* AForm)
{
    // �����������ڵĿؼ�����
    if (auto groupConn = FindControl<TGroupBox>(AForm, "GroupBoxConnection")) {
        int baseLeft = 12;
        int baseTop = 20;
        int lineHeight = 26;

        // IP��ַ��
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

        // �˿���
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

        // ���Ӱ�ť��
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

        // ����״̬��ǩ
        if (auto label = FindControl<TLabel>(AForm, "LabelConnectionStatus")) {
            label->Left = baseLeft + 170;
            label->Top = baseTop + lineHeight + 6;
            label->Width = 80;
        }
    }

    // �αȿ������ڵĿؼ�����
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

    // ���������ڵĿؼ�����
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

        // ��ˮ����ʾ
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
        // ���������ť
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

        // ������
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
// �ֲ���ʽ - ����ؼ�����ۡ����塢��ɫ��
//---------------------------------------------------------------------------
void __fastcall TModernQualityDetectionPresenter::ApplyLocalStyles(TForm* AForm)
{
    if (!AForm) return;

    ApplyFontStyles(AForm);    // ������ʽ
    ApplyColorStyles(AForm);   // ��ɫ��ʽ
    ApplyControlStyles(AForm); // �ؼ��ض���ʽ
}

void TModernQualityDetectionPresenter::ApplyFontStyles(TForm* AForm)
{
    // Ϊ����GroupBox���ñ�������
    for (int i = 0; i < AForm->ComponentCount; i++) {
        if (auto groupBox = dynamic_cast<TGroupBox*>(AForm->Components[i])) {
            groupBox->Font->Name = m_Style.FontName;
            groupBox->Font->Size = m_Style.TitleFontSize;
            groupBox->Font->Style = TFontStyles() << fsBold;
        }

        // Ϊ���б�ǩ��������
        else if (auto label = dynamic_cast<TLabel*>(AForm->Components[i])) {
            label->Font->Name = m_Style.FontName;
            label->Font->Size = m_Style.FontSize;
            label->Font->Style = TFontStyles();
        }

        // Ϊ���а�ť��������
        else if (auto button = dynamic_cast<TButton*>(AForm->Components[i])) {
            button->Font->Name = m_Style.FontName;
            button->Font->Size = m_Style.FontSize;
            button->Font->Style = TFontStyles();
        }

        // Ϊ���б༭����������
        else if (auto edit = dynamic_cast<TEdit*>(AForm->Components[i])) {
            edit->Font->Name = m_Style.FontName;
            edit->Font->Size = m_Style.FontSize;
        }
    }
}

void TModernQualityDetectionPresenter::ApplyColorStyles(TForm* AForm)
{
    // ���������ɫ
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

    // ����״̬��ؿؼ��ĳ�ʼ��ɫ
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
    // ���ñ����ʽ
    if (auto grid = FindControl<TStringGrid>(AForm, "GridResults")) {
        grid->Color = clWhite;
        grid->FixedColor = TColor(0x00F0F0F0);
        grid->GridLineWidth = 1;
        grid->DefaultDrawing = true;
        grid->Font->Name = m_Style.FontName;
        grid->Font->Size = m_Style.FontSize;

        // �����п�������ǹ̶�ֵ
        if (grid->ColCount >= 3) {
            int totalWidth = grid->ClientWidth - 2;
            grid->ColWidths[0] = totalWidth * 20 / 100; // ��ַ 20%
            grid->ColWidths[1] = totalWidth * 40 / 100; // ���� 40%
            grid->ColWidths[2] = totalWidth * 40 / 100; // ��ֵ 40%
        }
    }

    // ������־�ؼ���ʽ
    if (auto memo = FindControl<TMemo>(AForm, "MemoLog")) {
        memo->Color = clWhite;
        memo->Font->Name = "Consolas"; // ʹ�õȿ���������Ķ���־
        memo->Font->Size = 8;
        memo->ReadOnly = true;
        memo->ScrollBars = ssVertical;
        memo->WordWrap = false;
    }

    // ����ֻ���༭����ʽ
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
// �������������ҿؼ���ģ�庯��
//---------------------------------------------------------------------------
template<typename T>
T* TModernQualityDetectionPresenter::FindControl(TForm* AForm, const String& Name)
{
    if (!AForm) return nullptr;

    TComponent* comp = AForm->FindComponent(Name);
    return dynamic_cast<T*>(comp);
}

//---------------------------------------------------------------------------
// ҵ���߼�ʵ�֣��򻯰汾��������ʾ��
//---------------------------------------------------------------------------

bool TModernQualityDetectionPresenter::Connect(const char* ip, int port)
{
    try {
        m_IP = ip;
        m_Port = port;

        // ����Ӧ����ʵ�ʵ�Modbus TCP�����߼�
        // Ŀǰʹ��ģ��ʵ��
        if (String(ip).IsEmpty() || port <= 0) {
            m_LastError = "��Ч��IP��ַ��˿�";
            return false;
        }

        m_bConnected = true;
        m_LastError = "";
        return true;
    }
    catch (...) {
        m_LastError = "���ӹ����з����쳣";
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
        m_LastError = "�豸δ����";
        return false;
    }

    // ģ��д��Modbus��ַ103���αȿ���λ��
    m_ReferenceStatus = 1; // �ɼ���
    return true;
}

bool TModernQualityDetectionPresenter::StopReference()
{
    if (!m_bConnected) {
        m_LastError = "�豸δ����";
        return false;
    }

    m_ReferenceStatus = 0;
    return true;
}

bool TModernQualityDetectionPresenter::StartDetection()
{
    if (!m_bConnected) {
        m_LastError = "�豸δ����";
        return false;
    }

    // ģ��д��Modbus��ַ102��������λ��
    m_DetectionStatus = 1; // �ɼ���
    return true;
}

bool TModernQualityDetectionPresenter::StopDetection()
{
    if (!m_bConnected) {
        m_LastError = "�豸δ����";
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

    // ģ���ȡModbus��ַ111-129�ļ����
    // ���ﷵ��ģ������
    results.push_back(12.35f); // �Ƕ�
    results.push_back(0.68f);  // ���

    return results;
}

int TModernQualityDetectionPresenter::GetReferenceStatus()
{
    if (!m_bConnected) return 0;

    // ģ��״̬�仯
    static int counter = 0;
    counter++;
    if (m_ReferenceStatus == 1 && counter > 50) { // ģ��5������
        m_ReferenceStatus = 2; // �ɼ��ɹ�
        counter = 0;
    }

    return m_ReferenceStatus;
}

int TModernQualityDetectionPresenter::GetDetectionStatus()
{
    if (!m_bConnected) return 0;

    // ģ��״̬�仯
    static int counter = 0;
    counter++;
    if (m_DetectionStatus == 1 && counter > 30) { // ģ��3������
        m_DetectionStatus = 2; // �ɼ��ɹ�
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

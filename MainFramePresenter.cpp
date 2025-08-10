//---------------------------------------------------------------------------

#pragma hdrstop

#include <Vcl.Themes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include "MainFramePresenter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TMainFramePresenter::TMainFramePresenter()
{
    InitializeConfigs();
}

void __fastcall TMainFramePresenter::InitializeConfigs()
{
    // ��ʼ����������
    m_LayoutConfig.Margin = 20;
    m_LayoutConfig.Padding = 10;
	m_LayoutConfig.ButtonHeight = 50;
	m_LayoutConfig.ButtonWidth = 230;
    m_LayoutConfig.LabelHeight = 50;
    m_LayoutConfig.ControlSpacing = 15;

    // ��ʼ����ʽ����
    m_StyleConfig.FontSize = 12;
    m_StyleConfig.ButtonCaption = "Modern Click";
	m_StyleConfig.LabelText = "���׷ּ�ϵͳ1.0";
    m_StyleConfig.BackgroundColor = clWhite;
}

// ========== ���ֹ���ʵ�� ==========
void __fastcall TMainFramePresenter::ApplyLayout(TForm* AForm)
{
    // 1. ����ؼ�λ��
    CalculateControlPositions(AForm);

    // 2. ���ָ����ؼ�
    if (AForm->FindComponent("Button1")) {
        TButton* Button1 = static_cast<TButton*>(AForm->FindComponent("Button1"));
        LayoutButton(AForm, Button1);
    }

    if (AForm->FindComponent("Label1")) {
        TLabel* Label1 = static_cast<TLabel*>(AForm->FindComponent("Label1"));
        LayoutLabel(AForm, Label1);
    }
}

void __fastcall TMainFramePresenter::CalculateControlPositions(TForm* AForm)
{
    // ���ڴ����С����ؼ�λ��
    // �������ʵ����Ӧʽ�����߼�
    int ClientWidth = AForm->ClientWidth;
    int ClientHeight = AForm->ClientHeight;

    // �������̫С��������С�ߴ�
    if (ClientWidth < 300) {
        AForm->ClientWidth = 300;
        ClientWidth = 300;
    }
    if (ClientHeight < 200) {
        AForm->ClientHeight = 200;
        ClientHeight = 200;
    }
}

void __fastcall TMainFramePresenter::LayoutButton(TForm* AForm, TButton* Button)
{
    if (!Button) return;

    // ���ð�ť�Ĳ�������
    Button->Width = m_LayoutConfig.ButtonWidth;
    Button->Height = m_LayoutConfig.ButtonHeight;

    // ���ж�λ
    Button->Left = (AForm->ClientWidth - Button->Width) / 2;
    Button->Top = AForm->ClientHeight / 2;

    // ����ê����֧�ִ����С�仯
    Button->Anchors.Clear();
    Button->Anchors << akTop << akLeft;
}

void __fastcall TMainFramePresenter::LayoutLabel(TForm* AForm, TLabel* Label)
{
    if (!Label) return;

    // ���ñ�ǩ�Ĳ�������
    Label->Height = m_LayoutConfig.LabelHeight;
    Label->AutoSize = false;
    Label->Width = AForm->ClientWidth - (m_LayoutConfig.Margin * 2);

    // λ������
    Label->Left = m_LayoutConfig.Margin;
    Label->Top = m_LayoutConfig.Margin;

    // ���ж���
    Label->Alignment = taCenter;

    // ����ê��
    Label->Anchors.Clear();
    Label->Anchors << akTop << akLeft << akRight;
}

// ========== ��ʽ����ʵ�� ==========
void __fastcall TMainFramePresenter::ApplyLocalStyles(TForm* AForm)
{
    // Ӧ�ô�����ʽ
    AForm->Caption = "Modern Application";
    AForm->Color = m_StyleConfig.BackgroundColor;

    // Ӧ�ð�ť��ʽ
    if (AForm->FindComponent("Button1")) {
        TButton* Button1 = static_cast<TButton*>(AForm->FindComponent("Button1"));
        if (Button1) {
            Button1->Caption = m_StyleConfig.ButtonCaption;
            Button1->Font->Size = m_StyleConfig.FontSize;
            Button1->Font->Style = Button1->Font->Style << fsBold;
            Button1->Cursor = crHandPoint;

            // �ִ����İ�ť��ʽ
            Button1->StyleElements.Clear();
            Button1->StyleElements << seClient << seBorder;
        }
    }

    // Ӧ�ñ�ǩ��ʽ
    if (AForm->FindComponent("Label1")) {
        TLabel* Label1 = static_cast<TLabel*>(AForm->FindComponent("Label1"));
        if (Label1) {
            Label1->Caption = m_StyleConfig.LabelText;
            Label1->Font->Size = m_StyleConfig.FontSize + 2;
            Label1->Font->Color = clNavy;
            Label1->Font->Style = Label1->Font->Style << fsBold;

            // ͸������
            Label1->Transparent = true;
        }
    }
}

// ========== �������ʵ�� ==========
void __fastcall TMainFramePresenter::ApplyTheme(TForm* AForm)
{
    // Ӧ��VCL����
	//if (TStyleManager::IsValidStyle("Windows10 Dark")) {
        TStyleManager::SetStyle("Windows10 Dark");
    //}

    // ������ʽԪ��
    AForm->StyleElements.Clear();
    AForm->StyleElements << seFont << seClient << seBorder;
}

// ========== �¼����� ==========
void __fastcall TMainFramePresenter::OnButtonClick()
{
    ShowMessage("Modern Style: Layout first, then styling!");
}

// ========== �����ӿڷ��� ==========
void __fastcall TMainFramePresenter::SetLayoutMargin(int Margin)
{
    m_LayoutConfig.Margin = Margin;
}

void __fastcall TMainFramePresenter::SetControlSpacing(int Spacing)
{
    m_LayoutConfig.ControlSpacing = Spacing;
}

void __fastcall TMainFramePresenter::EnableResponsiveLayout(bool Enable)
{
    // ʵ����Ӧʽ���ֿ���
    // ���������������Ӧʽ���ֵ��߼�
}

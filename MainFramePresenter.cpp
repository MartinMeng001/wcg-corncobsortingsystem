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
    // 初始化布局配置
    m_LayoutConfig.Margin = 20;
    m_LayoutConfig.Padding = 10;
	m_LayoutConfig.ButtonHeight = 50;
	m_LayoutConfig.ButtonWidth = 230;
    m_LayoutConfig.LabelHeight = 50;
    m_LayoutConfig.ControlSpacing = 15;

    // 初始化样式配置
    m_StyleConfig.FontSize = 12;
    m_StyleConfig.ButtonCaption = "Modern Click";
	m_StyleConfig.LabelText = "玉米分拣系统1.0";
    m_StyleConfig.BackgroundColor = clWhite;
}

// ========== 布局管理实现 ==========
void __fastcall TMainFramePresenter::ApplyLayout(TForm* AForm)
{
    // 1. 计算控件位置
    CalculateControlPositions(AForm);

    // 2. 布局各个控件
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
    // 基于窗体大小计算控件位置
    // 这里可以实现响应式布局逻辑
    int ClientWidth = AForm->ClientWidth;
    int ClientHeight = AForm->ClientHeight;

    // 如果窗体太小，调整最小尺寸
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

    // 设置按钮的布局属性
    Button->Width = m_LayoutConfig.ButtonWidth;
    Button->Height = m_LayoutConfig.ButtonHeight;

    // 居中定位
    Button->Left = (AForm->ClientWidth - Button->Width) / 2;
    Button->Top = AForm->ClientHeight / 2;

    // 设置锚点以支持窗体大小变化
    Button->Anchors.Clear();
    Button->Anchors << akTop << akLeft;
}

void __fastcall TMainFramePresenter::LayoutLabel(TForm* AForm, TLabel* Label)
{
    if (!Label) return;

    // 设置标签的布局属性
    Label->Height = m_LayoutConfig.LabelHeight;
    Label->AutoSize = false;
    Label->Width = AForm->ClientWidth - (m_LayoutConfig.Margin * 2);

    // 位置设置
    Label->Left = m_LayoutConfig.Margin;
    Label->Top = m_LayoutConfig.Margin;

    // 居中对齐
    Label->Alignment = taCenter;

    // 设置锚点
    Label->Anchors.Clear();
    Label->Anchors << akTop << akLeft << akRight;
}

// ========== 样式管理实现 ==========
void __fastcall TMainFramePresenter::ApplyLocalStyles(TForm* AForm)
{
    // 应用窗体样式
    AForm->Caption = "Modern Application";
    AForm->Color = m_StyleConfig.BackgroundColor;

    // 应用按钮样式
    if (AForm->FindComponent("Button1")) {
        TButton* Button1 = static_cast<TButton*>(AForm->FindComponent("Button1"));
        if (Button1) {
            Button1->Caption = m_StyleConfig.ButtonCaption;
            Button1->Font->Size = m_StyleConfig.FontSize;
            Button1->Font->Style = Button1->Font->Style << fsBold;
            Button1->Cursor = crHandPoint;

            // 现代化的按钮样式
            Button1->StyleElements.Clear();
            Button1->StyleElements << seClient << seBorder;
        }
    }

    // 应用标签样式
    if (AForm->FindComponent("Label1")) {
        TLabel* Label1 = static_cast<TLabel*>(AForm->FindComponent("Label1"));
        if (Label1) {
            Label1->Caption = m_StyleConfig.LabelText;
            Label1->Font->Size = m_StyleConfig.FontSize + 2;
            Label1->Font->Color = clNavy;
            Label1->Font->Style = Label1->Font->Style << fsBold;

            // 透明背景
            Label1->Transparent = true;
        }
    }
}

// ========== 主题管理实现 ==========
void __fastcall TMainFramePresenter::ApplyTheme(TForm* AForm)
{
    // 应用VCL主题
	//if (TStyleManager::IsValidStyle("Windows10 Dark")) {
        TStyleManager::SetStyle("Windows10 Dark");
    //}

    // 启用样式元素
    AForm->StyleElements.Clear();
    AForm->StyleElements << seFont << seClient << seBorder;
}

// ========== 事件处理 ==========
void __fastcall TMainFramePresenter::OnButtonClick()
{
    ShowMessage("Modern Style: Layout first, then styling!");
}

// ========== 公共接口方法 ==========
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
    // 实现响应式布局开关
    // 可以在这里添加响应式布局的逻辑
}

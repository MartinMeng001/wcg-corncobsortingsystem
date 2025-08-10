object Form2: TForm2
  Left = 0
  Top = 0
  Caption = 'Form2'
  ClientHeight = 772
  ClientWidth = 1232
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnResize = FormResize
  TextHeight = 15
  object LabelIP: TLabel
    Left = 440
    Top = 232
    Width = 38
    Height = 15
    Caption = 'LabelIP'
  end
  object LabelPort: TLabel
    Left = 440
    Top = 272
    Width = 50
    Height = 15
    Caption = 'LabelPort'
  end
  object LabelConnectionStatus: TLabel
    Left = 432
    Top = 184
    Width = 122
    Height = 15
    Caption = 'LabelConnectionStatus'
  end
  object LabelReferenceStatus: TLabel
    Left = 440
    Top = 464
    Width = 112
    Height = 15
    Caption = 'LabelReferenceStatus'
  end
  object LabelDetectionStatus: TLabel
    Left = 592
    Top = 392
    Width = 111
    Height = 15
    Caption = 'LabelDetectionStatus'
  end
  object LabelSerialNumber: TLabel
    Left = 600
    Top = 400
    Width = 100
    Height = 15
    Caption = 'LabelSerialNumber'
  end
  object LabelErrorCode: TLabel
    Left = 608
    Top = 408
    Width = 81
    Height = 15
    Caption = 'LabelErrorCode'
  end
  object PanelTop: TPanel
    Left = 24
    Top = 24
    Width = 185
    Height = 41
    Caption = 'PanelTop'
    TabOrder = 0
  end
  object PanelCenter: TPanel
    Left = 24
    Top = 104
    Width = 185
    Height = 41
    Caption = 'PanelCenter'
    TabOrder = 1
  end
  object PanelBottom: TPanel
    Left = 24
    Top = 184
    Width = 185
    Height = 41
    Caption = 'PanelBottom'
    TabOrder = 2
  end
  object GroupBoxConnection: TGroupBox
    Left = 24
    Top = 256
    Width = 185
    Height = 105
    Caption = 'GroupBoxConnection'
    TabOrder = 3
  end
  object EditIP: TEdit
    Left = 424
    Top = 328
    Width = 121
    Height = 23
    TabOrder = 4
    Text = 'EditIP'
  end
  object EditPort: TEdit
    Left = 456
    Top = 400
    Width = 121
    Height = 23
    TabOrder = 5
    Text = 'EditPort'
  end
  object ButtonConnect: TButton
    Left = 24
    Top = 399
    Width = 75
    Height = 25
    Caption = 'ButtonConnect'
    TabOrder = 6
    OnClick = ButtonConnectClick
  end
  object ButtonDisconnect: TButton
    Left = 24
    Top = 456
    Width = 75
    Height = 25
    Caption = 'ButtonDisconnect'
    TabOrder = 7
    OnClick = ButtonDisconnectClick
  end
  object GroupBoxReference: TGroupBox
    Left = 24
    Top = 512
    Width = 185
    Height = 105
    Caption = 'GroupBoxReference'
    TabOrder = 8
  end
  object ButtonStartReference: TButton
    Left = 184
    Top = 399
    Width = 75
    Height = 25
    Caption = 'ButtonStartReference'
    TabOrder = 9
    OnClick = ButtonStartReferenceClick
  end
  object ButtonStopReference: TButton
    Left = 184
    Top = 456
    Width = 75
    Height = 25
    Caption = 'ButtonStopReference'
    TabOrder = 10
    OnClick = ButtonStopReferenceClick
  end
  object GroupBoxDetection: TGroupBox
    Left = 24
    Top = 659
    Width = 185
    Height = 105
    Caption = 'GroupBoxDetection'
    TabOrder = 11
  end
  object ButtonStartDetection: TButton
    Left = 448
    Top = 560
    Width = 75
    Height = 25
    Caption = 'ButtonStartDetection'
    TabOrder = 12
    OnClick = ButtonStartDetectionClick
  end
  object ButtonStopDetection: TButton
    Left = 584
    Top = 392
    Width = 75
    Height = 25
    Caption = 'ButtonStopDetection'
    TabOrder = 13
    OnClick = ButtonStopDetectionClick
  end
  object EditSerialNumber: TEdit
    Left = 568
    Top = 384
    Width = 121
    Height = 23
    TabOrder = 14
    Text = 'EditSerialNumber'
  end
  object GroupBoxResults: TGroupBox
    Left = 360
    Top = 40
    Width = 185
    Height = 105
    Caption = 'GroupBoxResults'
    TabOrder = 15
  end
  object GridResults: TStringGrid
    Left = 832
    Top = 152
    Width = 320
    Height = 120
    TabOrder = 16
  end
  object ButtonReadResults: TButton
    Left = 592
    Top = 400
    Width = 75
    Height = 25
    Caption = 'ButtonReadResults'
    TabOrder = 17
    OnClick = ButtonReadResultsClick
  end
  object ButtonClearResults: TButton
    Left = 600
    Top = 408
    Width = 75
    Height = 25
    Caption = 'ButtonClearResults'
    TabOrder = 18
    OnClick = ButtonClearResultsClick
  end
  object GroupBoxStatus: TGroupBox
    Left = 592
    Top = 40
    Width = 185
    Height = 105
    Caption = 'GroupBoxStatus'
    TabOrder = 19
  end
  object MemoLog: TMemo
    Left = 784
    Top = 584
    Width = 185
    Height = 89
    Lines.Strings = (
      'MemoLog')
    TabOrder = 20
  end
  object EditErrorCode: TEdit
    Left = 576
    Top = 392
    Width = 121
    Height = 23
    TabOrder = 21
    Text = 'EditErrorCode'
  end
  object TimerPolling: TTimer
    OnTimer = TimerPollingTimer
    Left = 608
    Top = 392
  end
end

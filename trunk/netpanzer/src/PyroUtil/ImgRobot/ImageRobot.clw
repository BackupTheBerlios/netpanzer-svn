; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CImageRobotDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "imagerobot.h"
LastPage=0

ClassCount=7
Class1=CColorPakDialog
Class2=CropInfoDialog
Class3=CImageRobotApp
Class4=CAboutDlg
Class5=CImageRobotDlg
Class6=PackFileDialog
Class7=RenameInfoDialog

ResourceCount=6
Resource1=IDD_CROPINFO_DIALOG
Resource2=IDD_ABOUTBOX
Resource3=IDD_IMAGEROBOT_DIALOG
Resource4=IDD_RENAME
Resource5=IDD_PAKFILE_DIALOG
Resource6=IDD_COLOR_PAK_DIALOG

[CLS:CColorPakDialog]
Type=0
BaseClass=CDialog
HeaderFile=ColorPakDialog.h
ImplementationFile=ColorPakDialog.cpp

[CLS:CropInfoDialog]
Type=0
BaseClass=CDialog
HeaderFile=cropinfodialog.h
ImplementationFile=cropinfodialog.cpp

[CLS:CImageRobotApp]
Type=0
BaseClass=CWinApp
HeaderFile=ImageRobot.h
ImplementationFile=ImageRobot.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=ImageRobotDlg.cpp
ImplementationFile=ImageRobotDlg.cpp

[CLS:CImageRobotDlg]
Type=0
BaseClass=CDialog
HeaderFile=ImageRobotDlg.h
ImplementationFile=ImageRobotDlg.cpp
LastObject=IDC_BUTTON_PALTOACT
Filter=D
VirtualFilter=dWC

[CLS:PackFileDialog]
Type=0
BaseClass=CDialog
HeaderFile=PackFileDialog.h
ImplementationFile=PackFileDialog.cpp

[CLS:RenameInfoDialog]
Type=0
BaseClass=CDialog
HeaderFile=RenameInfoDialog.h
ImplementationFile=RenameInfoDialog.cpp

[DLG:IDD_COLOR_PAK_DIALOG]
Type=1
Class=CColorPakDialog
ControlCount=18
Control1=IDC_ORIGINAL_BUTTON,button,1342242816
Control2=IDC_GRAY_BUTTON,button,1342242816
Control3=IDC_RED_BUTTON,button,1342242816
Control4=IDC_GREEN_BUTTON,button,1342242816
Control5=IDC_BLUE_BUTTON,button,1342242816
Control6=IDC_SPIN_GRAY,msctls_updown32,1342177312
Control7=IDC_SPIN_RED,msctls_updown32,1342177312
Control8=IDC_SPIN_GREEN,msctls_updown32,1342177312
Control9=IDC_SPIN_BLUE,msctls_updown32,1342177312
Control10=IDCANCEL,button,1342242816
Control11=IDC_STATIC,button,1342177287
Control12=IDC_GRADIENT,button,1476395019
Control13=IDC_PERCENT_GRAY,static,1342308864
Control14=IDC_PERCENT_RED,static,1342308864
Control15=IDC_PERCENT_GREEN,static,1342308864
Control16=IDC_PERCENT_BLUE,static,1342308864
Control17=IDC_STATIC,button,1342177287
Control18=IDOK,button,1342242817

[DLG:IDD_CROPINFO_DIALOG]
Type=1
Class=CropInfoDialog
ControlCount=16
Control1=IDOK,button,1342242816
Control2=IDCANCEL,button,1342242816
Control3=IDC_MINXEDIT,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_MAXXEDIT,edit,1350631552
Control6=IDC_STATIC,static,1342308352
Control7=IDC_MINYEDIT,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_MAXYEDIT,edit,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,button,1342177287
Control12=IDC_IMAGEXEDIT,edit,1350631552
Control13=IDC_IMAGEYEDIT,edit,1350631552
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,button,1342177287

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_IMAGEROBOT_DIALOG]
Type=1
Class=CImageRobotDlg
ControlCount=19
Control1=IDC_BUTTON_UNITPACKER,button,1342242817
Control2=IDC_BUTTON_SHADOWPACKER,button,1342242816
Control3=IDC_COLOR_PAK_BUTTON,button,1342242816
Control4=IDC_PAKFILE_BUTTON,button,1342242816
Control5=IDC_CROP,button,1342242816
Control6=IDC_RENAME_BUTTON,button,1342242816
Control7=IDC_FILES,button,1342242817
Control8=IDC_CLEARALL,button,1342242816
Control9=IDCANCEL,button,1342242816
Control10=IDC_SOURCELIST,listbox,1353777411
Control11=IDC_OUTPUTLIST,listbox,1353777411
Control12=IDC_STATIC,static,1342308864
Control13=IDC_OUTPUTDIR_EDIT,edit,1350631552
Control14=IDC_STATIC,button,1342177287
Control15=IDC_STATIC,button,1342177287
Control16=IDC_STATIC,button,1342177287
Control17=IDC_SRC_FILE_COUNT,static,1342308352
Control18=IDC_OUT_FILE_COUNT,static,1342308352
Control19=IDC_BUTTON_PALTOACT,button,1342242817

[DLG:IDD_PAKFILE_DIALOG]
Type=1
Class=PackFileDialog
ControlCount=4
Control1=IDOK,button,1342242816
Control2=IDCANCEL,button,1342242816
Control3=IDC_PAKFILE_EDIT,edit,1350631552
Control4=IDC_STATIC,button,1342177287

[DLG:IDD_RENAME]
Type=1
Class=RenameInfoDialog
ControlCount=4
Control1=IDOK,button,1342242816
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT1,edit,1350631552
Control4=IDC_STATIC,button,1342177287


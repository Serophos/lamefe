; CLW-Datei enthält Informationen für den MFC-Klassen-Assistenten

[General Info]
Version=1
LastClass=EncodingStatusDlg
LastTemplate=generic CWnd
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "lameFE.h"

ClassCount=20
Class1=CLameFEApp
Class2=CLameFEDlg
Class3=CAboutDlg

ResourceCount=33
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_LAMEFE_PAGE (Englisch (USA) - _ENGLISH)
Resource4=IDD_PLUGINPAGE (Englisch (USA))
Resource5=IDD_LAMEFE_DIALOG (Englisch (USA) - _ENGLISH)
Resource6=IDD_TAGEDITOR (Englisch (USA))
Resource7=IDD_PATH_SEL (Englisch (USA))
Class4=CTagEditor
Resource8=IDD_PRESET (Englisch (USA))
Class5=CPresetDlg
Resource9=IDD_CDDB_SETUP (Englisch (USA) - _ENGLISH)
Class6=CSettingsDlg
Resource10=IDD_MAIN_SETTINGS (Englisch (USA))
Class7=CPathDialog
Resource11=IDD_CDDB_SETUP (_GERMAN)
Class8=CID3TagEditor
Resource12=IDD_CDDB_SETTINGS (Englisch (USA))
Class9=EncodingStatusDlg
Resource13=IDD_ENCODER_PAGE (_GERMAN)
Class10=CCDDBSettingsDlg
Resource14=IDD_LAMEFE_DIALOG (_GERMAN)
Class11=CSettingsDialog
Resource15=IDD_CDDB_QUERY (Englisch (USA) - _ENGLISH)
Class12=CEncoderPage
Class13=CSettingsSheet
Class14=CEncoderSheet
Resource16=IDD_SETTINGS (Neutral)
Class15=CMainSettingsPage
Resource17=IDR_SYSTRAY_MENU (Englisch (USA) - _ENGLISH)
Class16=CCDROMPage
Resource18=IDD_CDRIP_PAGE (Englisch (USA) - _ENGLISH)
Class17=CCDDBPage
Resource19=IDD_CDDB_QUERY (_GERMAN)
Resource20=IDR_MAIN_MENU (_GERMAN)
Class18=CCDdbQueryDlg
Resource21=IDR_SYSTRAY_MENU (_GERMAN)
Resource22=IDD_ID3EDIT (Englisch (USA) - _ENGLISH)
Resource23=IDD_ENCODER_PAGE (Englisch (USA) - _ENGLISH)
Resource24=IDD_CDRIP_PAGE (_GERMAN)
Resource25=IDD_ID3EDIT (_GERMAN)
Resource26=IDD_STATUS_DIALOG (_GERMAN)
Resource27=IDD_STATUS_DIALOG (Englisch (USA) - _ENGLISH)
Resource28=IDD_LAMEFE_PAGE (_GERMAN)
Resource29=IDR_LIST_CONTEXT (Englisch (USA))
Resource30=IDR_ACCELERATOR (Englisch (USA) - _ENGLISH)
Resource31=IDR_ACCELERATOR (_GERMAN)
Resource32=IDD_ABOUTBOX (Neutral)
Class19=CPluginPage
Class20=CLameFESplash
Resource33=IDR_MAIN_MENU (Englisch (USA) - _ENGLISH)

[CLS:CLameFEApp]
Type=0
HeaderFile=lameFE.h
ImplementationFile=lameFE.cpp
Filter=N
LastObject=CLameFEApp

[CLS:CLameFEDlg]
Type=0
HeaderFile=lameFEDlg.h
ImplementationFile=lameFEDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=ID_HELP_HELPINDEX

[CLS:CAboutDlg]
Type=0
HeaderFile=lameFEDlg.h
ImplementationFile=lameFEDlg.cpp
Filter=D
BaseClass=CPropertyPage
VirtualFilter=idWC
LastObject=CCDDBPage

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg


[CLS:CTagEditor]
Type=0
HeaderFile=TagEditor.h
ImplementationFile=TagEditor.cpp
BaseClass=CDialog
Filter=D
LastObject=CTagEditor
VirtualFilter=dWC

[DLG:IDD_PRESET (Englisch (USA))]
Type=1
Class=CPresetDlg
ControlCount=16
Control1=IDC_STATIC,static,1342308352
Control2=IDC_PRESET,combobox,1344339970
Control3=IDC_STATIC,static,1342308864
Control4=IDC_SAMPLES,combobox,1344339971
Control5=IDC_STATIC,static,1342308864
Control6=IDC_BITRATE,combobox,1344339971
Control7=IDC_STATIC,static,1342308864
Control8=IDC_ENCODING,combobox,1344339971
Control9=IDC_ORIG,button,1342242819
Control10=IDC_COPY,button,1342242819
Control11=IDC_SAVE,button,1342242816
Control12=IDC_DELETE,button,1342242816
Control13=IDOK,button,1342242817
Control14=IDC_STATIC,static,1342177283
Control15=IDC_STATIC,button,1342177287
Control16=IDC_STATIC,button,1342177287

[CLS:CPresetDlg]
Type=0
HeaderFile=PresetDlg.h
ImplementationFile=PresetDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CPresetDlg
VirtualFilter=dWC

[CLS:CSettingsDlg]
Type=0
HeaderFile=SettingsDlg.h
ImplementationFile=SettingsDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CSettingsDlg
VirtualFilter=dWC

[DLG:IDD_TAGEDITOR (Englisch (USA))]
Type=1
Class=CTagEditor
ControlCount=19
Control1=IDC_STATIC,button,1342177287
Control2=IDC_FILES,listbox,1352728833
Control3=IDC_STATIC,static,1342308352
Control4=IDC_TITLE,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_INTERPRET,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_ALBUM,edit,1350631552
Control9=IDC_STATIC,static,1342308352
Control10=IDC_GENRE,combobox,1344348419
Control11=IDC_STATIC,static,1342308352
Control12=IDC_TRACK,edit,1350631552
Control13=IDC_STATIC,static,1342308352
Control14=IDC_YEAR,edit,1350631552
Control15=IDC_STATIC,static,1342308352
Control16=IDC_COMMENT,edit,1350631552
Control17=IDC_UPDATE,button,1342242817
Control18=IDOK,button,1342242816
Control19=IDC_STATIC,static,1342308865

[DLG:IDD_PATH_SEL (Englisch (USA))]
Type=1
Class=CPathDialog
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BROWSE,listbox,1352728835

[CLS:CPathDialog]
Type=0
HeaderFile=PathDialog.h
ImplementationFile=PathDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=CPathDialog
VirtualFilter=dWC

[CLS:CID3TagEditor]
Type=0
HeaderFile=ID3TagEditor.h
ImplementationFile=ID3TagEditor.cpp
BaseClass=CDialog
Filter=D
LastObject=CID3TagEditor
VirtualFilter=dWC

[CLS:EncodingStatusDlg]
Type=0
HeaderFile=EncodingStatusDlg.h
ImplementationFile=EncodingStatusDlg.cpp
BaseClass=CTrayDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_ELA_TIME

[DLG:IDD_CDDB_SETTINGS (Englisch (USA))]
Type=1
Class=CCDDBSettingsDlg
ControlCount=17
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342177283
Control4=IDC_STATIC,button,1342177287
Control5=IDC_ADD_SERVER,button,1342242816
Control6=IDC_REMOVE,button,1342242816
Control7=IDC_SERVER_LIST,listbox,1344340227
Control8=IDC_STATIC,button,1342177287
Control9=IDC_SERVER_PROMPT,edit,1350631552
Control10=IDC_EMAIL,edit,1350631552
Control11=IDC_STATIC,static,1342308864
Control12=IDC_STATIC,button,1342177287
Control13=IDC_ENABLE_PROXY,button,1342242819
Control14=IDC_PROXY_ADDRESS,edit,1350631568
Control15=IDC_PROX_PORT,edit,1350639744
Control16=IDC_STATIC,static,1342308866
Control17=IDC_STATIC,static,1342308866

[CLS:CCDDBSettingsDlg]
Type=0
HeaderFile=CDDBSettingsDlg.h
ImplementationFile=CDDBSettingsDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDOK

[DLG:IDD_MAIN_SETTINGS (Englisch (USA))]
Type=1
Class=CSettingsDialog
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_SETTINGS_TABS,SysTabControl32,1342177280
Control4=IDC_STATIC,static,1342177283

[CLS:CSettingsDialog]
Type=0
HeaderFile=SettingsDialog.h
ImplementationFile=SettingsDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=CSettingsDialog
VirtualFilter=dWC

[CLS:CEncoderPage]
Type=0
HeaderFile=EncoderPage.h
ImplementationFile=EncoderPage.cpp
BaseClass=CDialog
Filter=D
LastObject=CEncoderPage

[CLS:CSettingsSheet]
Type=0
HeaderFile=SettingsSheet.h
ImplementationFile=SettingsSheet.cpp
BaseClass=CPropertySheet
Filter=W
LastObject=CSettingsSheet

[CLS:CEncoderSheet]
Type=0
HeaderFile=EncoderSheet.h
ImplementationFile=EncoderSheet.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=IDC_MPEG
VirtualFilter=idWC

[CLS:CMainSettingsPage]
Type=0
HeaderFile=MainSettingsPage.h
ImplementationFile=MainSettingsPage.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=CMainSettingsPage
VirtualFilter=idWC

[CLS:CCDROMPage]
Type=0
HeaderFile=CDROMPage.h
ImplementationFile=CDROMPage.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=CCDROMPage
VirtualFilter=idWC

[CLS:CCDDBPage]
Type=0
HeaderFile=CDDBPage.h
ImplementationFile=CDDBPage.cpp
BaseClass=CPropertyPage
Filter=D
VirtualFilter=idWC
LastObject=IDC_REMOTE_SERVER

[CLS:CCDdbQueryDlg]
Type=0
HeaderFile=CDdbQueryDlg.h
ImplementationFile=CDdbQueryDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_PROTOCOLL
VirtualFilter=dWC

[MNU:IDR_LIST_CONTEXT (Englisch (USA))]
Type=1
Class=?
Command1=ID_POPUP_REMOVE
Command2=ID_POPUP_EDITID3TAG
CommandCount=2

[DLG:IDD_ABOUTBOX (Neutral)]
Type=1
Class=CAboutDlg
ControlCount=2
Control1=IDOK,button,1342373889
Control2=IDC_DISPLAY_STATIC,static,1350696960

[DLG:IDD_SETTINGS (Neutral)]
Type=1
Class=CSettingsSheet
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[DLG:IDD_STATUS_DIALOG (_GERMAN)]
Type=1
Class=EncodingStatusDlg
ControlCount=35
Control1=IDCANCEL,button,1342242817
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC,static,1342308352
Control4=IDC_IN,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_IN_SIZE,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_OUT,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EST_SIZE,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_FILE_MSG,static,1342308352
Control13=IDC_STATIC,static,1342308864
Control14=IDC_FILE_STATUS_BAR,msctls_progress32,1342177280
Control15=IDC_STATIC,static,1342308864
Control16=IDC_STATIC,static,1342308352
Control17=IDC_EST_TIME,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_ELA_TIME,static,1342308352
Control20=IDC_STATIC,static,1342308352
Control21=IDC_REM_TIME,static,1342308352
Control22=IDC_STATIC,static,1342308352
Control23=IDC_FILE_X_OF_Y,static,1342308352
Control24=IDC_LIST_MSG,static,1342308352
Control25=IDC_STATIC,static,1342308352
Control26=IDC_ERRORS,static,1342308352
Control27=IDC_STATIC,static,1342308352
Control28=IDC_LIST_STATUS_BAR,msctls_progress32,1342177284
Control29=IDC_STATIC,static,1342308352
Control30=IDC_STATIC,static,1342308352
Control31=IDC_JITTERSTATUS,msctls_progress32,1342177284
Control32=IDC_STATIC,static,1342308352
Control33=IDC_STATIC,static,1342308352
Control34=IDC_STATIC,static,1342308352
Control35=IDC_STATIC,static,1342308352

[DLG:IDD_CDRIP_PAGE (_GERMAN)]
Type=1
Class=CCDROMPage
ControlCount=18
Control1=IDC_STATIC,static,1342308864
Control2=IDC_CD_DRIVE,combobox,1344339971
Control3=IDC_STATIC,static,1342308864
Control4=IDC_CDROM_TYPE,combobox,1344339971
Control5=IDC_STATIC,static,1342308864
Control6=IDC_RIPPING_METHOD,combobox,1344339971
Control7=IDC_EJECT,button,1342242819
Control8=IDC_STATIC,static,1342308864
Control9=IDC_CD_SPEED,edit,1350639744
Control10=IDC_LOCK,button,1342242819
Control11=IDC_STATIC,static,1342308352
Control12=IDC_CD_SPIN_UP,edit,1350639744
Control13=IDC_SWAP,button,1342242819
Control14=IDC_NATIVE,button,1342242819
Control15=IDC_SELECT_ALL,button,1342242819
Control16=IDC_STATIC,button,1342177287
Control17=IDC_STATIC,button,1342177287
Control18=IDC_Version,static,1342308352

[MNU:IDR_MAIN_MENU (_GERMAN)]
Type=1
Class=CLameFEDlg
Command1=ID_ADD_FILE
Command2=ID_REMOVE_FILE
Command3=ID_EXIT
Command4=ID_CDDB_READFROMREMOTECDDBSERVER
Command5=ID_TOOLS_EDITID3TAG
Command6=ID_PREFERENCES_CDREADERSETTINGS
Command7=ID_PREFERENCES_PLUGINS
Command8=ID_PRESET
Command9=ICD_CDDB_SETTINGS
Command10=ID_PREFERENCES_TAGWRITERSETTINGS
Command11=ID_SETTINGS
Command12=ID_HELP
Command13=ID_HELP_HELPINDEX
Command14=ID_HELP_LICENSE
Command15=ID_ABOUT
CommandCount=15

[DLG:IDD_ENCODER_PAGE (Englisch (USA) - _ENGLISH)]
Type=1
Class=CEncoderSheet
ControlCount=18
Control1=IDC_STATIC,static,1342308864
Control2=IDC_BITSPERSAMPLE,combobox,1344339971
Control3=IDC_STATIC,static,1342308864
Control4=IDC_CHANNELS,combobox,1344339971
Control5=IDC_COPYRIGHT,button,1342242819
Control6=IDC_PRIVATE,button,1342242819
Control7=IDC_ORIGINAL,button,1342242819
Control8=IDC_CHECKSUM,button,1342242819
Control9=IDC_ID3_2,button,1342242819
Control10=IDC_RENAME,button,1342251011
Control11=IDC_FORMAT,edit,1350631552
Control12=IDC_ENCODER_VERSION,static,1342308352
Control13=IDC_STATIC,button,1342177287
Control14=IDC_STATIC,button,1342177287
Control15=IDC_STATIC,button,1342177287
Control16=IDC_STATIC,static,1476526592
Control17=IDC_MPEG,combobox,1478557699
Control18=IDC_STATIC,static,1342308353

[DLG:IDD_CDDB_QUERY (Englisch (USA) - _ENGLISH)]
Type=1
Class=CCDdbQueryDlg
ControlCount=5
Control1=IDC_PROTOCOLL,listbox,1352728833
Control2=IDC_START,button,1342242816
Control3=IDCANCEL,button,1476460544
Control4=IDC_STATIC,static,1342308353
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_CDDB_QUERY (_GERMAN)]
Type=1
Class=CCDdbQueryDlg
ControlCount=5
Control1=IDC_PROTOCOLL,listbox,1352728833
Control2=IDC_START,button,1342242816
Control3=IDCANCEL,button,1476460544
Control4=IDC_STATIC,static,1342308353
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_CDDB_SETUP (_GERMAN)]
Type=1
Class=CCDDBPage
ControlCount=26
Control1=IDC_STATIC,static,1342308864
Control2=IDC_REMOTE_SERVER,combobox,1344340227
Control3=IDC_UPDATE,button,1342242816
Control4=IDC_STATIC,static,1342308864
Control5=IDC_EMAIL,edit,1350631552
Control6=IDC_STATIC,static,1342308864
Control7=IDC_TIMEOUT,edit,1350631552
Control8=IDC_USE_PROXY,button,1342242819
Control9=IDC_PROXY,edit,1484849280
Control10=IDC_STATIC,static,1342308864
Control11=IDC_PROXY_PORT,edit,1484857472
Control12=IDC_AUTHENTICATION,button,1342242819
Control13=IDC_STATIC,static,1342308864
Control14=IDC_USERNAME,edit,1484849280
Control15=IDC_STATIC,static,1342308864
Control16=IDC_PASSWORD,edit,1484849312
Control17=IDC_STATIC,static,1342308864
Control18=IDC_STATIC,button,1342177287
Control19=IDC_LOCATION,edit,1350633600
Control20=IDC_STATIC,static,1342308864
Control21=IDC_ADDRESS,edit,1350633600
Control22=IDC_STATIC,static,1342308864
Control23=IDC_PATH,edit,1350633600
Control24=IDC_STATIC,static,1342308864
Control25=IDC_REMOTE_PORT,edit,1350641792
Control26=IDC_STATIC,button,1342177287

[DLG:IDD_CDDB_SETUP (Englisch (USA) - _ENGLISH)]
Type=1
Class=CCDDBPage
ControlCount=26
Control1=IDC_STATIC,static,1342308864
Control2=IDC_REMOTE_SERVER,combobox,1344340227
Control3=IDC_UPDATE,button,1342242816
Control4=IDC_STATIC,static,1342308864
Control5=IDC_EMAIL,edit,1350631552
Control6=IDC_STATIC,static,1342308864
Control7=IDC_TIMEOUT,edit,1350631552
Control8=IDC_USE_PROXY,button,1342242819
Control9=IDC_PROXY,edit,1484849280
Control10=IDC_STATIC,static,1342308864
Control11=IDC_PROXY_PORT,edit,1484857472
Control12=IDC_STATIC,static,1342308864
Control13=IDC_USERNAME,edit,1484849280
Control14=IDC_STATIC,static,1342308864
Control15=IDC_PASSWORD,edit,1484849312
Control16=IDC_STATIC,button,1342177287
Control17=IDC_LOCATION,edit,1350633600
Control18=IDC_STATIC,static,1342308864
Control19=IDC_STATIC,static,1342308864
Control20=IDC_ADDRESS,edit,1350633600
Control21=IDC_STATIC,static,1342308864
Control22=IDC_PATH,edit,1350633600
Control23=IDC_STATIC,static,1342308864
Control24=IDC_REMOTE_PORT,edit,1350641792
Control25=IDC_STATIC,button,1342177287
Control26=IDC_AUTHENTICATION,button,1342242819

[DLG:IDD_ENCODER_PAGE (_GERMAN)]
Type=1
Class=CEncoderSheet
ControlCount=18
Control1=IDC_STATIC,static,1342308864
Control2=IDC_BITSPERSAMPLE,combobox,1344339971
Control3=IDC_STATIC,static,1342308864
Control4=IDC_CHANNELS,combobox,1344339971
Control5=IDC_COPYRIGHT,button,1342242819
Control6=IDC_PRIVATE,button,1342242819
Control7=IDC_ORIGINAL,button,1342242819
Control8=IDC_CHECKSUM,button,1342242819
Control9=IDC_ID3_2,button,1342242819
Control10=IDC_RENAME,button,1342251011
Control11=IDC_FORMAT,edit,1350631552
Control12=IDC_ENCODER_VERSION,static,1342308352
Control13=IDC_STATIC,button,1342177287
Control14=IDC_STATIC,button,1342177287
Control15=IDC_STATIC,button,1342177287
Control16=IDC_STATIC,static,1476526592
Control17=IDC_MPEG,combobox,1478557699
Control18=IDC_STATIC,static,1342308353

[DLG:IDD_ID3EDIT (_GERMAN)]
Type=1
Class=CID3TagEditor
ControlCount=30
Control1=IDC_FILES,listbox,1353777409
Control2=IDC_SONG_A,button,1342242819
Control3=IDC_SONG,edit,1350631552
Control4=IDC_ARTIST_A,button,1342242819
Control5=IDC_ARTIST,edit,1350631552
Control6=IDC_ALBUM_A,button,1342242819
Control7=IDC_ALBUM,edit,1350631552
Control8=IDC_GENRE_A,button,1342242819
Control9=IDC_GENRE,combobox,1344340227
Control10=IDC_YEAR_A,button,1342242819
Control11=IDC_YEAR,edit,1350631552
Control12=IDC_TRACK_A,button,1342242819
Control13=IDC_TRACK,edit,1350631552
Control14=IDC_COMMENT_A,button,1342242819
Control15=IDC_COMMENT,edit,1350631552
Control16=IDC_ALBUM_MODE,button,1342242822
Control17=IDC_AM_ARTIST,button,1342242819
Control18=IDC_AM_ALBUM,button,1342242819
Control19=IDC_AM_GENRE,button,1342242819
Control20=IDC_AM_YEAR,button,1342242819
Control21=IDC_AM_COMMENT,button,1342242819
Control22=IDC_RENAME,button,1342242819
Control23=IDCANCEL,button,1342242816
Control24=IDOK,button,1342242817
Control25=IDC_ID3TAG,button,1342242822
Control26=IDC_STATIC,button,1342177287
Control27=IDC_STATUS,static,1342308353
Control28=IDC_FILE_NAME,button,1342177287
Control29=IDC_STATIC,static,1342308865
Control30=IDC_STATIC,static,1342177283

[DLG:IDD_LAMEFE_DIALOG (_GERMAN)]
Type=1
Class=CLameFEDlg
ControlCount=18
Control1=IDC_LIST,listbox,1352732033
Control2=IDC_ENCODE,button,1342251008
Control3=IDC_PRESETS,button,1342251008
Control4=IDC_CDDB,button,1342242880
Control5=IDC_ID3_TAG,button,1342251008
Control6=IDC_CD_MP3,button,1342255107
Control7=IDC_CD_WAV,button,1342255107
Control8=IDC_WAV_MP3,button,1342255107
Control9=IDC_REFRESH,button,1342242880
Control10=IDC_PLAY,button,1342242880
Control11=IDC_PAUSE,button,1342242880
Control12=IDC_STOP,button,1342242880
Control13=IDC_PREV,button,1342242880
Control14=IDC_NEXT,button,1342242880
Control15=IDC_EJECT_CD,button,1342242880
Control16=IDC_LIST_TITLE,button,1342177287
Control17=IDC_STATIC,static,1342177283
Control18=IDC_STATIC,button,1342177287

[DLG:IDD_LAMEFE_PAGE (_GERMAN)]
Type=1
Class=CMainSettingsPage
ControlCount=16
Control1=IDC_OUTPUT,button,1342242816
Control2=IDC_PLUGINS,button,1476460544
Control3=IDC_PLAYER,button,1342242816
Control4=IDC_SILENT,button,1342242819
Control5=IDC_FINISH_DLG,button,1342242819
Control6=IDC_BEEP,button,1342242819
Control7=IDC_PLAY_FILES,button,1342242819
Control8=IDC_M3U,button,1342242819
Control9=IDC_EXIT_FINISHED,button,1342242819
Control10=IDC_SHUTDOWN,button,1342242819
Control11=IDC_STATIC,button,1342177287
Control12=IDC_STATIC,button,1342177287
Control13=IDC_STATIC,button,1342177287
Control14=IDC_PLUGINPATH,static,1476526592
Control15=IDC_OUTPUT_PATH,static,1342308864
Control16=IDC_PLAYER_PATH,static,1342308864

[ACL:IDR_ACCELERATOR (Englisch (USA) - _ENGLISH)]
Type=1
Class=CLameFEDlg
Command1=ID_ADD
Command2=ID_REMOVE
Command3=ID_HELP
Command4=ID_EDIT_ID3
Command5=ID_SETTINGS
CommandCount=5

[ACL:IDR_ACCELERATOR (_GERMAN)]
Type=1
Class=CLameFEDlg
Command1=ID_ADD
Command2=ID_REMOVE
Command3=ID_HELP
Command4=ID_EDIT_ID3
Command5=ID_SETTINGS
CommandCount=5

[DLG:IDD_LAMEFE_DIALOG (Englisch (USA) - _ENGLISH)]
Type=1
Class=CLameFEDlg
ControlCount=18
Control1=IDC_LIST,listbox,1352731009
Control2=IDC_ENCODE,button,1342251008
Control3=IDC_PRESETS,button,1342251008
Control4=IDC_CDDB,button,1342242880
Control5=IDC_ID3_TAG,button,1342251008
Control6=IDC_CD_MP3,button,1342246979
Control7=IDC_CD_WAV,button,1342246979
Control8=IDC_WAV_MP3,button,1342255171
Control9=IDC_REFRESH,button,1342242816
Control10=IDC_PLAY,button,1342242880
Control11=IDC_PAUSE,button,1342242880
Control12=IDC_STOP,button,1342242880
Control13=IDC_PREV,button,1342242880
Control14=IDC_NEXT,button,1342242880
Control15=IDC_EJECT_CD,button,1342242880
Control16=IDC_LIST_TITLE,button,1342177287
Control17=IDC_STATIC,static,1342177283
Control18=IDC_STATIC,button,1342177287

[DLG:IDD_STATUS_DIALOG (Englisch (USA) - _ENGLISH)]
Type=1
Class=EncodingStatusDlg
ControlCount=36
Control1=IDCANCEL,button,1342242817
Control2=IDC_STATIC,button,1342177287
Control3=IDC_LIST_STATUS_BAR,msctls_progress32,1342177284
Control4=IDC_IN,static,1342308352
Control5=IDC_OUT,static,1342308352
Control6=IDC_IN_SIZE,static,1342308352
Control7=IDC_EST_SIZE,static,1342308352
Control8=IDC_STATIC,static,1342308864
Control9=IDC_STATIC,static,1342308864
Control10=IDC_FILE_STATUS_BAR,msctls_progress32,1342177280
Control11=IDC_FILE_MSG,static,1342308352
Control12=IDC_FILE_X_OF_Y,static,1342308352
Control13=IDC_ERRORS,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_LIST_MSG,static,1342308352
Control16=IDC_EST_TIME,static,1342308352
Control17=IDC_ELA_TIME,static,1342308352
Control18=IDC_REM_TIME,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,static,1342308352
Control23=IDC_STATIC,static,1342308352
Control24=IDC_STATIC,static,1342308352
Control25=IDC_STATIC,static,1342308352
Control26=IDC_STATIC,static,1342308352
Control27=IDC_STATIC,static,1342308352
Control28=IDC_STATIC,static,1342308352
Control29=IDC_STATIC,static,1342308352
Control30=IDC_STATIC,static,1342308352
Control31=IDC_LIST_STATUS_BAR2,msctls_progress32,1342177284
Control32=IDC_JITTERSTATUS,msctls_progress32,1342177284
Control33=IDC_STATIC,static,1342308352
Control34=IDC_STATIC,static,1342308352
Control35=IDC_STATIC,static,1342308352
Control36=IDC_STATIC,static,1342308352

[DLG:IDD_LAMEFE_PAGE (Englisch (USA) - _ENGLISH)]
Type=1
Class=CMainSettingsPage
ControlCount=16
Control1=IDC_OUTPUT,button,1342242816
Control2=IDC_PLUGINS,button,1476460544
Control3=IDC_PLAYER,button,1342242816
Control4=IDC_SILENT,button,1342242819
Control5=IDC_FINISH_DLG,button,1342242819
Control6=IDC_BEEP,button,1342242819
Control7=IDC_PLAY_FILES,button,1342242819
Control8=IDC_M3U,button,1342242819
Control9=IDC_EXIT_FINISHED,button,1342242819
Control10=IDC_SHUTDOWN,button,1342242819
Control11=IDC_OUTPUT_PATH,static,1342308864
Control12=IDC_PLAYER_PATH,static,1342308864
Control13=IDC_STATIC,button,1342177287
Control14=IDC_STATIC,button,1342177287
Control15=IDC_STATIC,button,1342177287
Control16=IDC_PLUGINPATH,static,1476526592

[DLG:IDD_CDRIP_PAGE (Englisch (USA) - _ENGLISH)]
Type=1
Class=CCDROMPage
ControlCount=18
Control1=IDC_STATIC,static,1342308864
Control2=IDC_CD_DRIVE,combobox,1344339971
Control3=IDC_STATIC,static,1342308864
Control4=IDC_CDROM_TYPE,combobox,1344339971
Control5=IDC_STATIC,static,1342308864
Control6=IDC_RIPPING_METHOD,combobox,1344339971
Control7=IDC_EJECT,button,1342242819
Control8=IDC_STATIC,static,1342308864
Control9=IDC_CD_SPEED,edit,1350639744
Control10=IDC_LOCK,button,1342242819
Control11=IDC_STATIC,static,1342308352
Control12=IDC_CD_SPIN_UP,edit,1350639744
Control13=IDC_SWAP,button,1342242819
Control14=IDC_NATIVE,button,1342242819
Control15=IDC_SELECT_ALL,button,1342242819
Control16=IDC_STATIC,button,1342177287
Control17=IDC_STATIC,button,1342177287
Control18=IDC_Version,static,1342308352

[DLG:IDD_ID3EDIT (Englisch (USA) - _ENGLISH)]
Type=1
Class=CID3TagEditor
ControlCount=30
Control1=IDC_FILES,listbox,1353777409
Control2=IDC_SONG_A,button,1342242819
Control3=IDC_SONG,edit,1350631552
Control4=IDC_ARTIST_A,button,1342242819
Control5=IDC_ARTIST,edit,1350631552
Control6=IDC_ALBUM_A,button,1342242819
Control7=IDC_ALBUM,edit,1350631552
Control8=IDC_GENRE_A,button,1342242819
Control9=IDC_GENRE,combobox,1344340227
Control10=IDC_YEAR_A,button,1342242819
Control11=IDC_YEAR,edit,1350631552
Control12=IDC_TRACK_A,button,1342242819
Control13=IDC_TRACK,edit,1350631552
Control14=IDC_COMMENT_A,button,1342242819
Control15=IDC_COMMENT,edit,1350631552
Control16=IDC_ALBUM_MODE,button,1342242822
Control17=IDC_AM_ARTIST,button,1342242819
Control18=IDC_AM_ALBUM,button,1342242819
Control19=IDC_AM_GENRE,button,1342242819
Control20=IDC_AM_YEAR,button,1342242819
Control21=IDC_RENAME,button,1342242819
Control22=IDCANCEL,button,1342242816
Control23=IDOK,button,1342242817
Control24=IDC_STATIC,button,1342177287
Control25=IDC_AM_COMMENT,button,1342242819
Control26=IDC_STATUS,static,1342308353
Control27=IDC_FILE_NAME,button,1342177287
Control28=IDC_STATIC,static,1342308865
Control29=IDC_STATIC,static,1342177283
Control30=IDC_ID3TAG,button,1342242822

[MNU:IDR_SYSTRAY_MENU (_GERMAN)]
Type=1
Class=?
Command1=IDC_ST_RESTORE
Command2=IDC_ST_EXIT
CommandCount=2

[MNU:IDR_MAIN_MENU (Englisch (USA) - _ENGLISH)]
Type=1
Class=CLameFEDlg
Command1=ID_ADD_FILE
Command2=ID_REMOVE_FILE
Command3=ID_EXIT
Command4=ID_CDDB_READFROMREMOTECDDBSERVER
Command5=ID_TOOLS_EDITID3TAG
Command6=ID_PREFERENCES_CDREADERSETTINGS
Command7=ID_PREFERENCES_PLUGINS
Command8=ID_PRESET
Command9=ICD_CDDB_SETTINGS
Command10=ID_PREFERENCES_TAGWRITERSETTINGS
Command11=ID_SETTINGS
Command12=ID_HELP
Command13=ID_HELP_HELPINDEX
Command14=ID_HELP_LICENSE
Command15=ID_ABOUT
CommandCount=15

[MNU:IDR_SYSTRAY_MENU (Englisch (USA) - _ENGLISH)]
Type=1
Class=EncodingStatusDlg
Command1=IDC_ST_RESTORE
Command2=IDC_ST_EXIT
CommandCount=2

[DLG:IDD_PLUGINPAGE (Englisch (USA))]
Type=1
Class=CPluginPage
ControlCount=7
Control1=IDC_STATIC,button,1476395015
Control2=IDC_INSTALLED_PLUGINS,listbox,1486946563
Control3=IDC_CONFIGURE,button,1476460544
Control4=IDC_INSTALL,button,1476460544
Control5=IDC_UNINSTALL,button,1476460544
Control6=IDC_STATIC,button,1476395015
Control7=IDC_STATIC,static,1476526080

[CLS:CPluginPage]
Type=0
HeaderFile=PluginPage.h
ImplementationFile=PluginPage.cpp
BaseClass=CDialog
Filter=D
LastObject=CPluginPage

[CLS:CLameFESplash]
Type=0
HeaderFile=LameFESplash.h
ImplementationFile=LameFESplash.cpp
BaseClass=generic CWnd
Filter=W
LastObject=ID_HELP_LICENSE


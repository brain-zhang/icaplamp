; �ű��� Inno Setup �ű��� ���ɣ�
; �йش��� Inno Setup �ű��ļ�����ϸ��������İ����ĵ���

#define MyAppName "��Ϣ��������߼��ϵͳ"
#define MyAppVersion "1.0"
#define MyAppPublisher "�Ϻ����ǿƼ����޹�˾"
#define MyAppURL "memoryboxes@gmail.com"
#define MyAppExeName "IcapLamp.exe"

[Setup]
; ע: AppId��ֵΪ������ʶ��Ӧ�ó���
; ��ҪΪ������װ����ʹ����ͬ��AppIdֵ��
; (�����µ�GUID����� ����|��IDE������GUID��)
AppId={{C3160695-507C-4DB6-A290-C484B01E16E3}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
InfoBeforeFile=F:\work\��Ϣ�����\software\IcapLamp\ini\info.txt
InfoAfterFile=F:\work\��Ϣ�����\software\IcapLamp\ini\readme.txt
OutputDir=F:\work\��Ϣ�����\software\IcapLamp\setup
OutputBaseFilename=setup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "F:\work\��Ϣ�����\software\IcapLamp\Release\IcapLamp.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\work\��Ϣ�����\software\IcapLamp\Release\IcapLamp.db"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\work\��Ϣ�����\software\IcapLamp\Release\IcapLamp.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\work\��Ϣ�����\software\IcapLamp\Release\libeay32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\work\��Ϣ�����\software\IcapLamp\Release\ssleay32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\work\��Ϣ�����\software\IcapLamp\Release\zlib1.dll"; DestDir: "{app}"; Flags: ignoreversion
; ע��: ��Ҫ���κι���ϵͳ�ļ���ʹ�á�Flags: ignoreversion��

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent


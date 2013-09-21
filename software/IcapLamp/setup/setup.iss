; 脚本由 Inno Setup 脚本向导 生成！
; 有关创建 Inno Setup 脚本文件的详细资料请查阅帮助文档！

#define MyAppName "信息化矿灯在线监控系统"
#define MyAppVersion "1.0"
#define MyAppPublisher "上海尘星科技有限公司"
#define MyAppURL "memoryboxes@gmail.com"
#define MyAppExeName "IcapLamp.exe"

[Setup]
; 注: AppId的值为单独标识该应用程序。
; 不要为其他安装程序使用相同的AppId值。
; (生成新的GUID，点击 工具|在IDE中生成GUID。)
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
InfoBeforeFile=F:\work\信息化矿灯\software\IcapLamp\ini\info.txt
InfoAfterFile=F:\work\信息化矿灯\software\IcapLamp\ini\readme.txt
OutputDir=F:\work\信息化矿灯\software\IcapLamp\setup
OutputBaseFilename=setup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "F:\work\信息化矿灯\software\IcapLamp\Release\IcapLamp.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\work\信息化矿灯\software\IcapLamp\Release\IcapLamp.db"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\work\信息化矿灯\software\IcapLamp\Release\IcapLamp.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\work\信息化矿灯\software\IcapLamp\Release\libeay32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\work\信息化矿灯\software\IcapLamp\Release\ssleay32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\work\信息化矿灯\software\IcapLamp\Release\zlib1.dll"; DestDir: "{app}"; Flags: ignoreversion
; 注意: 不要在任何共享系统文件上使用“Flags: ignoreversion”

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent


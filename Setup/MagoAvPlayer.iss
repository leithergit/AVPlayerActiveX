; 脚本由 Inno Setup 脚本向导 生成！
; 有关创建 Inno Setup 脚本文件的详细资料请查阅帮助文档！

#define MyAppName "迈高视频播放控件"
#define MyAppVersion "1.0.2.7"
#define PackageName "MagoAvPlayer.ver1.0.2.7"
#define MyAppPublisher "上海迈高网络技术有限公司"
#define MyAppURL "http://www.shmgwl.com/"

#define MyAppName_EN "Mago AVPlayer Control"
#define MyAppPublisher_EN "Shanghai Maigao Network Technology Co..Ltd"


[Setup]
; 注: AppId的值为单独标识该应用程序。
; 不要为其他安装程序使用相同的AppId值。
; (生成新的GUID，点击 工具|在IDE中生成GUID。)
AppId={{8B1264B2-FB4D-44F6-886B-C3671B2E173D}
AppName={cm:MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={cm:MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\MagoAVPlayer
DisableDirPage=yes
DefaultGroupName={cm:MyAppName}
OutputDir=D:\Git\AVPlayer.zpmc\AvPlayer.V1.0.0.24\Setup
OutputBaseFilename={#PackageName}
SetupIconFile=D:\Git\AVPlayer.zpmc\AvPlayer.V1.0.0.24\Setup\Mago.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Default.isl"
Name: "english"; MessagesFile: "compiler:Languages\English.isl"

[Messages]
english.BeveledLabel=englishlish
chinesesimp.BeveledLabel=Chineses

[CustomMessages]
chinesesimp.MyAppName={#MyAppName} 
chinesesimp.Type=zh_CN
chinesesimp.Title="迈高视频播放控件"
chinesesimp.MyAppPublisher = {#MyAppPublisher} 

english.MyAppName={#MyAppName_EN}
english.Type=en_US
english.Title="Mago AvPlayer Control"
english.MyAppPublisher = {#MyAppPublisher_EN} 


[Files]

Source: "D:\Git\AVPlayer.zpmc\AvPlayer.V1.0.0.24\Setup\AvPlayerActiveX\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\Git\AVPlayer.zpmc\AvPlayer.V1.0.0.24\Setup\vc2008redist_x86.exe"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\Git\AVPlayer.zpmc\AvPlayer.V1.0.0.24\Setup\vc2013redist_x86.exe"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

; 注意: 不要在任何共享系统文件上使用“Flags: ignoreversion”

[Icons]
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"

[Run]
Filename: "{app}\vc2008redist_x86.exe";Parameters:"/q";
Filename: "{app}\vc2013redist_x86.exe";Parameters:"/q";
Filename: "regsvr32.exe";Parameters:"/s ""{app}\avplayer.ocx""";
;Filename: "{sys}\netsh.exe";Parameters:"advfirewall firewall add rule name=""MediaContainer"" dir=out  action=allow program=""{app}\MediaContainer.bin"" enable=yes"
                                    
[UninstallRun]
Filename: "regsvr32.exe"; Parameters:"/u {app}\avplayer.ocx"

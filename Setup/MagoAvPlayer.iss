; �ű��� Inno Setup �ű��� ���ɣ�
; �йش��� Inno Setup �ű��ļ�����ϸ��������İ����ĵ���

#define MyAppName "������Ƶ���ſؼ�"
#define MyAppVersion "1.0.2.7"
#define PackageName "MagoAvPlayer.ver1.0.2.7"
#define MyAppPublisher "�Ϻ��������缼�����޹�˾"
#define MyAppURL "http://www.shmgwl.com/"

#define MyAppName_EN "Mago AVPlayer Control"
#define MyAppPublisher_EN "Shanghai Maigao Network Technology Co..Ltd"


[Setup]
; ע: AppId��ֵΪ������ʶ��Ӧ�ó���
; ��ҪΪ������װ����ʹ����ͬ��AppIdֵ��
; (�����µ�GUID����� ����|��IDE������GUID��)
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
chinesesimp.Title="������Ƶ���ſؼ�"
chinesesimp.MyAppPublisher = {#MyAppPublisher} 

english.MyAppName={#MyAppName_EN}
english.Type=en_US
english.Title="Mago AvPlayer Control"
english.MyAppPublisher = {#MyAppPublisher_EN} 


[Files]

Source: "D:\Git\AVPlayer.zpmc\AvPlayer.V1.0.0.24\Setup\AvPlayerActiveX\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\Git\AVPlayer.zpmc\AvPlayer.V1.0.0.24\Setup\vc2008redist_x86.exe"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\Git\AVPlayer.zpmc\AvPlayer.V1.0.0.24\Setup\vc2013redist_x86.exe"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

; ע��: ��Ҫ���κι���ϵͳ�ļ���ʹ�á�Flags: ignoreversion��

[Icons]
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"

[Run]
Filename: "{app}\vc2008redist_x86.exe";Parameters:"/q";
Filename: "{app}\vc2013redist_x86.exe";Parameters:"/q";
Filename: "regsvr32.exe";Parameters:"/s ""{app}\avplayer.ocx""";
;Filename: "{sys}\netsh.exe";Parameters:"advfirewall firewall add rule name=""MediaContainer"" dir=out  action=allow program=""{app}\MediaContainer.bin"" enable=yes"
                                    
[UninstallRun]
Filename: "regsvr32.exe"; Parameters:"/u {app}\avplayer.ocx"

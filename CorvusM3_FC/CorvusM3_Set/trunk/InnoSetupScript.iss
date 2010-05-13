; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{7E95C347-5CEB-4A34-9A9D-79A10E76D2DD}
AppName=CorvusM3 Set
AppVerName=CorvusM3 Set v0.1
AppPublisher=Thorsten Raab
AppPublisherURL=http://corvusm3.blogspot.com/
AppSupportURL=http://corvusm3.blogspot.com/
AppUpdatesURL=http://corvusm3.blogspot.com/
DefaultDirName={pf}\CorvusM3 Set
DefaultGroupName=CorvusM3
AllowNoIcons=yes
LicenseFile=D:\Data\Dokumente\Modellbau\CorvusM3\GoogleCode\CorvusM3_FC\CorvusM3_Set\trunk\bin\Release\License.txt
InfoBeforeFile=D:\Data\Dokumente\Modellbau\CorvusM3\GoogleCode\CorvusM3_FC\CorvusM3_Set\trunk\bin\Release\InstallInfo.txt
InfoAfterFile=D:\Data\Dokumente\Modellbau\CorvusM3\GoogleCode\CorvusM3_FC\CorvusM3_Set\trunk\bin\Release\ChangeLog.txt
OutputDir=D:\Data\Dokumente\Modellbau\CorvusM3\CorvusM3 Setup
OutputBaseFilename=setup CorvusM3 Set
SetupIconFile=D:\Data\Dokumente\Modellbau\CorvusM3\GoogleCode\CorvusM3_FC\CorvusM3_Set\trunk\cm3.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "german"; MessagesFile: "compiler:Languages\German.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "D:\Data\Dokumente\Modellbau\CorvusM3\GoogleCode\CorvusM3_FC\CorvusM3_Set\trunk\bin\Release\CorvusM3Set.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\Data\Dokumente\Modellbau\CorvusM3\GoogleCode\CorvusM3_FC\CorvusM3_Set\trunk\bin\Release\CorvusM3Set.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\Data\Dokumente\Modellbau\CorvusM3\GoogleCode\CorvusM3_FC\CorvusM3_Set\trunk\bin\Release\CorvusM3Set.exe.config"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\Data\Dokumente\Modellbau\CorvusM3\GoogleCode\CorvusM3_FC\CorvusM3_Set\trunk\bin\Release\CorvusM3Set.pdb"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\Data\Dokumente\Modellbau\CorvusM3\GoogleCode\CorvusM3_FC\CorvusM3_Set\trunk\bin\Release\CorvusM3Set.vshost.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\Data\Dokumente\Modellbau\CorvusM3\GoogleCode\CorvusM3_FC\CorvusM3_Set\trunk\bin\Release\CorvusM3Set.vshost.exe.config"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\Data\Dokumente\Modellbau\CorvusM3\GoogleCode\CorvusM3_FC\CorvusM3_Set\trunk\bin\Release\CorvusM3Set.vshost.exe.manifest"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\Data\Dokumente\Modellbau\CorvusM3\GoogleCode\CorvusM3_FC\CorvusM3_Set\trunk\bin\Release\ZedGraph.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\Data\Dokumente\Modellbau\CorvusM3\GoogleCode\CorvusM3_FC\CorvusM3_Set\trunk\bin\Release\Uploader\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\CorvusM3 Set"; Filename: "{app}\CorvusM3Set.exe"
Name: "{group}\{cm:ProgramOnTheWeb,CorvusM3 Set}"; Filename: "http://corvusm3.blogspot.com/"
Name: "{group}\{cm:UninstallProgram,CorvusM3 Set}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\CorvusM3 Set"; Filename: "{app}\CorvusM3Set.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\CorvusM3 Set"; Filename: "{app}\CorvusM3Set.exe"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\CorvusM3Set.exe"; Description: "{cm:LaunchProgram,CorvusM3 Set}"; Flags: nowait postinstall skipifsilent

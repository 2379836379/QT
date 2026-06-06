[Setup]
AppName=oj-client
AppVersion=0.1.0
DefaultDirName={autopf}\oj-client
DisableDirPage=no
UsePreviousAppDir=no
CloseApplications=force
CloseApplicationsFilter=oj.exe
RestartApplications=no
DefaultGroupName=oj-client
OutputDir=installer-out
OutputBaseFilename=oj-client-setup
Compression=lzma
SolidCompression=yes
WizardStyle=modern
ArchitecturesInstallIn64BitMode=x64compatible
SetupIconFile=E:\csqt\oj-client\images\icon-page-1.ico
UninstallDisplayIcon={app}\images\icon-page-1.ico

[Tasks]
Name: "desktopicon"; Description: "Create a desktop shortcut"; GroupDescription: "Additional icons:"

[Files]
Source: "E:\csqt\oj-client\build\oj.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\csqt\oj-client\build\*.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\csqt\oj-client\build\platforms\*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "E:\csqt\oj-client\build\iconengines\*"; DestDir: "{app}\iconengines"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "E:\csqt\oj-client\build\imageformats\*"; DestDir: "{app}\imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "E:\csqt\oj-client\build\styles\*"; DestDir: "{app}\styles"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "E:\csqt\oj-client\build\sqldrivers\*"; DestDir: "{app}\sqldrivers"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "E:\csqt\oj-client\build\tls\*"; DestDir: "{app}\tls"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "E:\csqt\oj-client\build\networkinformation\*"; DestDir: "{app}\networkinformation"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "E:\csqt\oj-client\build\multimedia\*"; DestDir: "{app}\multimedia"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "E:\csqt\oj-client\build\generic\*"; DestDir: "{app}\generic"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "E:\csqt\oj-client\build\qtwebview\*"; DestDir: "{app}\qtwebview"; Flags: ignoreversion recursesubdirs createallsubdirs skipifsourcedoesntexist
Source: "E:\csqt\oj-client\build\translations\*"; DestDir: "{app}\translations"; Flags: ignoreversion recursesubdirs createallsubdirs skipifsourcedoesntexist
Source: "E:\csqt\oj-client\images\light_mode\*"; DestDir: "{app}\images\light_mode"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "E:\csqt\oj-client\images\dark_mode\*"; DestDir: "{app}\images\dark_mode"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "E:\csqt\oj-client\images\icon-page-1.ico"; DestDir: "{app}\images"; Flags: ignoreversion
Source: "E:\csqt\oj-client\images\icon.svg"; DestDir: "{app}\images"; Flags: ignoreversion skipifsourcedoesntexist
Source: "E:\csqt\oj-client\vedio\哈基米起床 - MyRingtone.mp3"; DestDir: "{app}\vedio"; Flags: ignoreversion

[Icons]
Name: "{group}\oj-client"; Filename: "{app}\oj.exe"; IconFilename: "{app}\images\icon-page-1.ico"
Name: "{autodesktop}\oj-client"; Filename: "{app}\oj.exe"; IconFilename: "{app}\images\icon-page-1.ico"; Tasks: desktopicon

[Run]
Filename: "{app}\oj.exe"; Description: "Launch oj-client"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: filesandordirs; Name: "{app}\cache"
Type: filesandordirs; Name: "{app}\data"
Type: filesandordirs; Name: "{app}\images"
Type: filesandordirs; Name: "{app}\platforms"
Type: filesandordirs; Name: "{app}\iconengines"
Type: filesandordirs; Name: "{app}\imageformats"
Type: filesandordirs; Name: "{app}\styles"
Type: filesandordirs; Name: "{app}\sqldrivers"
Type: filesandordirs; Name: "{app}\tls"
Type: filesandordirs; Name: "{app}\networkinformation"
Type: filesandordirs; Name: "{app}\multimedia"
Type: filesandordirs; Name: "{app}\generic"
Type: filesandordirs; Name: "{app}\qtwebview"
Type: filesandordirs; Name: "{app}\translations"
Type: filesandordirs; Name: "{app}\vedio"
Type: files; Name: "{app}\*.dll"
Type: files; Name: "{app}\*.exe"
Type: files; Name: "{app}\*.toml"

[Code]
procedure CurStepChanged(CurStep: TSetupStep);
var
  AppStatePath: string;
  RingPath: string;
  Content: string;
begin
  if CurStep <> ssPostInstall then
    exit;

  AppStatePath := ExpandConstant('{app}\appstate.toml');
  RingPath := ExpandConstant('{app}\vedio\哈基米起床 - MyRingtone.mp3');
  Content := 'ring_path = "' + RingPath + '"' + #13#10;

  SaveStringToFile(AppStatePath, Content, False);
end;

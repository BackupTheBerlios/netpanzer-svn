; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
AppName=netPanzer
AppVerName=netpanzer 0.2.0 rc3
AppPublisher=netPanzer developers
AppPublisherURL=http://netpanzer.berlios.de
AppSupportURL=http://netpanzer.berlios.de
AppUpdatesURL=http://netpanzer.berlios.de
DefaultDirName={pf}\netPanzer
DefaultGroupName=netPanzer
; LicenseFile=D:\netpanzer\Copying

[Tasks]
; NOTE: The following entry contains English phrases ("Create a desktop icon" and "Additional icons"). You are free to translate them into another language if required.
Name: "desktopicon"; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"; Flags: unchecked

[Files]
Source: "D:\netpanzer\netpanzer.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\netpanzer\support\icon\netpanzer.ico"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\netpanzer\msvcr70.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\netpanzer\libphysfs-1-0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\netpanzer\Relnotes"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\netpanzer\SDL.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\netpanzer\SDL_image.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\netpanzer\SDL_mixer.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\netpanzer\Readme"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\netpanzer\Copying"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\netpanzer\docs\*.txt"; DestDir: "{app}\doc\"; Flags: ignoreversion
Source: "D:\netpanzer\docs\*.html"; DestDir: "{app}\doc\"; Flags: ignoreversion
Source: "D:\netpanzer\maps\*.*"; Excludes: "CVS"; DestDir: "{app}\maps\"; Flags: ignoreversion recursesubdirs
Source: "D:\netpanzer\pics\*.*"; Excludes: "CVS"; DestDir: "{app}\pics\"; Flags: ignoreversion recursesubdirs
Source: "D:\netpanzer\powerups\*.*"; Excludes: "CVS"; DestDir: "{app}\powerups\"; Flags: ignoreversion recursesubdirs
Source: "D:\netpanzer\sound\*.*"; Excludes: "CVS"; DestDir: "{app}\sound\"; Flags: ignoreversion recursesubdirs
Source: "D:\netpanzer\units\*.*"; Excludes: "CVS"; DestDir: "{app}\units\"; Flags: ignoreversion recursesubdirs
Source: "D:\netpanzer\wads\*.*"; Excludes: "CVS"; DestDir: "{app}\wads\"; Flags: ignoreversion recursesubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[INI]
Filename: "{app}\netpanzer.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://netpanzer.berlios.de"

[Icons]
Name: "{group}\netPanzer"; Filename: "{app}\netpanzer.exe"; WorkingDir: "{app}"; IconFilename: "{app}\netpanzer.ico"
Name: "{group}\netPanzer Dedicated Server"; Filename: "{app}\netpanzer.exe"; Parameters: "-d"; WorkingDir: "{app}";
; NOTE: The following entry contains an English phrase ("on the Web"). You are free to translate it into another language if required.
Name: "{group}\netPanzer on the Web"; Filename: "{app}\netpanzer.url"
; NOTE: The following entry contains an English phrase ("Uninstall"). You are free to translate it into another language if required.
Name: "{group}\Uninstall netPanzer"; Filename: "{uninstallexe}"
Name: "{userdesktop}\netPanzer"; Filename: "{app}\netpanzer.exe"; WorkingDir: "{app}"; IconFilename: "{app}\netpanzer.ico"; Tasks: desktopicon

[Run]
; NOTE: The following entry contains an English phrase ("Launch"). You are free to translate it into another language if required.
Filename: "{app}\netpanzer.exe"; Description: "Launch netPanzer"; WorkingDir: "{app}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: files; Name: "{app}\netpanzer.url"
Type: filesandordirs; Name: "{app}\.netpanzer\cache"


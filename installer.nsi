!include "MUI2.nsh"

Name "FileMonitorService"
OutFile "FileMonitorServiceSetup.exe"
InstallDir "$PROGRAMFILES64\FileMonitorService"

Page instfiles

Section "Install"
    SetOutPath $INSTDIR
    File "bin\Release\FileMonitorService.exe"
    ExecWait '"$INSTDIR\FileMonitorService.exe" install'
SectionEnd

Section "Uninstall"
    ExecWait '"$INSTDIR\FileMonitorService.exe" uninstall'
    Delete "$INSTDIR\FileMonitorService.exe"
    RMDir "$INSTDIR"
SectionEnd

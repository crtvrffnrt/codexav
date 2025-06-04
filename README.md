# FileMonitorService

This project contains a minimal Windows service written in C++ that monitors a specific file path. The service skeleton provides installation and uninstallation helpers and can be expanded with file monitoring logic.

## Build

Use Visual Studio (x64) to build `FileMonitorService.exe` from the sources in the `src` directory. Place the resulting binary in `bin\Release` for the provided NSIS installer script.

## Install

Run the built executable with the `install` argument to register the service:

```
FileMonitorService.exe install
```

To uninstall the service:

```
FileMonitorService.exe uninstall
```

An example NSIS script (`installer.nsi`) is included to create a simple installer.

#ifndef FILE_MONITOR_SERVICE_H
#define FILE_MONITOR_SERVICE_H

#include <windows.h>
#include <string>

class FileMonitorService {
public:
    static const wchar_t* ServiceName;

    static void WINAPI ServiceMain(DWORD argc, LPWSTR* argv);
    static void WINAPI ServiceCtrlHandler(DWORD ctrlCode);

    static bool Install();
    static bool Uninstall();

private:
    static SERVICE_STATUS_HANDLE serviceStatusHandle;
    static SERVICE_STATUS serviceStatus;
    static HANDLE stopEvent;

    static void ReportStatus(DWORD currentState, DWORD win32ExitCode, DWORD waitHint);
    static void WorkerThread();
};

#endif // FILE_MONITOR_SERVICE_H

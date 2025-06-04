#include "Service.h"
#include <fstream>
#include <shlobj.h>
#include <sstream>

const wchar_t* FileMonitorService::ServiceName = L"FileMonitorService";

SERVICE_STATUS_HANDLE FileMonitorService::serviceStatusHandle = nullptr;
SERVICE_STATUS FileMonitorService::serviceStatus = {0};
HANDLE FileMonitorService::stopEvent = nullptr;

void FileMonitorService::ReportStatus(DWORD currentState, DWORD win32ExitCode, DWORD waitHint)
{
    serviceStatus.dwCurrentState = currentState;
    serviceStatus.dwWin32ExitCode = win32ExitCode;
    serviceStatus.dwWaitHint = waitHint;
    SetServiceStatus(serviceStatusHandle, &serviceStatus);
}

void WINAPI FileMonitorService::ServiceMain(DWORD argc, LPWSTR* argv)
{
    serviceStatusHandle = RegisterServiceCtrlHandler(ServiceName, ServiceCtrlHandler);
    if (!serviceStatusHandle) {
        return;
    }

    serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    ReportStatus(SERVICE_START_PENDING, NO_ERROR, 3000);

    stopEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    if (!stopEvent) {
        ReportStatus(SERVICE_STOPPED, GetLastError(), 0);
        return;
    }

    ReportStatus(SERVICE_RUNNING, NO_ERROR, 0);

    WorkerThread();

    ReportStatus(SERVICE_STOPPED, NO_ERROR, 0);
}

void WINAPI FileMonitorService::ServiceCtrlHandler(DWORD ctrlCode)
{
    switch (ctrlCode) {
    case SERVICE_CONTROL_STOP:
        ReportStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);
        SetEvent(stopEvent);
        break;
    default:
        break;
    }
}

void FileMonitorService::WorkerThread()
{
    // Placeholder for file monitoring logic.
    while (WaitForSingleObject(stopEvent, 1000) == WAIT_TIMEOUT) {
        // Monitor directory or file changes here.
    }
}

bool FileMonitorService::Install()
{
    wchar_t path[MAX_PATH];
    if (!GetModuleFileName(nullptr, path, MAX_PATH)) {
        return false;
    }

    SC_HANDLE scm = OpenSCManager(nullptr, nullptr, SC_MANAGER_CREATE_SERVICE);
    if (!scm) {
        return false;
    }

    SC_HANDLE svc = CreateService(
        scm,
        ServiceName,
        ServiceName,
        SERVICE_ALL_ACCESS,
        SERVICE_WIN32_OWN_PROCESS,
        SERVICE_AUTO_START,
        SERVICE_ERROR_NORMAL,
        path,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr);

    if (!svc) {
        CloseServiceHandle(scm);
        return false;
    }

    CloseServiceHandle(svc);
    CloseServiceHandle(scm);
    return true;
}

bool FileMonitorService::Uninstall()
{
    SC_HANDLE scm = OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT);
    if (!scm) {
        return false;
    }

    SC_HANDLE svc = OpenService(scm, ServiceName, DELETE | SERVICE_STOP | SERVICE_QUERY_STATUS);
    if (!svc) {
        CloseServiceHandle(scm);
        return false;
    }

    SERVICE_STATUS status;
    ControlService(svc, SERVICE_CONTROL_STOP, &status);

    bool result = DeleteService(svc);

    CloseServiceHandle(svc);
    CloseServiceHandle(scm);
    return result;
}

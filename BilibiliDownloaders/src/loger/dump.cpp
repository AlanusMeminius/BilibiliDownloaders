#include <Windows.h>
#include <DbgHelp.h>

#include <QApplication>
#include <QDir>

#include "dump.h"


int GenerateMiniDump(PEXCEPTION_POINTERS pExceptionPointers)
{
    // ���庯��ָ��
    typedef BOOL(WINAPI* MiniDumpWriteDumpT)(
        HANDLE,
        DWORD,
        HANDLE,
        MINIDUMP_TYPE,
        PMINIDUMP_EXCEPTION_INFORMATION,
        PMINIDUMP_USER_STREAM_INFORMATION,
        PMINIDUMP_CALLBACK_INFORMATION);

    // �� "DbgHelp.dll" ���л�ȡ "MiniDumpWriteDump" ����
    MiniDumpWriteDumpT pfnMiniDumpWriteDump = nullptr;
    HMODULE hDbgHelp = LoadLibrary(L"DbgHelp.dll");
    if (nullptr == hDbgHelp)
    {
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    pfnMiniDumpWriteDump = (MiniDumpWriteDumpT)::GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
    if (nullptr == pfnMiniDumpWriteDump)
    {
        FreeLibrary(hDbgHelp);
        return EXCEPTION_CONTINUE_EXECUTION;
    }

    // ���� dump �ļ���
    wchar_t szFileName[MAX_PATH] = { 0 };
    QString dirPath = QApplication::applicationDirPath() + "/dump/";
    QDir dir;
    if (!dir.exists(dirPath))
    {
        dir.mkpath(dirPath);
    }

    // ���� dump �ļ�
    std::wstring appName = QApplication::applicationName().toStdWString();
    SYSTEMTIME stLocalTime;
    ::GetLocalTime(&stLocalTime);
    ::wsprintf(szFileName, L"%s%s-%04d%02d%02d-%02d%02d%02d.dmp", dirPath.toStdWString().c_str(),
        appName.c_str(), stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,
        stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond);
    HANDLE hDumpFile = ::CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);
    if (INVALID_HANDLE_VALUE == hDumpFile)
    {
        ::FreeLibrary(hDbgHelp);
        return EXCEPTION_CONTINUE_EXECUTION;
    }

    // д�� dump �ļ�
    MINIDUMP_EXCEPTION_INFORMATION expParam;
    expParam.ThreadId = ::GetCurrentThreadId();
    expParam.ExceptionPointers = pExceptionPointers;
    expParam.ClientPointers = false;
    pfnMiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(),
        hDumpFile, MiniDumpWithDataSegs, (pExceptionPointers ? &expParam : nullptr), nullptr, nullptr);
    
    // �ͷ��ļ�
    ::CloseHandle(hDumpFile);
    ::FreeLibrary(hDbgHelp);

    return EXCEPTION_EXECUTE_HANDLER;
}

LONG ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo)
{
    // ������һЩ�쳣�Ĺ��˻���ʾ
    if (IsDebuggerPresent())
    {
        return EXCEPTION_CONTINUE_SEARCH;
    }

    return GenerateMiniDump(lpExceptionInfo);
}

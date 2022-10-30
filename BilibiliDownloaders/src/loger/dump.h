#pragma once

#include <windows.h>


int GenerateMiniDump(PEXCEPTION_POINTERS pExceptionPointers);

LONG ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo);

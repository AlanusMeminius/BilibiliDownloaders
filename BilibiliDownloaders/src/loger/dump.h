#pragma once
#pragma execution_character_set("utf-8")

#include <windows.h>


int GenerateMiniDump(PEXCEPTION_POINTERS pExceptionPointers);

LONG ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo);

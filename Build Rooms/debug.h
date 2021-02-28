#pragma once

#if defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif // _DEBUG

inline void initialize_debugging() {
#if defined(_DEBUG)
	// Send all reports to STDERR
	// _CrtSetReportMode: https://goo.gl/DTCb8N
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
#endif // _DEBUG
}

inline void terminate_debugging() {
#if defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif // _DEBUG
}
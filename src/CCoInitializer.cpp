#include "CCoInitializer.h"
#ifdef _WIN32
#include <objbase.h>
#include "com_helpers.h"
#endif

CCoInitializer::CCoInitializer(DWORD dwCoInit)
#ifdef _WIN32
	: _coinitialized(false)
#endif
{
#ifdef _WIN32
	// Initialize the COM library on the current thread.
	ENSURE_SUCCEEDED(CoInitializeEx(NULL, dwCoInit));
	_coinitialized = true;
#endif
}

CCoInitializer::~CCoInitializer()
{
	#ifdef _WIN32
	// Free the COM library.
	if (_coinitialized)
		CoUninitialize();
	#endif
}

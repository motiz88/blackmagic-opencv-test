#ifndef CCOINITIALIZER_H_
#define CCOINITIALIZER_H_

#ifndef _WIN32
typedef enum tagCOINIT { 
  COINIT_APARTMENTTHREADED  = 0x2,
  COINIT_MULTITHREADED      = 0x0,
  COINIT_DISABLE_OLE1DDE    = 0x4,
  COINIT_SPEED_OVER_MEMORY  = 0x8
} COINIT;

typedef unsigned long DWORD;

#else

#include <intsafe.h>
#include <objbase.h>

#endif
// An exception-safe wrapper class that manages the lifetime 
// of the COM library in a given scope.
class CCoInitializer
{
public:
	explicit CCoInitializer(DWORD dwCoInit = COINIT_APARTMENTTHREADED);

	~CCoInitializer();
private:
	#ifdef _WIN32
	// Flags whether COM was properly initialized.
	bool _coinitialized;
	#endif

	// Hide copy constructor and assignment operator.
	CCoInitializer(const CCoInitializer&);
	CCoInitializer& operator=(const CCoInitializer&);
};

#endif /* CCOINITIALIZER_H_ */
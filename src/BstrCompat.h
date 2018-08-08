#ifndef BSTRCOMPAT_H_
#define BSTRCOMPAT_H_

#ifdef _WIN32
#include <WTypes.h>
#include <OleAuto.h>
#include <comutil.h>
#include <string>

static inline std::string BstrToString(BSTR bstrString) {
	return (const char*) _bstr_t(bstrString, false);
}

#else
//typedef const char* BSTR;
// typedef const CFStringRef  BSTR;
//
// static inline void SysFreeString(BSTR bstrString) {
// 	delete bstrString;
// }
// static inline const char* BstrToString(BSTR bstrString) {
// 	return (const char*)bstrString;
// }
#endif

#endif /* BSTRCOMPAT_H_ */

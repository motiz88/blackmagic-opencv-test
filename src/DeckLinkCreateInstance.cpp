
#ifdef _WIN32
#include "DeckLinkCreateInstance.h"
#include <objbase.h>
#include "com_helpers.h"

#define RETURN_NEW(NAME) \
	I ## NAME *instance; \
	ENSURE_SUCCEEDED( \
		CoCreateInstance(CLSID_C ## NAME, NULL, CLSCTX_ALL, IID_I ## NAME, (void**)&instance) \
	); \
	return instance \

IDeckLinkIterator* CreateDeckLinkIteratorInstance (void)
{
	RETURN_NEW(DeckLinkIterator);
}

IDeckLinkDiscovery* CreateDeckLinkDiscoveryInstance (void)
{
	RETURN_NEW(DeckLinkDiscovery);
}

IDeckLinkAPIInformation* CreateDeckLinkAPIInformationInstance (void)
{
	RETURN_NEW(DeckLinkAPIInformation);
}

IDeckLinkGLScreenPreviewHelper* CreateOpenGLScreenPreviewHelper (void)
{
	RETURN_NEW(DeckLinkGLScreenPreviewHelper);
}

IDeckLinkVideoConversion* CreateVideoConversionInstance (void) {
	RETURN_NEW(DeckLinkVideoConversion);
}

#endif

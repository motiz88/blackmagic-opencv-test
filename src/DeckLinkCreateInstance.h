#ifndef DECKLINKCREATEINSTANCE_H_
#define DECKLINKCREATEINSTANCE_H_

#include <DeckLinkAPI.h>

#ifdef _WIN32

IDeckLinkIterator* CreateDeckLinkIteratorInstance (void);
IDeckLinkDiscovery* CreateDeckLinkDiscoveryInstance (void);
IDeckLinkAPIInformation* CreateDeckLinkAPIInformationInstance (void);
IDeckLinkGLScreenPreviewHelper* CreateOpenGLScreenPreviewHelper (void);
IDeckLinkVideoConversion* CreateVideoConversionInstance (void);

#endif

#endif /* DECKLINKCREATEINSTANCE_H_ */

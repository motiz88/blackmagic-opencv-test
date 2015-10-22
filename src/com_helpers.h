#ifndef COM_HELPERS_H_
#define COM_HELPERS_H_

#define ENSURE_SUCCEEDED(hr) do {HRESULT _hrcode = (hr); if (!SUCCEEDED(_hrcode)) throw _hrcode; } while(false)

#endif /* COM_HELPERS_H_ */
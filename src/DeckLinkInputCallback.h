/*
 * DeckLinkInputCallback.h - Interfaz COM de callback de grabación de vídeo
 *
 *   Copyright 2013 Jesús Torres <jmtorres@ull.es>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef DECKLINKINPUTCALLBACK_H_
#define DECKLINKINPUTCALLBACK_H_

#include <atomic>
#include <condition_variable>
#include <mutex>

#include <DeckLinkAPI.h>

#include "ComPtr.h"

class DeckLinkCapture;

class DeckLinkInputCallback: public IDeckLinkInputCallback
{
    public:
        DeckLinkInputCallback();
        ~DeckLinkInputCallback();

        HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, LPVOID *ppv);
        ULONG STDMETHODCALLTYPE AddRef();
        ULONG STDMETHODCALLTYPE Release();

        HRESULT STDMETHODCALLTYPE VideoInputFormatChanged (
            BMDVideoInputFormatChangedEvents notificationEvents,
            IDeckLinkDisplayMode* newDisplayMode,
            BMDDetectedVideoInputFormatFlags detectedSignalFlags);
        HRESULT STDMETHODCALLTYPE VideoInputFrameArrived (IDeckLinkVideoInputFrame* videoFrame,
            IDeckLinkAudioInputPacket* audioPacket);

        ComPtr<IDeckLinkVideoInputFrame> getVideoFrame();

    private:
        std::atomic_ulong refCounter_;

        std::mutex videoFrameMutex_;
        std::condition_variable videoFrameArrive_;
        ComPtr<IDeckLinkVideoInputFrame> videoFrame_;
};

#endif /* DECKLINKINPUTCALLBACK_H_ */

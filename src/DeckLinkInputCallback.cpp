/*
 * DeckLinkInputCallback.cpp - Interfaz COM de callback de grabación de vídeo
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

#include <atomic>
#include <mutex>

#include <DeckLinkAPI.h>

#include "ComPtr.h"
#include "DeckLinkInputCallback.h"

DeckLinkInputCallback::DeckLinkInputCallback()
{
    refCounter_ = 1;
}

DeckLinkInputCallback::~DeckLinkInputCallback()
{
}

HRESULT DeckLinkInputCallback::QueryInterface(REFIID, LPVOID*)
{
    return E_NOINTERFACE;
}

ULONG DeckLinkInputCallback::AddRef()
{
    return ++refCounter_;
}

ULONG DeckLinkInputCallback::Release()
{
    ULONG refCounter = --refCounter_;
    if (refCounter == 0)
        delete this;
    return refCounter;
}

HRESULT DeckLinkInputCallback::VideoInputFormatChanged(
    BMDVideoInputFormatChangedEvents notificationEvents,
    IDeckLinkDisplayMode* newDisplayMode,
    BMDDetectedVideoInputFormatFlags detectedSignalFlags)
{
    return S_OK;
}

HRESULT DeckLinkInputCallback::VideoInputFrameArrived(
    IDeckLinkVideoInputFrame* videoFrame,
    IDeckLinkAudioInputPacket* audioPacket)
{
    std::lock_guard<std::mutex> lock(videoFrameMutex_);
    videoFrame_ = ComPtr<IDeckLinkVideoInputFrame>(videoFrame, true);
    videoFrameArrive_.notify_all();
    return S_OK;
}

ComPtr<IDeckLinkVideoInputFrame> DeckLinkInputCallback::getVideoFrame()
{
    std::unique_lock<std::mutex> lock(videoFrameMutex_);

    if (! videoFrame_)
        videoFrameArrive_.wait(lock);

    ComPtr<IDeckLinkVideoInputFrame> grabbedFrame_ = videoFrame_;
    videoFrame_.reset();

    return grabbedFrame_;
}

/*
 * DeckLinkCapture.h - Clase para capturar vídeo desde dispositivos DeckLink
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

#ifndef DECKLINKCAPTURE_H_
#define DECKLINKCAPTURE_H_

#include <boost/config.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <DeckLinkAPI.h>

#include "ComPtr.h"
#include "DeckLinkInputCallback.h"

class DeckLinkCapture
{
    public:
        DeckLinkCapture(ComPtr<IDeckLink> deckLink);
        DeckLinkCapture(DeckLinkCapture&& other) BOOST_NOEXCEPT;
        ~DeckLinkCapture();

        DeckLinkCapture& operator=(DeckLinkCapture&& other) BOOST_NOEXCEPT;

        // Mirar en DeckLinkApiMode.h para ver los modos soportados
        bool doesSupportVideoMode(BMDDisplayMode displayMode,
            BMDPixelFormat pixelFormat);

        std::string getDeviceModelName();
        std::string getDeviceDisplayName();

        bool start(BMDDisplayMode displayMode = bmdModeHD1080i6000,
            BMDPixelFormat pixelFormat = bmdFormat8BitYUV);
        void stop();

        bool grab();
        bool retrieve(cv::Mat& videoFrame);

        bool read(cv::Mat& videoFrame);
        DeckLinkCapture& operator>>(cv::Mat& videoFrame);

        HRESULT error() const;
        const std::string& errorString() const;

    private:
        ComPtr<IDeckLink> deckLink_;
        ComPtr<IDeckLinkInput> deckLinkInput_;
        ComPtr<DeckLinkInputCallback> deckLinkInputCallback_;
        ComPtr<IDeckLinkVideoInputFrame> grabbedVideoFrame_;

        HRESULT error_;
        std::string errorString_;

        friend HRESULT DeckLinkInputCallback::VideoInputFrameArrived(
            IDeckLinkVideoInputFrame* videoFrame,
            IDeckLinkAudioInputPacket* audioPacket);

        DeckLinkCapture(DeckLinkCapture const&) = delete;
        DeckLinkCapture& operator=(DeckLinkCapture const&) =  delete;
};

inline HRESULT DeckLinkCapture::error() const
{
    return error_;
}

inline const std::string& DeckLinkCapture::errorString() const
{
    return errorString_;
}

#endif /* DECKLINKCAPTURE_H_ */

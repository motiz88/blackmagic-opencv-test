/*
 * DeckLinkCapture.cpp - Clase para capturar vídeo desde dispositivos DeckLink
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

#include <DeckLinkAPI.h>

#include <boost/thread.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "ComPtr.h"
#include "DeckLinkCapture.h"
#include "DeckLinkInputCallback.h"
#include "DeckLinkOpenCv.h"
#include "BstrCompat.h"

DeckLinkCapture::DeckLinkCapture(ComPtr<IDeckLink> deckLink)
    : deckLink_(deckLink),
      error_(S_OK)
{
    IDeckLinkInput* deckLinkInput;
    error_ = deckLink_->QueryInterface(IID_IDeckLinkInput,
        reinterpret_cast<void**>(&deckLinkInput));
    if (SUCCEEDED(error_))
        deckLinkInput_ = ComPtr<IDeckLinkInput>(deckLinkInput);
    else {
        errorString_ = "La interfaz IDeckLinkInput no pudo ser encontrada";
        return;
    }
}

DeckLinkCapture::DeckLinkCapture(DeckLinkCapture&& other) noexcept
{
    deckLink_.swap(other.deckLink_);
    deckLinkInput_.swap(other.deckLinkInput_);
    deckLinkInputCallback_.swap(other.deckLinkInputCallback_);
    grabbedVideoFrame_.swap(other.grabbedVideoFrame_);
    errorString_.swap(other.errorString_);

    error_ = other.error_;
    other.error_ = S_OK;
}

DeckLinkCapture::~DeckLinkCapture()
{
    if (deckLink_)
        stop();
}

DeckLinkCapture& DeckLinkCapture::operator=(DeckLinkCapture&& other) noexcept
{
    deckLink_ = other.deckLink_;
    other.deckLink_.reset();

    deckLinkInput_ = other.deckLinkInput_;
    other.deckLinkInput_.reset();

    deckLinkInputCallback_ = other.deckLinkInputCallback_;
    other.deckLinkInputCallback_.reset();

    grabbedVideoFrame_ = other.grabbedVideoFrame_;
    other.grabbedVideoFrame_.reset();

    errorString_.swap(other.errorString_);
    other.errorString_.erase();

    error_ = other.error_;
    other.error_ = S_OK;

    return *this;
}

bool DeckLinkCapture::doesSupportVideoMode(BMDDisplayMode displayMode,
    BMDPixelFormat pixelFormat)
{
    BMDDisplayModeSupport support;
    error_ = deckLinkInput_->DoesSupportVideoMode(displayMode, pixelFormat, 0,
        &support, nullptr);
    if (SUCCEEDED(error_))
        return support == bmdDisplayModeSupported ||
            support == bmdDisplayModeSupportedWithConversion;
    else {
        errorString_ = "Error en la consulta a IDeckLinkInput::DoesSupportVideoMode()";
        return false;
    }
}

bool DeckLinkCapture::start(BMDDisplayMode displayMode,
    BMDPixelFormat pixelFormat)
{
    error_ = deckLinkInput_->EnableVideoInput(displayMode, pixelFormat, 0);
    if (FAILED(error_)) {
        switch (error_) {
        case E_INVALIDARG:
            errorString_ = "El modo de vídeo es inválido";
            break;
        case E_ACCESSDENIED:
            errorString_ = "Imposible acceder al hardware o el flujo de "
                "captura está activo.";
            break;
        case E_OUTOFMEMORY:
            errorString_ = "Imposible crear un nuevo frame";
            break;
        default:
            errorString_ = "Error al invocar a IDeckLinkInput::EnableVideoInput()";
        }
        return false;
    }

    ComPtr<DeckLinkInputCallback> callback(new DeckLinkInputCallback());
    error_ = deckLinkInput_->SetCallback(callback.get());
    if (FAILED(error_)) {
        errorString_ = "Error al invocar a IDeckLinkInput::SetCallback()";
        return false;
    }

    error_ = deckLinkInput_->StartStreams();
    if (FAILED(error_)) {
        errorString_ = "Error al invocar a IDeckLinkInput::StartStream()";
        return false;
    }

    deckLinkInputCallback_ = callback;
    return true;
}

void DeckLinkCapture::stop()
{
    error_ = deckLinkInput_->StopStreams();
    if (FAILED(error_))
        errorString_ = "Error al invocar a IDeckLinkInput::StopStreams()";

    error_ = deckLinkInput_->DisableVideoInput();
    if (FAILED(error_))
        errorString_ = "Error al invocar a IDeckLinkInput::DisableVideoInput()";

    deckLinkInputCallback_.reset();
    grabbedVideoFrame_.reset();
}

bool DeckLinkCapture::grab()
{
    if (! deckLinkInputCallback_)
        return false;

    grabbedVideoFrame_ = deckLinkInputCallback_->getVideoFrame();
    return true;
}

bool DeckLinkCapture::retrieve(cv::Mat& videoFrame)
{
    if (! deckLinkInputCallback_ || !grabbedVideoFrame_) {
        videoFrame.release();
        return false;
    }

    if (! deckLinkVideoFrameToCvMat(grabbedVideoFrame_, videoFrame)) {
        error_ = E_FAIL;
        errorString_ = "Error al convertir el formato de la imagen";
        videoFrame.release();
        return false;
    }

    return true;
}

bool DeckLinkCapture::read(cv::Mat& videoFrame)
{
    if (grab())
        return retrieve(videoFrame);
    else {
        videoFrame.release();
        return false;
    }
}

DeckLinkCapture& DeckLinkCapture::operator>>(cv::Mat& videoFrame)
{
    read(videoFrame);
    return *this;
}

std::string DeckLinkCapture::getDeviceModelName()
{
    BSTR name;
    error_ = deckLink_->GetModelName(&name);
    if (FAILED(error_)) {
        errorString_ = "Error al invocar a IDeckLinkInput::GetModelName()";
        return std::string();
    }

    std::string modelName = BstrToString(name);
    SysFreeString(name);
    return modelName;
}

std::string DeckLinkCapture::getDeviceDisplayName()
{
    BSTR name;
    error_ = deckLink_->GetDisplayName(&name);
    if (FAILED(error_)) {
        errorString_ = "Error al invocar a IDeckLinkInput::GetDisplayName()";
        return std::string();
    }

    std::string displayName = BstrToString(name);
    SysFreeString(name);
    return displayName;
}

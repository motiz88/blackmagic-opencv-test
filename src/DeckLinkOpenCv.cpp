/*
 * DeckLinkOpenCv.cpp - Conversión de formato DeckLink a OpenCV
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

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "ComPtr.h"
#include "DeckLinkOpenCv.h"
#include "DeckLinkCreateInstance.h"
 
#define MAT_REFCOUNT(mat) \
 (mat.u ? (mat.u->refcount) : 0)

class CvMatDeckLinkVideoFrame : public IDeckLinkVideoFrame
{
    public:
        cv::Mat mat;

        CvMatDeckLinkVideoFrame(int row, int cols)
            : mat(row, cols, CV_8UC4)
        {}

        //
        // IDeckLinkVideoFrame
        //

        long STDMETHODCALLTYPE GetWidth()
        { return mat.rows; }
        long STDMETHODCALLTYPE GetHeight()
        { return mat.cols; }
        long STDMETHODCALLTYPE GetRowBytes()
        { return mat.step; }
        BMDPixelFormat STDMETHODCALLTYPE GetPixelFormat()
        { return bmdFormat8BitBGRA; }
        BMDFrameFlags STDMETHODCALLTYPE GetFlags()
        { return 0; }
        HRESULT STDMETHODCALLTYPE GetBytes(void **buffer)
        {
            *buffer = mat.data;
            return S_OK;
        }

        HRESULT STDMETHODCALLTYPE GetTimecode(BMDTimecodeFormat format,
            IDeckLinkTimecode **timecode)
        { *timecode = nullptr; return S_OK; }
        HRESULT STDMETHODCALLTYPE GetAncillaryData(IDeckLinkVideoFrameAncillary **ancillary)
        { *ancillary = nullptr; return S_OK; }

        //
        // IDeckLinkVideoFrame
        //

        HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, LPVOID *ppv)
        { return E_NOINTERFACE; }

        ULONG STDMETHODCALLTYPE AddRef()
        { mat.addref(); return MAT_REFCOUNT(mat); }
        ULONG STDMETHODCALLTYPE Release()
        {
            mat.release();
            if (MAT_REFCOUNT(mat) == 0) delete this;
            return MAT_REFCOUNT(mat);
        }
};

bool deckLinkVideoFrameToCvMat(ComPtr<IDeckLinkVideoInputFrame> in,
    cv::Mat& out)
{
    switch (in->GetPixelFormat()) {
    case bmdFormat8BitYUV:
    {
        void* data;
        if (FAILED(in->GetBytes(&data)))
            return false;
        cv::Mat mat = cv::Mat(in->GetHeight(), in->GetWidth(), CV_8UC2, data,
            in->GetRowBytes());
        cv::cvtColor(mat, out, CV_YUV2BGR_UYVY);
        return true;
    }
    case bmdFormat8BitBGRA:
    {
        void* data;
        if (FAILED(in->GetBytes(&data)))
            return false;
        cv::Mat mat = cv::Mat(in->GetHeight(), in->GetWidth(), CV_8UC4, data);
        cv::cvtColor(mat, out, CV_BGRA2BGR);
        return true;
    }
    default:
    {
        ComPtr<IDeckLinkVideoConversion> deckLinkVideoConversion =
            CreateVideoConversionInstance();
        if (! deckLinkVideoConversion)
            return false;

        CvMatDeckLinkVideoFrame cvMatWrapper(in->GetHeight(), in->GetWidth());
        if (FAILED(deckLinkVideoConversion->ConvertFrame(in.get(), &cvMatWrapper)))
            return false;
        cv::cvtColor(cvMatWrapper.mat, out, CV_BGRA2BGR);
        return true;
    }}
}

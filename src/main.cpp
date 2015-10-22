/*
 * main.cpp - Programa de ejemplo de uso de la clase DeckLinkCapture
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

#include <iostream>
#include <utility>
#include <vector>

#include <errno.h>

#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include <DeckLinkAPI.h>

#include "ComPtr.h"
#include "DeckLinkCapture.h"
#include "CCoInitializer.h"
 
int main(int argc, char** argv)
{
    CCoInitializer coinit;
    ComPtr<IDeckLinkIterator> deckLinkIterator = CreateDeckLinkIteratorInstance();
    if (! deckLinkIterator) {
        std::cerr << argv[0]
                  << ": No se puede obtener el iterador de dispositivos DeckLink."
                  << std::endl;
        return 1;
    }

    std::vector<DeckLinkCapture> captures;
    std::vector<std::string> windows;

    IDeckLink* deckLink;
    while (deckLinkIterator->Next(&deckLink) == S_OK) {
        captures.push_back(
            std::move(DeckLinkCapture(ComPtr<IDeckLink>(deckLink))));

        std::string windowName = boost::str(boost::format("%s <%i>") %
            captures.back().getDeviceDisplayName() % captures.size());
        cv::namedWindow(windowName);
        windows.push_back(windowName);
    }

    if (captures.size() == 0) {
        std::cerr << argv[0]
                  << ": No se encontró ningún dispositivo DeckLink."
                  << std::endl;
        return 2;
    }

    BOOST_FOREACH(DeckLinkCapture& capture, captures)
    {
        if (! capture.start())
            std::cerr << argv[0]
                      << ": No se pudo iniciar la captura en el dispositivo '"
                      << capture.getDeviceDisplayName()
                      << "': "
                      << capture.errorString()
                      << std::endl;
    }

    while (true) {
        BOOST_FOREACH(DeckLinkCapture& capture, captures)
        {
            capture.grab();
        }
        for(unsigned i = 0; i < captures.size(); ++i) {
            cv::Mat frame;
            captures[i].retrieve(frame);
            cv::imshow(windows[i], frame);
        }
        if (cv::waitKey(10) >= 0)
            break;
    }

    BOOST_FOREACH(DeckLinkCapture& capture, captures)
    {
        capture.stop();
    }

    cv::destroyAllWindows();

    return 0;
}

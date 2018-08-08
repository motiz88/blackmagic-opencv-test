/*
 * ComPtr.h - Implementación de puntero inteligente para interfaces COM
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

#ifndef COMPTR_H_
#define COMPTR_H_

#ifdef _WIN32
#include <objbase.h>
#else
//#include <LinuxCOM.h>
#endif

#include <boost/intrusive_ptr.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>

#define CV_YUV2BGR_UYVY cv::COLOR_YUV2BGR_UYVY
#define CV_BGRA2BGR cv::COLOR_BGRA2BGR 

template <typename T>
class ComPtr : public boost::intrusive_ptr<T>
{
    BOOST_STATIC_ASSERT((boost::is_base_of<IUnknown, T>::value));

    public:
        ComPtr();
        ComPtr(T* p, bool add_ref = false);
        ComPtr(ComPtr const& r);
        template<typename Y>
        ComPtr(ComPtr<Y> const& r);
};

template <typename T>
ComPtr<T>::ComPtr()
    : boost::intrusive_ptr<T>()
{
}

template <typename T>
ComPtr<T>::ComPtr(T* p, bool add_ref)
    : boost::intrusive_ptr<T>(p, add_ref)
{
}

template <typename T>
ComPtr<T>::ComPtr(ComPtr const& r)
    : boost::intrusive_ptr<T>(r)
{
}

template <typename T>
template <typename Y>
ComPtr<T>::ComPtr(ComPtr<Y> const& r)
    : boost::intrusive_ptr<T>(r)
{
}

inline void intrusive_ptr_add_ref(IUnknown* p)
{
    p->AddRef();
}

inline void intrusive_ptr_release(IUnknown* p)
{
    p->Release();
}

#endif /* COMPTR_H_ */

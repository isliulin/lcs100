//
// NamedMutex.h
//
// $Id: //poco/1.4/Foundation/include/Poco/NamedMutex.h#2 $
//
// Library: Foundation
// Package: Processes
// Module:  NamedMutex
//
// Definition of the NamedMutex class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Foundation_NamedMutex_INCLUDED
#define Foundation_NamedMutex_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/ScopedLock.h"


#if defined(POCO_OS_FAMILY_WINDOWS) && defined(POCO_WIN32_UTF8)
#include "Poco/NamedMutex_WIN32U.h"
#elif defined(POCO_OS_FAMILY_WINDOWS)
#include "Poco/NamedMutex_WIN32.h"
#elif defined(POCO_ANDROID)
#include "Poco/NamedMutex_Android.h"
#elif defined(POCO_OS_FAMILY_UNIX)
#include "Poco/NamedMutex_UNIX.h"
#else
#include "Poco/NamedMutex_VMS.h"
#endif


namespace Poco
{


class Foundation_API NamedMutex: private NamedMutexImpl
    /// A NamedMutex (mutual exclusion) is a global synchronization
    /// mechanism used to control access to a shared resource
    /// in a concurrent (multi process) scenario.
    /// Using the ScopedLock class is the preferred way to automatically
    /// lock and unlock a mutex.
    ///
    /// Unlike a Mutex or a FastMutex, which itself is the unit of synchronization,
    /// a NamedMutex refers to a named operating system resource being the
    /// unit of synchronization.
    /// In other words, there can be multiple instances of NamedMutex referring
    /// to the same actual synchronization object.
    ///
    ///
    /// There should not be more than one instance of NamedMutex for
    /// a given name in a process. Otherwise, the instances may
    /// interfere with each other.
{
public:
    typedef Poco::ScopedLock<NamedMutex> ScopedLock;

    NamedMutex(const std::string& name);
    /// creates the Mutex.

    ~NamedMutex();
    /// destroys the Mutex.

    void lock();
    /// Locks the mutex. Blocks if the mutex
    /// is held by another process or thread.

    bool tryLock();
    /// Tries to lock the mutex. Returns false immediately
    /// if the mutex is already held by another process or thread.
    /// Returns true if the mutex was successfully locked.

    void unlock();
    /// Unlocks the mutex so that it can be acquired by
    /// other threads.

private:
    NamedMutex();
    NamedMutex(const NamedMutex&);
    NamedMutex& operator = (const NamedMutex&);
};


//
// inlines
//
inline void NamedMutex::lock()
{
    lockImpl();
}


inline bool NamedMutex::tryLock()
{
    return tryLockImpl();
}


inline void NamedMutex::unlock()
{
    unlockImpl();
}


} // namespace Poco


#endif // Foundation_NamedMutex_INCLUDED

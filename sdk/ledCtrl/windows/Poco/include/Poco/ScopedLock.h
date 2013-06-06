//
// ScopedLock.h
//
// $Id: //poco/1.4/Foundation/include/Poco/ScopedLock.h#3 $
//
// Library: Foundation
// Package: Threading
// Module:  Mutex
//
// Definition of the ScopedLock template class.
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


#ifndef Foundation_ScopedLock_INCLUDED
#define Foundation_ScopedLock_INCLUDED


#include "Poco/Foundation.h"


namespace Poco
{


template <class M>
class ScopedLock
    /// A class that simplifies thread synchronization
    /// with a mutex.
    /// The constructor accepts a Mutex (and optionally
    /// a timeout value in milliseconds) and locks it.
    /// The destructor unlocks the mutex.
{
public:
    explicit ScopedLock(M& mutex): _mutex(mutex)
    {
        _mutex.lock();
    }

    ScopedLock(M& mutex, long milliseconds): _mutex(mutex)
    {
        _mutex.lock(milliseconds);
    }

    ~ScopedLock()
    {
        _mutex.unlock();
    }

private:
    M& _mutex;

    ScopedLock();
    ScopedLock(const ScopedLock&);
    ScopedLock& operator = (const ScopedLock&);
};


template <class M>
class ScopedLockWithUnlock
    /// A class that simplifies thread synchronization
    /// with a mutex.
    /// The constructor accepts a Mutex (and optionally
    /// a timeout value in milliseconds) and locks it.
    /// The destructor unlocks the mutex.
    /// The unlock() member function allows for manual
    /// unlocking of the mutex.
{
public:
    explicit ScopedLockWithUnlock(M& mutex): _pMutex(&mutex)
    {
        _pMutex->lock();
    }

    ScopedLockWithUnlock(M& mutex, long milliseconds): _pMutex(&mutex)
    {
        _pMutex->lock(milliseconds);
    }

    ~ScopedLockWithUnlock()
    {
        unlock();
    }

    void unlock()
    {
        if (_pMutex)
        {
            _pMutex->unlock();
            _pMutex = 0;
        }
    }

private:
    M* _pMutex;

    ScopedLockWithUnlock();
    ScopedLockWithUnlock(const ScopedLockWithUnlock&);
    ScopedLockWithUnlock& operator = (const ScopedLockWithUnlock&);
};


} // namespace Poco


#endif // Foundation_ScopedLock_INCLUDED

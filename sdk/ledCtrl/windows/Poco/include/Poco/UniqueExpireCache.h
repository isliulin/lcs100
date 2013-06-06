//
// UniqueExpireCache.h
//
// $Id: //poco/1.4/Foundation/include/Poco/UniqueExpireCache.h#1 $
//
// Library: Foundation
// Package: Cache
// Module:  UniqueExpireCache
//
// Definition of the UniqueExpireCache class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
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


#ifndef Foundation_UniqueExpireCache_INCLUDED
#define Foundation_UniqueExpireCache_INCLUDED


#include "Poco/AbstractCache.h"
#include "Poco/UniqueExpireStrategy.h"


namespace Poco
{


template <
class TKey,
      class TValue,
      class TMutex = FastMutex,
      class TEventMutex = FastMutex
      >
class UniqueExpireCache: public AbstractCache<TKey, TValue, UniqueExpireStrategy<TKey, TValue>, TMutex, TEventMutex>
    /// An UniqueExpireCache caches entries for a given time amount. In contrast
    /// to ExpireCache which only allows to set a per cache expiration value, it allows to define
    /// expiration per CacheEntry.
    /// Each TValue object must thus offer the following method:
    ///
    ///    const Poco::Timestamp& getExpiration() const;
    ///
    /// which returns the absolute timepoint when the entry will be invalidated.
    /// Accessing an object will NOT update this absolute expire timepoint.
    /// You can use the Poco::ExpirationDecorator to add the getExpiration
    /// method to values that do not have a getExpiration function.
    ///
    /// Be careful when using an UniqueExpireCache. A cache is often used
    /// like cache.has(x) followed by cache.get x). Note that it could happen
    /// that the "has" call works, then the current execution thread gets descheduled, time passes,
    /// the entry gets invalid, thus leading to an empty SharedPtr being returned
    /// when "get" is invoked.
{
public:
    UniqueExpireCache():
        AbstractCache<TKey, TValue, UniqueExpireStrategy<TKey, TValue>, TMutex, TEventMutex>(UniqueExpireStrategy<TKey, TValue>())
    {
    }

    ~UniqueExpireCache()
    {
    }

private:
    UniqueExpireCache(const UniqueExpireCache& aCache);
    UniqueExpireCache& operator = (const UniqueExpireCache& aCache);
};


} // namespace Poco


#endif // Foundation_UniqueExpireCache_INCLUDED

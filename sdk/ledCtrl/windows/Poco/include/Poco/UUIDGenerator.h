//
// UUIDGenerator.h
//
// $Id: //poco/1.4/Foundation/include/Poco/UUIDGenerator.h#1 $
//
// Library: Foundation
// Package: UUID
// Module:  UUID
//
// Definition of the UUIDGenerator class.
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


#ifndef Foundation_UUIDGenerator_INCLUDED
#define Foundation_UUIDGenerator_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/UUID.h"
#include "Poco/Mutex.h"
#include "Poco/Random.h"
#include "Poco/Timestamp.h"
#include "Poco/Environment.h"


namespace Poco
{


class DigestEngine;


class Foundation_API UUIDGenerator
    /// This class implements a generator for Universal Unique Identifiers,
    /// as specified in Appendix A of the DCE 1.1 Remote Procedure
    /// Call Specification (http://www.opengroup.org/onlinepubs/9629399/),
    /// RFC 2518 (WebDAV), section 6.4.1 and the UUIDs and GUIDs internet
    /// draft by Leach/Salz from February, 1998
    /// (http://ftp.ics.uci.edu/pub/ietf/webdav/uuid-guid/draft-leach-uuids-guids-01.txt)
{
public:
    UUIDGenerator();
    /// Creates the UUIDGenerator.

    ~UUIDGenerator();
    /// Destroys the UUIDGenerator.

    UUID create();
    /// Creates a new time-based UUID, using the MAC address of
    /// one of the system's ethernet adapters.
    ///
    /// Throws a SystemException if no MAC address can be
    /// obtained.

    UUID createFromName(const UUID& nsid, const std::string& name);
    /// Creates a name-based UUID.

    UUID createFromName(const UUID& nsid, const std::string& name, DigestEngine& de);
    /// Creates a name-based UUID, using the given digest engine.

    UUID createRandom();
    /// Creates a random UUID.

    UUID createOne();
    /// Tries to create and return a time-based UUID (see create()), and,
    /// if that does not work due to the unavailability of a MAC address,
    /// creates and returns a random UUID (see createRandom()).
    ///
    /// The UUID::version() method can be used to determine the actual kind of
    /// the UUID generated.

    static UUIDGenerator& defaultGenerator();
    /// Returns a reference to the default UUIDGenerator.

protected:
    Timestamp::UtcTimeVal timeStamp();
    void getNode();

private:
    FastMutex           _mutex;
    Random              _random;
    Timestamp           _lastTime;
    int                 _ticks;
    Environment::NodeId _node;
    bool                _haveNode;

    UUIDGenerator(const UUIDGenerator&);
    UUIDGenerator& operator = (const UUIDGenerator&);
};


} // namespace Poco


#endif // Foundation_UUIDGenerator_INCLUDED

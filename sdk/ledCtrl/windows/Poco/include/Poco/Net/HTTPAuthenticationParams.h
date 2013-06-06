//
// HTTPAuthenticationParams.h
//
// $Id: //poco/1.4/Net/include/Poco/Net/HTTPAuthenticationParams.h#2 $
//
// Library: Net
// Package: HTTP
// Module:  HTTPAuthenticationParams
//
// Definition of the HTTPAuthenticationParams class.
//
// Copyright (c) 2011, Anton V. Yabchinskiy (arn at bestmx dot ru).
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
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


#ifndef Net_HTTPAuthenticationParams_INCLUDED
#define Net_HTTPAuthenticationParams_INCLUDED


#include "Poco/Net/NameValueCollection.h"


namespace Poco
{
namespace Net
{


class HTTPRequest;
class HTTPResponse;


class Net_API HTTPAuthenticationParams: public NameValueCollection
    /// Collection of name-value pairs of HTTP authentication header (i.e.
    /// "realm", "qop", "nonce" in case of digest authentication header).
{
public:
    HTTPAuthenticationParams();
    /// Creates an empty authentication parameters collection.

    explicit HTTPAuthenticationParams(const std::string& authInfo);
    /// See fromAuthInfo() documentation.

    explicit HTTPAuthenticationParams(const HTTPRequest& request);
    /// See fromRequest() documentation.

    explicit HTTPAuthenticationParams(const HTTPResponse& response);
    /// See fromResponse() documentation.

    virtual ~HTTPAuthenticationParams();
    /// Destroys the HTTPAuthenticationParams.

    HTTPAuthenticationParams& operator = (const HTTPAuthenticationParams& authParams);
    /// Assigns the content of another HTTPAuthenticationParams.

    void fromAuthInfo(const std::string& authInfo);
    /// Creates an HTTPAuthenticationParams by parsing authentication
    /// information.

    void fromRequest(const HTTPRequest& request);
    /// Extracts authentication information from the request and creates
    /// HTTPAuthenticationParams by parsing it.
    ///
    /// Throws a NotAuthenticatedException if no authentication
    /// information is contained in request.
    /// Throws a InvalidArgumentException if authentication scheme is
    /// unknown or invalid.

    void fromResponse(const HTTPResponse& response);
    /// Extracts authentication information from the response and creates
    /// HTTPAuthenticationParams by parsing it.
    ///
    /// Throws a NotAuthenticatedException if no authentication
    /// information is contained in response.
    /// Throws a InvalidArgumentException if authentication scheme is
    /// unknown or invalid.

    void setRealm(const std::string& realm);
    /// Sets the "realm" parameter to the provided string.

    const std::string& getRealm() const;
    /// Returns value of the "realm" parameter.
    ///
    /// Throws NotFoundException is there is no "realm" set in the
    /// HTTPAuthenticationParams.

    std::string toString() const;
    /// Formats the HTTPAuthenticationParams for inclusion in HTTP
    /// request or response authentication header.

    static const std::string REALM;

private:
    void parse(std::string::const_iterator first, std::string::const_iterator last);
};


}
} // namespace Poco::Net


#endif // Net_HTTPAuthenticationParams_INCLUDED

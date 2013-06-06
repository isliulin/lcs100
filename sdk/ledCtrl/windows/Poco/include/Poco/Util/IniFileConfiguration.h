//
// IniFileConfiguration.h
//
// $Id: //poco/1.4/Util/include/Poco/Util/IniFileConfiguration.h#1 $
//
// Library: Util
// Package: Configuration
// Module:  IniFileConfiguration
//
// Definition of the IniFileConfiguration class.
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


#ifndef Util_IniFileConfiguration_INCLUDED
#define Util_IniFileConfiguration_INCLUDED


#include "Poco/Util/Util.h"
#include "Poco/Util/AbstractConfiguration.h"
#include <map>
#include <istream>


namespace Poco
{
namespace Util
{


class Util_API IniFileConfiguration: public AbstractConfiguration
    /// This implementation of a Configuration reads properties
    /// from a legacy Windows initialization (.ini) file.
    ///
    /// The file syntax is implemented as follows.
    ///   - a line starting with a semicolon is treated as a comment and ignored
    ///   - a line starting with a square bracket denotes a section key [<key>]
    ///   - every other line denotes a property assignment in the form
    ///     <value key> = <value>
    ///
    /// The name of a property is composed of the section key and the value key,
    /// separated by a period (<section key>.<value key>).
    ///
    /// Property names are not case sensitive. Leading and trailing whitespace is
    /// removed from both keys and values.
{
public:
    IniFileConfiguration();
    /// Creates an empty IniFileConfiguration.

    IniFileConfiguration(std::istream& istr);
    /// Creates an IniFileConfiguration and loads the configuration data
    /// from the given stream, which must be in initialization file format.

    IniFileConfiguration(const std::string& path);
    /// Creates an IniFileConfiguration and loads the configuration data
    /// from the given file, which must be in initialization file format.

    void load(std::istream& istr);
    /// Loads the configuration data from the given stream, which
    /// must be in initialization file format.

    void load(const std::string& path);
    /// Loads the configuration data from the given file, which
    /// must be in initialization file format.

protected:
    bool getRaw(const std::string& key, std::string& value) const;
    void setRaw(const std::string& key, const std::string& value);
    void enumerate(const std::string& key, Keys& range) const;
    void removeRaw(const std::string& key);
    ~IniFileConfiguration();

private:
    void parseLine(std::istream& istr);

    struct ICompare
    {
        bool operator () (const std::string& s1, const std::string& s2) const;
    };
    typedef std::map<std::string, std::string, ICompare> IStringMap;

    IStringMap _map;
    std::string _sectionKey;
};


}
} // namespace Poco::Util


#endif // Util_IniFileConfiguration_INCLUDED

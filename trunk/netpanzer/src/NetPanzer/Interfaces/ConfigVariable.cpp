/*
Copyright (C) 2004 by Matthias Braun <matze@braunis.de>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include <config.h>

#include "Util/Exception.hpp"
#include "ConfigVariable.hpp"

ConfigVariable::ConfigVariable(const std::string& newname)
    : name(newname), defaultvalue(true)
{
}

ConfigVariable::~ConfigVariable()
{
}

//---------------------------------------------------------------------------

ConfigInt::ConfigInt(const std::string& newname, int newvalue,
                     int newmin, int newmax)
    : ConfigVariable(newname), min(newmin), max(newmax)
{
    *this = newvalue;
}

ConfigInt::~ConfigInt()
{
}

int ConfigInt::operator = (int newvalue)
{
    if(newvalue < min || newvalue > max)
        throw Exception("Configsetting '%s' out of range: %d (min: %d max: %d)",
                getName().c_str(), newvalue, min, max);

    value = newvalue;
    return value;
}

//---------------------------------------------------------------------------
ConfigXY::ConfigXY(const std::string& newname, const iXY &newvalue)
    : ConfigVariable(newname)
{
    *this = newvalue;
}

ConfigXY::~ConfigXY()
{
}
const iXY& ConfigXY::operator = (const iXY& newvalue)
{
    value = newvalue;
    return value;
}
//---------------------------------------------------------------------------

ConfigBool::ConfigBool(const std::string& newname, bool newvalue)
    : ConfigVariable(newname), value(newvalue)
{
}

ConfigBool::~ConfigBool()
{
}

bool ConfigBool::operator = (bool newvalue)
{
    value = newvalue;
    return value;
}

//---------------------------------------------------------------------------

ConfigString::ConfigString(const std::string& newname,
                           const std::string& newvalue)
    : ConfigVariable(newname), value(newvalue)
{
}

ConfigString::~ConfigString()
{
}

const std::string& ConfigString::operator= (const std::string& newvalue)
{
    value = newvalue;
    return value;
}

std::ostream& operator<< (std::ostream& o, const ConfigString& string)
{
    return o << ( (const std::string&) string);
}

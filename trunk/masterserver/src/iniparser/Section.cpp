/*
Copyright (C) 2004 Matthias Braun <matze@braunis.de>

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

#include "Section.hpp"

#include <sstream>
#include "Log.hpp"

namespace iniparser
{

using masterserver::log;

Section::Section(const std::string& newname)
    : name(newname)
{
}

Section::~Section()
{
}

const std::string&
Section::getName() const
{
    return name;
}

bool
Section::exists(const std::string& key) const
{
    std::map<std::string, std::string>::const_iterator i = values.find(key);

    return i != values.end();
}

const std::string&
Section::getValue(const std::string& key) const
{
    std::map<std::string, std::string>::const_iterator i = values.find(key);
    if(i == values.end()) {
        *log << "Couldn't find key '" << key << "' in config.\n";
        static std::string nokey;
        return nokey;
    }

    return i->second;
}

int
Section::getIntValue(const std::string& key) const
{
    const std::string& value = getValue(key);
    std::stringstream str(value);

    int result;
    str >> result;
    return result;
}

float
Section::getFloatValue(const std::string& key) const
{
    const std::string& value = getValue(key);
    std::stringstream str(value);

    float result;
    str >> result;
    return result;
}

void
Section::setValue(const std::string& key, const std::string& value)
{
    values.insert(std::make_pair(key, value));
}

void
Section::setIntValue(const std::string& key, int value)
{
    std::stringstream str;
    str << value;

    setValue(key, str.str());
}

void
Section::setFloatValue(const std::string& key, float value)
{
    std::stringstream str;
    str << value;

    setValue(key, str.str());
}

} // end of namespace INI
/*
Copyright (C) 2003 Matthias Braun
 
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
#ifndef __CONFIGVARIABLE_HPP__
#define __CONFIGVARIABLE_HPP__

#include <string>

class ConfigVariable
{
public:
    ConfigVariable(const std::string& name);
    virtual ~ConfigVariable();

    const std::string& getName() const
    { return name; }
    
private:
    std::string name;
};

class ConfigInt : public ConfigVariable
{
public:
    ConfigInt(const std::string& name, int value=0,
                   int min=0, int max=0x7fffffff);
    ~ConfigInt();

    operator const int () const
    { return value; }

    int getMax() const
    { return max; }

    int getMin() const
    { return min; }
    
    int operator = (int newvalue);
    
private:
    int value;
    int min, max;
};

class ConfigBool : public ConfigVariable
{
public:
    ConfigBool(const std::string& name, bool newvalue = false);
    ~ConfigBool();

    operator bool() const
    { return value; }

    bool operator = (bool newvalue);

    void toggle()
    { value = !value; }

private:
    bool value;
};

class ConfigString : public ConfigVariable
{
public:
    ConfigString(const std::string& name, const std::string& value="");
    ~ConfigString();

    operator const std::string& () const
    { return value; }

    const std::string& operator= (const std::string& newvalue);

private:
    std::string value;
};

#endif


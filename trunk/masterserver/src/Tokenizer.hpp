#ifndef __TOKENIZER_HPP__
#define __TOKENIZER_HPP__

#include <string>
#include <iostream>

/** a class that splits a string into multiple tokens that are separated by a
 * \ delimiter character
 */
class Tokenizer
{
public:
    Tokenizer(std::istream& newin)
        : in(newin)
    {
    }

    std::string getNextToken()
    {
        if(in.eof()) {
            return "";
        }
        
        std::string result;

        char c;
        do {
            in >> c;
        } while(c == '\\');
        
        do {
            result += c;
            in >> c;
        } while((!in.eof()) && c != '\\');

        return result;
    }

private:
    std::istream& in;
};

#endif


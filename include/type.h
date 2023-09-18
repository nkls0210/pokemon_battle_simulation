#ifndef TYPE_H
#define TYPE_H

#include <string>
#include <cassert>
#include <iostream>

using std::cout, std::string;

class Type{
    public:
    string typeName;
    unsigned typeValue;

    void typeNameToValue(string);

    Type();
    Type(string);
};

#endif
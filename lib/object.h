#ifndef _OBJECT_H_
#define _OBJECT_H_
#include "strings.h"

//class String;

class Object
{
public:
    virtual String getClassName()
    {
        return "Object";
    }
    
    bool operator==(Object & rsh)
    {
        return this == &rsh;
    }
};


#endif

#ifndef _FILE_H_
#define _FILE_H_
#include <stdio.h>
#include "iobase.h"
#include "strings.h"

typedef enum
{
    ReadOnly,
    WriteOnly,
    ReadWrite
} FileMode;

class File: public IOBase
{
private:
    FILE * fp;
public:
    File(String file, FileMode mode = ReadOnly);
    ~File();
    size_t read(void * ptr, size_t size);
    long position();
    void seek(long pos, SeekOrigin origin = SeekSet);
    size_t size();
    void reset();
    size_t write(const void * ptr, size_t size);
};

#endif

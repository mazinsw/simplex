#ifndef _IOBASE_H_
#define _IOBASE_H_
#include <stddef.h>

typedef unsigned char byte_t;

typedef enum
{
    SeekSet,
    SeekCur,
    SeekEnd,
} SeekOrigin;

class IOBase
{
public:
    virtual size_t read(void * ptr, size_t size) = 0;
    virtual long position() = 0;
    virtual void seek(long pos, SeekOrigin origin = SeekSet) = 0;
    virtual size_t size() = 0;
    virtual void reset() = 0;
    virtual size_t write(const void * ptr, size_t size) = 0;
};

#endif

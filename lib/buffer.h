#ifndef _BUFFER_H_
#define _BUFFER_H_
#include "iobase.h"

class Buffer: public IOBase
{
private:
    byte_t * m_buff;
    size_t m_size;
    size_t m_alloc_size;
    size_t m_alloc_by;
    long m_pos;
public:
    Buffer();
    ~Buffer();
    const byte_t * data() const;
    size_t read(void * ptr, size_t size);
    long position();
    void seek(long pos, SeekOrigin origin = SeekSet);
    size_t size();
    void reset();
    size_t write(const void * ptr, size_t size);
};

#endif


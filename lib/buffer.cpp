#include "buffer.h"
#include <string.h>

Buffer::Buffer()
{
    m_alloc_by = 512;
    m_alloc_size = m_alloc_by;
    m_buff = new byte_t[m_alloc_size];
    m_pos = 0;
    m_size = 0;
}

Buffer::~Buffer()
{
    delete[] m_buff;
}

const byte_t * Buffer::data() const
{
    return m_buff;
}

size_t Buffer::read(void* ptr, size_t size)
{
    size_t readed;
    
    readed = size;
    if(size + m_pos > m_size)
        readed = m_size - m_pos;
    memcpy(ptr, m_buff + m_pos, readed);
    m_pos += readed;
    return readed;
}

long Buffer::position()
{
    return m_pos;
}

void Buffer::seek(long pos, SeekOrigin origin)
{
    if(origin == SeekSet)
    {
        if(pos <= (long)m_size && pos >= 0)
            m_pos = pos;
        else if(pos < 0)
            m_pos = 0;
        else
            m_pos = m_size;
    }
    else if(origin == SeekCur)
    {
        if(m_pos + pos >= 0 && m_pos + pos <= (long)m_size)
            m_pos += pos;
        else if(m_pos + pos < 0)
            m_pos = 0;
        else
            m_pos = m_size;
    }
    else if(origin == SeekEnd)
    {
        if(m_pos - pos >= 0 && m_pos - pos <= (long)m_size)
            m_pos -= pos;
        else if(m_pos - pos < 0)
            m_pos = 0;
        else
            m_pos = m_size;
    }
}

size_t Buffer::size()
{
    return m_size;
}

void Buffer::reset()
{
    m_pos = 0;
}

size_t Buffer::write(const void* ptr, size_t size)
{
    size_t m_new_size;
    byte_t * tmp;
    
    m_new_size = m_pos + size;
    if(m_new_size > m_alloc_size - (m_alloc_by / 2))
    {
        m_alloc_size = m_new_size + m_alloc_by;
        tmp = new byte_t[m_alloc_size];
        memcpy(tmp, m_buff, m_size);
        delete[] m_buff;
        m_buff = tmp;
    }
    memcpy(&m_buff[m_pos], ptr, size);
    m_size = m_new_size;
    m_pos += size;
    return size;
}

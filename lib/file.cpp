#include "file.h"

File::File(String file, FileMode mode)
{
    const char * modes[] = 
    {
        "rb",
        "wb",
        "w+b"
    };
    const char * mode_str;
    if(mode == ReadOnly)
        mode_str = modes[0];
    else if(mode == WriteOnly)
        mode_str = modes[1];
    else
        mode_str = modes[2];
    fp = fopen(file.c_str(), mode_str);
    if(fp == NULL)
        throw std::exception();
}

File::~File()
{
    fclose(fp);
}

size_t File::read(void* ptr, size_t size)
{
    return fread(ptr, 1, size, fp);
}

long File::position()
{
    return ftell(fp);
}

void File::seek(long pos, SeekOrigin origin)
{
    if(origin == SeekSet)
        fseek(fp, pos, SEEK_SET);
    else if(origin == SeekCur)
        fseek(fp, pos, SEEK_CUR);
    else if(origin == SeekEnd)
        fseek(fp, pos, SEEK_END);
}

size_t File::size()
{
    long pos;
    size_t size;
    
    pos = position();
    seek(0, SeekEnd);
    size = position();
    seek(pos);
    return size;
}

void File::reset()
{
    rewind(fp);
}

size_t File::write(const void* ptr, size_t size)
{
    return fwrite(ptr, 1, size, fp);
}

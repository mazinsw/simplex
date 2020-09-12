#ifndef _STRINGS_H_
#define _STRINGS_H_
#ifdef USE_STL_STRING
#include <string>
#endif
#include <vector>
#include <utility>
//#define USE_STL_STRING
//#define TEST_STRING

#ifndef USE_STL_STRING
/* Special string */
class String
{
private:
    char* m_buff;
    size_t m_length;
    size_t m_allocsize;
    size_t m_allocby;
    void realloc(size_t size, bool copy = true);
public:
    String();
    String(const String& ref);
    String(const char * s);
    String(const char * s, size_t n, size_t pos = 0);
    String(size_t n, char ch);
    ~String();
    /* Functions  */
    size_t length() const;
    void clear();
    const char * c_str() const;
    void format(const char * format, ...);
    /* Access */
    char& operator[](int pos) const;
    /* ? */
    const String operator+(char ch) const;
    const String operator+(const char * s);
    const String operator+(const String& ref) const;
    /* Assign */
    String& operator=(const char * s);
    String& operator=(const String& ref);
    /* Append */
    String& operator+=(char ch);
    String& operator+=(const char * s);
    String& operator+=(const String& ref);
    /* Compare */
    bool operator==(const String& ref) const;
    bool operator!=(const String& ref) const;
    bool operator<(const String& ref) const;
    bool operator>(const String& ref) const;
};

const String operator+(const char * s, const String &ref);
const String operator+(char ch, const String &ref);
#else
#  define String std::string
#endif

#ifndef TEST_STRING

class Strings
{
private:
    std::vector<std::pair<String, void*> > v;
public:
    int add(String s);
    int addObject(String s, void* ptr);
    void insert(int index, String s);
    void insertObject(int index, String s, void* ptr);
    int indexOf(String s);
    void erase(int index);
    void remove(String s);
    bool load(String file);
    bool save(String file);
    void setText(String text);
    String getText();
    int count();
    void clear();
    String& operator[](int index);
    Strings& operator+=(String& rsh);
    Strings& operator+=(const char* rsh);
    String get(int index);
    void* getObject(int index);
};
#endif

#endif

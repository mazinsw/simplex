#include "strings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef USE_STL_STRING
#define STRING_ALLOC_BY 64

const String operator+(const char * s, const String &ref)
{
    return String(s) + ref;
}

const String operator+(char ch, const String &ref)
{
    return String(1, ch) + ref;
}

/* String */
String::String()
{
    m_allocby = STRING_ALLOC_BY;
    m_allocsize = m_allocby + 1;
    m_buff = new char[m_allocsize];
    m_length = 0;
    m_buff[m_length] = 0;
}

String::String(const String& ref)
{
    m_allocby = ref.m_allocby;
    m_allocsize = ref.m_allocsize;
    m_length = ref.m_length;
    m_buff = new char[m_allocsize];
    memcpy(m_buff, ref.m_buff, sizeof(char) * m_length);
    m_buff[m_length] = 0;
}

String::String(const char * s)
{
    m_length = strlen(s);
    m_allocby = STRING_ALLOC_BY;
    m_allocsize = m_length + m_allocby + 1;
    m_buff = new char[m_allocsize];
    memcpy(m_buff, s, sizeof(char) * m_length);
    m_buff[m_length] = 0;
}

String::String(const char * s, size_t n, size_t pos)
{
    m_length = n;
    m_allocby = STRING_ALLOC_BY;
    m_allocsize = m_length + m_allocby + 1;
    m_buff = new char[m_allocsize];
    memcpy(m_buff, s + pos, sizeof(char) * m_length);
    m_buff[m_length] = 0;
    m_length = strlen(m_buff);
}

String::String(size_t n, char ch)
{
    register size_t i;

    m_length = n;
    m_allocby = STRING_ALLOC_BY;
    m_allocsize = m_length + m_allocby + 1;
    m_buff = new char[m_allocsize];
    for(i = 0; i < n; i++)
        m_buff[i] = ch;
    m_buff[m_length] = 0;
}

String::~String()
{
    delete[] m_buff;
}

void String::realloc(size_t size, bool copy)
{
    char * tmp_buff;

    if(size + m_allocby / 2 > m_allocsize || size + 2 * m_allocby < m_allocsize)
    {
        m_allocsize = size + m_allocby + 1;
        tmp_buff = new char[m_allocsize];
        if(copy)
            memcpy(tmp_buff, m_buff, sizeof(char) * (m_length + 1));
        delete[] m_buff;
        m_buff = tmp_buff;
    }
}

size_t String::length() const
{
    return m_length;
}

const char * String::c_str() const
{
    return m_buff;
}

void String::clear()
{
    realloc(0, false);
    m_length = 0;
    m_buff[m_length] = 0;
}

void String::format(const char * format, ... )
{
    va_list args;
    va_start(args, format);
    realloc(1024, false);
    m_length = vsprintf(m_buff, format, args);
    realloc(m_length);
    va_end(args);
}

/* Access */
char& String::operator[](int pos) const
{
    return m_buff[pos];
}

/* ? */
const String String::operator+(char ch) const
{
    return String(*this) += ch;
}

const String String::operator+(const char * s)
{
    return String(*this) += s;
}

const String String::operator+(const String& ref) const
{
    return String(*this) += ref;
}

/* Assign */
String& String::operator=(const char * s)
{
    m_length = strlen(s);
    realloc(m_length, false);
    memcpy(m_buff, s, sizeof(char) * m_length);
    m_buff[m_length] = 0;
    return *this;
}

String& String::operator=(const String& ref)
{
    m_length = ref.m_length;
    realloc(m_length, false);
    memcpy(m_buff, ref.m_buff, sizeof(char) * m_length);
    m_buff[m_length] = 0;
    return *this;
}

/* Append */
String& String::operator+=(char ch)
{
    if(ch == 0)
        return *this;
    realloc(m_length + 1);    
    m_buff[m_length] = ch;
    m_length++;
    m_buff[m_length] = 0;
    return *this;
}

String& String::operator+=(const char * s)
{
    size_t len;

    len = strlen(s);
    realloc(m_length + len);
    memcpy(m_buff + m_length, s, sizeof(char) * len);
    m_length += len;
    m_buff[m_length] = 0;
    return *this;
}

String& String::operator+=(const String& ref)
{
    realloc(m_length + ref.m_length);
    memcpy(m_buff + m_length, ref.m_buff, sizeof(char) * ref.m_length);
    m_length += ref.m_length;
    m_buff[m_length] = 0;
    return *this;
}

/* Compare */
bool String::operator==(const String& ref) const
{
    return strcmp(m_buff, ref.m_buff) == 0;
}

bool String::operator!=(const String& ref) const
{
    return !(*this == ref);
}

bool String::operator<(const String& ref) const
{
    return strcmp(m_buff, ref.m_buff) < 0;
}

bool String::operator>(const String& ref) const
{
    return strcmp(m_buff, ref.m_buff) > 0;
}
#endif /* USE_STL_STRING */

#ifdef TEST_STRING

int main(int argc, char *argv[])
{
    String a("My a string");
    String b = "My b string";
    String c;
    String d(a);

    c = a + b;
    printf("a: %s\nb: %s\nc: %s\nd: %s\n\n",
           a.c_str(), b.c_str(), c.c_str(), d.c_str());
    b = "insert into alunos (matricula, nome, email, senha, acesso) values ('" +
        a + "','" + c + "','" + d + "','" + d + "', 1)";
    c = a + " c_str" + d + c + 'A';
    d = "c_str " + c;
    a += ": number\n";
    a += ": real number\n";
    a += 'C' + String(": character\n");
    a += "S" + String(": string");
    a += 'C';
    printf("a: %s\nb: %s\nc: %s\nd: %s\n\n",
           a.c_str(), b.c_str(), c.c_str(), d.c_str());
    return 0;
}

#endif

#ifndef TEST_STRING
/* Strings */
int Strings::add(String s)
{
    return addObject(s, NULL);
}

int Strings::addObject(String s, void* ptr)
{
    v.push_back(std::pair<String, void*>(s, ptr));
    return count() - 1;
}

void Strings::insert(int index, String s)
{
    insertObject(index, s, NULL);
}

void Strings::insertObject(int index, String s, void* ptr)
{
    v.insert(v.begin() + index, 1, std::pair<String, void*>(s, ptr));
}

int Strings::indexOf(String s)
{
    std::vector<std::pair<String, void*> >::iterator ite;
    int i = 0;

    for(ite = v.begin(); ite != v.end(); ite++)
    {
        if(ite->first == s)
            return i;
        i++;
    }
    return -1;
}

void Strings::erase(int index)
{
    if(index >= 0 && index < count())
        v.erase(v.begin() + index);
}

void Strings::remove(String s)
{
    int index = indexOf(s);

    if(index < 0)
        return;
    erase(index);
}

int Strings::count()
{
    return v.size();
}

void Strings::clear()
{
    v.clear();
}

String& Strings::operator[](int index)
{
    return v[index].first;
}

Strings& Strings::operator+=(String& rsh)
{
    add(rsh);
    return *this;
}

Strings& Strings::operator+=(const char* rsh)
{
    add(String(rsh));
    return *this;
}

String Strings::get(int index)
{
    return v[index].first;
}

void* Strings::getObject(int index)
{
    return v[index].second;
}

bool Strings::load(String file)
{
    FILE *fp;
    long size, bread;
    char * text;

    fp = fopen(file.c_str(), "rb");
    if(fp == NULL)
        return false;
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    text = new char[size + 1];
    bread = fread(text, sizeof(char), size, fp);
    fclose(fp);
    text[size] = 0;
    if(bread == size)
        setText(text);
    delete[] text;
    return bread == size;
}

bool Strings::save(String file)
{
    FILE *fp;
    String text;
    size_t bwrite;

    fp = fopen(file.c_str(), "wb");
    if(fp == NULL)
        return false;
    text = getText();
    bwrite = fwrite(text.c_str(), sizeof(char), text.length(), fp);
    fclose(fp);
    return bwrite == text.length();
}

void Strings::setText(String text)
{
    clear();
    const char * s, *tmp;
    size_t n;
    int i = 0;

    s = text.c_str();
    tmp = s;
    while(s[0] != 0)
    {
        if(s[0] == '\r' || s[0] == '\n')
        {
            n = s - tmp;
            add(String(tmp, n));
            i++;
            if(s[1] == '\n')
                s++;
            tmp = s + 1;
            if(s[0] == 0)
                break;
        }
        s++;
    }
    if(s != tmp || i > 0)
    {
        n = s - tmp;
        add(String(tmp, n));
    }
}

String Strings::getText()
{
    String text;
    int i;

    if(count() > 0)
        text = get(0);
    for(i = 1; i < count(); i++)
        text += "\r\n" + get(i);
    return text;
}
#endif

//#define TEST_STRINGS
#ifdef TEST_STRINGS

int main(int argc, char *argv[])
{
    Strings s;
    char str[10];
    int i;
    String a = "Conteudo 1";
    String b = "Conteudo 1";

    if(a == b)
    {
        printf("A: %s eh igual a B: %s", a.c_str(), b.c_str());
    }
    else
        printf("A: %s eh diferente a B: %s", a.c_str(), b.c_str());
    s.add("string 1");
    for(i = 0; i < 5; i++)
    {
        sprintf(str, "%d", i + 2);
        s.add("string " + String(str));
    }
    s.erase(2);
    s.remove("string 5");
    s[1] = "string alterada";
    s += "adicionada com +=";
    s += "última linha";
    printf("count: %d\n", s.count());
    for(i = 0; i < s.count(); i++)
    {
        printf("string %d: %s\n", i, s[i].c_str());
    }
    if(!s.save("Strings save test.txt"))
        printf("Nao foi possivel salvar!\n");
    if(!s.load("src/strings.cpp"))
        printf("Nao foi possivel carregar o arquivo 'Strings load test.txt'!\n");
    printf("count: %d\n", s.count());

    printf("%s\n", s.getText().c_str());
    /*for(i = 0; i < s.count(); i++)
    {
        printf("%s\n", s[i].c_str());
    }*/
    if(!s.save("Strings save loaded text.txt"))
        printf("Nao foi possivel salvar!\n");
    s.setText("");
    printf("count: %d\n", s.count());
    return 0;
}

#endif


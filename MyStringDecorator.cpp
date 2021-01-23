//
// Created by kondr on 19.12.2020.
//

#include "MyStringDecorator.h"
#include <fstream>

using namespace std;

int MyStringDecorator::ReversFind(const std::string &substring) const
{
   return ReversFind(substring, m_pMyString->length() - substring.length());
}

int MyStringDecorator::ReversFind(const std::string &substring, t_dword index) const
{
    if (substring.length() > m_pMyString->length())
        return -1;
    int i;
    if (m_pMyString->length() - index >= substring.length())
        i = index;
    else
        i = m_pMyString->length() - substring.length();
    for (; i >= 0; --i)
    {
        t_dword j = 0;
        while ((*m_pMyString)[i + j] == substring[j])
            ++j;
        if (j >= substring.length())
            return i;
    }
    return -1;
}

MyStringDecorator::MyStringDecorator()
{
    m_pMyString = new MyString();
}

MyStringDecorator::MyStringDecorator(std::string &string)
{
    m_pMyString = new MyString(string);
}

MyStringDecorator::MyStringDecorator(const char *base_string)
{
    m_pMyString = new MyString(base_string);
}

MyStringDecorator::MyStringDecorator(t_dword count, char symbol)
{
    m_pMyString = new MyString(count, symbol);
}

MyStringDecorator::MyStringDecorator(std::initializer_list<char> list)
{
    m_pMyString = new MyString(list);
}

MyStringDecorator::MyStringDecorator(const MyStringDecorator &base_string_object)
{
    m_pMyString = new MyString(*base_string_object.m_pMyString);
}

MyStringDecorator::MyStringDecorator(const MyString& base_string_object)
{
    m_pMyString = new MyString(base_string_object);
}

MyStringDecorator::MyStringDecorator(const char *base_string, t_dword count)
{
    m_pMyString = new MyString(base_string, count);
}

MyStringDecorator::~MyStringDecorator()
{
    delete m_pMyString;
}

void MyStringDecorator::Insert(t_dword index, const char *string)
{
    m_pMyString->Insert(index, string);
}

void MyStringDecorator::Insert(t_dword index, std::string string)
{
    m_pMyString->Insert(index, string);
}

void MyStringDecorator::Insert(t_dword index, t_dword count, char symbol)
{
    m_pMyString->Insert(index, count, symbol);
}

void MyStringDecorator::Insert(t_dword index, const char *string, t_dword count)
{
    m_pMyString->Insert(index, string, count);
}

void MyStringDecorator::Insert(t_dword index, std::string string, t_dword count)
{
    m_pMyString->Insert(index, string, count);
}

void MyStringDecorator::Erase(t_dword index, t_dword count)
{
    m_pMyString->Erase(index, count);
}

void MyStringDecorator::Append(const char *string)
{
    m_pMyString->Append(string);
}

void MyStringDecorator::Append(std::string string)
{
    m_pMyString->Append(string);
}

void MyStringDecorator::Append(t_dword count, char symbol)
{
    m_pMyString->Append(count, symbol);
}

void MyStringDecorator::Append(const char *string, t_dword index, t_dword count)
{
    m_pMyString->Append(string, index, count);
}

void MyStringDecorator::Append(std::string string, t_dword index, t_dword count)
{
    m_pMyString->Append(string, index, count);
}

void MyStringDecorator::Replace(t_dword index, t_dword count, const char *string)
{
    m_pMyString->Replace(index, count, string);
}

void MyStringDecorator::Replace(t_dword index, t_dword count, std::string string)
{
    m_pMyString->Replace(index, count, string);
}

const char * MyStringDecorator::SubStr(t_dword index) const
{
    return m_pMyString->SubStr(index);
}

const char * MyStringDecorator::SubStr(t_dword index, t_dword count) const
{
    return m_pMyString->SubStr(index, count);
}

int MyStringDecorator::Find(const char *substring) const
{
    return m_pMyString->Find(substring);
}

int MyStringDecorator::Find(const std::string substring) const
{
    return m_pMyString->Find(substring);
}

int MyStringDecorator::Find(const char *substring, t_dword index) const
{
    return m_pMyString->Find(substring, index);
}

int MyStringDecorator::Find(const std::string substring, t_dword index) const
{
    return m_pMyString->Find(substring, index);
}

MyStringDecorator MyStringDecorator::operator+(const char *string) const
{
    return MyStringDecorator(*m_pMyString + string);
}

MyStringDecorator MyStringDecorator::operator+(const std::string &string) const
{
    return MyStringDecorator(*m_pMyString + string);
}

MyStringDecorator MyStringDecorator::operator+(const MyStringDecorator &string_object) const
{
    return MyStringDecorator(*m_pMyString + *string_object.m_pMyString);
}

MyStringDecorator & MyStringDecorator::operator=(char symbol){
    *m_pMyString = symbol;
    return *this;
}

MyStringDecorator & MyStringDecorator::operator=(const char *string){
    *m_pMyString = string;
    return *this;
}

MyStringDecorator & MyStringDecorator::operator=(std::string &string){
    *m_pMyString = string;
    return *this;
}

void MyStringDecorator::operator+=(const char *string){
    *m_pMyString += string;
}

void MyStringDecorator::operator+=(std::string string){
    *m_pMyString += string;
}

char MyStringDecorator::operator[](t_dword index) const
{
    return (*m_pMyString)[index];
}

bool MyStringDecorator::operator<(MyStringDecorator &string_object) const
{
    return *m_pMyString < *string_object.m_pMyString;
}

bool MyStringDecorator::operator>(MyStringDecorator &string_object) const
{
    return *m_pMyString > *string_object.m_pMyString;
}

bool MyStringDecorator::operator==(MyStringDecorator &string_object) const
{
    return *m_pMyString == *string_object.m_pMyString;
}

bool MyStringDecorator::operator!=(MyStringDecorator &string_object) const
{
    return *m_pMyString != *string_object.m_pMyString;
}

bool MyStringDecorator::operator<=(MyStringDecorator &string_object) const
{
    return *m_pMyString <= *string_object.m_pMyString;
}

bool MyStringDecorator::operator>=(MyStringDecorator &string_object) const
{
    return *m_pMyString >= *string_object.m_pMyString;
}

const char * MyStringDecorator::c_str() const
{
    return m_pMyString->c_str();
}

const void * MyStringDecorator::data() const
{
    return m_pMyString->data();
}

t_dword MyStringDecorator::size() const
{
    return m_pMyString->size();
}

t_dword MyStringDecorator::length() const
{
    return m_pMyString->length();
}

bool MyStringDecorator::empty() const
{
    return m_pMyString->empty();
}

t_dword MyStringDecorator::capacity() const
{
    return m_pMyString->capacity();
}

void MyStringDecorator::shrink_to_fit()
{
    m_pMyString->shrink_to_fit();
}

void MyStringDecorator::clear()
{
    m_pMyString->clear();
}

std::ostream& operator<<(std::ostream& os, MyStringDecorator& obj)
{
    return os << *obj.m_pMyString;
}

std::istream& operator>>(std::istream& is, MyStringDecorator& obj)
{
    return is >> *obj.m_pMyString;
}
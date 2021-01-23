#include "MyString.h"

MyString::MyString()
{
    a_data_ = new char[DEFAULT_BLOCK];
    a_trash_ = new char[DEFAULT_BLOCK];
    a_allocated_memory = DEFAULT_BLOCK;
    a_length = 0;
}

MyString::MyString(const MyString &base_string_object)
{
    a_allocated_memory = base_string_object.capacity();
    a_length = base_string_object.length();
    a_data_ = new char[a_allocated_memory];
    memcpy(a_data_, base_string_object.c_str(), a_length);
    a_trash_ = new char[DEFAULT_BLOCK];
}

MyString::MyString(const char *base_string)
{
    a_length = StrLen(base_string);
    a_allocated_memory = ((a_length / DEFAULT_BLOCK) + 1) * DEFAULT_BLOCK;
    a_data_ = new char[a_allocated_memory];
    memcpy(a_data_, base_string, a_length);
    a_trash_ = new char[DEFAULT_BLOCK];
}

MyString::MyString(std::initializer_list<char> list)
{
    a_length = list.size();
    a_allocated_memory = ((a_length / DEFAULT_BLOCK) + 1) * DEFAULT_BLOCK;
    a_data_ = new char[a_allocated_memory];
    std::initializer_list<char>::iterator it;
    t_dword i = 0;
    for (it = list.begin(); it != list.end(); ++it)
    {
        a_data_[i++] = *it;
    }
    a_trash_ = new char[DEFAULT_BLOCK];
}

MyString::MyString(std::string string)
{
    a_length = string.length();
    a_allocated_memory = ((a_length / DEFAULT_BLOCK) + 1) * DEFAULT_BLOCK;
    a_data_ = new char[a_allocated_memory];
    memcpy(a_data_, string.c_str(), a_length);
    a_trash_ = new char[DEFAULT_BLOCK];
}

MyString::MyString(const char *base_string, t_dword count)
{
    a_length = (count > StrLen(base_string)) ? StrLen(base_string) : count;
    a_allocated_memory = ((a_length / DEFAULT_BLOCK) + 1) * DEFAULT_BLOCK;
    a_data_ = new char[a_allocated_memory];
    memcpy(a_data_, base_string, a_length);
    a_trash_ = new char[DEFAULT_BLOCK];
}

MyString::MyString(t_dword count, char symbol)
{
    a_length = count;
    a_allocated_memory = ((a_length / DEFAULT_BLOCK) + 1) * DEFAULT_BLOCK;
    a_data_ = new char[a_allocated_memory];
    StrFill(a_data_, count, symbol);
    a_trash_ = new char[DEFAULT_BLOCK];
}

MyString::~MyString()
{
    delete[] a_data_;
    delete[] a_trash_;
}

void MyString::Insert(t_dword index, const char *string)
{
    if (index > a_length)
    {
        return;
    }
    t_dword string_length = StrLen(string);

    if (a_allocated_memory < a_length + string_length)
    {
        a_allocated_memory = (((a_length + string_length)/DEFAULT_BLOCK) + 1) * DEFAULT_BLOCK;
        char * temp = new char[a_allocated_memory];
        memcpy(temp, a_data_, a_length);
        delete[] a_data_;
        a_data_ = temp;
    }
    if (a_length > index)
        memmove(&a_data_[index + string_length], &a_data_[index], a_length - index);
    memcpy(&a_data_[index], string, string_length);
    a_length += string_length;
}

void MyString::Insert(t_dword index, t_dword count, char symbol)
{
    if (index > a_length)
    {
        return;
    }
    if (a_allocated_memory < a_length + count)
    {
        a_allocated_memory = (((a_length + count)/DEFAULT_BLOCK) + 1) * DEFAULT_BLOCK;
        char * temp = new char[a_allocated_memory];
        memcpy(temp, a_data_, a_length);
        delete[] a_data_;
        a_data_ = temp;
    }
    if (a_length > index)
        memmove(&a_data_[index + count], &a_data_[index], a_length - index);
    StrFill(&a_data_[index], count, symbol);
    a_length += count;
}

void MyString::Insert(t_dword index, const char *string, t_dword count)
{
    char *temp = new char[count + 1];
    memcpy(temp, string, count);
    temp[count] = '\0';
    Insert(index, temp);
    delete[] temp;
}

void MyString::Insert(t_dword index, std::string string)
{
    Insert(index, string.c_str());
}

void MyString::Insert(t_dword index, std::string string, t_dword count)
{
    Insert(index, string.c_str(), count);
}

void MyString::Erase(t_dword index, t_dword count)
{
    if (index >= a_length)
    {
        return;
    }
    if (count >= a_length - index) {
        a_length -= a_length - index;
        return;
    }
    memmove(&a_data_[index], &a_data_[index + count], a_length - (index + count));
    a_length -= count;
}

void MyString::Append(const char *string)
{
    Insert(a_length, string);
}

void MyString::Append(t_dword count, char symbol)
{
    Insert(a_length, count, symbol);
}

void MyString::Append(const char *string, t_dword index, t_dword count)
{
    if (index >= StrLen(string))
    {
        return;
    }
    Insert(a_length, &string[index], count);
}

void MyString::Append(std::string string)
{
    Insert(a_length, string.c_str());
}

void MyString::Append(std::string string, t_dword index, t_dword count)
{
    Append(string.c_str(), index, count);
}

void MyString::Replace(t_dword index, t_dword count, const char *string)
{
    Erase(index, count);
    Insert(index, string);
}

void MyString::Replace(t_dword index, t_dword count, std::string string)
{
    Erase(index, count);
    Insert(index, string.c_str());
}

const char * MyString::SubStr(t_dword index, t_dword count) const
{
    if (index >= a_length)
    {
        return nullptr;
    }
    delete[] a_trash_;
    a_trash_ = new char[count + 1];
    memcpy(a_trash_, &a_data_[index], count);
    a_trash_[count] = '\0';
    return a_trash_;
}

const char * MyString::SubStr(t_dword index) const
{
    if (index >= a_length)
    {
      return nullptr;
    }
    delete[] a_trash_;
    a_trash_ = new char[a_length - index + 1];
    memcpy(a_trash_, &a_data_[index], a_length - index);
    a_trash_[a_length - index] = '\0';
    return a_trash_;
}

int MyString::Find(const char *substring, t_dword index) const
{
    t_dword substring_length = StrLen(substring);
    if ((index >= a_length) || (substring_length > a_length))
    {
        return -1;
    }
    t_dword i = index;
    for (; i <= a_length - substring_length; ++i)
    {
        t_dword j = 0;
        while (a_data_[i + j] == substring[j])
            ++j;
        if (j >= substring_length)
            return i;
    }
    return -1;
}

int MyString::Find(const char *substring) const
{
    return Find(substring, 0);
}

int MyString::Find(std::string substring, t_dword index) const
{
    return Find(substring.c_str(), index);
}

int MyString::Find(std::string substring) const
{
    return Find(substring.c_str(), 0);
}

MyString MyString::operator+(const char *string) const
{
    char *temp = new char[a_length + StrLen(string) + 1];
    memcpy(temp, a_data_, a_length);
    memcpy(temp + a_length, string, StrLen(string) + 1);
    MyString r_obj(temp);
    delete[] temp;
    return r_obj;
}

MyString MyString::operator+(const std::string string) const
{
    return operator +(string.c_str());
}

MyString MyString::operator+(const MyString &string_object) const
{
    return operator +(string_object.c_str());
}

MyString &MyString::operator=(const char *string) {
    Replace(0, a_length, string);
    return *this;
}

MyString &MyString::operator=(std::string string) {
    Replace(0, a_length, string.c_str());
    return *this;
}

MyString &MyString::operator=(char symbol) {
    char temp[2];
    temp[0] = symbol;
    temp[1] = '\0';
    Replace(0, a_length, temp);
    return *this;
}

void MyString::operator+=(const char *string) {
    Append(string);
}

void MyString::operator+=(std::string string) {
    Append(string);
}

char MyString::operator[](t_dword index) const
{
    if (index >= a_length)
    {
        return '\0';
    }
    return a_data_[index];
}

bool MyString::operator<(MyString &string_object) const
{
    t_dword i = 0;
    for(;i < a_length; ++i)
    {
        if ((string_object[i] == '\0') || (a_data_[i] > string_object[i]))
            return false;
        if (a_data_[i] < string_object[i])
            return true;
    }
    return (string_object[i] != '\0');
}

bool MyString::operator>(MyString &string_object) const
{
    t_dword i = 0;
    for(;i < a_length; ++i)
    {
        if ((string_object[i] == '\0') || (a_data_[i] > string_object[i]))
            return true;
        if (a_data_[i] < string_object[i])
            return false;
    }
    return false;
}

bool MyString::operator<=(MyString &string_object) const
{
    return !operator>(string_object);
}

bool MyString::operator>=(MyString &string_object) const
{
    return !operator<(string_object);
}

bool MyString::operator==(MyString &string_object) const
{
    t_dword i = 0;
    for(;i < a_length; ++i)
    {
        if (a_data_[i] != string_object[i])
            return false;
    }
    return (string_object[i] == '\0');
}

bool MyString::operator!=(MyString &string_object) const
{
    return !operator==(string_object);
}

const char * MyString::c_str() const
{
    delete[] a_trash_;
    a_trash_ = new char[a_length + 1];
    memcpy(a_trash_, a_data_, a_length);
    a_trash_[a_length] = '\0';
    return a_trash_;
}

const void * MyString::data() const
{
    return a_data_;
}

t_dword MyString::size() const
{
    return a_length;
}

t_dword MyString::length() const
{
    return a_length;
}

bool MyString::empty() const
{
    return (a_length == 0);
}

t_dword MyString::capacity() const
{
    return a_allocated_memory;
}

void MyString::shrink_to_fit()
{
    if (a_allocated_memory != a_length + 1)
    {
        a_allocated_memory = a_length + 1;
        char * temp = new char[a_allocated_memory];
        memcpy(temp, a_data_, a_length);
        delete[] a_data_;
        a_data_ = temp;
    }
}

void MyString::clear()
{
    Erase(0, a_length);
}

std::ostream& operator<<(std::ostream& os, MyString& obj)
{
    return os << obj.c_str();
}

std::istream& operator>>(std::istream& is, MyString& obj)
{
    char BUFF[2048];

    is.getline(BUFF, sizeof BUFF);
    obj = BUFF;

    return is;
}

void MyString::StrFill(char *pointer, t_dword count, char symbol)
{
    t_dword i = 0;
    for (; i < count; ++i)
    {
        pointer[i] = symbol;
    }
}

t_dword MyString::StrLen(const char *str) const
{
    t_dword count = 0;
    while (str[count++] != '\0');
    return count - 1;
}
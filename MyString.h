//
// Created by kondr on 10.10.2020.
//

#ifndef MYSTRING_MYSTRING_H
#define MYSTRING_MYSTRING_H
#include <memory.h>
#include <initializer_list>
#include <string>
#include <iostream>

typedef unsigned int t_dword;
const t_dword DEFAULT_BLOCK = 100; //size of memory block that dynamically allocated for a string

class IMyString
{
public:
    virtual void Insert(t_dword index, const char * string) = 0;
    virtual void Insert(t_dword index, t_dword count, char symbol) = 0;
    virtual void Insert(t_dword index, const char * string, t_dword count) = 0;
    virtual void Insert(t_dword index, std::string string) = 0;
    virtual void Insert(t_dword index, std::string string, t_dword count) = 0;

    virtual void Erase(t_dword index, t_dword count) = 0;

    virtual void Append(const char * string) = 0;
    virtual void Append(t_dword count, char symbol) = 0;
    virtual void Append(const char * string, t_dword index, t_dword count) = 0;
    virtual void Append(std::string string) = 0;
    virtual void Append(std::string string, t_dword index, t_dword count) = 0;

    virtual void Replace(t_dword index, t_dword count, const char * string) = 0;
    virtual void Replace(t_dword index, t_dword count, std::string string) = 0;

    virtual const char * SubStr(t_dword index) const = 0;
    virtual const char * SubStr(t_dword index, t_dword count) const = 0;

    virtual int Find(const char * substring, t_dword index) const = 0;
    virtual int Find(const char * substring) const = 0;
    virtual int Find(std::string substring, t_dword index) const = 0;
    virtual int Find(std::string substring) const = 0;

    virtual void operator +=(const char * string) = 0;
    virtual void operator +=(std::string string) = 0;

    virtual char operator [](t_dword index) const = 0;

    virtual const char * c_str() const = 0;
    virtual const void * data() const = 0;
    virtual t_dword size() const = 0;
    virtual t_dword length() const = 0;
    virtual bool empty() const = 0;
    virtual t_dword capacity() const = 0;
    virtual void shrink_to_fit() = 0;
    virtual void clear() = 0;

    virtual ~IMyString(){};
};

class MyString : public IMyString
{
public:
    //Constructors
    MyString();
    MyString(const MyString &base_string_object);
    MyString(const char* base_string);
    MyString(std::initializer_list<char> list);
    MyString(std::string string);
    MyString(const char* base_string, t_dword count);
    MyString(t_dword count, char symbol);
    //Destructor
    ~MyString();

    void Insert(t_dword index, const char * string) override;
    void Insert(t_dword index, t_dword count, char symbol) override;
    void Insert(t_dword index, const char * string, t_dword count) override;
    void Insert(t_dword index, std::string string) override;
    void Insert(t_dword index, std::string string, t_dword count) override;

    void Erase(t_dword index, t_dword count) override;

    void Append(const char * string) override;
    void Append(t_dword count, char symbol) override;
    void Append(const char * string, t_dword index, t_dword count) override;
    void Append(std::string string) override;
    void Append(std::string string, t_dword index, t_dword count) override;

    void Replace(t_dword index, t_dword count, const char * string) override;
    void Replace(t_dword index, t_dword count, std::string string) override;

    const char * SubStr(t_dword index) const override;
    const char * SubStr(t_dword index, t_dword count) const override;

    int Find(const char * substring, t_dword index) const override;
    int Find(const char * substring) const override;
    int Find(std::string substring, t_dword index) const override;
    int Find(std::string substring) const override;

    void operator +=(const char * string) override;
    void operator +=(std::string string) override;

    char operator [](t_dword index) const override;

    const char * c_str() const override;
    const void * data() const override;
    t_dword size() const override;
    t_dword length() const override;
    bool empty() const override;
    t_dword capacity() const override;
    void shrink_to_fit() override;
    void clear() override;

    MyString operator +(const MyString &string_object) const;
    MyString operator +(const char * string) const;
    MyString operator +(const std::string string) const;

    MyString &operator =(const char * string);
    MyString &operator =(std::string string);
    MyString &operator =(char symbol);

    bool operator <(MyString &string_object) const;
    bool operator >(MyString &string_object) const;
    bool operator ==(MyString &string_object) const;
    bool operator !=(MyString &string_object) const;
    bool operator <=(MyString &string_object) const;
    bool operator >=(MyString &string_object) const;

    friend std::ostream& operator<<(std::ostream&, MyString&);
    friend std::istream& operator>>(std::istream&, MyString&);

private:
    //Filling area of memory started from pointer with count of symbols
    void StrFill(char *pointer, t_dword count, char symbol);
    //Return length of the null-terminated string
    t_dword StrLen(const char *str) const;
    char * a_data_; //Pointer to the string data
    mutable char * a_trash_; //Pointer to the additional array
    t_dword a_allocated_memory; //Current amount of allocated memory in bytes
    t_dword a_length; //Current length of the string
};

#endif //MYSTRING_MYSTRING_H


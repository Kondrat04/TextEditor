//
// Created by kondr on 19.12.2020.
//

#ifndef TEXTEDITOR_MYSTRINGDECORATOR_H
#define TEXTEDITOR_MYSTRINGDECORATOR_H
#include "MyString.h"

typedef unsigned int t_dword;

class MyStringDecorator : public IMyString{
public:
    MyStringDecorator();
    MyStringDecorator(const MyStringDecorator &base_string_object);
    MyStringDecorator(const MyString& base_string_object);
    MyStringDecorator(const char* base_string);
    MyStringDecorator(std::initializer_list<char> list);
    MyStringDecorator(std::string &string);
    MyStringDecorator(const char* base_string, t_dword count);
    MyStringDecorator(t_dword count, char symbol);
    //Destructor
    ~MyStringDecorator();


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
    int Find(const std::string substring, t_dword index) const override;
    int Find(const std::string substring) const override;

    int ReversFind(const std::string &substring, t_dword index) const;
    int ReversFind(const std::string &substring) const;

    MyStringDecorator operator +(const MyStringDecorator &string_object) const;
    MyStringDecorator operator +(const char * string) const;
    MyStringDecorator operator +(const std::string &string) const;

    MyStringDecorator &operator =(const char * string);
    MyStringDecorator &operator =(std::string &string);
    MyStringDecorator &operator =(char symbol);

    void operator +=(const char * string) override;
    void operator +=(std::string string) override;

    char operator [](t_dword index) const override;

    bool operator <(MyStringDecorator &string_object) const;
    bool operator >(MyStringDecorator &string_object) const;
    bool operator ==(MyStringDecorator &string_object) const;
    bool operator !=(MyStringDecorator &string_object) const;
    bool operator <=(MyStringDecorator &string_object) const;
    bool operator >=(MyStringDecorator &string_object) const;

    const char * c_str() const override;
    const void * data() const override;
    t_dword size() const override;
    t_dword length() const override;
    bool empty() const override;
    t_dword capacity() const override;
    void shrink_to_fit() override;
    void clear() override;

    friend std::ostream& operator<<(std::ostream&, MyStringDecorator&);
    friend std::istream& operator>>(std::istream&, MyStringDecorator&);
private:
    MyString *m_pMyString;
};


#endif //TEXTEDITOR_MYSTRINGDECORATOR_H

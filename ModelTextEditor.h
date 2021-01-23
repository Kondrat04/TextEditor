//
// Created by kondr on 05.12.2020.
//

#ifndef TEXTEDITOR_MODELTEXTEDITOR_H
#define TEXTEDITOR_MODELTEXTEDITOR_H
#include "MyStringDecorator.h"
#include "ViewTextEditor.h"
#include "IPublisher.h"
#include <list>

class ViewTextEditor;
using namespace std;

class ModelTextEditor : IPublisher
{
public:
    ModelTextEditor() : m_cursor_position_({0, 0}), m_lines_(0), m_insert_mode_(false), m_edited_(false), m_revers_search_(false)
    {
        m_buffer_ = new MyStringDecorator();
    }
    ~ModelTextEditor()
    {
        for (auto line : m_data_)
            delete line;
        delete m_buffer_;
    }
    void SetFilePath(string file_path);
    void AddLine(string str);
    int ExecuteCommand(const char *command);
    int ExecuteCommand(char command);
    const char *m_getLine(SHORT index);
    void AddSubscriber(ISubscriber* subscriber) override;
    void RemoveSubscriber() override;
    void NotifyCursorUpdate(COORD position) override;
    void NotifyStatusBarUpdate(SHORT lines_total, string filename, bool insert_mode, const char* message) override;
    void NotifyScreenBufferInfoUpdate(bool up) override;
    void NotifyScreenBufferInfoUpdate(SHORT start_line, SHORT lines_count, SHORT shift) override;
    void NotifyLineUpdate(SHORT index, const char* line) override;
    void NotifyNewFileUpdate(ifstream& in, string& filepath) override;
private:
    void insert_line(DWORD index, string str);
    void erase_line(DWORD index);
    DWORD navigate_command(char command);
    bool navigate_help_file(char command);
    void move_cursor(COORD position);
    void word_detect(WORD &begin_index, WORD &end_index);
    DWORD save_file();
    void direct_search(const string& search_string);
    void revers_search(const string& search_string);
    void insert_mode();
    WORD help_mode();
    bool insert_handler(char symbol);
    bool help_handler(char symbol);
    string m_file_path_;
    string m_file_name_;
    string m_last_search_;
    bool m_insert_mode_;
    COORD m_cursor_position_;
    DWORD m_lines_;
    bool m_edited_;
    bool m_revers_search_;
    vector<MyStringDecorator*> m_data_;
    MyStringDecorator *m_buffer_;
    ISubscriber* m_pSubscriber_;
};


#endif //TEXTEDITOR_MODELTEXTEDITOR_H

//
// Created by kondr on 21.12.2020.
//

#ifndef TEXTEDITOR_STATUSLINE_H
#define TEXTEDITOR_STATUSLINE_H
#include <string>
#include <windows.h>

using namespace std;

const SHORT cStatusLineNumber = 29;

class StatusLine {
public:
    StatusLine(HANDLE hConsoleOut, string filename = "", DWORD lines_total = 1);
    string getFileName();
    DWORD getLinesTotal();
    DWORD getCurrentLine();
    void setFileName(string filename);
    void setLinesTotal(DWORD lines_total);
    void setCurrentLine(DWORD line_number);
    void setText(const char *text);
    void setMode(BYTE mode);
    void RefreshBar();
private:
    HANDLE m_hConsoleOut_;
    string m_filename_;
    BYTE m_mode_;
    string m_command_text_;
    DWORD m_lines_total_;
    DWORD m_current_line_;
};


#endif //TEXTEDITOR_STATUSLINE_H

//
// Created by kondr on 21.12.2020.
//

#include "StatusLine.h"
#include <iostream>

StatusLine::StatusLine(HANDLE hConsoleOut, string filename, DWORD lines_total)
{
    m_hConsoleOut_ = hConsoleOut;
    m_filename_ = filename;
    m_command_text_ = "";
    m_lines_total_ = lines_total;
    m_mode_ = 0;
    m_current_line_ = 1;
}

string StatusLine::getFileName()
{
    return m_filename_;
}

DWORD StatusLine::getLinesTotal()
{
    return m_lines_total_;
}

DWORD StatusLine::getCurrentLine()
{
    return m_current_line_;
}

void StatusLine::setFileName(string filename)
{
    m_filename_ = filename;
}

void StatusLine::setLinesTotal(DWORD lines_total)
{
    m_lines_total_ = lines_total;
}

void StatusLine::setCurrentLine(DWORD line_number)
{
    m_current_line_ = line_number;
}

void StatusLine::setMode(BYTE mode)
{
    m_mode_ = mode;
}

void StatusLine::setText(const char *text)
{
    m_command_text_ = text;
}

void StatusLine::RefreshBar()
{
    DWORD l;
    COORD old_position;
    CONSOLE_SCREEN_BUFFER_INFO cbsi;
    GetConsoleScreenBufferInfo(m_hConsoleOut_, &cbsi);
    old_position = cbsi.dwCursorPosition;
    FillConsoleOutputAttribute(m_hConsoleOut_, 0, 120, {0, static_cast<SHORT>(cbsi.srWindow.Top + cStatusLineNumber)}, &l);
    SetConsoleCursorPosition(m_hConsoleOut_, {0, static_cast<SHORT>(cbsi.srWindow.Top + cStatusLineNumber)});
    switch (m_mode_)
    {
        case 0:
        {
            cout << "Navigation/Editing";
            break;
        }
        case 1:
        {
            cout << "==INSERT==";
            break;
        }
        case 2:
        {
            cout << m_command_text_;
            break;
        }
    }
    SetConsoleCursorPosition(m_hConsoleOut_, {50, static_cast<SHORT>(cbsi.srWindow.Top + cStatusLineNumber)});
    cout << m_filename_;
    SetConsoleCursorPosition(m_hConsoleOut_, {100, static_cast<SHORT>(cbsi.srWindow.Top + cStatusLineNumber)});
    cout << "Line : " << m_current_line_ << "/" << m_lines_total_;
    SetConsoleCursorPosition(m_hConsoleOut_, old_position);
}
//
// Created by kondr on 06.12.2020.
//

#include "ViewTextEditor.h"

ViewTextEditor::ViewTextEditor()
{
    system("cls");
    m_hConsoleOut_ = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(m_hConsoleOut_, {0, 0});
    m_pStatusLine_ = new StatusLine(m_hConsoleOut_);
}

ViewTextEditor::~ViewTextEditor()
{
    delete m_pStatusLine_;
}

void ViewTextEditor::SetController(ControllerTextEditor* controller)
{
    m_pControllerTextEditor_ = controller;
}

void ViewTextEditor::WaitForCommand()
{
    m_hConsoleIn_ = GetStdHandle( STD_INPUT_HANDLE );
    GetConsoleMode( m_hConsoleIn_, &m_console_mode_);
    SetConsoleMode( m_hConsoleIn_, 0 );
    screen_info_refresh();
    char command;
    char buffer[100] = "";
    DWORD i = 0;
    bool global_exit_flag = false;
    while(true)
    {
        m_pStatusLine_->RefreshBar();
        command = _getch();
        if (command == -32) {
            m_pControllerTextEditor_->ExecuteCommand(command);
            memset(buffer, 0, 100);
            i = 0;
            continue;
        }
        if ((command == '0') && (i == 0))
        {
            m_pControllerTextEditor_->ExecuteCommand(command);
            continue;
        }
        if (isdigit(command))
        {
            buffer[i++] = command;
            continue;
        } else if (i > 0)
        {
            if (command == 'G')
            {
                buffer[i] = command;
                m_pControllerTextEditor_->ExecuteCommand(buffer);
            }
            memset(buffer, 0, 100);
            i = 0;
            continue;
        }
        switch (command)
        {
            case 'g':
            {
                command = _getch();
                if (command == 'g')
                {
                    buffer[i++] = command;
                    buffer[i] = command;
                    m_pControllerTextEditor_->ExecuteCommand(buffer);
                    memset(buffer, 0, 100);
                    i = 0;
                }
                continue;
            }
            case 'd':
            {
                buffer[i++] = command;
                command = _getch();
                if (command == 'd')
                {
                    buffer[i] = command;
                    m_pControllerTextEditor_->ExecuteCommand(buffer);

                } else if (command == 'i')
                {
                    buffer[i++] = command;
                    command = _getch();
                    if (command == 'w')
                    {
                        buffer[i] = command;
                        m_pControllerTextEditor_->ExecuteCommand(buffer);
                    }
                }
                memset(buffer, 0, 100);
                i = 0;
                continue;
            }
            case 'y':
            {
                char command2 = _getch();
                if (command2 == 'w')
                {
                    buffer[i++] = command;
                    buffer[i] = command2;
                    m_pControllerTextEditor_->ExecuteCommand(buffer);
                    memset(buffer, 0, 100);
                    i = 0;
                }
                else
                    m_pControllerTextEditor_->ExecuteCommand(command);
                continue;
            }
            case ':':
            {
                m_pStatusLine_->setMode(2);
                buffer[i++] = ':';
                m_pStatusLine_->setText(buffer);
                char command2;
                bool exit_flag = false;
                while(true)
                {
                    m_pStatusLine_->RefreshBar();
                    command2 = _getch();
                    switch(command2)
                    {
                        case 27: //esc
                        {
                            memset(buffer, 0, 100);
                            i = 0;
                            exit_flag = true;
                            break;
                        }
                        case 13: //enter
                        {
                            if (m_pControllerTextEditor_->ExecuteCommand(buffer) == -1)
                                global_exit_flag = true;
                            memset(buffer, 0, 100);
                            i = 0;
                            exit_flag = true;
                            break;
                        }
                        //TODO: обработать нажатие backspace в пустой строке
                        case 8: //backspase
                        {
                            buffer[--i] = '\0';
                            m_pStatusLine_->setText(buffer);
                            break;
                        }
                        default:
                            buffer[i++] = command2;
                            m_pStatusLine_->setText(buffer);
                    }
                    if (exit_flag)
                    {
                        m_pStatusLine_->setMode(0);
                        m_pStatusLine_->RefreshBar();
                        break;
                    }
                }
                break;
            }
            case '?':
            case '/':
            {
                m_pStatusLine_->setMode(2);
                if (command == '/')
                    buffer[i++] = '/';
                else
                    buffer[i++] = '?';
                m_pStatusLine_->setText(buffer);
                char command2;
                bool exit_flag = false;
                while(true)
                {
                    m_pStatusLine_->RefreshBar();
                    command2 = _getch();
                    switch(command2)
                    {
                        case 27: //esc
                        {
                            memset(buffer, 0, 100);
                            i = 0;
                            exit_flag = true;
                            break;
                        }
                        case 13: //enter
                        {
                            m_pControllerTextEditor_->ExecuteCommand(buffer);
                            memset(buffer, 0, 100);
                            i = 0;
                            exit_flag = true;
                            break;
                        }
                        //TODO: обработать нажатие backspace в пустой строке
                        case 8: //backspase
                        {
                            buffer[--i] = '\0';
                            m_pStatusLine_->setText(buffer);
                            break;
                        }
                        default:
                            buffer[i++] = command2;
                            m_pStatusLine_->setText(buffer);
                    }
                    if (exit_flag)
                    {
                        m_pStatusLine_->setMode(0);
                        m_pStatusLine_->RefreshBar();
                        break;
                    }
                }
                break;
            }
            default:
            {
                m_pControllerTextEditor_->ExecuteCommand(command);
            }
        }
        if (global_exit_flag)
            break;
    }
    SetConsoleMode(m_hConsoleIn_, m_console_mode_);
    system("cls");
}

void ViewTextEditor::screen_info_refresh()
{
    if (!GetConsoleScreenBufferInfo(m_hConsoleOut_, &m_csbiInfo_))
    {
        printf("GetConsoleScreenBufferInfo failed %d\n", GetLastError());
        return;
    }
}

void ViewTextEditor::OpenExistingFile(ifstream &in, string filepath)
{
    system("cls");
    string line, all_lines;
    DWORD lines_count = 0;
    while (getline(in, line))
    {
        cout << line << endl;
        m_pControllerTextEditor_->AddLine(line);
        ++lines_count;
    }
    MyStringDecorator filename = filepath;
    int slash_index = filename.ReversFind("/");
    if (slash_index == -1)
        m_pStatusLine_->setFileName(filepath);
    else
        m_pStatusLine_->setFileName(string(filename.SubStr(slash_index + 1)));
    m_pStatusLine_->setCurrentLine(1);
    SetConsoleCursorPosition(m_hConsoleOut_, {0, 0});
    m_pStatusLine_->setLinesTotal(lines_count);
    m_pStatusLine_->RefreshBar();
}

void ViewTextEditor::print_line(SHORT index , const char *line)
{
    SetConsoleCursorPosition(m_hConsoleOut_, {0, index});
    cout << "\33[2K\r";
    cout << line << endl;
}

void ViewTextEditor::CursorUpdate(COORD position)
{
    screen_info_refresh();
    if (position.Y >= m_csbiInfo_.srWindow.Bottom)
    {
        print_line(m_csbiInfo_.srWindow.Bottom, m_pControllerTextEditor_->m_getLine(m_csbiInfo_.srWindow.Bottom));
        SMALL_RECT srctWindow;
        srctWindow.Right = m_csbiInfo_.srWindow.Right;
        srctWindow.Left = m_csbiInfo_.srWindow.Left;
        srctWindow.Bottom = position.Y + 1;
        srctWindow.Top = srctWindow.Bottom - (m_csbiInfo_.srWindow.Bottom - m_csbiInfo_.srWindow.Top);
        SetConsoleWindowInfo(m_hConsoleOut_, TRUE, &srctWindow);
    }
    else if (position.Y < m_csbiInfo_.srWindow.Top)
        print_line(m_csbiInfo_.srWindow.Bottom, m_pControllerTextEditor_->m_getLine(m_csbiInfo_.srWindow.Bottom));
    SetConsoleCursorPosition(m_hConsoleOut_, position);
    m_pStatusLine_->setCurrentLine(position.Y + 1);
    m_pStatusLine_->RefreshBar();
}

void ViewTextEditor::StatusBarUpdate(SHORT lines_total, string filename, bool insert_mode, const char* message)
{
    m_pStatusLine_->setLinesTotal(lines_total);
    m_pStatusLine_->setFileName(filename);
    BYTE mode = (insert_mode)? 1 : 0;
    m_pStatusLine_->setMode(mode);
    if (message != nullptr)
    {
        m_pStatusLine_->setText(message);
        m_pStatusLine_->setMode(2);
        m_pStatusLine_->RefreshBar();
        _getch();
    }
    m_pStatusLine_->RefreshBar();
}

void ViewTextEditor::ScreenBufferInfoUpdate(bool up)
{
    SMALL_RECT srctWindow;
    screen_info_refresh();
    print_line(m_csbiInfo_.srWindow.Bottom, m_pControllerTextEditor_->m_getLine(m_csbiInfo_.srWindow.Bottom));
    srctWindow.Right = srctWindow.Left = 0;
    if (up)
    {
        srctWindow.Top = srctWindow.Bottom = m_csbiInfo_.srWindow.Top - m_csbiInfo_.srWindow.Bottom;
    }
    else
    {
        srctWindow.Top = srctWindow.Bottom = m_csbiInfo_.srWindow.Bottom - m_csbiInfo_.srWindow.Top;
    }
    SetConsoleWindowInfo(m_hConsoleOut_, FALSE, &srctWindow);
}

void ViewTextEditor::ScreenBufferInfoUpdate(SHORT start_line, SHORT lines_count, SHORT shift)
{
    screen_info_refresh();
    print_line(m_csbiInfo_.srWindow.Bottom, m_pControllerTextEditor_->m_getLine(m_csbiInfo_.srWindow.Bottom));
    SMALL_RECT srctScrollRect;
    CHAR_INFO chiFill;
    COORD coordDest;

    srctScrollRect.Top = start_line;
    srctScrollRect.Bottom = start_line + lines_count - 1;
    srctScrollRect.Left = 0;
    srctScrollRect.Right = m_csbiInfo_.dwSize.X - 1;
    coordDest.X = 0;
    coordDest.Y = start_line + shift;
    chiFill.Attributes = 0;
    chiFill.Char.AsciiChar = (char)' ';
    ScrollConsoleScreenBuffer(m_hConsoleOut_, &srctScrollRect, NULL, coordDest, &chiFill);
    m_pStatusLine_->RefreshBar();
}

void ViewTextEditor::LineUpdate(SHORT index, const char* line)
{
    print_line(index, line);
}

void ViewTextEditor::NewFileUpdate(ifstream& in, string& filepath)
{
    OpenExistingFile(in, filepath);
}
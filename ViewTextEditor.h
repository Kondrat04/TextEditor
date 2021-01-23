//
// Created by kondr on 06.12.2020.
//

#ifndef TEXTEDITOR_VIEWTEXTEDITOR_H
#define TEXTEDITOR_VIEWTEXTEDITOR_H
#include <windows.h>
#include <iostream>
#include <fstream>
#include <conio.h>
#include "ModelTextEditor.h"
#include "ControllerTextEditor.h"
#include "StatusLine.h"
#include "ISubscriber.h"

class ControllerTextEditor;
using namespace std;

class ViewTextEditor : public ISubscriber
{
public:
    ViewTextEditor();
    ~ViewTextEditor();
    void SetController(ControllerTextEditor* controller);
    void WaitForCommand();
    void OpenExistingFile(ifstream& in, string filepath);
    void CursorUpdate(COORD position) override;
    void StatusBarUpdate(SHORT lines_total, string filename, bool insert_mode, const char* message) override;
    void ScreenBufferInfoUpdate(bool up) override;
    void ScreenBufferInfoUpdate(SHORT start_line, SHORT lines_count, SHORT shift) override;
    void LineUpdate(SHORT index, const char* line) override;
    void NewFileUpdate(ifstream& in, string& filepath) override;
private:
    void print_line(SHORT index, const char* line);
    void screen_info_refresh();
    HANDLE m_hConsoleOut_;
    HANDLE m_hConsoleIn_;
    CONSOLE_SCREEN_BUFFER_INFO m_csbiInfo_;
    DWORD m_console_mode_;
    ControllerTextEditor *m_pControllerTextEditor_;
    StatusLine *m_pStatusLine_;
};


#endif //TEXTEDITOR_VIEWTEXTEDITOR_H

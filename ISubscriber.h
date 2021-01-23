#pragma once

#include <windows.h>
#include <string>
#include <iostream>

using namespace std;

class ISubscriber 
{
public:
	virtual ~ISubscriber() {};
	virtual void CursorUpdate(COORD position) = 0;
	virtual void StatusBarUpdate(SHORT lines_total, string filename, bool insert_mode, const char *message) = 0;
	virtual void ScreenBufferInfoUpdate(bool up) = 0;
	virtual void ScreenBufferInfoUpdate(SHORT start_line, SHORT lines_count, SHORT shift) = 0;
	virtual void LineUpdate(SHORT index, const char* line) = 0;
	virtual void NewFileUpdate(ifstream& in, string& filepath) = 0;
};
#pragma once

#include "ISubscriber.h"
#include <vector>

using namespace std;
class ISubscriber;

class IPublisher 
{
public:
	virtual ~IPublisher() {};
	virtual void AddSubscriber(ISubscriber* subscriber) = 0;
	virtual void RemoveSubscriber() = 0;
	virtual void NotifyCursorUpdate(COORD position) = 0;
	virtual void NotifyStatusBarUpdate(SHORT lines_total, string filename, bool insert_mode, const char* message) = 0;
	virtual void NotifyScreenBufferInfoUpdate(bool up) = 0;
	virtual void NotifyScreenBufferInfoUpdate(SHORT start_line, SHORT lines_count, SHORT shift) = 0;
	virtual void NotifyLineUpdate(SHORT index, const char* line) = 0;
	virtual void NotifyNewFileUpdate(ifstream& in, string& filepath) = 0;
};
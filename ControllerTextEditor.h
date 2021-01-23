//
// Created by kondr on 06.12.2020.
//

#ifndef TEXTEDITOR_CONTROLLERTEXTEDITOR_H
#define TEXTEDITOR_CONTROLLERTEXTEDITOR_H
#include "ModelTextEditor.h"
#include "ISubscriber.h"

class ViewTextEditor;
class ModelTextEditor;

using namespace std;

class ControllerTextEditor {
public:
    ControllerTextEditor();
    ~ControllerTextEditor();
    void SetModel(ModelTextEditor* model);
    void AddLine(string str);
    int ExecuteCommand(const char *command);
    int ExecuteCommand(char command);
    const char *m_getLine(SHORT index);
private:
    ModelTextEditor *m_pModelTextEditor_;
};


#endif //TEXTEDITOR_CONTROLLERTEXTEDITOR_H

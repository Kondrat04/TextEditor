//
// Created by kondr on 06.12.2020.
//

#include "ControllerTextEditor.h"

ControllerTextEditor::ControllerTextEditor()
{

}

ControllerTextEditor::~ControllerTextEditor()
{
    
}

void ControllerTextEditor::SetModel(ModelTextEditor* model)
{
    m_pModelTextEditor_ = model;
}

void ControllerTextEditor::AddLine(string str)
{
    m_pModelTextEditor_->AddLine(str);
}

int ControllerTextEditor::ExecuteCommand(const char *command)
{
    return m_pModelTextEditor_->ExecuteCommand(command);
}

int ControllerTextEditor::ExecuteCommand(char command)
{
    return m_pModelTextEditor_->ExecuteCommand(command);
}

const char * ControllerTextEditor::m_getLine(SHORT index)
{
    return m_pModelTextEditor_->m_getLine(index);
}
#include <iostream>
#include "ViewTextEditor.h"
#include "ControllerTextEditor.h"
#include "ModelTextEditor.h"
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc > 2)
    {
        cout << "Wrong count of arguments";
        return 0;
    }
    if (argc == 2)
    {
        ifstream in(argv[1]);
        if (in.is_open()){
            ViewTextEditor view_text_editor;
            ControllerTextEditor controller_text_editor;
            ModelTextEditor model_text_editor;
            view_text_editor.SetController(&controller_text_editor);
            controller_text_editor.SetModel(&model_text_editor);
            model_text_editor.AddSubscriber(&view_text_editor);
            model_text_editor.SetFilePath(string(argv[1]));
            view_text_editor.OpenExistingFile(in, string(argv[1]));
            in.close();
            view_text_editor.WaitForCommand();
        }
        else{
            cout << "File not found";
            return 0;
        }
    }
    else
    {
        ViewTextEditor view_text_editor;
        ControllerTextEditor controller_text_editor;
        ModelTextEditor model_text_editor;
        view_text_editor.SetController(&controller_text_editor);
        controller_text_editor.SetModel(&model_text_editor);
        model_text_editor.AddSubscriber(&view_text_editor);
        model_text_editor.AddLine(string(""));
        view_text_editor.WaitForCommand();
    }
}

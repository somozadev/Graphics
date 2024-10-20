#pragma once
#include "assimpLoader/Model.h"

class ImguiHandler
{
private:
    static void newFrame();

public:
    static void startWindow(const char* name);
    static void addCheckBox(const char* text, bool* reference);
    static void addColorModifier(const char* text, vector<GLfloat>& colorReference);
    static void scrollGroup(std::vector<Model> models);
    static void draw();
    static void endWindow();
};

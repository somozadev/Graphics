#pragma once
#include "assimpLoader/Model.h"
#include "Lights/BaseLight.h"

class ImguiHandler
{
private:
    static void newFrame();

public:
    static void startWindow(const char* name);
    static void addCheckBox(const char* text, bool* reference);
    static void addColorModifier(const char* text, vector<GLfloat>& colorReference);
    static void addSingleModel(const char* text, Transform* transform);
    static void scrollGroup(std::vector<Model> models);
    static void mainLight(BaseLight* light);
    static void draw();
    static void endWindow();
    static void update(bool &wireframe,bool &see_grid, Model* light,std::vector<GLfloat> bg_color);
};

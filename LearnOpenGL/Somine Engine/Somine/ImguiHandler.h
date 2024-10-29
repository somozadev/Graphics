#pragma once
#include "assimpLoader/Model.h"
#include "Lights/DirectionalLight.h"

class ImguiHandler
{
private:
    static void newFrame();

public:
    static void startWindow(const char* name);
    static void addCheckBox(const char* text, bool* reference);
    static void addColorModifier(const char* text, vector<GLfloat>& colorReference);
    static void addSingleModel(const char* text, Transform* transform);
    static void addInteger(const char* text, int* reference);
    static void addModel(const char* modelName, Model* model);
    static void mainLight(DirectionalLight* light);
    static void draw();
    static void endWindow();
    static void update(bool &wireframe,bool &see_grid, Model* light,std::vector<GLfloat> bg_color);
};

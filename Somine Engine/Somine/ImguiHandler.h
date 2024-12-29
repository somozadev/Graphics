#pragma once
#include "assimpLoader/Model.h"
#include "Lights/DirectionalLight.h"
#include "Lights/PointLight.h"
#include "Lights/SpotLight.h"
/*
 *This has some limitations, such as only managing cleanly 1 window...
 *Should get improved in an object per window basis 
 */
class ImguiHandler 
{
private:
    static void newFrame();

public:
    static void startWindow(const char* name);
    static void showFrameRate(const char* text, float deltaTime);
    static void showMs(const char* text, float ms);
    static void addCheckBox(const char* text, bool* reference);
    static void addColorModifier(const char* text, vector<GLfloat>& colorReference);
    static void addSingleModel(const char* text, Transform* transform);
    static void addInteger(const char* text, int* reference, int min, int max);
    static void addFloat(const char* text, float* reference, float min, float max);
    static void addInteger(const char* text, int* reference);
    static void addModel(const char* modelName, Model* model);
    static void addPointLights(std::vector<PointLight*> lights);
    static void addSpotLights(std::vector<SpotLight*> lights);
    static void mainLight(DirectionalLight* light);
    static void draw();
    static void endWindow();
    static void update(bool &wireframe,bool &see_grid, Model* light,std::vector<GLfloat> bg_color);
};

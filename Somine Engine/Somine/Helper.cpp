#include "Helper.h"
#include <cstdlib>
#include <ctime>
#include <string>


float Helper::getRnd(float min, float max)
{
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

float Helper::getRndDegrees()
{
    return getRnd(0.0f, 360.0f);
}

void Helper::initRnd()
{
    srand(static_cast<unsigned int>(time(0)));
}

std::string Helper::getPath(const std::string& path)
{
    return getBasePath() + "/" + path;
}
std::string Helper::getBasePath()
{
    return "C:/Users/mesom/Documents/GitHub/Graphics/LearnOpenGL/Somine Engine/Somine/models"; 
}
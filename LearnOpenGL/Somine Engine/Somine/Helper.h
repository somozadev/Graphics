#pragma once
#include <xstring>

class Helper
{
private:
    typedef std::string (*Builder)(const std::string& path);

public:
    static float getRnd(float min, float max);
    static void initRnd();
    static std::string getPath(const std::string& path);
    static std::string getBasePath();
    static float getRndDegrees();
};

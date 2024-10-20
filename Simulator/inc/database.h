#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>

namespace DataBase
{
    std::vector<double> getValsByKey(std::string, std::string);
    void writeToFile(std::string, std::string);
};
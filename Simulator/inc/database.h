#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace DataBase
{
    std::vector<double> getValsByKey(std::string, std::string);
    void writeToFile(std::string, std::string);
};
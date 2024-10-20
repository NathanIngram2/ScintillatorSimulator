#include "../inc/database.h"

#include <mutex>

std::mutex mtx;

std::vector<double> DataBase::getValsByKey(std::string key, std::string filename)
{
    std::ifstream t(filename);
    if (!t)
    {
        std::cout << "Trouble opening file: " + filename + ". Check file name and path.\n";
        throw new std::exception();
    }
    std::stringstream buffer;
    buffer << t.rdbuf();
    std::string buff = buffer.str();

    int valStartPos = buff.find(key + ": [");
    buff.erase(0, (valStartPos + key.length() + 3));
    int valEndPos = buff.find("]");
    buff.resize(valEndPos);

    std::vector<double> values = {};
    buffer = std::stringstream(buff);

    for (double i; buffer >> i;)
    {
        values.push_back(i);
        if (buffer.peek() == ',')
        {
            buffer.ignore();
        }
    }

    return values;
}

void DataBase::writeToFile(std::string info, std::string filename)
{
    mtx.lock();
    std::ofstream t(filename, std::ios_base::app);
    if (!t)
    {
        std::cout << "Trouble opening file: " + filename + ". Check file name and path.\n";
        throw new std::exception();
    }

    t << info + "\n";
    t.close();
    mtx.unlock();
}
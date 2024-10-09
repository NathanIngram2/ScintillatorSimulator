#pragma once

#include <string>
#include <vector>
#include <mutex>

class Environment
{
private:
    Environment();
    void operator=(const Environment&) = delete;

    std::vector<std::vector<float>> m_positions;
    
    float m_diameter;
    float m_spacing;
    float m_width = 0.3357; // width (and height) of scintillator we're simulating in the x and y (meters), change as needed
    bool m_fibersExist = true;

    static Environment* m_instance;

public:
    // letting this be public so MC can destruct it
    ~Environment();

    // called once in the MC setup, and then only called by photons later; should exist by then and won't create anything new
    static Environment* getInstance()
    {
        if (m_instance == nullptr){
            m_instance = new Environment();
        }
        return m_instance;
    }

    std::vector<float> reconfig(float, float, std::string);
    std::vector<double> fiberCollision(const std::vector<double>&, const std::vector<double>&, float) const;
    std::vector<double> findClosestFiberInDirection(const std::vector<double>&, const std::vector<double>&) const;
    bool isThereFibers() const;
};
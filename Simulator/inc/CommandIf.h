#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "./MonteCarlo.h"

class CommandIf
{
public:
    std::vector<std::string> m_inputs;
    std::string m_outText;
    MonteCarlo* m_monteCarloInstance = MonteCarlo::getInstance();
    std::string m_name = "";

    virtual void CommandInit(std::string out, const std::vector<std::string>& in)
    {
        m_outText = out;
        m_inputs = in;
    }
    virtual ~CommandIf() {};
    virtual void Execute() const = 0;
    virtual std::string to_string() const
    {
        return m_name;
    }
};
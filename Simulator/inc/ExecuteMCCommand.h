#pragma once

#include "./CommandIf.h"

class ExecuteMCCommand : public CommandIf
{
public:
    explicit ExecuteMCCommand(std::string, const std::vector<std::string>&);
    ~ExecuteMCCommand();
    virtual void Execute() const override;

private:
    std::string m_currentState;
};
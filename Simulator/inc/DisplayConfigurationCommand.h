#pragma once

#include "./CommandIf.h"

class DisplayConfigurationCommand : public CommandIf
{
public:
    explicit DisplayConfigurationCommand(std::string, const std::vector<std::string>&);
    ~DisplayConfigurationCommand();
    virtual void Execute() const override;
};
#pragma once

#include "CommandIf.h"

class VisualizeCommand : public CommandIf
{
public:
    explicit VisualizeCommand(std::string, const std::vector<std::string>&);
    ~VisualizeCommand();
    virtual void Execute() const override;
};
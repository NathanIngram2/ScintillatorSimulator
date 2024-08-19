#pragma once

#include "CommandIf.h"

class AnimateDemoCommand : public CommandIf
{
public:
    explicit AnimateDemoCommand(std::string, const std::vector<std::string>&);
    ~AnimateDemoCommand();
    virtual void Execute() const override;
};
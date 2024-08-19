#pragma once

#include "./CommandIf.h"

class SetFiberSpacingCommand : public CommandIf
{
public:
    explicit SetFiberSpacingCommand(std::string, const std::vector<std::string>&);
    ~SetFiberSpacingCommand();
    virtual void Execute() const override;
};
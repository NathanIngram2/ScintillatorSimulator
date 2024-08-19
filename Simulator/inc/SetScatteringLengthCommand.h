#pragma once

#include "./CommandIf.h"

class SetScatteringLengthCommand : public CommandIf
{
public:
    explicit SetScatteringLengthCommand(std::string, const std::vector<std::string>&);
    ~SetScatteringLengthCommand();
    virtual void Execute() const override;
};
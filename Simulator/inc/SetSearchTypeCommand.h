#pragma once

#include "./CommandIf.h"

class SetSearchTypeCommand : public CommandIf
{
public:
    explicit SetSearchTypeCommand(std::string, const std::vector<std::string>&);
    ~SetSearchTypeCommand();
    virtual void Execute() const override;
};
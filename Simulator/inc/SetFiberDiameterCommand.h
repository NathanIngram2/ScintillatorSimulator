#pragma once

#include "./CommandIf.h"

class SetFiberDiameterCommand : public CommandIf
{
public:
    explicit SetFiberDiameterCommand(std::string, const std::vector<std::string>&);
    ~SetFiberDiameterCommand();
    virtual void Execute() const override;
};
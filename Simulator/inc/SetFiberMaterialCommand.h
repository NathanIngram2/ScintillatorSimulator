#pragma once

#include "./CommandIf.h"

class SetFiberMaterialCommand : public CommandIf
{
public:
    explicit SetFiberMaterialCommand(std::string, const std::vector<std::string>&);
    ~SetFiberMaterialCommand();
    virtual void Execute() const override;
};
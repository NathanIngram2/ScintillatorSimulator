#pragma once

#include "./CommandIf.h"

class SetScintillatorMaterialCommand : public CommandIf
{
public:
    explicit SetScintillatorMaterialCommand(std::string, const std::vector<std::string>&);
    ~SetScintillatorMaterialCommand();
    virtual void Execute() const override;
};
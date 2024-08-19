#pragma once

#include "./CommandIf.h"

class SetNumOfPhotonsCommand : public CommandIf
{
public:
    explicit SetNumOfPhotonsCommand(std::string, const std::vector<std::string>&);
    ~SetNumOfPhotonsCommand();
    virtual void Execute() const override;
};
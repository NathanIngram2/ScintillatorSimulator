#pragma once

#include "./CommandIf.h"

class StubCommand : public CommandIf
{
public:
    explicit StubCommand(std::string, const std::vector<std::string>&);
    ~StubCommand();
    virtual void Execute() const override;
};
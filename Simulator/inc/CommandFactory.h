#pragma once

#include <iostream>

#include "./CommandIf.h"
#include "./VisualizeCommand.h"
#include "./DisplayConfigurationCommand.h"
#include "./ExecuteMCCommand.h"
#include "./SetFiberDiameterCommand.h"
#include "./SetFiberMaterialCommand.h"
#include "./SetFiberSpacingCommand.h"
#include "./SetNumOfPhotonsCommand.h"
#include "./SetScintillatorMaterialCommand.h"
#include "./SetScatteringLengthCommand.h"
#include "./SetSearchTypeCommand.h"
#include "./StubCommand.h"
#include "./MenuOptions.h"


class CommandFactory
{
protected:
    CommandFactory();
    static CommandFactory* factory_singleton;

public:
    ~CommandFactory();
    CommandFactory(CommandFactory& other) = delete;
    void operator=(const CommandFactory&) = delete;

    static CommandFactory* getInstance();
    CommandIf* createCommand(MenuOptions);

private:
    bool m_alreadyExists = false;
};
#include "../inc/CommandFactory.h"

CommandFactory* CommandFactory::factory_singleton = nullptr;

CommandFactory::CommandFactory() {}

CommandFactory::~CommandFactory()
{
    std::cout << "Destruct Command Factory\n";
}

CommandFactory* CommandFactory::getInstance()
{
    return factory_singleton;
}

CommandIf* CommandFactory::createCommand(MenuOptions option)
{
    switch(option)
    {
      case DisplayConfiguration:
        return new DisplayConfigurationCommand("Display Configuration", {""});

      case SetNumOfPhotons:
        return new SetNumOfPhotonsCommand("Set Number of Photons between 100 and 50000", {"Number"});

      case SetScintillatorMaterial:
        return new SetScintillatorMaterialCommand("Set Scintillator Material", {"LAB", "DIN"});

      case SetScatteringLength:
        return new SetScatteringLengthCommand("Set Scattering Length", {"Length (mm)"});

      case SetFiberMaterial:
        return new SetFiberMaterialCommand("Set Fiber Material", {""});

      case SetFiberSpacing:
        return new SetFiberSpacingCommand("Set Fiber Spacing", {"Spacing (mm)"});

      case SetFiberDiameter:
        return new SetFiberDiameterCommand("Set Fiber Diameter", {"Diameter (mm)"});

      case AnimateDemo:
        return new VisualizeCommand("Visualize Scattering (matplotlib)", {""});

      case ExecuteMC:
        return new ExecuteMCCommand("Execute Monte Carlo", {""});

      default:
        std::cout << "Invalid menuOption" << std::endl;
        return nullptr;
    }
}
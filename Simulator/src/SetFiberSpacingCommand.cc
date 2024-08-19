#include "../inc/SetFiberSpacingCommand.h"

SetFiberSpacingCommand::SetFiberSpacingCommand(std::string out, const std::vector<std::string>& in)
{
    CommandInit(out, in);
    m_name = "Set Fiber Spacing";
}

SetFiberSpacingCommand::~SetFiberSpacingCommand()    
{

}

void SetFiberSpacingCommand::Execute() const
{
    std::cout << "Enter a fiber spacing length in mm: ";
    float fiberSpacing;
    std::cin >> fiberSpacing;

    if (fiberSpacing <= 0 || fiberSpacing > 100) // change bounds as needed - mm
    {
        std::cout << "Invalid fiber spacing. Please enter a value in the range 0-100mm\n";
        std::cout << "Defualting to 15mm.";
        fiberSpacing = 15.0;
    }

    m_monteCarloInstance->setFiberSpacing(std::to_string(fiberSpacing));
}
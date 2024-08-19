#include "../inc/SetFiberDiameterCommand.h"

SetFiberDiameterCommand::SetFiberDiameterCommand(std::string out, const std::vector<std::string>& in)
{
    CommandInit(out, in);
    m_name = "Set Fiber Diameter";
}

SetFiberDiameterCommand::~SetFiberDiameterCommand()    
{

}

void SetFiberDiameterCommand::Execute() const
{
    std::cout << "Enter a fiber diameter length in mm: ";
    float fiberDiameter;
    std::cin >> fiberDiameter;

    if (fiberDiameter <= 0 || fiberDiameter > 100) // change bounds as needed - mm
    {
        std::cout << "Invalid fiber diameter. Please enter a value in the range 0-100mm\n";
        std::cout << "Defualting to 5mm.";
        fiberDiameter = 5.0;
    }

    m_monteCarloInstance->setFiberDiameter(std::to_string(fiberDiameter));
}
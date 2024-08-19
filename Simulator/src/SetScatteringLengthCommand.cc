#include "../inc/SetScatteringLengthCommand.h"

SetScatteringLengthCommand::SetScatteringLengthCommand(std::string out, const std::vector<std::string>& in)
{
    CommandInit(out, in);
    m_name = "Set Scattering Length";
}

SetScatteringLengthCommand::~SetScatteringLengthCommand()    
{

}

void SetScatteringLengthCommand::Execute() const
{
    std::cout << "Enter an average scattering length in mm: ";
    float scatteringLength;
    std::cin >> scatteringLength;

    if (scatteringLength <= 0 || scatteringLength > 100000) // what are the bounds?
    {
        std::cout << "Invalid scattering length. Please enter a value in the range 0-100mm\n";
        std::cout << "Defualting to 5mm.";
        scatteringLength = 5.0;
    }

    m_monteCarloInstance->setScatteringLength(std::to_string(scatteringLength));
}
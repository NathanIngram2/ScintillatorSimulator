#include "../inc/SetNumOfPhotonsCommand.h"

SetNumOfPhotonsCommand::SetNumOfPhotonsCommand(std::string out, const std::vector<std::string>& in)
{
    CommandInit(out, in);
    m_name = "Set Number of Photons";
}

SetNumOfPhotonsCommand::~SetNumOfPhotonsCommand()    
{

}

void SetNumOfPhotonsCommand::Execute() const
{
    std::cout << "Enter a number of photons to simulate: ";
    int photonNum;
    std::cin >> photonNum;

    if (photonNum <= 1 || photonNum > 10000) // change bounds as needed
    {
        std::cout << "Invalid iteration number. Please enter a value in the range 0-10000 photons\n";
        std::cout << "Defualting to 300.";
        photonNum = 300;
    }

    m_monteCarloInstance->setNumOfPhotons(std::to_string(photonNum));
}
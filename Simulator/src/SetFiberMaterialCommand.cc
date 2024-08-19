#include "../inc/SetFiberMaterialCommand.h"

SetFiberMaterialCommand::SetFiberMaterialCommand(std::string out, const std::vector<std::string>& in)
{
    CommandInit(out, in);
    m_name = "Set Fiber Material";
}

SetFiberMaterialCommand::~SetFiberMaterialCommand()    
{

}

void SetFiberMaterialCommand::Execute() const
{
    std::cout << "Enter an average scattering length in mm: ";
    int optionNo;
    std::string scintillatingMaterial;
    std::cin >> optionNo;

    switch(optionNo)
    {
        case 1:
            scintillatingMaterial = "LAB";
            break;
        case 2:
            scintillatingMaterial = "DIN";
            break;
        default:
            scintillatingMaterial = " ";
    }

    m_monteCarloInstance->setFiberMaterial(scintillatingMaterial);
}
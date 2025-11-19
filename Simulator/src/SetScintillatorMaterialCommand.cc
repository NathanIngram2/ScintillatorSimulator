#include "../inc/SetScintillatorMaterialCommand.h"

SetScintillatorMaterialCommand::SetScintillatorMaterialCommand(std::string out, const std::vector<std::string>& in)
{
    CommandInit(out, in);
    m_name = "Set Scintillator Material";
}

SetScintillatorMaterialCommand::~SetScintillatorMaterialCommand()    
{

}

void SetScintillatorMaterialCommand::Execute() const
{
    std::cout << "Enter the number corresponding to a scintillating material of the supported list:\n 1 - LAB\n 2 - DIN ";
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

    if (scintillatingMaterial == " ")
    {
        std::cout << "Input is invalid. Scintillating material was not changed.\n";
    }
    else
    {
        m_monteCarloInstance->setScintillatorMaterial(scintillatingMaterial);
    }
}
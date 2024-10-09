#include "../inc/SetFiberMaterialCommand.h"

SetFiberMaterialCommand::SetFiberMaterialCommand(std::string out, const std::vector<std::string>& in)
{
    CommandInit(out, in);
    m_name = "Set Fiber Material";
}

SetFiberMaterialCommand::~SetFiberMaterialCommand()    
{

}

// Here for the extension of the functionality of existing simulation capabilities
void SetFiberMaterialCommand::Execute() const
{
    std::cout << "Enter a fiber material of the supported types [...]: ";
    std::string fiberMaterial = "";

    m_monteCarloInstance->setFiberMaterial(fiberMaterial);
}
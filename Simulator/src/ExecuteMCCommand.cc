#include "../inc/ExecuteMCCommand.h"

ExecuteMCCommand::ExecuteMCCommand(std::string out, const std::vector<std::string>& in)
{
    CommandInit(out, in);
    m_name = "Execute Monte Carlo";

}

ExecuteMCCommand::~ExecuteMCCommand()    
{

}

void ExecuteMCCommand::Execute() const
{
    m_monteCarloInstance->executeMC();
}
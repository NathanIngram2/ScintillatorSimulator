#include "../inc/DisplayConfigurationCommand.h"

DisplayConfigurationCommand::DisplayConfigurationCommand(std::string out, const std::vector<std::string>& in)
{
    CommandInit(out, in);
    m_name = "Display Configuration";
}

DisplayConfigurationCommand::~DisplayConfigurationCommand()    
{

}

void DisplayConfigurationCommand::Execute() const
{
    m_monteCarloInstance->displayConfiguration();
}
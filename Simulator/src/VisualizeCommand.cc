#include "../inc/VisualizeCommand.h"

VisualizeCommand::VisualizeCommand(std::string out, const std::vector<std::string>& in)
{
    CommandInit(out, in);
    m_name = "Visualize with Python";
}

VisualizeCommand::~VisualizeCommand()    
{

}

void VisualizeCommand::Execute() const
{

}


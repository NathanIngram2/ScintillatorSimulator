#include "../inc/SetSearchTypeCommand.h"

SetSearchTypeCommand::SetSearchTypeCommand(std::string out, const std::vector<std::string>& in)
{
    CommandInit(out, in);
    m_name = "Set Search Type";
}

SetSearchTypeCommand::~SetSearchTypeCommand()    
{

}

void SetSearchTypeCommand::Execute() const
{
    std::cout << "Enter the number corresponding to a search style used for collision detection:\n 1 - Cartesian Comparison (BF)\n 2 - Cartesian Comparison w/ Directionality\n 3 - KDTree Search\n";
    int optionNo;
    std::string searchType;
    std::cin >> optionNo;

    switch(optionNo)
    {
        case 1:
            searchType = "Basic";
            break;
        case 2:
            searchType = "Directional";
            break;
        case 3:
            searchType = "KDTree";
            break;
        default:
            searchType = " ";
    }

    if (searchType == " ")
    {
        std::cout << "Input is invalid. Defaulting to basic search.\n";
    }
    else
    {
        m_monteCarloInstance->setSearchType(searchType);
    }
}
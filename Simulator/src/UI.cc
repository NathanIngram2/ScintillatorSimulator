#include "../inc/UI.h"
#include "../inc/CommandFactory.h"

UI::UI()
{
    m_commandFactory = CommandFactory::getInstance();
    m_commandMap = new std::map<MenuOptions, CommandIf*>();
    for (int menuOption = MenuOptions::DisplayConfiguration; menuOption != MenuOptions::END;
            menuOption++)
    {
        auto command = static_cast<MenuOptions>(menuOption);
        m_commandMap->insert({command, m_commandFactory->createCommand(command)});
    }
}

UI::~UI()
{
    // destroy map
    std::cout << "Destruct UI and Commands\n";
    m_commandMap->clear();
    delete m_commandMap;
    m_commandMap = nullptr;

    // destroy factory
    // -> this should be the only place we need to do this cause singleton
    delete CommandFactory::getInstance();
    m_commandFactory = nullptr;
}

void UI::printUI() const
{
    std::cout << '\n';

    std::cout << "====== ENPH455 Scintillator Simulation - A Monte Carlo Approach ======\n" << std::endl;

    std::cout << "===================== Please Enter a Menu Option ======================" << std::endl;
    std::map<MenuOptions, CommandIf*>::iterator iter;
    int i = 1;
    for (iter = m_commandMap->begin(); iter != m_commandMap->end(); iter++)
    {
        std::cout << i;
        std::cout << ": " << iter->second->m_outText << std::endl;

        if (iter->second->m_inputs.size() != 0)
        {
            std::cout << "\t<";
            for (int i = 0; i < iter->second->m_inputs.size(); i++)
            {
                std::cout << iter->second->m_inputs.at(i);
                if (i != iter->second->m_inputs.size() - 1)
                {
                    std::cout << ", ";
                }
            }
            std::cout << '>' << std::endl;
        }
        i++;
    }
    std::cout << "===================== -------------------------- ======================" << std::endl;
}

void UI::displayResponse(std::vector<std::string>) const
{

}

std::string UI::retrieveUserInput()
{
    // TODO: Why is this duplicating the input?
    std::string inputString;
    std::cin >> inputString;
    // std::cout << inputString << std::endl;
    
    if (inputString == "q" || inputString == "Q" || inputString == "quit" || inputString == "exit")
    {
        std::cout << "End Program, Goodbye\n";
        return "ENDLOOP";
    }

    try
    {
        int option = std::stoi(inputString);

        if (option > 0 && option <= static_cast<int>(MenuOptions::END))
        {
            CommandIf* selectedCommand = m_commandMap->at(static_cast<MenuOptions>(option-1)); // options stored with their int representation as keys
            std::cout << selectedCommand->to_string() << std::endl;
            selectedCommand->Execute();
        }
        else
        {
            // std::cout << "got here" << std::endl;
            throw new std::invalid_argument("Input outside of menu range.");
        }
    }
    catch (std::exception e)
    {
        std::cout << "Invalid value exception triggered" << std::endl;
        std::cout << e.what() << std::endl;
    }

    return "\n";
}
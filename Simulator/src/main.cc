// Monte Carlo Main - point of entry for entire program dealing with scintillator simulation
#include "../inc/UI.h"
#include <cstdlib>

std::string mainLoop(UI* ui)
{
    ui->printUI();
    std::string commandString = ui->retrieveUserInput();
    return commandString;
}

int main() {
    auto ui = new UI();
    auto photonManager = new PhotonManager();
    MonteCarlo::getInstance()->setPhotonManagerRef(photonManager);

    std::string temp = "";

    while (temp != "ENDLOOP")
    {
        temp = mainLoop(ui);
        std::cout << temp << std::endl;
    }

    // calling these two should destroy everything
    delete ui;
    delete MonteCarlo::getInstance();

    return 0;
}
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
    std::cout << "Which plot do you want to generate?\n";
    std::cout << "  1: 2d plot of photon paths in the x-y plane\n";
    std::cout << "  2: X-Y plane fiber collisions and heatmap showing most probable photon-fiber interactions\n";
    std::cout << "  3: 3d view of all photon paths in the simulated volume\n";
    std::cout << "  4: Histogram of all generated photon wavelengths and wavelength probability distribution\n";
    int optionNo;
    std::cin >> optionNo;
    std::string pyCommandBuff = "python ../plotting/ShowScattering.py";

    switch(optionNo)
    {
        case 1:
            pyCommandBuff += " -func plot2d";
            break;
        case 2:
            pyCommandBuff += " -func plot2d_collisions";
            break;
        case 3:
            pyCommandBuff += " -func plot3d";
            break;
        case 4:
            pyCommandBuff += " -func plot_wavelength_hist";
            break;
        default:
            std::cout << "Not an accepted plot type, please try again.\n";
    }

    m_monteCarloInstance->visualizePhotons(pyCommandBuff);
}


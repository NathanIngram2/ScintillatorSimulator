#include "MonteCarlo.h"

MonteCarlo* MonteCarlo::MC_singleton = nullptr;

MonteCarlo::MonteCarlo()
{
    // TODO: change these to better reflect default configuration
    m_scintillatorMaterial = "LAB";
    m_fiberDiameter = 0.5; // mm
    m_fiberMaterial = "<--->";
    m_fiberSpacing = 10; // mm
    m_numOfTotalIterations = 300;
    m_scatteringLen = 2.0; // mm

    m_processorCount = std::thread::hardware_concurrency();
    // m_processorCount = 1;

    std::cout << m_processorCount << " possible concurrent threads" << std::endl;
    if (m_processorCount == 0)
    {
        std::cout << "Had trouble reading threading capabilities. Using a sequential approach.\n";
        m_processorCount = 1;
    }

    m_environment = Environment::getInstance();
}

MonteCarlo* MonteCarlo::getInstance()
{
    if(MC_singleton == nullptr){
            MC_singleton = new MonteCarlo();
        }
        return MC_singleton;
}

MonteCarlo::~MonteCarlo()
{
    std::cout << "Destruct MC\n";
    delete m_photonManager;
    m_photonManager = nullptr;
    delete m_environment;
    m_environment = nullptr;

}

void MonteCarlo::setPhotonManagerRef(PhotonManager* photonManager)
{
    m_photonManager = photonManager;
}

std::string MonteCarlo::getScintillatorMaterial() const
{
    return m_scintillatorMaterial;
}

float MonteCarlo::getFiberDiameter() const
{
    return m_fiberDiameter;
}

float MonteCarlo::getFiberSpacing() const
{
    return m_fiberSpacing;
}

std::string MonteCarlo::getFiberMaterial() const
{
    return m_fiberMaterial;
}

std::string MonteCarlo::toString() const
{
    std::string buff = "";
    buff.append("Monte Carlo Current Configuration is as follows: \n");
    buff.append("Scintillator Material: " + m_scintillatorMaterial + "\n");
    buff.append("Fiber Material: " + m_fiberMaterial + "\n");
    buff.append("Fiber Diameter: " + std::to_string(m_fiberDiameter) + "mm\n");
    buff.append("Fiber Spacing: " + std::to_string(m_fiberSpacing) + "mm\n");
    buff.append("Number of iterations (photon num): " + std::to_string(m_numOfTotalIterations) + "\n");
    buff.append("Mean scattering length: " + std::to_string(m_scatteringLen) + "mm\n");
    return buff;
}

void MonteCarlo::visualizePhotons() const
{
    // TODO
}

void MonteCarlo::displayConfiguration() const
{
    std::cout << this->toString() << std::endl;
    std::cout << "\nPress enter to continue";
    std::getchar();
}

void MonteCarlo::executeMC()
{
    remove("../out/FiberPositions.txt");
    remove("../out/PhotonPaths.txt");
    remove("../out/testPositions.txt");

    m_fiberCollisions = 0;

    auto middle = Environment::getInstance()->reconfig(m_fiberSpacing/1000, m_fiberDiameter/1000, m_fiberMaterial);
    std::vector<std::thread> threadVec = {};
    
    m_photonManager->setStartingPos(middle, m_fiberSpacing, m_fiberDiameter/2);
    auto startTime = std::chrono::high_resolution_clock::now();
    std::cout << "Scattering Photons..." << std::endl;

    for (int i = 0; i < m_numOfTotalIterations; i++)
    {
        if (m_photonManager->getNumberOfRunningThreads() < m_processorCount)
        {
            threadVec.emplace_back(m_photonManager->initializePhoton(i, m_scatteringLen, m_scintillatorMaterial));
        }
        else
        {
            m_photonManager->waitForOpening();
            threadVec.emplace_back(m_photonManager->initializePhoton(i, m_scatteringLen, m_scintillatorMaterial));
        }
    }

    // join any remaining threads
    for (auto iter = threadVec.begin(); iter != threadVec.end(); iter++)
    {
        iter->join();
    }
    threadVec.clear();

    std::cout << "Total number of fiber collisions: " << m_fiberCollisions << std::endl;

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "Execution Time: " << duration.count() << "ms" << std::endl;
}

void MonteCarlo::setFiberDiameter(const std::string& input)
{
    m_fiberDiameter = std::stod(input);
}

void MonteCarlo::setFiberMaterial(const std::string& input)
{
    m_fiberMaterial = input;
}

void MonteCarlo::setFiberSpacing(const std::string& input)
{
    m_fiberSpacing = std::stod(input);
}

void MonteCarlo::setNumOfPhotons(const std::string& input)
{
    m_numOfTotalIterations = std::stoi(input);
}

void MonteCarlo::setScatteringLength(const std::string& input)
{
    m_scatteringLen = std::stof(input);
}

void MonteCarlo::setScintillatorMaterial(const std::string& input)
{
    if (input == " ")
    {
        std::cout << "Input is invalid. Scintillating material was not changed.\n";
    }
    else
    {
        m_scintillatorMaterial = input;
    }
}

// TODO: This forces the program into a more sequential form, if time, allow photons to
//       be destroyed without informing higher-level code
void MonteCarlo::notify(int identifier, bool hitFiber)
{
    if (hitFiber == true)
    {
        m_fiberCollisions += 1;
    }
    m_photonManager->destroyPhoton(identifier);
}
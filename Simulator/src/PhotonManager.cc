#include "../inc/PhotonManager.h"
#include "../inc/database.h"

PhotonManager::PhotonManager()
{
    std::cout << "Constructing Photon Manager\n";
    auto success = init();
    if (!success)
    {
        std::cout << "PhotonManager init() failed due to database file read-in faults. Check file path(s) and names.\n";
        throw new std::exception;
    }
}

PhotonManager::~PhotonManager()
{
    std::cout << "Destruct PhotonManager\n";

    for (int i = 0; i < m_activeThreads.size(); i++)
    {
        m_activeThreads[i].join();
    }

    for (int i = 0; i < m_activePhotons.size(); i++)
    {
        m_activePhotons.erase(m_activePhotons.begin() + i);
    }
}

bool PhotonManager::init()
{
    try
    {
        std::cout << "Calling database from init()\n";
        auto emissionProbabilities = DataBase::getValsByKey("EMISSION_y_value_interp", "../database/emission.txt");
        m_wavelengths = DataBase::getValsByKey("EMISSION_x_value_interp", "../database/emission.txt");
        m_emissionProbabilities = std::discrete_distribution<int>(emissionProbabilities.begin(), emissionProbabilities.end());
    }
    catch (std::exception e)
    {
        return false;
    }
    return true;
}

void PhotonManager::generateWavelengths(int iterations)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::map<int,int> m;

    for (int i = 0; i < iterations; i++)
    {
        int key = m_emissionProbabilities(gen);
        ++m[m_wavelengths[key]];
    }

    for (auto p : m)
    {
        std::cout << p.first << "nm generated: " << p.second << " times\n";
    }

    std::cout << "\n\n[";

    for (auto p : m)
    {
        std::cout << p.second << ",";
    }

    std::cout << "]\n\n";

    std::cout << "\n\n[";

    for (auto p : m)
    {
        std::cout << p.first << ",";
    }

    std::cout << "]\n\n";
}

std::thread PhotonManager::initializePhoton(int iterNum, float scatteringLen, std::string scintillatorMaterial)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    int key = m_emissionProbabilities(gen);
    auto photon = new Photon(m_startingPos, iterNum, m_wavelengths[key], scatteringLen, scintillatorMaterial);
    auto bindedThread = std::thread(std::bind(&Photon::startPhotonThread, photon));

    m_activePhotons.push_back(photon);
    
    // wait for this thread to finish
    return bindedThread;
}

int PhotonManager::getNumberOfRunningThreads() const
{
    return m_activePhotons.size();
}

void PhotonManager::waitForOpening()
{
    // check to see if number of active photons has been decremented
    auto size = m_activePhotons.size();
    while (m_activePhotons.size() >= size)
    {
        // just pausing here
    }
    return;
}

void PhotonManager::destroyPhoton(int identifier)
{
    for (auto it = m_activePhotons.begin(); it != m_activePhotons.end(); it++)
    {
        if ((*it)->getID() == identifier)
        {
            m_activePhotons.erase(it);
            return;
        }
    }

    std::cout << "Photon of that identifier is not registered as an active photon!\n";
    throw new std::exception();
}

void PhotonManager::setStartingPos(std::vector<float> middle, float fiberSpacing, float fiberRadius)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis((-fiberSpacing/2)+fiberRadius, (fiberSpacing/2)-fiberRadius);
    for (int i = 0; i < 3; i++)
    {
        m_startingPos[i] = middle[i] + (dis(gen) / 1000);
    }

    std::cout << "Scintillation Burst Location X,Y,Z: " << m_startingPos[0] << ", " << m_startingPos[1] << ", " << m_startingPos[2] << "\n" << std::endl;
}
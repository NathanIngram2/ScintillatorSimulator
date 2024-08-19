#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <random>
#include <map>
#include <thread>
#include <functional>

#include "./Photon.h"
#include "./database.h"

/*
PhotonManager acts as a middleground between the MC and active photons and
takes care of all photon updates and interactions within the monte carlo
*/
class PhotonManager
{
public:
	PhotonManager();
	~PhotonManager();
	PhotonManager(const PhotonManager&) = delete;
	void operator=(const PhotonManager&) = delete;

    bool init(); // read datafiles, return true if success, else false
    void generateWavelengths(int);
    const std::vector<Photon*> getActivePhotons();
    std::thread initializePhoton(int, float, std::string);    // create and start the thread for a new photon
    void destroyPhoton(int);
    int getNumberOfRunningThreads() const;
    void waitForOpening();
    int getNumberOfFiberCollisions() const;

private:
    std::vector<Photon*> m_activePhotons = {};
    std::vector<std::thread> m_activeThreads = {};
    std::discrete_distribution<int> m_emissionProbabilities;
    std::vector<double> m_wavelengths;
    std::vector<double> m_startingPos = {0,0,0}; // default
    int m_numberOfFiberCollisions = 0;
};
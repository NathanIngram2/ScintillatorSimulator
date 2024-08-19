#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <thread>

#include "./PhotonManager.h"
#include "./Environment.h"

/*
MonteCarlo class which is the main point of entry into the MC operations from main.
Makes calls to the PhotonManager to create, destroy, and update photon positions during the MC execution.

Acts as the reciever of the command pattern interactions such that main->command->here
*/
class MonteCarlo
{
protected:
    MonteCarlo();
    static MonteCarlo* MC_singleton;

public:
	~MonteCarlo();
	MonteCarlo(const MonteCarlo&) = delete;
	void operator=(const MonteCarlo&) = delete;
    static MonteCarlo* getInstance();

    void start();
    void update();
	void setMonteCarloParameters(const std::vector<std::string>&);	//set inital parameters based on parsed user inputs
    
	int getIterationNum() const;
    std::string getEstimatedTimeToCompletion(const int) const;
    std::string getScintillatorMaterial() const;
    float getFiberDiameter() const;
    float getFiberSpacing() const;
    std::string getFiberMaterial() const;
    std::string toString() const;

    void animateDemo() const;
    void displayConfiguration() const;
    void executeMC();
    void setFiberDiameter(const std::string&);
    void setFiberMaterial(const std::string&);
    void setFiberSpacing(const std::string&);
    void setLoggingLevel(const std::string&);
    void setNumOfPhotons(const std::string&);
    void setScintillatorMaterial(const std::string&);
    void setScatteringLength(const std::string&);
    void setPhotonManagerRef(PhotonManager*);
    void notify(int, bool);

private:
	float m_fiberSpacing;
    float m_fiberDiameter;
    std::string m_fiberMaterial;
    std::string m_scintillatorMaterial;   // changed by user, but defaults to LAB
    int m_numOfTotalIterations;
    int m_numOfActivePhotons = 0;     // increment on creation
    bool m_isRunning = false;
    int m_fiberCollisions;
    float m_scatteringLen;

    PhotonManager* m_photonManager;
    Environment* m_environment;

    // this should give a platform independent number of
    // supported concurrent threads for whatever machine this code is running on
    // int m_processorCount = std::thread::hardware_concurrency();
    int m_processorCount;
};

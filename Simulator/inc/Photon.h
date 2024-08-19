#pragma once

#include <string>
#include <vector>
#include <thread>
#include <iostream>

// 
class Photon
{
public:
    Photon(const std::vector<double>&, int, int, float, std::string);
	~Photon();
	Photon(const Photon&) = delete;
	void operator=(const Photon&) = delete;

    void updatePosition();
    const std::vector<double> getCurrentPosition();
    int getID() const;
    void calculateStepLength();
    void changeVelocity();
    std::string whatInteraction();
    
    bool readyToTerminate() const;
    static void startPhotonThread(Photon*);

private:
    std::vector<double> m_position = {0,0,0}; // to be overwritten
    std::vector<double> m_velocity = {0,0,0};
    int m_wavelength; // nm
    int m_identifier;
    float m_stepLen;
    float m_interactionProb;
    double m_ppoAbs;
    double m_scintillatorAbs;

    bool m_fibersExist = true;
    bool m_finished = false;
    bool m_hitFiber = false;

    std::string m_pathBuffer = "";
};

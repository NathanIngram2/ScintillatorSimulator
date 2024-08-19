#include "../inc/Photon.h"
#include "../inc/MonteCarlo.h"
#include "../inc/Environment.h"
#include <cstdlib>

Photon::Photon(const std::vector<double>& startPos, int iterNum, int waveLength, float scatteringLen, std::string scintillatorMat)
{
    m_identifier = iterNum;
    m_position = startPos;
    m_wavelength = waveLength;

    m_pathBuffer.append(std::to_string(m_identifier) + "; " + std::to_string(m_wavelength) + ": [");

    std::vector<double> ppoAbs;
    std::vector<double> scintillatorAbs;

    // std::cout << "database calls\n";
    try
    {
        ppoAbs = DataBase::getValsByKey("PPO_abs_length_interp", "../database/PPO_absorbtion.txt");
        scintillatorAbs = (scintillatorMat == "DIN") ? DataBase::getValsByKey("DIN_abs_length", "../database/CleanDIN_abs_length.txt")
                    : DataBase::getValsByKey("LAB_abs_length", "../database/CleanLab_abs_length.txt");
    }
    catch (std::exception e)
    {
        std::cout << "Photon init failed during database read calls.\n";
        exit(1);
    }

    // std::cout << "database calls end\n";

    // debug
    // if (m_identifier == 0)
    // {
    //     for (auto st : scintillatorAbs)
    //     {
    //         std::cout << st << std::endl;
    //     }
    // }

    m_ppoAbs = ppoAbs[m_wavelength-200];
    m_scintillatorAbs = scintillatorAbs[m_wavelength-250];

    m_interactionProb = (1/(scatteringLen/1000) + 1/m_scintillatorAbs + 1/m_ppoAbs);
    // std::cout << "First velocity call\n";
    this->changeVelocity();
    // std::cout << "got through constructor\n";
    if (Environment::getInstance()->isThereFibers() == false)
    {
        m_fibersExist = false;
    }
}

Photon::~Photon()
{
    // TODO
}

void Photon::updatePosition()
{
    // velocities are normalized, so this should step correctly
    m_pathBuffer.append(std::to_string(m_position[0]) + "," + std::to_string(m_position[1]) + "," + std::to_string(m_position[2]) + "\n");

    if (m_fibersExist == true)
    {
        // auto theta = atan(m_velocity[2]/(sqrt(m_velocity[0]*m_velocity[0] + m_velocity[1]*m_velocity[1])));

        // auto stepLenXY = m_stepLen/cos(theta);
        // if (stepLenXY < 0)
        // {
        //     stepLenXY = -stepLenXY;
        // }

        // std::cout << m_stepLen << ", " << stepLenXY << std::endl;
        // std::cout << "Angle for XY step length: " << theta << std::endl;
        // std::cout << "Calling Fiber Collision at position: " << m_position[0] << ", " << m_position[1] << std::endl;

        auto stepLenXY = sqrt(m_velocity[0]*m_stepLen*m_velocity[0]*m_stepLen + m_velocity[1]*m_stepLen*m_velocity[1]*m_stepLen);
        auto fiberWeHit = Environment::getInstance()->fiberCollision(m_position, m_velocity, stepLenXY);

        if (fiberWeHit.empty() == true)
        {
            // didn't hit anything, regular step
            m_position[0] += m_velocity[0]*m_stepLen;
            m_position[1] += m_velocity[1]*m_stepLen;
            m_position[2] += m_velocity[2]*m_stepLen;
        }
        else if (fiberWeHit == std::vector<double>({-999,-999}))
        {
            std::cout << "Photon out of simulator range, terminating at last position\n";
            m_finished = true;
        }
        else
        {
            auto xdiff = fiberWeHit[0]-m_position[0];
            auto fullstepx = m_velocity[0]*m_stepLen;

            auto scale = fullstepx/xdiff;
            m_position[0] = fiberWeHit[0];
            m_position[1] = fiberWeHit[1];
            m_position[2] += (m_velocity[2]*m_stepLen) / scale;
            m_finished = true;
            m_hitFiber = true;
        }
    }
    else
    {
        m_position[0] += m_velocity[0]*m_stepLen;
        m_position[1] += m_velocity[1]*m_stepLen;
        m_position[2] += m_velocity[2]*m_stepLen;
    }

    // auto finalPos = getCurrentPosition();
    // m_pathBuffer.append(std::to_string(finalPos[0]) + "," + std::to_string(finalPos[1]) + "," + std::to_string(finalPos[2]) + "] hitFiber=" + std::to_string(m_hitFiber) + "\n");
    // DataBase::writeToFile(m_pathBuffer, "../out/PhotonPaths.txt");
    // exit(0);
}

void Photon::changeVelocity()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

    double magnitude = 0;
    for (int i = 0; i < 3; i++)
    {
        m_velocity[i] = dis(gen);
        magnitude += m_velocity[i]*m_velocity[i];
    }
    magnitude = sqrt(magnitude);

    // std::cout << m_velocity[0] << ", " << m_velocity[1] << ", " << m_velocity[2] << "\n";
    // std::cout << magnitude << "\n";

    for (int i = 0; i < 3; i++)
    {
        m_velocity[i] = m_velocity[i]/magnitude;
    }
}

const std::vector<double> Photon::getCurrentPosition()
{
    return m_position;
}

int Photon::getID() const
{
    return m_identifier;
}

void Photon::calculateStepLength()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    float random_float = dis(gen);
    m_stepLen = ((-1/m_interactionProb) * log(1 - random_float));
}

std::string Photon::whatInteraction()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, m_interactionProb);
    float random_float = dis(gen);

    // std::cout << random_float << ", " << 1/m_ppoAbs << std::endl;

    if (random_float < (1/m_ppoAbs))
    {
        return "PPO Absorbtion";
    }
    else if (random_float < ((1/m_scintillatorAbs) + (1/m_ppoAbs)))
    {
        return "Scintillator Absorbtion";
    }
    else
    {
        return "Scattering";
    }
}

bool Photon::readyToTerminate() const
{
    return m_finished;
}

void Photon::startPhotonThread(Photon* photon)
{
    std::cout << "Launched thread from photon class with wavelength: " << photon->m_wavelength << std::endl;
    //std::cout << "Getting update from photon id: " << photon->m_identifier << " With wavelength: " << photon->m_wavelength << std::endl;

    while (1)
    {
        photon->calculateStepLength();
        photon->updatePosition();

        if (photon->readyToTerminate() == true)
        {
            break;
        }

        // bool hitAFiber = photon->checkForCollision();
        // if (hitAFiber)
        // {
        //     break;
        // }

        auto interaction = photon->whatInteraction();
        if (interaction == "PPO Absorbtion")
        {
            // std::cout << photon->m_identifier << " Absorbed by PPO\n";
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float> dis(0.0f, 1.0f);
            float random_float = dis(gen);
            if (random_float > 0.86)
            {
                break;
            }
            else
            {
                std::cout << photon->m_identifier << " Scattered by PPO\n";
                photon->changeVelocity();
            }
        }
        else if (interaction == "Scintillator Absorbtion")
        {
            std::cout << photon->m_identifier << " Absorbed by scintillator\n";
            break;
        }
        else if (interaction == "Scattering")
        {
            // DataBase::write(m_identifier, m_position, "file.csv");
            std::cout << photon->m_identifier << " Scattered\n";
            photon->changeVelocity();
        }
        else
        {
            std::cout << "Not an accepted interaction type. Failure in photon propagation.\n";
            exit(1);
        }

    }

    // tell the MC we're done here and are ready to be destroyed
    auto finalPos = photon->getCurrentPosition();
    photon->m_pathBuffer.append(std::to_string(finalPos[0]) + "," + std::to_string(finalPos[1]) + "," + std::to_string(finalPos[2]) + "] hitFiber=" + std::to_string(photon->m_hitFiber) + "\n");
    DataBase::writeToFile(photon->m_pathBuffer, "../out/PhotonPaths.txt");
    MonteCarlo::getInstance()->notify(photon->m_identifier, photon->m_hitFiber);
}
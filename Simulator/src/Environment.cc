#include "../inc/Environment.h"
#include "../inc/MonteCarlo.h"

#include <mutex>

constexpr float PI = 3.14159265359;
Environment* Environment::m_instance = nullptr;

Environment::Environment()
{

}

Environment::~Environment()
{

}

// this function may be accelerated with the use of a tree search algortihm such as a KD tree
std::vector<double> Environment::findClosestFiberInDirection(const std::vector<double>& pos, const std::vector<double>& velo) const
{
    auto xdir = 1;
    auto ydir = 1;

    if (velo[0] < 0)
    {
        xdir = -1;
    }
    if (velo[1] < 0)
    {
        ydir = -1;
    }

    // construct tempVec of any fibers that may be within our range
    std::vector<std::vector<double>> tempFiberPositions = {};
    float xdist;
    float ydist;
    float distance;
    for (auto fiberPos : m_positions)
    {
        if ((fiberPos[0]-pos[0] > 0 && xdir > 0) || (fiberPos[0]-pos[0] < 0 && xdir < 0))
        {
            if ((fiberPos[1]-pos[1] > 0 && ydir > 0) || (fiberPos[1]-pos[1] < 0 && ydir < 0))
            {
                xdist = fiberPos[0]-pos[0];
                ydist = fiberPos[1]-pos[1];
                distance = sqrt((xdist*xdist) + (ydist*ydist));
                tempFiberPositions.push_back({fiberPos[0], fiberPos[1], distance});
            }
        }
    }

    if (tempFiberPositions.empty() == false)
    {
        auto min = tempFiberPositions[0][2];
        std::vector<double> closest = tempFiberPositions[0];
        for (auto temp : tempFiberPositions)
        {
            if (temp[2] < min)
            {
                min = temp[2];
                closest = {temp[0],temp[1]};
            }
        }

        return closest;
    }
    else
    {
        std::cout << "Cannot find any close fiber in direction of velocity. Killing photon.\n";
        return {};
    }
}

// find the location of the first collision along a path
//  - return {} if no collision detected within path length
//  - return {-999, -999} (arbitrary) if photon leaves simulation bounds
std::vector<double> Environment::fiberCollision(const std::vector<double>& position, const std::vector<double>& velocity, float stepLen) const
{
    auto xdir = 1;
    auto ydir = 1;

    if (velocity[0] < 0)
    {
        xdir = -1;
    }
    if (velocity[1] < 0)
    {
        ydir = -1;
    }

    float totalDistTravelled = 0;
    float veloAngle;
    if (velocity[0] != 0)
    {
        veloAngle = atan(velocity[1]/velocity[0]);
        std::cout << veloAngle << std::endl;
    }
    else if (velocity[1] < 0)
    {
        veloAngle = PI/2;
    }
    else
    {
        std::cout << "Unexpected velocities being checked for collision.\n";
        exit(1);
    }

    std::string buff = "";

    auto totalDistTravelledX = xdir*totalDistTravelled*cos(veloAngle);
    auto totalDistTravelledY = ydir*totalDistTravelled*sin(veloAngle);
    buff.append(std::to_string(position[0] + totalDistTravelledX) + ", " + std::to_string(position[1] + totalDistTravelledY) + "\n");
    while (totalDistTravelled < stepLen)
    {
        totalDistTravelledX = xdir*totalDistTravelled*cos(veloAngle);
        totalDistTravelledY = ydir*totalDistTravelled*sin(veloAngle);
        buff.append(std::to_string(position[0] + totalDistTravelledX) + ", " + std::to_string(position[1] + totalDistTravelledY) + "\n");
        std::vector<double> pos = {position[0]+totalDistTravelledX, position[1]+totalDistTravelledY};
        auto closestFiberPosition = findClosestFiberInDirection(pos, velocity);
        if (closestFiberPosition.empty() == true)
        {
            return {NULL, NULL};
        }
        std::vector<double> posDiff = {closestFiberPosition[0]-pos[0], closestFiberPosition[1]-pos[1]};
        double posDiffMag = sqrt(posDiff[0]*posDiff[0] + posDiff[1]*posDiff[1]);
        double veloMag = sqrt(velocity[0]*velocity[0] + velocity[1]*velocity[1]); // isn't 1 even though we normalize at photon level because we're only considering x-y
        double dotProd = (posDiff[0]*velocity[0] + posDiff[1]*velocity[1]);
        double angle = acos(dotProd/(posDiffMag*veloMag));
        double dist = posDiffMag*sin(angle);
        if (dist <= m_diameter/2 && posDiffMag-(m_diameter/2) < stepLen)
        {
            std::cout << "Collision with fiber at positon: " << closestFiberPosition[0] << ", " << closestFiberPosition[1] << std::endl;
            DataBase::writeToFile(buff, "../out/testPositions.txt");
            totalDistTravelled += (dist / tan(angle));
            totalDistTravelledX = xdir*totalDistTravelled*cos(veloAngle);
            totalDistTravelledY = ydir*totalDistTravelled*sin(veloAngle);
            auto finalPos = std::vector<double>({position[0]+totalDistTravelledX, position[1]+totalDistTravelledY});
            buff.append(std::to_string(finalPos[0]) + ", " + std::to_string(finalPos[1]) + "\n");
            return closestFiberPosition;
        }
        else
        {
            totalDistTravelled += (dist / tan(angle));
        }
    }

    buff.append(std::to_string(position[0] + totalDistTravelledX) + ", " + std::to_string(position[1] + totalDistTravelledY) + "\n");
    DataBase::writeToFile(buff, "../out/testPositions.txt");
    return {};
}

// reconfigure environment so a full teardown is not needed to simulate different designs
std::vector<float> Environment::reconfig(float spacing, float diameter, std::string material)
{
    if (spacing == 0 || diameter == 0 || spacing <= diameter/2)
    {
        std::cout << "Either no spacing or diamater specified, or not enough spacing by diameter. No fibers will be simulated." << std::endl;
        m_fibersExist = false;
        return {0.f, 0.f, 0.f};
    }
    else
    {
        m_fibersExist = true;
    }
    std::cout << "Configuring Environment...\n";
    m_positions.clear();
    m_spacing = spacing;
    m_diameter = diameter;

    int numOfCylinders = m_width / spacing;
    for (int i = 0; i < numOfCylinders; i++)
    {
        for (int j = 0; j < numOfCylinders; j++)
        {
           float x = -m_width / 2 + i * spacing;
           float y = -m_width / 2 + j * spacing;
           m_positions.push_back({x, y});
        }
    }

    std::string buff = "";
    for (auto positions : m_positions)
    {
        buff.append(std::to_string(positions[0]) + ", " + std::to_string(positions[1]) + ",\n");
    }

    buff.pop_back(); // remove last \n char
    DataBase::writeToFile(buff, "../out/FiberPositions.txt");

    // find one of the innermost square units to begin simulation from
    auto middle = std::vector<float>({(m_positions.front()[0]+m_positions.back()[0])/2, (m_positions.front()[1]+m_positions.back()[1])/2});
    float closestX = fabs(middle[0] - m_positions.front()[0]);
    float closestY = fabs(middle[1] - m_positions.front()[1]);
    int xind = 0;
    int yind = 0;
    for (int i = 1; i < m_positions.size(); i++)
    {
        if (fabs(middle[0] - m_positions[i][0]) < closestX)
        {
            closestX = fabs(middle[0] - m_positions[i][0]);
            xind = i;
        }
        if (fabs(middle[1] - m_positions[i][1]) < closestY)
        {
            closestY = fabs(middle[1] - m_positions[i][1]);
            yind = i;
        }
    }
    auto innerSquareMiddle = std::vector<float>({(m_positions[xind-1][0] + m_positions[xind][0])/2, (m_positions[yind-1][1] + m_positions[yind][1])/2, 0.f});

    std::cout << "Done!\n";
    return innerSquareMiddle;
}

bool Environment::isThereFibers() const
{
    return m_fibersExist;
}
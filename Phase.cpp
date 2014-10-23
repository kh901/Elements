#include "Phase.h"

std::string const  Phase::phases[] = {
    "Submission",
    "Allocation",
    "Reviewing",
    "Finalising",
    "Completed"
};

Phase::Phase()
{
    currentPhase = phases[0];
    phaseID = 0;
}

std::string Phase::getCurrentPhase()
{
    return currentPhase;
}

void Phase::advancePhase()
{
    phaseID++;
    currentPhase = phases[phaseID];
}

void Phase::writeFile(std::ofstream &ofs) const
{
	appendData<int>(ofs, this->phaseID);	
}
void Phase::readFile(std::ifstream &ifs)
{	
	readData<int>(ifs, this->phaseID);
}

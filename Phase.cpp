#include "Phase.h"

std::string const  Phase::phases[] = {
    "Submission",
    "Allocation",
    "Reviewing",
    "Discussing",
    "Finalising",
    "Completed"
};

Phase::Phase()
{
    currentPhase = phases[0];
    phaseID = 0;
}

std::string Phase::getCurrentPhase() const
{
    return currentPhase;
}

int Phase::getCurrentId() const
{
	return phaseID;
}

void Phase::setCurrentId(const int val)
{
	if (val >= 0 && val < MAX_PHASE_NUM)
	{
		phaseID = val;
	}
}

void Phase::advancePhase()
{
	if (phaseID < MAX_PHASE_NUM)
	{
    	phaseID++;
    }
    currentPhase = phases[phaseID];
}

void Phase::writeFile(std::ofstream &ofs) const
{
	appendData<int>(ofs, this->phaseID);	
}
void Phase::readFile(std::ifstream &ifs)
{	
	readData<int>(ifs, this->phaseID);
	currentPhase = phases[phaseID];	
}

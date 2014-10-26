#include <iostream>
#include <string>
#include "File.h"

#define MAX_PHASE_NUM 6

class Phase : public FileIO
{
    public:
        Phase();
        void advancePhase();
        std::string getCurrentPhase() const;
        int getCurrentId() const;
        void setCurrentId(const int);
        
        // write and read this class to a binary file stream
		void writeFile(std::ofstream &) const;
		void readFile(std::ifstream &);
    private:
        std::string currentPhase;
        int phaseID;
        static const std::string phases[];
};

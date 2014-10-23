#include <iostream>
#include <string>

class Phase
{
    public:
        Phase();
        void advancePhase();
        std::string getCurrentPhase();
        
    private:
        std::string currentPhase;
        int phaseID;
        static const std::string phases[];
};
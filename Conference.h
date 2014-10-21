#include <string>

#ifndef CONFERENCE_H
#define CONFERENCE_H

class Conference
{
    public:
        Conference();
        
        
        enum phase {
            Phase_Submission,
            Phase_Allocation,
            Phase_Reviewing,
            Phase_Finalising,
            Phase_Completed
        };
    private:
        string name;
        string date;
        string location;
        
};
#endif

#ifndef SQUARE_H_
#define SQUARE_H_

#include <vector>
#include "Location.h"

/*
    struct for representing a square in the grid.
*/
struct Square
{
    bool isVisible, isWater, isHill, isFood, isSeen, done;
    int ant, hillPlayer;
    std::vector<int> deadAnts;

    double diff;

    int agent[5],diff_agent[20];
    int food[100],channel;

    bool passable;

    Square()
    {
        done = isVisible = isWater = isHill = isFood = isSeen = 0;
        ant = hillPlayer = -1;
        diff = 0.0;
        passable = 1;
        channel=0;
    };

    //resets the information for the square except water information
    void reset()
    {
        isVisible = 0;
        isHill = 0;
        isFood = 0;
        ant = hillPlayer = -1;
        deadAnts.clear();
        diff=done=0;
        passable = 1;
        channel=0;

        for (int i = 0; i < 100; ++i)
        {
            food[i]=0;
            if(i<20)
                diff_agent[i]=0;
            if(i<5)
                agent[i]=0;
        }

 /*       agent[0]=agent[1]=agent[2]=agent[3]=agent[4]=0;
        diff_agent[0]=diff_agent[1]=diff_agent[2]=0;
        diff_agent[3]=diff_agent[4]=diff_agent[5]=0;
        diff_agent[6]=diff_agent[7]=diff_agent[10]=0;
        diff_agent[8]=diff_agent[9]=diff_agent[11]=0;
        diff_agent[12]=diff_agent[13]=diff_agent[14]=0;
        diff_agent[15]=diff_agent[16]=diff_agent[17]=0;*/
    };
};

#endif //SQUARE_H_

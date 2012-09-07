#ifndef STATE_H_
#define STATE_H_

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <list>

#include "Timer.h"
#include "Bug.h"
#include "Square.h"
#include "Location.h"
#include "Visualize.h"
// #include "Astar.h"

using namespace std;

/*
    constants
*/
const int TDIRECTIONS = 4;
const char CDIRECTIONS[4] = {'N', 'E', 'S', 'W'};
const int DIRECTIONS[4][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };      //{N, E, S, W}

enum {N,E,S,W};
enum goals { FOOD,ENHILL,EXPLORE,WATER,FIGHT };
enum JOBS{NO_JOB,FOOD_GATHER,ATTACK_HILL,DEFEND_HILL,BACKUP,PATROL,ATTACK};
enum players{ME,ENEMY};
enum modes{NONE,NORMAL_RAD_SEARCH,EXTEND_RAD_SEARCH,NORMAL_RAD_CHECK,EXTEND_RAD_CHECK,SEMI_RAD_SEARCH,SEMISEMI_RAD_SEARCH};

/*
    struct to store current state information
*/
struct State
{
    /*
        Variables
    */
    int rows, cols,
        turn, turns,
        noPlayers,id,TotSeeds,TotFood;
    double attackradius,attackradius2, spawnradius, viewradius;
    double loadtime, turntime;
    std::vector<double> scores;
    bool gameover;



    std::vector<std::vector<Square> > grid;
    std::vector<Location> enemyAnts, visible;
    std::vector<Location> orders,targets;
    std::list<Location> myHills,food,old_food,myHills2,enemyHills,enemyHills2;
    std::list<Ant> myAnts;
    std::vector<Location> vision_offset;
    std::vector<Location> attack_offset,attack_offset1,attack_offset2,attack_offset3;
    
    Timer timer;
    Bug bug;
    Bug csv;
    Visualizer v;

    /*
        Functions
    */
    State();
    ~State();

    void setup();
    void reset();

    void makeMove(const Location &loc, int direction);
    void moveAnt(Ant &);

    bool passable(Location loc);
    int direction(Location,Location);
    bool intargets(Location);
    Location nearest_passable(Location );
    Location nearest_attackable(Ant ant);
    int nearest_safe(Ant &);
    bool existin(Location sLoc,Location cLoc,int mode);
    int attack(Ant);
    void bfs_diffuse();
    void diffuse_move(Ant &ant);
    void diffuse_move_agent(Ant &ant);
    int farthest_passable(Ant &ant);
    int dist(Location sLoc,Location dLoc);
    int hash_ants(Location sLoc,int player,int mode,std::list<Location> &);
    
    double distance(const Location &loc1, const Location &loc2);
    Location getLocation(const Location &startLoc, int direction);

    void updateVisionInformation();
};

std::ostream& operator<<(std::ostream &os, const State &state);
std::istream& operator>>(std::istream &is, State &state);

#endif //STATE_H_

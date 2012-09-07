#include "Bot.h"

using namespace std;
//constructor
Bot::Bot()
{

};

bool invector(std::list<Location> &lis,Location &loc)
{
    std::list<Location>::iterator it;

    for(it=lis.begin();it!=lis.end();++it)
        if((*it)==loc)
           return true;
    return false;
}



//plays a single game of Ants.
void Bot::playGame()
{
    //reads the game parameters and sets up    
    cin >> state;   
    state.setup();    
    endTurn();

    //continues making moves while the game is not over
    while(cin >> state)
    {
        state.updateVisionInformation();
        makeMoves();
        endTurn();
    }
};

//makes the bots moves for the turn
void Bot::makeMoves()
{
    state.bug <<endl<<endl<< "turn " << state.turn << ":" << endl;
   // state.bug << state << endl;
   state.bug<<"food size:"<<state.food.size()<<endl;
    // state.orders.clear();
    // state.targets.clear();

    // state.diffusion();
    state.bfs_diffuse();
    // state.agent_diffusion();

    int en,my;
    std::list<Location> dummy;

    std::list<Ant>::iterator ant,inant;
    /*identify ants need new path*/
    for(ant = state.myAnts.begin(); ant != state.myAnts.end(); ++ant)
    {
        // state.v.setFillColor(200,0,0,1);

        // for(std::vector<Location>::iterator itv=state.attack_offset2.begin();itv != state.attack_offset2.end(); ++itv)
        // {
        //     state.v.tileSubTile((*ant).loc.row+(*itv).row,(*ant).loc.col+(*itv).col,"MM");
        // }
        // if((*ant).job == FOOD_GATHER && !state.grid[(*ant).aim.row][(*ant).aim.col].isFood)
        // {
        //     (*ant).job=NO_JOB;
        //     (*ant).req_path=true;            
        // }
        // else if((*ant).job == ATTACK_HILL && !state.grid[(*ant).aim.row][(*ant).aim.col].isHill)
        // {
        //     (*ant).job=NO_JOB;
        //     (*ant).req_path=true;            
        // }
        // else if((*ant).job == DEFEND_HILL && !state.grid[(*ant).aim.row][(*ant).aim.col].isHill)
        // {
        //     (*ant).job=NO_JOB;
        //     (*ant).req_path=true;            
        // }

        // else if((*ant).job == BACKUP && (state.inattackrad((*ant).aim,1)==0 || state.inattackrad((*ant).aim,0)==0))
        // {
        //     (*ant).job=NO_JOB;
        //     (*ant).req_path=true;            
        // }
        // else /*if((*ant).job == PATROL)*/
        // {
            (*ant).job=NO_JOB;
            (*ant).req_path=false;            
        // }
        std::list<Location> loclist;
        (*ant).req_backup=false;
        (*ant).attackmode=false;
        (*ant).avail_units=(*ant).req_units=(*ant).send_units=0;
        // if();
        // {
        //     (*ant).job=PATROL;
        // }

        // if(((*ant).path.size() - (*ant).pos ) < 3)
            // (*ant).req_path=true;

        // if((*ant).aimset)
        // {
        //     if(state.distance((*ant).loc,(*ant).aim) <= 1)
        //         (*ant).aimset = false;
        // }
        
        
        // en = state.inattackrad((*ant).loc,0,r,c);

        en = state.hash_ants((*ant).loc,ENEMY,EXTEND_RAD_SEARCH,loclist);

        state.bug<<"Ant:"<<(*ant).id<<" loc=("<<(*ant).loc.row<<","<<(*ant).loc.col<<")"<<endl<<"enemy_ants ="<<en<<endl;        
        if(en > 0)
        {
            // my = state.noants((*ant).loc,1,r,c);
            std::list<Location>::iterator it=loclist.begin();
            my = 0;

            // for(it = loclist.begin(); it != loclist.end(); ++it)
                my += state.hash_ants((*it),ME,SEMISEMI_RAD_SEARCH,dummy);
            state.bug<<"my_ants="<<my<<endl;
            if(en<my)
            {
                (*ant).attackmode=true;
                (*ant).aim=loclist.front();
                (*ant).req_backup=false;
            }
            else
            {
                (*ant).attackmode=false;
                (*ant).req_backup=true;
                (*ant).req_units=en+1;
                (*ant).avail_units=my;
                (*ant).aim=loclist.front();
            }
            state.bug<<"attack="<<(*ant).attackmode<<" backup="<<(*ant).req_backup<<endl;
            state.bug<<"req_units="<<(*ant).req_units<<" avail_units="<<(*ant).avail_units<<endl;
        }
    }

    std::list<LocPair> enhilllp,foodlp,myhilllp,backuplp;
    std::list<Location>::iterator loc;
    std::list<LocPair>::iterator lp; //iterator

/*send backup*/
    for(ant = state.myAnts.begin(); ant != state.myAnts.end(); ++ant)
    {

        if( (*ant).req_backup && !(*ant).made_move)
        {
            state.bug<<"Ant "<<(*ant).id<<": job="<<(*ant).job<<" pos="<<(*ant).pos;
            state.bug<<" loc=("<<(*ant).loc.row<<","<<(*ant).loc.col<<")"<<" aim=("<<(*ant).aim.row<<","<<(*ant).aim.col<<")";
            state.bug<<" attackmode="<<(*ant).attackmode<<" reqbackup="<<(*ant).req_backup<<" mademode="<<(*ant).made_move<<endl;


            (*ant).dir = state.nearest_safe((*ant));
            (*ant).req_path = false;

            // (*ant).aim = state.nearest_attackable((*ant));
            // (*ant).job = ATTACK;
            // (*ant).req_path = true;
            // (*ant).req_astar = true;

            state.moveAnt(*ant);
            if(state.timer.getTime() > 0.9*state.turntime)
                    return;

            for(inant = state.myAnts.begin(); inant != state.myAnts.end(); ++inant)
            {
                if((*inant).job != DEFEND_HILL /*&& (*inant).job != BACKUP*/ && !(*inant).attackmode && !(*inant).made_move && !((*inant).loc == (*ant).loc))
                {
                    backuplp.push_back(LocPair(&(*inant),&(*ant),state.dist((*inant).loc,(*ant).loc)));
                }
            }
            backuplp.sort();
        }
        if((*ant).attackmode && !(*ant).made_move)
        {
            state.bug<<"Ant "<<(*ant).id<<": job="<<(*ant).job<<" pos="<<(*ant).pos;
            state.bug<<" loc=("<<(*ant).loc.row<<","<<(*ant).loc.col<<")"<<" aim=("<<(*ant).aim.row<<","<<(*ant).aim.col<<")";
            state.bug<<" attackmode="<<(*ant).attackmode<<" reqbackup="<<(*ant).req_backup<<" mademode="<<(*ant).made_move<<endl;


            (*ant).dir=state.attack((*ant));
            (*ant).req_path = false;
            state.moveAnt(*ant);

            if(state.timer.getTime() > 0.9*state.turntime)
                    return;
        }
    }
    

    for(lp=backuplp.begin();lp!=backuplp.end();lp++)
    {
        if( (*(*lp).ant2).send_units < (*(*lp).ant2).req_units && (*lp).dist<=state.viewradius)
        {
            (*(*lp).ant).aim = state.nearest_attackable((*(*lp).ant2));
            (*(*lp).ant).job = BACKUP;
            (*(*lp).ant2).req_path = true;
            (*(*lp).ant2).req_astar = true; 
            (*(*lp).ant2).send_units++;          
        }
    }

    /*calculate Location distance pair fo targets*/


//     for(ant = state.myAnts.begin(); ant != state.myAnts.end(); ++ant)
//     {
//         if((*ant).attackmode==false && (*ant).req_backup==false)
//         {
//             for(loc=state.food.begin();loc != state.food.end();++loc)
//             {
//                 foodlp.push_back(LocPair(&(*ant),&(*loc),state.distance((*ant).loc,*loc)));
//             }
//             for(loc=state.enemyHills.begin();loc != state.enemyHills.end();++loc)
//             {
//                 enhilllp.push_back(LocPair(&(*ant),&(*loc),state.distance((*ant).loc,*loc)));
//             }
//             for(loc=state.myHills.begin();loc != state.myHills.end();++loc)
//             {
//                 myhilllp.push_back(LocPair(&(*ant),&(*loc),state.distance((*ant).loc,*loc)));
//             }
//         }
//     }
//     if(state.timer.getTime() > 0.9*state.turntime)
//         return;
// /*sort all locations*/
//     foodlp.sort();
//     enhilllp.sort();
//     myhilllp.sort();



// /*assign jobs w.r.t location*/
// int c=0;
//     for(lp=myhilllp.begin();lp!=myhilllp.end();lp++)
//     {
//         if(state.hash_ants((*(*lp).dLoc),ENEMY,EXTEND_RAD_SEARCH,dummy) ||(!((*(*lp).dLoc).target > 3) && (*(*lp).dLoc).target <(0.1*state.myAnts.size()/state.myHills.size()) && (*(*lp).ant).job == NO_JOB))
//         {
//             (*(*lp).ant).aim = state.nearest_passable((*(*lp).dLoc));
//             (*(*lp).ant).job = DEFEND_HILL;
//             (*(*lp).dLoc).target++;
//             c++;
//         }
//         if(c>5) break;
//     }
//     for(lp=enhilllp.begin();lp!=enhilllp.end();lp++)
//     {
//         if((*(*lp).dLoc).target <= 0.1*state.myAnts.size() && (*(*lp).ant).job == NO_JOB)
//         {
//             (*(*lp).ant).aim = (*(*lp).dLoc);
//             (*(*lp).ant).job = ATTACK_HILL;
//             (*(*lp).dLoc).target++;
//         }
//     }
//     int foodgather=0;
//     for(lp=foodlp.begin();lp!=foodlp.end();lp++)
//     {
//         if(/*(*(*lp).dLoc).target <= 2 && */(*(*lp).ant).job == NO_JOB)
//         {
//             (*(*lp).ant).aim = (*(*lp).dLoc);
//             (*(*lp).ant).job = FOOD_GATHER;
//             (*(*lp).dLoc).target++;
//             foodgather++;
//         }
//         if(foodgather >= (int)0.7*state.myAnts.size())
//             break;
//     }   
//     int i=0;

    /*identify whether a* is needed*/
    // Location tLoc;
    // for(ant = state.myAnts.begin(); ant != state.myAnts.end(); ++ant)
    // {
    //     if((*ant).req_path==false && (*ant).made_move==false && (*ant).pos>0)
    //     {
    //         tLoc = (*ant).path[(*ant).pos+1];
    //         if(!state.passable(tLoc))
    //             (*ant).req_astar=true;
    //     }
    // }
 

    for(ant = state.myAnts.begin(); ant != state.myAnts.end(); ++ant)
    {
        if(((*ant).attackmode==false && (*ant).req_backup==false && (*ant).job == NO_JOB && (*ant).req_path == false) || invector(state.myHills,(*ant).loc))
        {
            (*ant).job = PATROL;

            state.bug<<"Ant "<<(*ant).id<<": job="<<(*ant).job<<" pos="<<(*ant).pos;
            state.bug<<" loc=("<<(*ant).loc.row<<","<<(*ant).loc.col<<")"<<" aim=("<<(*ant).aim.row<<","<<(*ant).aim.col<<")";
            state.bug<<" attackmode="<<(*ant).attackmode<<" reqbackup="<<(*ant).req_backup<<" mademode="<<(*ant).made_move<<endl;

            /*need to set aim here*/
            //(*ant).dir=
            // if((*ant).aimset == false)
                // state.farthest_passable((*ant));
            //(*ant).dir=-2;
            // state.diffuse_move(*ant);
            state.diffuse_move_agent(*ant);
            (*ant).req_path=false;
            state.moveAnt((*ant));

            if(state.timer.getTime() > 0.9*state.turntime)
                    return;
        }
    }
/*move ants*/
    for(ant = state.myAnts.begin(); ant != state.myAnts.end(); ++ant)
    {
        state.bug<<"Ant "<<(*ant).id<<": job="<<(*ant).job<<" pos="<<(*ant).pos;
        state.bug<<" loc=("<<(*ant).loc.row<<","<<(*ant).loc.col<<")"<<" aim=("<<(*ant).aim.row<<","<<(*ant).aim.col<<")";
        state.bug<<" attackmode="<<(*ant).attackmode<<" reqbackup="<<(*ant).req_backup<<" mademode="<<(*ant).made_move<<endl;

        if((*ant).req_path==true && (*ant).made_move==false)
        {
            if((*ant).req_astar || true)
            {
                state.bug<<"in astar"<<endl;
                Astar astar(state);
                astar.set_path((*ant));
                (*ant).pos=0;
                (*ant).req_astar=false;
            }
            else
            {
                //state.directions((*ant));
                //(*ant).pos=0;
            }
            (*ant).req_path=false;
        }

        if((*ant).made_move==false)
            state.moveAnt((*ant));
        (*ant).alive=false;

        if(state.timer.getTime() > 0.9*state.turntime)
                    return;
    }

    state.bug << "time taken: " << state.timer.getTime() << "ms" << endl << endl;
};

//finishes the turn
void Bot::endTurn()
{
    if(state.turn > 0)
        state.reset();
    state.turn++;

    cout << "go" << endl;
};

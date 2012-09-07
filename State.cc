#include "State.h"
#include "Astar.h"
#define MAX 1000

using namespace std;
bool astar_dist_mode=1;

std::list<Ant>::iterator G_it;
bool inlist(std::list<Ant> &ants,Location loc)
{
    std::list<Ant>::iterator it;

    for(it=ants.begin();it != ants.end(); ++it)
    {
        if((*it).loc == loc)
        {
            G_it = it;
            return true;
        }
    }
    return false;
}

std::list<Location>::iterator GL_it;
bool inlist(std::list<Location> &loclist,Location loc)
{
    std::list<Location>::iterator it;

    for(it=loclist.begin();it != loclist.end(); ++it)
    {
        if((*it) == loc)
        {
            GL_it = it;
            return true;
        }
    }
    return false;
}


//constructor
State::State()
{
    gameover = 0;
    turn = 0;
    id=0;
    bug.open("./debug.txt");
    bug<<"Debug file opened.."<<endl<<endl;
};

//deconstructor
State::~State()
{
    bug.close();
};

//sets the state up
void State::setup()
{
    grid = vector<vector<Square> >(rows, vector<Square>(cols, Square()));

    /*create vision offset*/
    
    double d;
    for(double d_row=-int(viewradius+2);d_row <= int(viewradius+2); d_row++)
        for(double d_col=-int(viewradius+2);d_col <=int(viewradius+2); d_col++)
        {
            d = d_row*d_row + d_col*d_col;
            if(d <= viewradius * viewradius)
            {    vision_offset.push_back(Location(d_row,d_col));
            }
        }    
    for(double d_row=(int)-(attackradius*2+1)-1;d_row <= (attackradius*2+1)+1; d_row++)
    {
        for(double d_col=(int)-(attackradius*2+1)-1;d_col <=(attackradius*2+1)+1; d_col++)
        {
            d = d_row*d_row + d_col*d_col;
            if(d <= (attackradius+3) * (attackradius+3))
            { 
                attack_offset3.push_back(Location(d_row,d_col));
            }
            if(d <= (attackradius+2) * (attackradius+2))
            { 
                attack_offset2.push_back(Location(d_row,d_col));
            }
            if(d <= (attackradius+1) * (attackradius+1))
            { 
                attack_offset1.push_back(Location(d_row,d_col));
            }
            if(d <= attackradius2)
            { 
                attack_offset.push_back(Location(d_row,d_col));
            }
        }
    }
};

void State::bfs_diffuse()
{
    int x,y,t,b,l,r;
    Location sLoc,nLoc,cLoc;
    int max_scent;
    double rad;
    
    double starttime = timer.getTime();

    list<Location> explore_seed;

//y-> x-> ok
    for(y=0; y < rows; ++y)
    {
        for(x = 0; x < cols; ++x)
        {
            if(!grid[y][x].isSeen && !grid[y][x].isWater)
            {
                explore_seed.push_back(Location(y,x));
                break;
            }
        }
        if(x<cols) break;
    }
//x-> y-> ok
    for(x = cols/2; x < cols; ++x)
    {
        for(y=0; y < rows; ++y)
        {        
            if(!grid[y][x].isSeen && !grid[y][x].isWater)
            {
                explore_seed.push_back(Location(y,x));
                break;
            }
        }
        if(y<rows) break;
    }
//y-> <-x ok
    for(y=0; y < rows; ++y)
    {
        for(x = cols-1; x >= 0; --x)
        {
            if(!grid[y][x].isSeen && !grid[y][x].isWater)
            {
                explore_seed.push_back(Location(y,x));
                break;
            }
        }
        if(x>=0) break;
    }
//<-x y->
    for(x = 0; x < cols; ++x)
    {
        for(y=rows/2; y < rows; ++y)
        {        
            if(!grid[y][x].isSeen && !grid[y][x].isWater)
            {
                explore_seed.push_back(Location(y,x));
                break;
            }
        }
        if(y<rows) break;
    }
//<-y x->
    for(y=rows-1; y >= 0; --y)
    {
        for(x = 0; x < cols; ++x)
        {
            if(!grid[y][x].isSeen && !grid[y][x].isWater)
            {
                explore_seed.push_back(Location(y,x));
                break;
            }
        }
        if(x<cols) break;
    }
//x-> <-y
    for(x = cols/2; x < cols; ++x)
    {
        for(y=rows-1; y >= 0; --y)
        {        
            if(!grid[y][x].isSeen && !grid[y][x].isWater)
            {
                explore_seed.push_back(Location(y,x));
                break;
            }
        }
        if( y >= 0) break;
    }
//<-y <-x
    for(y=rows-1; y >= 0; --y)
    {
        for(x = cols-1; x >= 0; --x)
        {
            if(!grid[y][x].isSeen && !grid[y][x].isWater)
            {
                explore_seed.push_back(Location(y,x));
                break;
            }
        }
        if(x>=0) break;
    }
//<-x <-y
    for(x = cols-1; x >= 0; --x)
    {   
        for(y=rows/2; y >= 0; --y)
        {        
            if(!grid[y][x].isSeen && !grid[y][x].isWater)
            {
                explore_seed.push_back(Location(y,x));
                break;
            }
        }
        if(y>=0) break;
    }

    for(y=0; y < rows; ++y)
    {
        for(x = 0; x < cols; ++x)
        {
            t = y-1;
            if(t<0) t += rows;
            b = y+1;
            b = b%rows;

            l=x-1;
            if(l<0) l += cols;
            r = x+1;
            r = r%cols;

            if(grid[y][x].isSeen && !grid[y][x].isVisible && !grid[y][x].isWater && 
                  ( grid[t][x].isVisible ||
                    grid[b][x].isVisible ||
                    grid[y][l].isVisible ||
                    grid[y][r].isVisible ))
            {
                explore_seed.push_back(Location(y,x));
                break;
            }
        }
        if(x<cols) break;
    }

    for(y=0; y < rows; ++y)
    {
        for(x = cols-1; x>=0; --x)
        {
            t = y-1;
            if(t<0) t += rows;
            b = y+1;
            b = b%rows;

            l=x-1;
            if(l<0) l += cols;
            r = x+1;
            r = r%cols;

            if(grid[y][x].isSeen && !grid[y][x].isVisible && !grid[y][x].isWater && 
                  ( grid[t][x].isVisible ||
                    grid[b][x].isVisible ||
                    grid[y][l].isVisible ||
                    grid[y][r].isVisible ))
            {
                explore_seed.push_back(Location(y,x));
                break;
            }
        }
        if(x>=0) break;
    }
    for(y=rows-1; y >=0; --y)
    {
        for(x = 0; x < cols; ++x)
        {
            t = y-1;
            if(t<0) t += rows;
            b = y+1;
            b = b%rows;

            l=x-1;
            if(l<0) l += cols;
            r = x+1;
            r = r%cols;

            if(grid[y][x].isSeen && !grid[y][x].isVisible && !grid[y][x].isWater && 
                  ( grid[t][x].isVisible ||
                    grid[b][x].isVisible ||
                    grid[y][l].isVisible ||
                    grid[y][r].isVisible ))
            {
                explore_seed.push_back(Location(y,x));
                break;
            }
        }
        if(x<cols) break;
    }
    
    for(y=rows-1; y >=0; --y)
    {
        for(x = cols-1; x>=0; --x)
        {
            t = y-1;
            if(t<0) t += rows;
            b = y+1;
            b = b%rows;

            l=x-1;
            if(l<0) l += cols;
            r = x+1;
            r = r%cols;

            if(grid[y][x].isSeen && !grid[y][x].isVisible && !grid[y][x].isWater && 
                  ( grid[t][x].isVisible ||
                    grid[b][x].isVisible ||
                    grid[y][l].isVisible ||
                    grid[y][r].isVisible ))
            {
                explore_seed.push_back(Location(y,x));
                break;
            }
        }
        if(x>=0) break;
    }
    for(y=0; y < rows; ++y)
    {
        for(x = 0; x < cols; ++x)
        {
            t = y-1;
            if(t<0) t += rows;
            b = y+1;
            b = b%rows;

            l=x-1;
            if(l<0) l += cols;
            r = x+1;
            r = r%cols;

            if(!grid[y][x].isSeen && !grid[y][x].isWater && 
                  ( grid[t][x].isVisible ||
                    grid[b][x].isVisible ||
                    grid[y][l].isVisible ||
                    grid[y][r].isVisible ))
            {
                explore_seed.push_back(Location(y,x));
                break;
            }
        }
        if(x<cols) break;
    }

    for(y=0; y < rows; ++y)
    {
        for(x = cols-1; x>=0; --x)
        {
            t = y-1;
            if(t<0) t += rows;
            b = y+1;
            b = b%rows;

            l=x-1;
            if(l<0) l += cols;
            r = x+1;
            r = r%cols;

            if(!grid[y][x].isSeen && !grid[y][x].isWater && 
                  ( grid[t][x].isVisible ||
                    grid[b][x].isVisible ||
                    grid[y][l].isVisible ||
                    grid[y][r].isVisible ))
            {
                explore_seed.push_back(Location(y,x));
                break;
            }
        }
        if(x>=0) break;
    }
    for(y=rows-1; y >=0; --y)
    {
        for(x = 0; x < cols; ++x)
        {
            t = y-1;
            if(t<0) t += rows;
            b = y+1;
            b = b%rows;

            l=x-1;
            if(l<0) l += cols;
            r = x+1;
            r = r%cols;

            if(!grid[y][x].isSeen && !grid[y][x].isWater && 
                  ( grid[t][x].isVisible ||
                    grid[b][x].isVisible ||
                    grid[y][l].isVisible ||
                    grid[y][r].isVisible ))
            {
                explore_seed.push_back(Location(y,x));
                break;
            }
        }
        if(x<cols) break;
    }
    
    for(y=rows-1; y >=0; --y)
    {
        for(x = cols-1; x>=0; --x)
        {
            t = y-1;
            if(t<0) t += rows;
            b = y+1;
            b = b%rows;

            l=x-1;
            if(l<0) l += cols;
            r = x+1;
            r = r%cols;

            if(!grid[y][x].isSeen && !grid[y][x].isWater && 
                  ( grid[t][x].isVisible ||
                    grid[b][x].isVisible ||
                    grid[y][l].isVisible ||
                    grid[y][r].isVisible ))
            {
                explore_seed.push_back(Location(y,x));
                break;
            }
        }
        if(x>=0) break;
    }

   // explore_seed = myHills;
    TotSeeds = 0;

    for (std::list<Location>::iterator i = explore_seed.begin(); i != explore_seed.end(); ++i,TotSeeds++)
    {
        v.star((*i).row,(*i).col,1,2,5,true);
        sLoc = *i;

        max_scent = 500 + (int(timer.getTime()/100)%10)*10;

        std::queue<Location> Queue;

        std::vector<std::vector<bool> > visited(rows, std::vector<bool>(cols, 0));
        visited[sLoc.row][sLoc.col] = 1;
        grid[sLoc.row][sLoc.col].diff_agent[TotSeeds] = max_scent;

        for(int d=0; d<TDIRECTIONS; d++)
        {
            nLoc = getLocation(sLoc,d);
            nLoc.target = 1;
            if(!grid[nLoc.row][nLoc.col].isWater)
                Queue.push(nLoc);
            visited[nLoc.row][nLoc.col] = 1;
        }

        while(!Queue.empty())
        {
            cLoc = Queue.front();
            Queue.pop();            

            // grid[cLoc.row][cLoc.col].diff_agent[TotSeeds] = cLoc.target;
            grid[cLoc.row][cLoc.col].diff_agent[TotSeeds] = (max_scent - cLoc.target)+ timer.getTime()/100;
            // v.setFillColor(200,10,10,grid[cLoc.row][cLoc.col].diff/max_scent);
            // v.tile(cLoc.row,cLoc.col);
            
            for(int d=0; d<TDIRECTIONS; d++)
            {
                nLoc = getLocation(cLoc,d);
                nLoc.target = cLoc.target + 1;
                if(!grid[nLoc.row][nLoc.col].isWater && !visited[nLoc.row][nLoc.col])// && distance(sLoc, nLoc) <= viewradius*2)
                    Queue.push(nLoc);
                visited[nLoc.row][nLoc.col]=1;
            }
        }
    }
    
    TotFood=1;
    for (std::list<Location>::iterator i = food.begin(); i != food.end(); ++i,TotFood++)
    {
        sLoc = *i;

        max_scent = 1500 +(int(timer.getTime()/100)%10)*50;

        std::queue<Location> Queue;

        std::vector<std::vector<bool> > visited(rows, std::vector<bool>(cols, 0));
        visited[sLoc.row][sLoc.col] = 1;
        grid[sLoc.row][sLoc.col].food[TotFood] = max_scent;

        for(int d=0; d<TDIRECTIONS; d++)
        {
            nLoc = getLocation(sLoc,d);
            nLoc.target = 1;
            if(!grid[nLoc.row][nLoc.col].isWater /*&& grid[nLoc.row][nLoc.col].isVisible*/)
                Queue.push(nLoc);
            visited[nLoc.row][nLoc.col] = 1;
        }

        while(!Queue.empty())
        {
            cLoc = Queue.front();
            Queue.pop();    

            grid[cLoc.row][cLoc.col].food[TotFood] = (max_scent - cLoc.target);

            if(grid[cLoc.row][cLoc.col].channel == 0 && grid[cLoc.row][cLoc.col].ant==0)
            {
                grid[cLoc.row][cLoc.col].channel = TotFood;
                break;
            }
            
            // v.setFillColor(255,(grid[cLoc.row][cLoc.col].diff/max_scent)*256,100,0.1);
            // v.tile(cLoc.row,cLoc.col);

            for(int d=0; d<TDIRECTIONS; d++)
            {
                nLoc = getLocation(cLoc,d);
                nLoc.target = cLoc.target + 1;
                if(!grid[nLoc.row][nLoc.col].isWater /*&& grid[nLoc.row][nLoc.col].isVisible*/ && !visited[nLoc.row][nLoc.col] && distance(sLoc, nLoc) <= viewradius*3/2)
                    Queue.push(nLoc);
                visited[nLoc.row][nLoc.col]=1;
            }
        }

    }

    for (std::list<Location>::iterator i = enemyHills.begin(); i != enemyHills.end(); ++i)
    {
        sLoc = *i;
        max_scent=2000;
        std::queue<Location> Queue;

        std::vector<std::vector<bool> > visited(rows, std::vector<bool>(cols, 0));
        visited[sLoc.row][sLoc.col] = 1;
        grid[sLoc.row][sLoc.col].agent[ENHILL] += max_scent;

        for(int d=0; d<TDIRECTIONS; d++)
        {
            nLoc = getLocation(sLoc,d);
            nLoc.target = 1;
            if(!grid[nLoc.row][nLoc.col].isWater && grid[nLoc.row][nLoc.col].isVisible)
                Queue.push(nLoc);
            visited[nLoc.row][nLoc.col] = 1;
        }

        while(!Queue.empty())
        {
            cLoc = Queue.front();
            Queue.pop();

      
            grid[cLoc.row][cLoc.col].agent[ENHILL] += (max_scent - cLoc.target); // (grid[t][cLoc.col].diff+grid[b][cLoc.col].diff+grid[cLoc.row][l].diff+grid[cLoc.row][r].diff)/4;
      
            // v.setFillColor(10,10,grid[cLoc.row][cLoc.col].diff*256/max_scent,0.1);
            // v.tile(cLoc.row,cLoc.col);

            for(int d=0; d<TDIRECTIONS; d++)
            {
                nLoc = getLocation(cLoc,d);
                nLoc.target = cLoc.target + 1;
                if(!grid[nLoc.row][nLoc.col].isWater && !visited[nLoc.row][nLoc.col] && grid[nLoc.row][nLoc.col].isVisible)// && distance(sLoc, nLoc) <= viewradius*2)
                    Queue.push(nLoc);
                visited[nLoc.row][nLoc.col]=1;
            }

        }
    }
 
   //  if(turn == 100)
   //  {
   //      csv.open("turn100.csv");

   //      for(y=0; y < rows; ++y)
   //      {       
   //          for(x = 0; x < cols; ++x)
   //          {
   //              csv<<grid[y][x].agent[FOOD]<<",";
   //          }
   //          csv<<endl;
   //      }
   //      csv.close();
   // }
   // if(turn == 101)
   //  {
   //      csv.open("turn101.csv");

   //      for(y=0; y < rows; ++y)
   //      {       
   //          for(x = 0; x < cols; ++x)
   //          {
   //              csv<<grid[y][x].agent[FOOD]<<",";
   //          }
   //          csv<<endl;
   //      }
   //      csv.close();
   // }
   
    bug<<"Diffusion time :"<<timer.getTime() - starttime<<endl;

}

void State::diffuse_move_agent(Ant &ant)
{
    unsigned short goal=10,best_dir=4;
    int best_agent,best_explore,best_water;
    Location nLoc;

    if( grid[ant.loc.row][ant.loc.col].agent[ENHILL] > 0)
        goal = ENHILL;
    // else if(grid[ant.loc.row][ant.loc.col].agent[FOOD] > 0)
    //     goal = FOOD;
    else if(grid[ant.loc.row][ant.loc.col].channel > 0)
    {      
            best_agent = 0;
            int pos = grid[ant.loc.row][ant.loc.col].channel;

            bug<<endl<<"diffuse food values:"<<endl;
            for(int d=0; d<TDIRECTIONS; d++)
            {
                nLoc = getLocation(ant.loc,d);
                bug<<grid[nLoc.row][nLoc.col].food[pos]<<endl;
                if(passable(nLoc) && grid[nLoc.row][nLoc.col].food[pos] > best_agent)
                {
                    best_agent = grid[nLoc.row][nLoc.col].food[pos];
                    best_dir = d;
                }             
            }            
            ant.dir = best_dir;
            return;
    }

    if(goal == ENHILL)
    {
        best_agent = 0;//grid[ant.loc.row][ant.loc.col].agent[goal];

        bug<<endl<<"diffuse enhill values:"<<endl;
        for(int d=0; d<TDIRECTIONS; d++)
        {
            nLoc = getLocation(ant.loc,d);
            bug<<grid[nLoc.row][nLoc.col].agent[goal]<<endl;
            if(passable(nLoc) && grid[nLoc.row][nLoc.col].agent[goal] > best_agent)
            {
                best_agent = grid[nLoc.row][nLoc.col].agent[goal];
                best_dir = d;
            }             
        }        
    }
    else
    {
        bug<<"entered explore mode"<<endl;
        best_explore = 0;//grid[ant.loc.row][ant.loc.col].agent[EXPLORE];
        int u=0;

        if(!TotSeeds) TotSeeds++;

        while(!grid[ant.loc.row][ant.loc.col].diff_agent[(ant.id+u)%TotSeeds]) 
        {
            u++;
            if(u > TotSeeds) break;
        }
        
        bug<<endl<<"diffuse explore values:"<<endl;
        for(int d=0; d<TDIRECTIONS; d++)
        {
            nLoc = getLocation(ant.loc,d);

            bug<<TotSeeds<<endl;
            bug<<grid[nLoc.row][nLoc.col].diff_agent[(ant.id+u)%TotSeeds]<<endl;

            if(passable(nLoc) && grid[nLoc.row][nLoc.col].diff_agent[(ant.id+u)%TotSeeds] > best_explore)
            {
                best_explore = grid[nLoc.row][nLoc.col].diff_agent[(ant.id+u)%TotSeeds];
                best_dir = d;
            }
         }
     }
    ant.dir = best_dir;
    return;
}

void State::diffuse_move(Ant &ant)
{
    /*int row,col;
    Location nLoc;

    std::list<DiffDirPair> diffs;

    bug<<endl<<"diffuse values:"<<endl;
    for(int d=0; d<TDIRECTIONS; d++)
    {
        nLoc = getLocation(ant.loc,d);
        diffs.push_back(DiffDirPair(d,grid[nLoc.row][nLoc.col].diff));
        bug<<grid[nLoc.row][nLoc.col].diff<<endl;
    }
    diffs.sort();

    for(std::list<DiffDirPair>::iterator it = diffs.begin(); it != diffs.end(); ++it)
    {
        nLoc = getLocation(ant.loc,(*it).dir);
        if(passable(nLoc) )
        {
            ant.dir = (*it).dir;
            return;
        }
    }
    ant.dir = diffs.front().dir;*/

    Location nLoc;
    double best_diff=0;grid[ant.loc.row][ant.loc.col].diff;
    int best_dir = 4,dir;

    for(int d=0; d<TDIRECTIONS; d++)
    {
        dir = (d+ant.id)%TDIRECTIONS;
        nLoc = getLocation(ant.loc,dir);
        if(grid[nLoc.row][nLoc.col].diff > best_diff)
        {
            best_diff = grid[nLoc.row][nLoc.col].diff;
            best_dir = dir;
        }
    }
    ant.dir = best_dir;
}

int State::hash_ants(Location sLoc,int player,int mode,std::list<Location> &dLoc)
{
    long r,c,ants=0;

    if(mode==SEMISEMI_RAD_SEARCH) //search in normal attack radius
    {
        for(int l=0;l<(int) attack_offset2.size();++l)
        {
            r = (sLoc.row+attack_offset2[l].row)%rows;
            c = (sLoc.col+attack_offset2[l].col)%cols;
            if(r<0) r+=rows;
            if(c<0) c+=cols;

            if(player==0)
            {
                if(grid[r][c].ant == 0)
                {
                    dLoc.push_back(Location(r,c));
                    ants++;
                }      
            }
            else if(grid[r][c].ant > 0)
            {
                    dLoc.push_back(Location(r,c));
                    ants++;
            }
        }
        return ants;
    }
    if(mode==SEMI_RAD_SEARCH) //search in normal attack radius
    {
        for(int l=0;l<(int) attack_offset1.size();++l)
        {
            r = (sLoc.row+attack_offset1[l].row)%rows;
            c = (sLoc.col+attack_offset1[l].col)%cols;
            if(r<0) r+=rows;
            if(c<0) c+=cols;

            if(player==0)
            {
                if(grid[r][c].ant == 0)
                {
                    dLoc.push_back(Location(r,c));
                    ants++;
                }      
            }
            else if(grid[r][c].ant > 0)
            {
                    dLoc.push_back(Location(r,c));
                    ants++;
            }
        }
        return ants;
    }
    if(mode==NORMAL_RAD_SEARCH) //search in normal attack radius
    {
        for(int l=0;l<(int) attack_offset.size();++l)
        {
            r = (sLoc.row+attack_offset[l].row)%rows;
            c = (sLoc.col+attack_offset[l].col)%cols;
            if(r<0) r+=rows;
            if(c<0) c+=cols;

            if(player==0)
            {
                if(grid[r][c].ant == 0)
                {
                    dLoc.push_back(Location(r,c));
                    ants++;
                }      
            }
            else if(grid[r][c].ant > 0)
            {
                    dLoc.push_back(Location(r,c));
                    ants++;
            }
        }
        return ants;
    }
    if(mode == EXTEND_RAD_SEARCH) //search in extended attack radius
    {
        for(int l=0;l<(int) attack_offset3.size();++l)
        {
            r = (sLoc.row+attack_offset3[l].row)%rows;
            c = (sLoc.col+attack_offset3[l].col)%cols;
            if(r<0) r+=rows;
            if(c<0) c+=cols;

            if(player==0)
            {
                if(grid[r][c].ant == 0)
                {
                    dLoc.push_back(Location(r,c));
                    ants++;
                }      
            }
            else if(grid[r][c].ant > 0)
            {
                    dLoc.push_back(Location(r,c));
                    ants++;
            }
        }
        return ants;
    }
    std::list<Location>::iterator it;
    if(mode == NORMAL_RAD_CHECK) //return if dLoc is present in normal rad
    {
        for(it=dLoc.begin(); it != dLoc.end(); ++it)
            for(int l=0;l<(int) attack_offset.size();++l)
            {
                r = ((*it).row+attack_offset[l].row)%rows;
                c = ((*it).col+attack_offset[l].col)%cols;
                if(r<0) r+=rows;
                if(c<0) c+=cols;

                if(r == sLoc.row && c == sLoc.col)
                    return 1;
            }
    
        return 0;
    }
    if(mode == EXTEND_RAD_CHECK) //return if dLoc is present in extended rad
    {
        for(it=dLoc.begin(); it != dLoc.end(); ++it)
            for(int l=0;l<(int) attack_offset3.size();++l)
            {
                r = (sLoc.row+attack_offset3[l].row)%rows;
                c = (sLoc.col+attack_offset3[l].col)%cols;
                if(r<0) r+=rows;
                if(c<0) c+=cols;

                if(inlist(dLoc,Location(r,c)))
                    return 1;                
            }
        return 0;
    }
}

bool State::existin(Location sLoc,Location cLoc,int mode)
{
    int r,c;
    if(mode == SEMISEMI_RAD_SEARCH)
    {
        for(int l=0;l<(int) attack_offset2.size();++l)
        {
            r = (sLoc.row+attack_offset2[l].row)%rows;
            c = (sLoc.col+attack_offset2[l].col)%cols;
            if(r<0) r+=rows;
            if(c<0) c+=cols;

            if(r == cLoc.row && c == cLoc.col)
                return 1;
        }
        return 0;
    }
    if(mode == SEMI_RAD_SEARCH)
    {
        for(int l=0;l<(int) attack_offset1.size();++l)
        {
            r = (sLoc.row+attack_offset1[l].row)%rows;
            c = (sLoc.col+attack_offset1[l].col)%cols;
            if(r<0) r+=rows;
            if(c<0) c+=cols;

            if(r == cLoc.row && c == cLoc.col)
                return 1;
        }
        return 0;
    }
    if(mode == NORMAL_RAD_SEARCH)
    {
        for(int l=0;l<(int) attack_offset.size();++l)
        {
            r = (sLoc.row+attack_offset[l].row)%rows;
            c = (sLoc.col+attack_offset[l].col)%cols;
            if(r<0) r+=rows;
            if(c<0) c+=cols;

            if(r == cLoc.row && c == cLoc.col)
                return 1;
        }
        return 0;
    }
}

//resets all non-water squares to land and clears the bots ant vector
void State::reset()
{
    //myAnts.clear();
    enemyAnts.clear();

    //myHills2=myHills;
    myHills.clear();

    //enemyHills2=enemyHills;
    enemyHills.clear();

    //food2 = food;
    // food.clear();

    for(int row=0; row<rows; row++)
        for(int col=0; col<cols; col++)
            if(!grid[row][col].isWater)
                grid[row][col].reset();
};

void State::moveAnt(Ant &ant)
{
    int dir;

    if(ant.made_move) return;

    if(ant.dir>=0 && ant.dir < TDIRECTIONS)
        dir = ant.dir;
    else if(ant.dir == TDIRECTIONS)
    {
        bug<<"stood still"<<endl;
        ant.alive=false;
        ant.made_move=true;
        //orders.push_back(ant.loc);
        grid[ant.loc.row][ant.loc.col].passable=0;
        bug<<"made_move"<<endl;
        return;
    }
    else if(ant.path.size()>0)
    {
        dir = direction(ant.path[ant.pos],ant.path[ant.pos+1]);
        ant.pos++;
    }
    else
    {
        bug<<"move dir:"<<ant.dir<<endl;
        bug<<"not movable"<<endl;
        grid[ant.loc.row][ant.loc.col].passable=0;
        ant.dir=-1;
        return;
    }

    bug<<"move dir:"<<dir<<endl;

    
    Location nLoc = getLocation(ant.loc,dir);
    if(passable(nLoc))
    {
        makeMove(ant.loc,dir);
        // orders.push_back(nLoc);
        grid[nLoc.row][nLoc.col].passable=0;
        ant.loc = nLoc;
        ant.alive=false;
        ant.made_move=true;
        ant.req_path=false;
        bug<<"made_move"<<endl;
    }
    else bug<<"not movable"<<endl;
    ant.dir = -1;
}

//outputs move information to the engine
void State::makeMove(const Location &loc, int direction)
{
    cout << "o " << loc.row << " " << loc.col << " " << CDIRECTIONS[direction] << endl;    

    Location nLoc = getLocation(loc, direction);
    grid[nLoc.row][nLoc.col].ant = grid[loc.row][loc.col].ant;
    grid[loc.row][loc.col].ant = -1;
};

//returns the euclidean distance between two locations with the edges wrapped
double State::distance(const Location &loc1, const Location &loc2)
{
    int d1 = abs(loc1.row-loc2.row),
        d2 = abs(loc1.col-loc2.col),
        dr = min(d1, rows-d1),
        dc = min(d2, cols-d2);
    return sqrt(dr*dr + dc*dc);
};

//returns the new location from moving in a given direction with the edges wrapped
Location State::getLocation(const Location &loc, int direction)
{
    return Location( (loc.row + DIRECTIONS[direction][0] + rows) % rows,
                     (loc.col + DIRECTIONS[direction][1] + cols) % cols );
};

/*
    This function will update update the lastSeen value for any squares currently
    visible by one of your live ants.

    BE VERY CAREFUL IF YOU ARE GOING TO TRY AND MAKE THIS FUNCTION MORE EFFICIENT,
    THE OBVIOUS WAY OF TRYING TO IMPROVE IT BREAKS USING THE EUCLIDEAN METRIC, FOR
    A CORRECT MORE EFFICIENT IMPLEMENTATION, TAKE A LOOK AT THE GET_VISION FUNCTION
    IN ANTS.PY ON THE CONTESTS GITHUB PAGE.
*/
State *temp_state;
bool killed(const Ant &ant)
{
    return (ant.alive==false);
}

bool ntexist(const Location &food)
{
    if(temp_state->grid[food.row][food.col].isVisible && !temp_state->grid[food.row][food.col].isFood)
        return true;
    return false;
}

void State::updateVisionInformation()
{
    

    std::list<Ant>::iterator a;
    //remove targets of killed ants
    /*
    for(a = myAnts.begin();a != myAnts.end(); a++)
    {
        if((*a).alive == false)
        {
            if((*a).job==FOOD_GATHER)
            {
                if(inlist(food,(*a).aim))
                    (*GL_it).target--;
            }
            else if((*a).job == ATTACK_HILL)
            {
                if(inlist(enemyHills,(*a).aim))
                    (*GL_it).target--;
            }
            else if((*a).job == DEFEND_HILL)
            {
                if(inlist(myHills,(*a).aim))
                    (*GL_it).target--;
            }
        }
    }*/

    temp_state = this;
/*clear killed ants*/
    myAnts.remove_if(killed);
    

    long r,c;
    visible.clear();
       
/*set visible squares using offset*/
    for(a = myAnts.begin();a != myAnts.end(); a++)
    {
        grid[(*a).loc.row][(*a).loc.col].isVisible=1;
        grid[(*a).loc.row][(*a).loc.col].isSeen=1;
        for(vector<Location>::iterator l=vision_offset.begin();
            l != vision_offset.end(); ++l)
        {
            r = ((*a).loc.row+(*l).row)%rows;
            c = ((*a).loc.col+(*l).col)%cols;
            if(r<0) r+=rows;
            if(c<0) c+=cols;
            grid[r][c].isVisible=1;
            grid[r][c].isSeen = true;
            // bug<<"("<<r<<","<<c<<") \n";
            //visible.push_back(Location(r,c));
        }
    }

    food.remove_if(ntexist);
};

int State::farthest_passable(Ant &ant)
{
    Location sLoc=ant.loc;
    Location &dLoc=ant.aim;
    int id=(int)timer.getTime()%10;
    std::queue<Location> locStack;
    Location nLoc,cLoc;
    std::vector<std::vector<bool> > visited(rows, std::vector<bool>(cols, 0));
    visited[sLoc.row][sLoc.col] = 1;

    sLoc.target = 0;
    sLoc.dir=0;

    for(int d=0; d<TDIRECTIONS; d++)
    {
        nLoc = getLocation(sLoc, (d+id)%TDIRECTIONS);
        nLoc.target=1;
        nLoc.dir=(d+id)%TDIRECTIONS;
        if(passable(nLoc))
            locStack.push(nLoc);
    }
    while(!locStack.empty())
    {
        cLoc = locStack.front();
        locStack.pop();

        if(timer.getTime() > 0.5*turntime)
        {        
            ant.aim = cLoc;
            ant.aimset = true;
            ant.req_path=true;
            ant.dir=-1;
            return -2;
        }

        for(int d=0; d<TDIRECTIONS; d++)
        {
            nLoc = getLocation(cLoc, (d+id)%TDIRECTIONS);
            nLoc.target = cLoc.target + 1;
            nLoc.dir=cLoc.dir;
            if(!grid[nLoc.row][nLoc.col].isVisible || distance(sLoc,cLoc) >= viewradius-2)
            {
                ant.aim = cLoc;
                ant.aimset = true;
                ant.req_path = true;
                ant.dir=-1;
                return cLoc.dir;
            }
            else if(passable(nLoc) && !visited[nLoc.row][nLoc.col] && distance(sLoc,nLoc) < viewradius)
                locStack.push(nLoc);
            else 
            visited[nLoc.row][nLoc.col]=1;
        }
       
    }
    ant.dir = cLoc.dir;
    ant.aimset=false;
}

int State::attack(Ant ant)
{
    std::queue<Location> locQueue;
    Location nLoc,cLoc,sLoc = ant.loc;
    std::vector<std::vector<bool> > visited(rows, std::vector<bool>(cols, 0));
    visited[sLoc.row][sLoc.col] = 1;

    std::list<Location> enAnts;
    hash_ants(ant.aim,ENEMY,NORMAL_RAD_CHECK,enAnts);

    sLoc.target = 0;
    sLoc.dir=0;

    for(int d=TDIRECTIONS-1; d>=0; d--)
    {
        nLoc = getLocation(sLoc, d);
        nLoc.target=1;
        nLoc.dir=d;
        if(passable(nLoc))
            locQueue.push(nLoc);
    }
    while(!locQueue.empty())
    {
        cLoc = locQueue.front();
        locQueue.pop();

        if(inlist(enemyHills,cLoc) || existin(ant.aim,cLoc,NORMAL_RAD_SEARCH))//hash_ants(cLoc,ENEMY,NORMAL_RAD_CHECK,enAnts)/*isinattackrad(ant.aim,cLoc,1)*/ || cLoc==ant.aim)
            return cLoc.dir;
        
        for(int d=TDIRECTIONS-1; d>=0; d--)
        {
            nLoc = getLocation(cLoc, d);
            nLoc.target = cLoc.target + 1;
            nLoc.dir=cLoc.dir;
            if(passable(nLoc) && !visited[nLoc.row][nLoc.col])
                locQueue.push(nLoc);
            visited[nLoc.row][nLoc.col]=1;
        }
    }
    return -2;
}

int State::dist(Location sLoc,Location dLoc)
{
    if(astar_dist_mode && timer.getTime() < 0.7*turntime)
    {
        Astar astar(*this);
        return astar.dist(sLoc,dLoc);
    }
    return distance(sLoc,dLoc);    
}

int State::nearest_safe(Ant &ant)
{

    if(passable(ant.loc) && (!existin(ant.aim,ant.loc,SEMI_RAD_SEARCH) || !existin(ant.aim,ant.loc,EXTEND_RAD_SEARCH)))//&& existin(sLoc,nLoc,SEMISEMI_RAD_SEARCH))
    {
        return 4;
    }

    std::queue<Location> locQueue;
    Location nLoc,cLoc,sLoc = ant.loc;
    std::vector<std::vector<bool> > visited(rows, std::vector<bool>(cols, 0));
    visited[sLoc.row][sLoc.col] = 1;

    sLoc.target = 0;
    sLoc.dir=0;

    std::list <Location> dummy;
    for(int d=0; d<TDIRECTIONS; d++)
    {
        nLoc = getLocation(sLoc, d);
        nLoc.target=1;
        nLoc.dir=d;
        // if(passable(nLoc) && !hash_ants(nLoc,ENEMY,SEMISEMI_RAD_SEARCH,dummy))
        //     return d;//locQueue.push(nLoc);
        if(passable(nLoc) && (!existin(ant.aim,nLoc,SEMI_RAD_SEARCH) || !existin(ant.aim,nLoc,EXTEND_RAD_SEARCH)))//&& existin(sLoc,nLoc,SEMISEMI_RAD_SEARCH))
            return nLoc.dir;
    }
    return -2;        
}

Location State::nearest_passable(Location sLoc)
{
    std::queue<Location> locQueue;
    Location nLoc;
    std::vector<std::vector<bool> > visited(rows, std::vector<bool>(cols, 0));
    visited[sLoc.row][sLoc.col] = 1;

    while(visited[sLoc.row][sLoc.col])
    {
        for(int d=0; d<TDIRECTIONS; d++)
        {
            nLoc = getLocation(sLoc, d);
            if(passable(nLoc))
                return nLoc;
            else
            {
                visited[nLoc.row][nLoc.col]=true;
                locQueue.push(nLoc);
            }
        }

        sLoc = locQueue.front();
        locQueue.pop();
    }
}
Location State::nearest_attackable(Ant ant)
{
    std::queue<Location> locQueue;
    Location nLoc,sLoc = ant.loc;
    std::list<Location> dummy;
    std::vector<std::vector<bool> > visited(rows, std::vector<bool>(cols, 0));
    visited[sLoc.row][sLoc.col] = 1;

    int i=0;
    while(i<viewradius*viewradius)
    {
        for(int d=0; d<TDIRECTIONS; d++)
        {
            nLoc = getLocation(sLoc, d);
            if(!visited[nLoc.row][nLoc.col])
            {
                if(passable(nLoc))
                {
                    //if(!hash_ants(nLoc,ENEMY,SEMI_RAD_SEARCH,dummy) && hash_ants(nLoc,ENEMY,SEMISEMI_RAD_SEARCH,dummy))//isinattackrad(ant.aim,nLoc,2))
                    if(!existin(sLoc,nLoc,SEMI_RAD_SEARCH) && existin(sLoc,nLoc,SEMISEMI_RAD_SEARCH))
                        return nLoc;            
                    visited[nLoc.row][nLoc.col]=true;
                    locQueue.push(nLoc);
                }
                i++;
            }
        }
        sLoc = locQueue.front();
        locQueue.pop();
    }
    return ant.aim;
}

bool State::passable(Location loc)
{
    
    if( grid[loc.row][loc.col].isWater)
        return false;
    if( !grid[loc.row][loc.col].passable)
        return false;
    if( grid[loc.row][loc.col].ant ==0)
        return false;

    return true;
    int i;
    for(i=0; i < orders.size();++i)
        if( orders[i].row == loc.row && orders[i].col == loc.col)
           return false;
    
    // std::list<Location>::iterator lloc;
    // for(lloc = myHills.begin(); lloc != myHills.end(); lloc++)
        // if( *lloc == loc)
            // return false;
    
    // std::list<Ant>::iterator a;
            
    // for(a = myAnts.begin();a != myAnts.end(); a++)
    //     if( (*a).loc.row == loc.row && (*a).loc.col == loc.col)
    //         return false;

    return true;
}


int State::direction(Location loc1,Location loc2)
{
    if(loc1.row < loc2.row)
    {
        if(loc2.row-loc1.row >= rows/2)
            return N;
        else
            return S;
    }
    if(loc1.row > loc2.row)
    {
        if(loc1.row-loc2.row >= rows/2)
            return S;
        else
            return N;
    }
    if(loc1.col < loc2.col)
    {
        if(loc2.col-loc1.col >= cols/2)
            return W;
        else
            return E;
    }
    if(loc1.col > loc2.col)
    {
        if(loc1.col-loc2.col >= cols/2)
            return E;
        else
            return W;
    }    
}

bool State::intargets(Location loc)
{
    for(int i=0; i < targets.size();++i)
        if( targets[i].row == loc.row && targets[i].col == loc.col)
           return false;
    return true;
}


/*
    This is the output function for a state. It will add a char map
    representation of the state to the output stream passed to it.

    For example, you might call "cout << state << endl;"
*/
ostream& operator<<(ostream &os, const State &state)
{
    for(int row=0; row<state.rows; row++)
    {
        for(int col=0; col<state.cols; col++)
        {
            if(state.grid[row][col].isWater)
                os << '%';
            else if(state.grid[row][col].isFood)
                os << '*';
            else if(state.grid[row][col].isHill)
                os << (char)('A' + state.grid[row][col].hillPlayer);
            else if(state.grid[row][col].ant >= 0)
                os << (char)('a' + state.grid[row][col].ant);
            else if(state.grid[row][col].isVisible)
                os << '.';
            else
                os << '?';
        }
        os << endl;
    }

    return os;
};

//input function
istream& operator>>(istream &is, State &state)
{
    int row, col, player;
    string inputType, junk;

    //finds out which turn it is
    while(is >> inputType)
    {
        if(inputType == "end")
        {
            state.gameover = 1;
            break;
        }
        else if(inputType == "turn")
        {
            is >> state.turn;
            break;
        }
        else //unknown line
            getline(is, junk);
    }

    if(state.turn == 0)
    {
        //reads game parameters
        while(is >> inputType)
        {
            if(inputType == "loadtime")
                is >> state.loadtime;
            else if(inputType == "turntime")
                is >> state.turntime;
            else if(inputType == "rows")
                is >> state.rows;
            else if(inputType == "cols")
                is >> state.cols;
            else if(inputType == "turns")
                is >> state.turns;
            else if(inputType == "viewradius2")
            {
                is >> state.viewradius;
                state.viewradius = sqrt(state.viewradius);
            }
            else if(inputType == "attackradius2")
            {
                is >> state.attackradius2;
                state.attackradius = sqrt(state.attackradius2);
            }
            else if(inputType == "spawnradius2")
            {
                is >> state.spawnradius;
                state.spawnradius = sqrt(state.spawnradius);
            }
            else if(inputType == "ready") //end of parameter input
            {
                state.timer.start();
                break;
            }
            else    //unknown line
                getline(is, junk);
        }
    }
    else
    {
        //reads information about the current turn
        while(is >> inputType)
        {
            if(inputType == "w") //water square
            {
                is >> row >> col;
                state.grid[row][col].isWater = 1;
            }
            else if(inputType == "f") //food square
            {
                is >> row >> col;
                state.grid[row][col].isFood = 1;
                if(!inlist(state.food,Location(row,col)))              
                    state.food.push_back(Location(row, col));
            }
            else if(inputType == "a") //live ant square
            {
                is >> row >> col >> player;
                state.grid[row][col].ant = player;
                state.grid[row][col].isWater = 0;
                if(player == 0)
                {
                    if(inlist(state.myAnts,Location(row,col)))
                    {
                        (*G_it).alive = true;
                        (*G_it).made_move=false;
                    }
                    else
                        state.myAnts.push_back(Ant(Location(row, col),state.id++));
                }
                else
                    state.enemyAnts.push_back(Location(row, col));
            }
            else if(inputType == "d") //dead ant square
            {
                is >> row >> col >> player;
                state.grid[row][col].deadAnts.push_back(player);
            }
            else if(inputType == "h")
            {
                is >> row >> col >> player;
                state.grid[row][col].isHill = 1;
                state.grid[row][col].hillPlayer = player;
                if(player == 0)
                {
                   /* if(inlist(state.myHills2,Location(row,col))) 
                        state.myHills.push_back(Location(row, col,(*GL_it).target));
                    else*/
                        state.myHills.push_back(Location(row, col));
                }
                else
                {
                    /*if(inlist(state.enemyHills2,Location(row,col))) 
                        state.enemyHills.push_back(Location(row, col,0/*(*GL_it).target/));
                    else*/
                        state.enemyHills.push_back(Location(row, col));
                }

            }
            else if(inputType == "players") //player information
                is >> state.noPlayers;
            else if(inputType == "scores") //score information
            {
                state.scores = vector<double>(state.noPlayers, 0.0);
                for(int p=0; p<state.noPlayers; p++)
                    is >> state.scores[p];
            }
            else if(inputType == "go") //end of turn input
            {
                if(state.gameover)
                    is.setstate(std::ios::failbit);
                else
                    state.timer.start();
                break;
            }
            else //unknown line
                getline(is, junk);
        }
    }

    return is;
};

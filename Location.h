#ifndef LOCATION_H_
#define LOCATION_H_

/*
    struct for representing locations in the grid.
*/
struct Location
{
    int row, col;
    int target,dir,owner;

    Location()
    {
        row = col = 0;
        target=0;
    };

    Location(int r, int c)
    {
        row = r;
        col = c;
        target=0;
    };

    Location(int r, int c,int t)
    {
        row = r;
        col = c;
        target =t;
    };
    Location(int r, int c,int t,int d)
    {
        row = r;
        col = c;
        target =t;
        dir=d;
    };

    bool operator==(const Location& loc) const
    {
        return (row==loc.row && col==loc.col);
    }
};

struct LocDistDir
{
    Location loc;
    int dist;
    int dir;
    
    LocDistDir(Location l,int d,int dr)  
    {
        loc = l;
        dist = d;
        dir = dr;
    }
    LocDistDir()
    {
        loc = Location();
        dist=dir=0;
    }
};

class Ant
{
  public:
  
  Location loc;
  Location aim;  
  bool req_path;
  std::vector<Location> path;
  unsigned short int pos;
  bool alive;
  int job;
  int dir,id;
  int avail_units,req_units,send_units;
  bool attackmode;
  bool req_backup;
  bool made_move;
  bool req_astar;
  bool aimset;

  Ant(Location l,int i)
  {
      loc = l;
      aim = Location();
      path.clear();
      pos=job=0;
      dir=-1;
      alive=req_path=true;
      attackmode=req_backup=false;
      req_astar=made_move=false;
      avail_units=req_units=send_units=0;
      id=i;
      aimset=false;
  }
  Ant(){};
};

class LocPair
{
  public:
    Ant *ant,*ant2;
    Location *dLoc;
    int dist;

    LocPair(Ant *an,Location *dL,int d)
    {
      ant =an;
      dLoc =dL;
      dist =d;
    }
    LocPair(Ant *an,Ant *an2,int d)
    {
      ant =an;
      ant2=an2;
      dist =d;
    }

    bool operator<(const LocPair lp) const
    {
      return (dist<lp.dist);
    }
};

class DiffDirPair
{
    public:
      int dir;
      double diff;

    DiffDirPair(int dr,double df)
    {
      dir = dr;
      diff = df;
    }

    bool operator<(DiffDirPair ddp)
    {
      return (diff>ddp.diff);
    }
};
#endif //LOCATION_H_

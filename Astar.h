#ifndef ASTAR_H_
#define ASTAR_H_

#include "stlastar.h"
#include "Location.h"
#include "State.h"

class AStarNode
{
	
	State *s;

public:
	Location loc;
	AStarNode()
	{
		loc = Location();
	}
	AStarNode(State *st,Location &l)
	{
		loc = l;
		s = st;
	}

	float GoalDistanceEstimate( AStarNode &nodeGoal );
	bool IsGoal( AStarNode &nodeGoal );
	bool GetSuccessors( AStarSearch<AStarNode> *astarsearch, AStarNode *parent_node );
	float GetCost( AStarNode &successor );
	bool IsSameState( AStarNode &rhs );
};

class Astar
{
		AStarSearch<AStarNode> astarsearch;
		State *s;
	public:
		Astar(State &st)
		{
			s = &st;
		}
		int find_path(Location &sloc,Location &dLoc);
		int direction(Location &loc1,Location &loc2);
		double dist(const Location &loc1,const Location &loc2);
		void set_path(Ant &ant);
};

#endif
#include "Astar.h"

float AStarNode::GoalDistanceEstimate(AStarNode &node)
{
	float dx = abs(loc.row - node.loc.row);
	float dy = abs(loc.col - node.loc.col);

	return 0.91*(dx+dy);//.91
}

bool AStarNode::IsGoal(AStarNode &node)
{
	if(loc.row==node.loc.row && loc.col==node.loc.col)
		return true;
	return false;
}

bool AStarNode::IsSameState(AStarNode &node)
{
	if(loc.row==node.loc.row && loc.col==node.loc.col)
		return true;
	return false;
}

bool AStarNode::GetSuccessors( AStarSearch<AStarNode> *astarsearch, AStarNode *parent_node )
{
	Location nLoc;
	AStarNode nNode;
	
	for(int d=0; d<4; d++)
   	{
   		if(parent_node==NULL) return false;
   		nLoc = s->getLocation((*parent_node).loc,d);

   		if(s->passable(nLoc))
   		{
   			nNode = AStarNode(s,nLoc);
   			astarsearch->AddSuccessor(nNode);
   		}
   	}
   	return true;
}

float AStarNode::GetCost(AStarNode &node)
{
	if( s->grid[node.loc.row][node.loc.col].isVisible )
		return 1;
	if( s->grid[node.loc.row][node.loc.col].isVisible == false )
		return 1.5;
}
void Astar::set_path(Ant &ant)
{
	
	AStarNode sNode = AStarNode(s,ant.loc);
	AStarNode dNode = AStarNode(s,ant.aim);

	if(ant.loc == ant.aim)
	{
		ant.dir=-2;
		return;
	}

	int SearchState,dir=-1;

	astarsearch.SetStartAndGoalStates(sNode,dNode);

	do
	{
		SearchState = astarsearch.SearchStep();
	}
	while( SearchState == AStarSearch<AStarNode>::SEARCH_STATE_SEARCHING );

	if( SearchState == AStarSearch<AStarNode>::SEARCH_STATE_SUCCEEDED )
	{
		AStarNode *Node = astarsearch.GetSolutionStart();		

		//ant.path.push_back(Node->loc);
		Node = astarsearch.GetSolutionNext();
		//ant.path.push_back(Node->loc);

		ant.dir = direction(ant.loc,Node->loc);

		/*while(true)
		{
			Node = astarsearch.GetSolutionNext();

			if(!Node) break;

			//ant.path.push_back(Node->loc);
		}*/

		//dir = direction(pNode->loc,nNode->loc);

		astarsearch.FreeSolutionNodes();
	}
	else if( SearchState == AStarSearch<AStarNode>::SEARCH_STATE_FAILED ) 
	{
		ant.dir = -2;
		astarsearch.FreeSolutionNodes();
	}
}

double Astar::dist(const Location &sLoc,const Location &dLoc)
{
	
	Location loc1=sLoc,loc2=dLoc,nLoc;
	int valid=0;

	for(int d=0; d<TDIRECTIONS; d++)
    {
        nLoc = s->getLocation(loc1, d);
        if(s->passable(nLoc)) valid++;
    }

    if(valid)
    {
		AStarNode sNode = AStarNode(s,loc1);
		AStarNode dNode = AStarNode(s,loc2);

		int SearchState,dist=10000,step=0;

		astarsearch.SetStartAndGoalStates(sNode,dNode);

		do
		{
			SearchState = astarsearch.SearchStep();
		}
		while( SearchState == AStarSearch<AStarNode>::SEARCH_STATE_SEARCHING );

		if( SearchState == AStarSearch<AStarNode>::SEARCH_STATE_SUCCEEDED )
		{
			AStarNode *Node = astarsearch.GetSolutionStart();
			// Node = astarsearch.GetSolutionNext();
			while(true)
			{			
				Node = astarsearch.GetSolutionNext();

				if(!Node) break;
				step++;
			}

			//dir = direction(pNode->loc,nNode->loc);

			astarsearch.FreeSolutionNodes();
		}
		else if( SearchState == AStarSearch<AStarNode>::SEARCH_STATE_FAILED ) 
		{
			step = 10000;	
		}
		return step;
	}
	else return 10000;
}

int Astar::find_path(Location &sLoc,Location &dLoc)
{
	
	AStarNode sNode = AStarNode(s,sLoc);
	AStarNode dNode = AStarNode(s,dLoc);
	int valid=0;
	Location nLoc;

	for(int d=0; d<TDIRECTIONS; d++)
    {
        nLoc = s->getLocation(sLoc, d);
        if(s->passable(nLoc)) valid++;
    }
    if(valid)
    {
		int SearchState,dir=-1;

		astarsearch.SetStartAndGoalStates(sNode,dNode);

		do
		{
			SearchState = astarsearch.SearchStep();
		}
		while( SearchState == AStarSearch<AStarNode>::SEARCH_STATE_SEARCHING );

		if( SearchState == AStarSearch<AStarNode>::SEARCH_STATE_SUCCEEDED )
		{
			AStarNode *pNode = astarsearch.GetSolutionStart();
			AStarNode *nNode = astarsearch.GetSolutionNext();

			dir = direction(pNode->loc,nNode->loc);

			astarsearch.FreeSolutionNodes();
		}
		else if( SearchState == AStarSearch<AStarNode>::SEARCH_STATE_FAILED ) 
		{
			dir = -1;	
		}
		return dir;
	}
	else return -1;
}

int Astar::direction(Location &loc1,Location &loc2)
{
    if(loc1.row < loc2.row)
    {
    	if(loc2.row-loc1.row >= s->rows/2)
        	return N;
        else
        	return S;
    }
    if(loc1.row > loc2.row)
    {
    	if(loc1.row-loc2.row >= s->rows/2)
        	return S;
        else
        	return N;
    }
    if(loc1.col < loc2.col)
    {
        if(loc2.col-loc1.col >= s->cols/2)
        	return W;
        else
        	return E;
    }
    if(loc1.col > loc2.col)
    {
	    if(loc1.col-loc2.col >= s->cols/2)
        	return E;
        else
        	return W;
    }
}
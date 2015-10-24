#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <climits>
#include <queue>
#include <cstring>
#include <list>
using namespace std;

int adj[22][22] = {0};
vector< vector< int > > short_dist;
int visited[101][101];
int cost[101][101];

class Orienteering {
public:
		void main();
};

// structure of points to define the matrix
struct point
{
	int x;
	int y;
	point(int a, int b) {x=a;y=b;}
};

//function that find length of shortest path from start node to goal node covering all checkpoints
int find_shortest_dist(int subset, int x ,int n) 
{
 	if(short_dist[subset][x] != -1 )
		return short_dist[subset][x];
	short_dist[subset][x] = INT_MAX;
	int temp = 1 << x;
	for(int i =0;i<n;i++)
	{
		if(i!= x && (subset & ( 1 << i )))
			short_dist[subset][x] = min( short_dist[subset][x], find_shortest_dist(subset - temp,i,n) + adj[i][x]);
	}
	return short_dist[subset][x];
}

//function finds minimum distance between two points (x1,y1) & (x2,y2) using dijkstra's algorithm
int find_distance(int x1,int y1,int x2,int y2,string input[],int W, int H)
{
	point p1(x1,y1);
	queue<point> l;
	//if we have calculated costs for (x1,y1) in previous call, then no need to calculate it again
	if(cost[x1][y1] == 0)
		return cost[x2][y2];
	
	//intialize cost and visited matrix
	memset(visited,0,sizeof(visited));
	
	for(int i=0;i<H;i++)
		for(int j=0;j<W;j++)
			cost[i][j] = INT_MAX;
	cost[x1][y1] = 0;
	
	l.push(p1);
	
	while(!l.empty())
	{
		point curr = l.front();
		l.pop();
		x1 = (curr).x;
		y1 = (curr).y;
		
		if(visited[x1][y1])
			continue;
		
		visited[x1][y1] = 1;
		//modify the cost of all eligible neighbours of curr and put them in a queue
		if(x1+1 <H && input[x1+1][y1] != '#')
		{
			point p3(x1+1,y1);
			if( cost[x1+1][y1] > 1+cost[x1][y1]) 
			{
				l.push(p3);	
			}
			cost[x1+1][y1] = min( cost[x1+1][y1] , 1+cost[x1][y1]);
			
		}
		if(y1+1 <W  && input[x1][y1+1] != '#')
		{
			point p3(x1,y1+1);
			if(cost[x1][y1+1] > 1+cost[x1][y1])
				l.push(p3);
			cost[x1][y1+1] = min( cost[x1][y1+1] , 1+cost[x1][y1]);
			
		}
		if(y1-1 >=0 && input[x1][y1-1] != '#')
		{
			point p3(x1,y1-1);
			if( cost[x1][y1-1] > 1+cost[x1][y1])
				l.push(p3);
			cost[x1][y1-1] = min( cost[x1][y1-1] , 1+cost[x1][y1]);
			
		}
		if(x1-1 >=0 && input[x1-1][y1] != '#')
		{
			point p3(x1-1,y1);
			if(cost[x1-1][y1] > 1+cost[x1][y1])
				l.push(p3);
			cost[x1-1][y1] = min( cost[x1-1][y1] , 1+cost[x1][y1]);
			
		}
	}
	return cost[x2][y2];
}

//finds minimum distance between two points all chackpoints and start node and goal node
// returns -1 if any two checkpoints dont have path between them
int find_all_distance(list<point> &chkPnts, map<pair<pair<int,int>,pair<int,int> >,int> &distances, string input[], int W, int H)
{
	int x1,x2,y1,y2;
	list<point> ::iterator i1;
	for(i1= chkPnts.begin(); i1 != chkPnts.end(); i1++)
	{
		list<point> ::iterator i2 = i1;
		i2++;
		for(; i2 != chkPnts.end(); i2++)
		{
			x1 = (*i1).x;
			y1 = (*i1).y;
			x2 = (*i2).x;
			y2 = (*i2).y;
			int short_dist = find_distance(x1,y1,x2,y2,input,W,H);
			if(short_dist < INT_MAX && short_dist > 0)
				distances[make_pair(make_pair(x1,y1), make_pair(x2,y2))] = short_dist;
			else
				return 0;
		}
	}
	return 1;
}

void Orienteering::main() 
{
	int W,H;
	cin>>W>>H;
	//input width and height
	if(W!=H)
	{
		cout<<"width and height needs to be same";
		return;
	}
	if(W<=0 || W >100 || H<=0 || H >100)
	{
		cout<< -1<<endl;
		return;
	}
	
	//input matrix
	string input[H];
	list<point>checkPoints;
	//Goal node and start node
	point Goal(-1,-1);
	point Start(-1,-1);
	//map that stores minimum distance beween two checkpoints
	map<pair<pair<int,int>,pair<int,int> >, int> distances;
	//assign unique id to each checkpoint
	map<pair<int,int>,int> chkpointID;
	//cost array that stores length of shortest path between a particular node and other nodes
	//it will different for each checkpoint
	for(int i=0;i<W;i++)
	{	
		for(int j=0;j<H;j++)
			cost[i][j] = INT_MAX;
	}
	
	for(int i=0; i<H; i++)//filling the main grid and checkpoints now
	{
		cin>>input[i];
		//reading matrix row wise
		for(int j=0; j<W; j++)
		{
			point P1(i,j);
			
			if(input[i][j]== '@')//checkpoint
				checkPoints.push_back(P1);
			else if(input[i][j] == 'G')
			{
				checkPoints.push_back(P1);
				Goal = P1;
			}
			else if(input[i][j] == 'S')
			{
				checkPoints.push_back(P1);
				Start = P1;
			}
			//if character is not any of 'G','S','@','.','#' returns 0
			else if(input[i][j] != '#' && input[i][j]!= '.')
			{
				cout<<"wrong input"<<endl;
				return;
			}
		}
	}
	int flag = find_all_distance(checkPoints, distances, input, W,H);
	//if find_all_distance returns 0 then there exists two nodes not having path
	if(flag ==0)
	{
		cout <<"no path exsists between some of the checkpoints"<<endl;
		return ;
	}
	
	int l = checkPoints.size();
	int index=0;
	list<point>::iterator i1;
	for(i1= checkPoints.begin(); i1!=checkPoints.end();i1++)
	{
		chkpointID[make_pair((*i1).x,(*i1).y)] = index;
		index++;
	}
	
	//form adjacency matrix of all checkpoints, start node and goal node
	map<pair<pair<int,int>,pair<int,int> >, int>::iterator it1;
	for(it1= distances.begin(); it1!=distances.end();it1++)
	{
		pair<int,int> p1=make_pair((((*it1).first).first).first , (((*it1).first).first).second);
		pair<int,int> p2=make_pair((((*it1).first).second).first , (((*it1).first).second).second);
		adj[chkpointID[p1]][chkpointID[p2]] = (*it1).second;
		adj[chkpointID[p2]][chkpointID[p1]] = (*it1).second;
	}
	
	//now apply travelling salesman algorithm on adjacency matrix
	short_dist = vector< vector< int > >( 1 << l, vector< int >( l, -1 ) );
	for ( int i = 0; i < l; ++i )
		short_dist[ 1 << i ][ i ] = adj[ chkpointID[make_pair(Start.x,Start.y)] ][ i ];
	
	int path_length = find_shortest_dist( ( 1 << l ) - 1, chkpointID[make_pair(Goal.x,Goal.y)],l );
	
	if(path_length < 0 || path_length==INT_MAX)
		cout<< -1 <<endl;
	else
		cout<< path_length<<endl;
}

int main(int argc, char* argv[])
{
	Orienteering o;
	o.main();
	return 0;
}

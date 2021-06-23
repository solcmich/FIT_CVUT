#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>  
#include <time.h>
#include <map>
#include <fstream>
#include <string>
#include <list>
#include <stack>
#include <set>
#include <thread>        
#include <chrono>         
#include <queue>
#include <functional>

constexpr auto START = 'S';
constexpr auto WALL = 'X';
constexpr auto OPENED_NODE = '#';
constexpr auto PATH = 'o';
constexpr auto END = 'E';
constexpr auto EMPTY = ' ';
constexpr auto Step = 50;

using namespace std;

enum Algo { RANDOM_SEARCH = 1, DFS = 2, BFS = 4, GREEDY_SEARCH = 8, DJIKSTRA = 16, A_STAR = 32 };

void Print(const char* s)
{
	cout << s << endl;
}
class PathVisualizer
{
public:
	void Init(istream& in);
	void Visualize(int algo);
	void BFS();
	void DFS();
	void RandomSearch();
	void GreedySearch();
	void Dijkstra();
	void AStar();
private:
	void ParseLine(string line);
	void OpenNode(pair<int, int> pos, vector<vector<char>>* visual);
	void PathNode(pair<int, int> pos, vector<vector<char>>* visual);
	void StartNode(pair<int, int> pos, vector<vector<char>>* visual);
	void EndNode(pair<int, int> pos, vector<vector<char>>* visual);
	void PrintByParentMap(pair<int, int> end, vector<vector<char>>* visual, map<pair<int, int>, pair<int, int>> parentMap, int & pathLenght);
	float Dist(pair<int, int> a, pair <int, int> b);
	vector<pair<int, int>> GetNeighbours(pair<int, int> coors, vector<vector<char>>* visual);
	map<int, set<int>> m_adjMatrix;
	vector<vector<char>> m_visual;
	int m_rows = 0;
	int m_cols = 0;
	pair<int, int> m_start;
	pair<int, int> m_end;
};
void PathVisualizer::OpenNode(pair<int, int> pos, vector<vector<char>>* visual)
{
	(*visual)[pos.first][pos.second] = OPENED_NODE;
}
void PathVisualizer::PathNode(pair<int, int> pos, vector<vector<char>>* visual)
{
	(*visual)[pos.first][pos.second] = PATH;
}
void PathVisualizer::StartNode(pair<int, int> pos, vector<vector<char>>* visual)
{
	(*visual)[pos.first][pos.second] = START;
}
void PathVisualizer::EndNode(pair<int, int> pos, vector<vector<char>>* visual)
{
	(*visual)[pos.first][pos.second] = END;
}
void PathVisualizer::PrintByParentMap(pair<int, int> end, vector<vector<char>>* visual, map<pair<int, int>, pair<int, int>> parentMap, int & pathLenght)
{
	pair<int, int> path = end;
	EndNode(path, visual);
	while (path != m_start)
	{
		path = parentMap.at(path);
		PathNode(path, visual);
		pathLenght++;
	}
	StartNode(path, visual);
}
void Wait(int ms)
{
	this_thread::sleep_for(chrono::milliseconds(ms));
}
void Print(vector<vector<char>>* visual)
{
	for (size_t i = 0; i < (*visual).size(); i++)
	{
		for (size_t x = 0; x < (*visual)[i].size(); x++)
		{
			cout << (*visual)[i][x];
		}
		cout << endl;
	}
	cout << endl << endl;
}
vector<pair<int, int>> PathVisualizer::GetNeighbours(pair<int, int> coors, vector<vector<char>>* visual)
{
	vector <pair<int, int>> ret;
	int x = coors.first;
	int y = coors.second;

	if (y - 1 > 0 && (*visual)[x][y - 1] == EMPTY)//Bottom
		ret.push_back(make_pair(x, y - 1));
	if (x - 1 > 0 && (*visual)[x - 1][y] == EMPTY)//Left
		ret.push_back(make_pair(x - 1, y));
	if (y + 1 < (*visual)[0].size() && (*visual)[x][y + 1] == EMPTY)//Top
		ret.push_back(make_pair(x, y + 1));
	if (x + 1 < (*visual).size() && (*visual)[x + 1][y] == EMPTY)//Right
		ret.push_back(make_pair(x + 1, y));


	return ret;
}

void PathVisualizer::RandomSearch()
{
	Print("RANDOM SEARCH");
	Wait(1500);
	int i = 0;

	vector<vector<char>>* visual = new vector<vector<char>>(m_visual);
	vector<pair<int, int>> res = GetNeighbours(m_start, visual);
	StartNode(m_start, visual);
	vector<pair<int, int>> leafs = vector<pair<int, int>>();
	leafs.push_back(m_start);
	map<pair<int, int>, pair<int, int>> parentMap;
	while (leafs.size())
	{
		int r = rand() % leafs.size();
		pair<int, int> next = leafs[r];
		if (++i % (Step * 10) == 0) //Took forever, speed up
		{
			Print(visual);
			Wait(400);
		}
		leafs.erase(leafs.begin() + r);

		if (next == m_end)
		{
			int l = 0;
			PrintByParentMap(m_end, visual, parentMap, l);
			Print(visual);
			Print("RANDOM SEARCH:");
			cout << "Expanded: " << i - 1 << endl;
			cout << "Path length: " << l << endl;
			break;
		}
		res = GetNeighbours(next, visual);
		
		for (auto a : res)
		{
			leafs.emplace_back(a);

			parentMap.emplace(make_pair(a, next));
		}
		if(next != m_start)
			OpenNode(next, visual);
	}
	Print("***********************");
	Wait(5000);
}

void PathVisualizer::DFS()
{
	Print("DFS");
	Wait(1500);
	int i = 0;
	vector<vector<char>>* visual = new vector<vector<char>>(m_visual);
	stack<pair<int, int>> s;
	s.push(m_start);
	map<pair<int, int>, pair<int, int>> parentMap;
	StartNode(m_start, visual);
	while (!s.empty())
	{
		pair<int, int> curr = s.top();
		if (++i % Step == 0)
		{
			Print(visual);
			Wait(400);
		}
		if(curr != m_start)
			OpenNode(curr, visual);

		if (curr == m_end)
		{
			int l = 0;
			PrintByParentMap(m_end, visual, parentMap, l);
			Print(visual);
			Print("DFS:");
			cout << "Expanded: " << i - 1 << endl;
			cout << "Path length: " << l << endl;
			break;
		}
		s.pop();
		vector<pair<int, int>> adj = GetNeighbours(curr, visual);
		for (auto a : adj)
		{
			s.push(a);
			parentMap.emplace(make_pair(a, curr));
		}
	}

	Print("***********************");
	Wait(5000);
}
void PathVisualizer::BFS()
{
	Print("BFS");
	Wait(1500);
	int i = 0;
	vector<vector<char>>* visual = new vector<vector<char>>(m_visual);
	queue<pair<int, int>> q;
	q.push(m_start);
	map<pair<int, int>, pair<int, int>> parentMap;
	while (!q.empty())
	{
		pair<int, int> curr = q.front();
		if (++i % Step == 0)
		{
			Print(visual);
			Wait(400);
		}
	
		if (curr == m_end)
		{
			int l = 0;
			PrintByParentMap(m_end, visual, parentMap, l);
			Print(visual);
			Print("BFS:");
			cout << "Expanded: " << i - 1 << endl;
			cout << "Path length: " << l << endl;
			break;
		}
		q.pop();
		vector<pair<int, int>> adj = GetNeighbours(curr, visual);
		for (auto a : adj)
		{
			q.push(a);
			parentMap.emplace(make_pair(a, curr));
			OpenNode(a, visual);
		}
	}
	Print("***********************");
	Wait(5000);
}
float PathVisualizer::Dist(pair<int, int> a, pair <int, int> b)
{
	return sqrt((a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second));

}
void PathVisualizer::GreedySearch()
{
	int i = 0;
	Print("GREEDY SEARCH");
	Wait(1500);
	vector<vector<char>>* visual = new vector<vector<char>>(m_visual);
	auto cmp = [&](pair<int, int> a, pair<int, int> b) {return Dist(a, m_end) > Dist(b, m_end); }; //Eucledian distance heuristic
	priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)> q(cmp);
	q.push(m_start);
	map<pair<int, int>, pair<int, int>> parentMap;
	while (!q.empty())
	{
		pair<int, int> curr = q.top();
		if (curr == m_start)
			StartNode(curr, visual);


		if (++i % Step == 0)
		{
			Print(visual);
			Wait(400);
		}
		if (curr == m_end)
		{
			int l = 0;
			PrintByParentMap(m_end, visual, parentMap, l);
			Print(visual);
			Print("GREEDY SEARCH:");
			cout << "Expanded: " << i - 1 << endl;
			cout << "Path length: " << l << endl;
			break;
		}
		q.pop();
		vector<pair<int, int>> adj = GetNeighbours(curr, visual);
		for (auto a : adj)
		{
			q.push(a);
			parentMap.emplace(make_pair(a, curr));
		}
		OpenNode(curr, visual);
	}
	
	Print("***********************");
	Wait(5000);
}
void PathVisualizer::Dijkstra()
{
	Print("DIJKSTRA");
	Wait(1000);
	//Simply BFS on this type of graph...
	int i = 0;
	vector<vector<char>>* visual = new vector<vector<char>>(m_visual);
	map<pair<int, int>, pair<int, int>> parentMap;
	map<pair<int, int>, int> dist;

	auto cmp = [&](pair<int, int> a, pair<int, int> b) {return dist[a] > dist[b]; }; //Distance heuristic
	priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)> q(cmp);
	q.push(m_start);

	for (size_t i = 0; i < (m_visual).size(); i++)
	{
		for (size_t x = 0; x < m_visual[i].size(); x++)
		{
			pair<int, int> p = make_pair(i, x);
			dist.insert(make_pair(p, INT_MAX));
		}
	}

	dist[m_start] = 0;

	while (!q.empty())
	{
		pair<int, int> curr = q.top();
		q.pop();

		
		if (curr == m_start)
			StartNode(curr, visual);

		if (++i % Step == 0)
		{
			Print(visual);
			Wait(400);
		}
		if (curr == m_end)
		{
			int l = 0;
			PrintByParentMap(m_end, visual, parentMap, l);
			Print(visual);
			Print("DIJKSTRA:");
			cout << "Expanded: " << i - 1 << endl;
			cout << "Path length: " << l << endl;
			break;
		}

		vector<pair<int, int>> adj = GetNeighbours(curr, visual);
		for (auto a : adj)
		{
			int i = dist[curr] + 1;
			if (i < dist[a])
			{
				dist[a] = i;
				parentMap.emplace(make_pair(a, curr));
				q.push(a);
			}
			OpenNode(a, visual);
		}
	}
	Wait(5000);
	Print("DIJKSTRA");
	Print("***********************");
}
int DistManhattan(pair<int, int> a, pair <int, int> b)
{
	return abs(a.first - b.first) + abs(a.second - b.second);
}
void PathVisualizer::AStar()
{
	Print("***********************");
	Print("A*");
	Wait(1000);
	int i = 0;
	vector<vector<char>>* visual = new vector<vector<char>>(m_visual);
	map<pair<int, int>, pair<int, int>> parentMap;
	map<pair<int, int>, int> dist;

	auto cmp = [&](pair<int, int> a, pair<int, int> b) {return dist[a] > dist[b]; }; //Distance heuristic
	priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)> q(cmp);
	q.push(m_start);

	for (size_t i = 0; i < (m_visual).size(); i++)
	{
		for (size_t x = 0; x < m_visual[i].size(); x++)
		{
			pair<int, int> p = make_pair(i, x);
			dist.insert(make_pair(p, INT_MAX));
		}
	}

	dist[m_start] = 0;

	while (!q.empty())
	{
		pair<int, int> curr = q.top();
		q.pop();

		if (curr == m_start)
			StartNode(curr, visual);

		if (++i % Step == 0)
		{
			Print(visual);
			Wait(400);
		}
		if (curr == m_end)
		{
		
			int l = 0;
			PrintByParentMap(m_end, visual, parentMap, l);
			Print(visual);
			Print("A*:");
			cout << "Expanded: " << i - 1 << endl;
			cout << "Path length: " << l << endl;
			break;
		}

		vector<pair<int, int>> adj = GetNeighbours(curr, visual);
		for (auto a : adj)
		{
			int i = dist[curr] + DistManhattan(a, curr);  
			if (i < dist[a])
			{
				
				dist[a] = i;
				parentMap.emplace(make_pair(a, curr));
				q.push(a);
			}
			OpenNode(a, visual);
		}
		OpenNode(curr, visual);
	}
	Print("***********************");
	Wait(5000);
}
void PathVisualizer::Visualize(int algo)
{
	bool run = false;
	if ((Algo::RANDOM_SEARCH & algo) == Algo::RANDOM_SEARCH)
			RandomSearch();
	if ((Algo::DFS & algo) == Algo::DFS)
		DFS();
	if ((Algo::BFS & algo) == Algo::BFS)
		BFS();
	if ((Algo::GREEDY_SEARCH & algo) == Algo::GREEDY_SEARCH)
		GreedySearch();
	if ((Algo::DJIKSTRA & algo) == Algo::DJIKSTRA)
		Dijkstra();
	if ((Algo::A_STAR & algo) == Algo::A_STAR)
		AStar();
}

void PathVisualizer::ParseLine(string line)
{
	if (line.find("start") != string::npos)
	{
		string x = line.substr(6, line.length() - line.find(",", 0) - 1);
		string y = line.substr(line.find(",") + 2, line.size());
		m_start = make_pair(stoi(y), stoi(x));
		return;
	}
	if (line.find("end") != string::npos)
	{
		string x = line.substr(4, line.length() - line.find(",", 0) - 1);
		string y = line.substr(line.find(",") + 2, line.size());
		m_end = make_pair(stoi(y), stoi(x));
		return;
	}
	m_cols = line.size();
	vector<char> rowVisual;

	for (size_t i = 0; i < m_cols; i++)
		rowVisual.push_back(line[i]);

	m_visual.emplace_back(rowVisual);
	m_rows++;
}

void PathVisualizer::Init(istream& in)
{
	string line;
	while (getline(in, line))
	{
		ParseLine(line);
		if (line.find("end") != string::npos)
			break;
	}
	m_cols = line.length();
}

int main()
{
	Print("Welcome to path finding visualizer, please enter the maze with start and end points");
	PathVisualizer pv;
	pv.Init(cin);
	int a = Algo::GREEDY_SEARCH;
	a = a | Algo::A_STAR;
	a = a | Algo::DJIKSTRA;
	a = a | Algo::BFS;
	a = a | Algo::DFS;
	a = a | Algo::RANDOM_SEARCH;
	pv.Visualize(a);
}

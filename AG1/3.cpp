#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>  
#include <time.h>
#include <map>
#include <fstream>
#include <string>

class Restaurant
{
public:
	Restaurant(unsigned int order) : order(order)
	{
	}
	unsigned int order;
};
template <typename T>
class Node
{
public:
	Node(T* data, int index) : data(data), index(index)
	{
		neighbours = std::vector<Node*>();
	}
	std::vector<Node*> neighbours;
	T* data;
	int index;
};

struct Beer
{
public:
	Beer(unsigned int price, int initialIndex) : initialIndex(initialIndex)
	{
		m_price = price;
	}
	bool operator < (const Beer& b) const {
		return m_price < b.m_price;
	}
	unsigned int m_price;
	int initialIndex;
};

class RiverNetwork
{
public:
	RiverNetwork(int beers, int rest)
	{
		m_beers = std::vector<Beer>();
		m_max_beers = std::min((int)std::log2(rest) + 1, beers);
		m_restaurants = std::vector< Node<Restaurant>*>();
		curr_beer = 1;
		m_dpTable = std::map<std::tuple<int, int, int>, std::tuple<int, std::map<int, int>*>>();
	}
	void AddBeer(unsigned int price)
	{
		m_beers.emplace_back(Beer(price, curr_beer++));
	}
	void AddRestaurant(unsigned int order)
	{
		m_restaurants.emplace_back(new Node<Restaurant>(new Restaurant(order), m_restaurants.size()));
	}

	void ConnectRestaurants(int first, int second)
	{
		m_restaurants.at(first - 1)->neighbours.emplace_back(m_restaurants.at(second - 1));
		m_restaurants.at(second - 1)->neighbours.emplace_back(m_restaurants.at(first - 1));
	}

	void Solve()
	{
		std::map<int, int>*  sq = new std::map<int, int>();
		m_beers_init = std::vector<Beer>(m_beers);

		std::sort(m_beers.begin(), m_beers.end());
		int price = Prices(*m_restaurants.begin(), 0, -1, &sq);
		std::cout << price << std::endl;
		unsigned int cnt = 0;
	
		for (auto i : *sq)
		{
			if (++cnt < (*sq).size())
				std::cout << i.second << " ";
			else
				std::cout << i.second;

		}
	}

	int Prices(Node<Restaurant>* n, int prevR, int prevB, std::map<int, int>** beersUsed)
	{
		//dp
		auto it = m_dpTable.find(std::tuple<int, int, int>(prevR, n->index, prevB));
		if (it != m_dpTable.end())
		{
			*beersUsed = std::get<1>(it->second);
			return std::get<0>(it->second);
		}
		
		int min = 2147483647;

		if (n->neighbours.size() == 1 && prevR != n->index)
		{
			int minIndex = -1;
			for (int i = 0; i < 2; i++)
				if (prevB != i) {
					int ins = n->data->order * m_beers.at(i).m_price;
					if (ins < min)
					{
						min = ins;
						minIndex = m_beers.at(i).initialIndex;
					}
				}
			(*beersUsed)->insert(std::pair<int, int>(n->index, minIndex));
			return min;
		}
		int iter = std::min(m_max_beers, (int)n->neighbours.size() + 1);
		int minBeer = -1;
		std::map<int, int>* minMap = nullptr, * currMap = nullptr;
		for (int b = 0; b < iter; b++)
		{
			int prev = 0;
			currMap = new std::map<int, int>();
			for (unsigned int i = 0; i < n->neighbours.size(); i++)
			{
				if (n->neighbours.at(i)->index != prevR && b != prevB)
				{
					std::map<int, int> * tmp = new std::map<int, int>();
					prev += Prices(n->neighbours.at(i), n->index, b, &tmp);
					currMap->insert(tmp->begin(), tmp->end());
				}

			}
			if (prev > 0)
			{
				prev += n->data->order * m_beers.at(b).m_price;
				if (prev < min)
				{
					min = prev;

					if (minMap)
						delete minMap;

					minMap = currMap;

					minBeer = m_beers.at(b).initialIndex;
				}
				else
				{
					delete currMap;
				}
			}
		}
		*beersUsed = minMap;
		(*beersUsed)->insert(std::pair<int, int>(n->index, minBeer));
		m_dpTable.insert(std::pair<std::tuple<int, int, int>, std::tuple<int, std::map<int, int>*>>(std::tuple<int, int, int>(prevR, n->index, prevB), std::tuple<int, std::map<int, int>*>(min, *beersUsed)));
		return min;
	}
private:
	std::vector<Beer> m_beers;
	std::vector<Beer> m_beers_init;
	std::vector<Node<Restaurant>*> m_restaurants;
	int m_max_beers;
	std::map<std::tuple<int, int, int>, std::tuple<int, std::map<int, int>*>> m_dpTable;
	int curr_beer;
};

RiverNetwork LoadInput()
{

	std::string line;
	int cnt = 0, beers = 0;

	if (std::getline(std::cin, line))
	{
		cnt = std::stoi(line.substr(0, line.find(' ')));
		beers = std::stoi(line.substr(line.find(' ') + 1, line.length()));
	}
	RiverNetwork ret = RiverNetwork(beers, cnt);

	if (std::getline(std::cin, line))
	{
		for (int i = 0; i < beers; i++)
		{
			ret.AddBeer(std::stoi(line.substr(0, line.find(' '))));
			line = line.substr(line.find(' ') + 1, line.length());
		}
	}


	if (std::getline(std::cin, line))
	{
		for (int i = 0; i < cnt; i++)
		{
			ret.AddRestaurant(std::stoi(line.substr(0, line.find(' '))));
			line = line.substr(line.find(' ') + 1, line.length());
		}
	}

	for (int i = 0; i < cnt - 1; i++)
	{
		std::getline(std::cin, line);
		int first, second;
		first = std::stoi(line.substr(0, line.find(' ')));
		second = std::stoi(line.substr(line.find(' ') + 1, line.length()));
		ret.ConnectRestaurants(first, second);
	}

	return ret;

}

int main()
{
	RiverNetwork rn = LoadInput();
	rn.Solve();
	return 0;
}

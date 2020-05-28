
#include <iostream>
#include <vector>
#include <cassert>
#include <iomanip>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <memory>
#include <functional>
#include <queue>
#include <fstream>

using namespace std;
class Place
{

public:
	Place(bool first, int index, int adjancent) : GetFirst(first), GetIndex(index), GetAdjanctent(adjancent)
	{

	}
	bool GetFirst;
	int GetIndex;
	int GetAdjanctent;
};

int Init();
int FindShortest(string firstQueue, string secondQueue, int size, int special);


int main()
{
	cout << Init() << endl;
}


int Init()
{
	string tmp;
	string tmp1;
	int special;
	int size;
	getline(cin, tmp);
	size = stoi(tmp.substr(0, tmp.find(" ")));
	special = stoi(tmp.substr(tmp.find(" ") + 1));
	getline(cin, tmp);
	getline(cin, tmp1);
	int shortest = FindShortest(tmp, tmp1, size, special);
	return shortest;
}


int FindShortest(string firstQueue, string secondQueue, int size, int special)
{
	queue<Place*> places;
	places.push(new Place(true, 0, 0));
	vector<int> firstVisited;
	vector<int> secondVisited;

	firstVisited.reserve(size);
	secondVisited.reserve(size);

	firstVisited.emplace_back(0);

	while (!places.empty())
	{
		Place* currentPlace = places.front();
		places.pop();

		int total = currentPlace->GetIndex + special;

		if (total >= size)
			return currentPlace->GetAdjanctent + 1;

		char posBack = 'x';
		if (currentPlace->GetIndex > 0)
		{
			posBack = (currentPlace->GetFirst ? firstQueue[currentPlace->GetIndex - 1] : secondQueue[currentPlace->GetIndex - 1]);
			if (posBack == '.')
			{
				int adj = currentPlace->GetAdjanctent + 1;
				int search = (currentPlace->GetIndex - 1);
				if (currentPlace->GetIndex >= adj)
				{
					bool found = false;
					vector<int>::iterator a;

					if (currentPlace->GetFirst)
						found = binary_search(firstVisited.begin(), firstVisited.end(), search);				
					else
						found = binary_search(secondVisited.begin(), secondVisited.end(), search);					

					if (!found)
					{
						places.push(new Place(currentPlace->GetFirst, search, adj));
						if (currentPlace->GetFirst)
						{
							a = lower_bound(firstVisited.begin(), firstVisited.end(), search);
							firstVisited.insert(a, search);
						}
						else
						{
							a = lower_bound(secondVisited.begin(), secondVisited.end(), search);
							secondVisited.insert(a, search);
						}
							
					}


				}
			}
		}


		char posFront = 'x';
		if (currentPlace->GetIndex < size - 1)
		{
			int search = currentPlace->GetIndex + 1;
			posFront = (currentPlace->GetFirst ? firstQueue[search] : secondQueue[search]);
			if (posFront == '.')
			{
				int adj = currentPlace->GetAdjanctent + 1;

				bool found = false;
				vector<int>::iterator a;

				if (currentPlace->GetFirst)
					found = binary_search(firstVisited.begin(), firstVisited.end(), search);
				else
					found = binary_search(secondVisited.begin(), secondVisited.end(), search);

				if (!found)
				{
					places.push(new Place(currentPlace->GetFirst, search, adj));
					if (currentPlace->GetFirst)
					{
						a = lower_bound(firstVisited.begin(), firstVisited.end(), search);
						firstVisited.insert(a, search);
					}
					else
					{
						a = lower_bound(secondVisited.begin(), secondVisited.end(), search);
						secondVisited.insert(a, search);
					}

				}

			}
		}

		char posSpecial = 'x';

		posSpecial = (!currentPlace->GetFirst ? firstQueue[currentPlace->GetIndex + special] : secondQueue[currentPlace->GetIndex + special]);
		if (posSpecial == '.')
		{
			int search = currentPlace->GetIndex + special;
			int adj = currentPlace->GetAdjanctent + 1;

			bool found = false;
			vector<int>::iterator a;

			if (!currentPlace->GetFirst)
				found = binary_search(firstVisited.begin(), firstVisited.end(), search);
			else
				found = binary_search(secondVisited.begin(), secondVisited.end(), search);

			if (!found)
			{
				places.push(new Place(!currentPlace->GetFirst, search, adj));
				if (!currentPlace->GetFirst)
				{
					a = lower_bound(firstVisited.begin(), firstVisited.end(), search);
					firstVisited.insert(a, search);
				}
				else
				{
					a = lower_bound(secondVisited.begin(), secondVisited.end(), search);
					secondVisited.insert(a, search);
				}

			}
		}
	}



	return -1;
}
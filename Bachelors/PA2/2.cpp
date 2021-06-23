#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
#include <string>
using namespace std;
#endif /* __PROGTEST__ */
const string DEFAULT_OWNER = "Default56985";


struct Land {
	string owner;
	string city;
	string addr;
	string region;
	unsigned int id;
};

bool compareChar(char & c1, char & c2)
{
	if (c1 == c2)
	{
		return true;
	}
	else if (tolower(c1) == tolower(c2))
	{
		return true;
	}
	return false;
}

bool caseInsensitiveStringCompare(string & str1, string &str2)
{
	return ((str1.size() == str2.size()) &&
		equal(str1.begin(), str1.end(), str2.begin(), &compareChar));
}

//Binary searches for land in given vector, if found, returns givne position, otherwise returns -1
int GetLand(const string & city, const string & addr, int cnt, vector <Land*> toSearch)
{
	// if(toSearch == NULL) return -1; 
	int l = 0;
	int r = cnt - 1;
	int middle = 0;
	while (l <= r)
	{
		middle = (l + r) / 2;
		if (toSearch.at(middle)->city == city && toSearch.at(middle)->addr == addr)
		{
			return middle;
		}

		if (toSearch.at(middle)->city < city)
		{
			l = middle + 1;
		}
		else if (toSearch.at(middle)->city > city)
		{
			r = middle - 1;
		}
		else
		{
			if (toSearch.at(middle)->addr < addr)
			{
				l = middle + 1;
			}
			else
			{
				r = middle - 1;
			}
		}
	}
	return -1;
}

//Binary searches for land in given vector, if found, returns givne position, otherwise returns -1
int GetLand(const string & region, const unsigned & id, int cnt, vector<Land*> toSearch)
{
	int l = 0;
	int r = cnt - 1;
	int middle = 0;
	while (l <= r)
	{
		middle = (l + r) / 2;
		if (toSearch.at(middle)->region == region && toSearch.at(middle)->id == id)
			return middle;

		if (toSearch.at(middle)->id < id)
			l = middle + 1;
		else if (toSearch.at(middle)->id > id)
			r = middle - 1;
		else
		{
			if (toSearch.at(middle)->region < region)
				l = middle + 1;
			else
				r = middle - 1;
		}
	}
	return -1;
}

//Helper function for sort
bool CompareCity(Land *  l, Land * r)
{
	string city = l->city;
	string city2 = r->city;
	if (city > city2)
		return false;
	else if (city < city2)
		return true;
	else
	{
		string addr1 = l->addr;
		string addr2 = r->addr;
		if (addr1 > addr2)
			return false;
		else if (addr1 < addr2)
			return true;
		else
			//Not supposed to end up here.
			return false;
	}
}

//Helper function for sort
bool CompareId(Land * l, Land * r)
{
	int id = l->id;
	int id2 = r->id;
	if (id > id2)
		return false;
	else if (id < id2)
		return true;
	else
	{
		string reg1 = l->region;
		string reg2 = r->region;

		if (reg1 > reg2)
			return false;
		else if (reg1 < reg2)
			return true;
		else
			//Not supposed to end up here.
			return false;
	}
}


//Helper class for working with owners
class COwner
{
public:
	COwner(string name) { _name = name; };
	int GetCount() { return _owned.size(); };
	string GetName() { return _name; };
	vector <Land *> * GetOwned() { return &_owned; };

	void AssignNew(Land * l);
	bool Delete(const string & city, const string & addr);
	bool Delete(const string & region, const unsigned & id);
	int GetLandForOwnerById(const string & region, const unsigned & id);
	int GetLandForOwnerByName(const string & city, const string & addr);
private:
	vector <Land *> _owned;
	string _name;
	int _count;
};

void COwner::AssignNew(Land * l)
{
	_count++;
	_owned.push_back(l);
}

bool COwner::Delete(const string & city, const string & addr)
{
	int pos = 0;
	for (std::vector<Land *>::iterator it = _owned.begin(); it != _owned.end(); ++it, pos++) {
		if (city == (*it)->city && addr == (*it)->addr)
		{
			_owned.erase(it);
			_count--;
			return true;
		}
	}
	return false;
}

bool COwner::Delete(const string & region, const unsigned & id)
{
	int pos = 0;
	for (std::vector<Land *>::iterator it = _owned.begin(); it != _owned.end(); ++it, pos++) {
		if (region == (*it)->region && id == (*it)->id)
		{
			_owned.erase(it);
			_count--;
			return true;
		}
	}
	return false;
}

int COwner::GetLandForOwnerById(const string & region, const unsigned & id)
{
	int pos = 0;
	for (std::vector<Land *>::iterator it = _owned.begin(); it != _owned.end(); ++it, pos++) {
		if (region == (*it)->region && id == (*it)->id)
		{
			return pos;
		}
	}
	return -1;
}
int COwner::GetLandForOwnerByName(const string & city, const string & addr)
{
	int pos = 0;
	for (std::vector<Land *>::iterator it = _owned.begin(); it != _owned.end(); ++it, pos++) {
		if (addr == (*it)->addr && city == (*it)->city)
		{
			return pos;
		}
	}
	return -1;
}


class CIterator
{
public:
	void Init(const vector<Land *> * self)
	{
		if (self == NULL)
		{
			_endPos = 0;
			return;
		}
		_endPos = self->size();
		_selfLand = self;
	};
	bool AtEnd(void) const { return _endPos <= _currentPos; };
	void Next(void) {

		if (_endPos > _currentPos)
			_currentPos++;
	};
	string City(void) const { return (*_selfLand)[_currentPos]->city; };
	string Addr(void) const { return (*_selfLand)[_currentPos]->addr; };
	string Region(void) const { return (*_selfLand)[_currentPos]->region; };
	unsigned ID(void) const { return (*_selfLand)[_currentPos]->id; };
	string Owner(void) const { return (*_selfLand)[_currentPos]->owner; };
private:
	int _currentPos = 0;
	int _endPos = 0;
	const vector<Land *> * _selfLand;
};



//Helper function for sort.
bool CompareOwner(COwner *  l, COwner * r)
{
	string city = l->GetName();
	string city2 = r->GetName();
	if (city < city2)
	{
		return false;
	}
	else if (city > city2)
	{
		return true;
	}
	else
	{
		return false;
		//Not supposed to end up here
	}
}



class CLandRegister
{
public:
	CLandRegister();
	~CLandRegister();

	bool Add(const string & city, const string & addr, const string & region, unsigned int id);

	bool Del(const string & city, const string & addr);
	bool Del(const string & region, unsigned int id);

	void RemoveFromOwner(const string & city, const string & addr, const string & owner) const;
	void RemoveFromOwner(const string & region, unsigned int id, const string & owner) const;

	bool GetOwner(const string & city, const string & addr, string & owner) const;
	bool GetOwner(const string    & region, unsigned int id, string & owner) const;

	bool NewOwner(const string & city, const string    & addr, const string & owner);
	bool NewOwner(const string    & region, unsigned int id, const string & owner);

	unsigned Count(const string    & owner) const;

	CIterator  ListByAddr(void) const;

	CIterator   ListByOwner(const string    & owner) const;

	int GetOwnerPos(const string & id) const;

	void SortByLocation();
	void SortById();
private:
	vector<Land*> _landByName;
	vector<Land*> _landById;
	vector <COwner *> _owners;
	int _ownersCount = 0;
	int _count = 0;
};

CLandRegister::CLandRegister()
{
	_count = 0;
	_ownersCount = 0;
}

CLandRegister::~CLandRegister()
{
	for (std::vector<Land *>::iterator it = _landById.begin(); it != _landById.end(); ++it) {
		delete (*it);
	}
	for (std::vector<COwner *>::iterator it = _owners.begin(); it != _owners.end(); ++it) {
		delete (*it);
	}

	_landById.clear();
	_landByName.clear();
	_owners.clear();
}

bool CLandRegister::Add(const string & city, const string & addr, const string & region, unsigned int id)
{
	if (GetLand(city, addr, _count, _landByName) != -1 || GetLand(region, id, _count, _landById) != -1)
	{
		return false;
	}

	Land * l = new Land();
	(*l).addr = addr;
	(*l).city = city;
	(*l).region = region;
	(*l).id = id;
	(*l).owner = DEFAULT_OWNER;

	_count++;

	auto it = lower_bound(_landByName.begin(), _landByName.end(), l, CompareCity);
	_landByName.insert(it, l);

	it = lower_bound(_landById.begin(), _landById.end(), l, CompareId);
	_landById.insert(it, l);


	NewOwner(city, addr, "");

	return true;

}

void eraseFromVec(vector<Land *> &vec, Land * value)
{
	vec.erase(remove(begin(vec), end(vec), value), end(vec));
}

bool CLandRegister::Del(const string & city, const string & addr)
{
	int landPosName = GetLand(city, addr, _count, _landByName);

	if (landPosName == -1)
	{
		return false;
	}

	Land *  l = _landByName.at(landPosName);

	//int landPosId = GetLand(l->region, l->id, _count, _landById);

	RemoveFromOwner(l->region, l->id, l->owner);
	_count--;
	eraseFromVec(_landById, l);
	eraseFromVec(_landByName, l);
	delete l;

	return true;
}

bool CLandRegister::Del(const string & region, unsigned int id)
{
	int landPosId = GetLand(region, id, _count, _landById);

	if (landPosId == -1)
	{
		return false;
	}

	Land *  l = _landById[landPosId];

	RemoveFromOwner(l->region, l->id, l->owner);
	_count--;
	eraseFromVec(_landById, l);
	eraseFromVec(_landByName, l);

	delete l;

	SortById();
	SortByLocation();
	return true;
}

void CLandRegister::RemoveFromOwner(const string & city, const string & addr, const string & owner) const
{
	int pos = GetOwnerPos(owner);
	if (pos > -1)
	{
		COwner * o = _owners.at(pos);
		o->Delete(city, addr);
	}
}


void CLandRegister::RemoveFromOwner(const string & region, unsigned int id, const string & owner) const
{
	int pos = GetOwnerPos(owner);
	if (pos > -1)
	{
		COwner * o = _owners.at(pos);
		o->Delete(region, id);
	}

}


bool CLandRegister::GetOwner(const string & city, const string & addr, string & owner) const
{
	int pos = GetLand(city, addr, _count, _landByName);
	if (pos < 0)
	{
		return false;
	}

	Land * l = _landByName.at(pos);
	owner = l->owner;

	return true;
}

bool CLandRegister::GetOwner(const string & region, unsigned int id, string & owner) const
{
	int pos = GetLand(region, id, _count, _landById);
	if (pos < 0)
	{
		return false;
	}

	Land * l = _landById.at(pos);
	owner = l->owner;

	return true;
}

bool CLandRegister::NewOwner(const string & city, const string & addr, const string & owner)
{
	int pos = GetLand(city, addr, _count, _landByName);
	if (pos < 0)
	{
		return false;
	}
	Land * l = _landByName.at(pos);
	int ownerPos = GetOwnerPos(owner);
	COwner * o;

	if (ownerPos > -1)
	{
		o = _owners.at(ownerPos);
	}
	else
	{
		o = new COwner(owner);
		_ownersCount++;
		auto it = lower_bound(_owners.begin(), _owners.end(), o, CompareOwner);
		_owners.insert(it, o);

	}
	string s = owner;
	if (caseInsensitiveStringCompare(l->owner, s))
	{
		return false;
	}
	else
	{
		if (l->owner != DEFAULT_OWNER)
		{
			RemoveFromOwner(city, addr, l->owner);
		}
	}

	o->AssignNew(l);
	l->owner = owner;
	return true;
}

bool CLandRegister::NewOwner(const string & region, unsigned int id, const string & owner)
{
	int pos = GetLand(region, id, _count, _landById);
	if (pos < 0)
	{
		return false;
	}
	Land * l = _landById.at(pos);

	int ownerPos = GetOwnerPos(owner);
	COwner * o;
	if (ownerPos > -1)
	{
		o = _owners.at(ownerPos);
	}
	else
	{
		o = new COwner(owner);
		_ownersCount++;
		auto it = lower_bound(_owners.begin(), _owners.end(), o, CompareOwner);
		_owners.insert(it, o);
	}


	string s = owner;
	if (caseInsensitiveStringCompare(l->owner, s))
	{
		return false;
	}
	else
	{
		RemoveFromOwner(region, id, l->owner);
	}
	o->AssignNew(l);
	l->owner = owner;
	return true;
}

unsigned CLandRegister::Count(const string & owner) const
{
	int pos = GetOwnerPos(owner);
	if (pos < 0)
	{
		return 0;
	}
	COwner * o = _owners.at(pos);
	return o->GetCount();
}

CIterator CLandRegister::ListByAddr(void) const
{
	CIterator c;
	c.Init(&_landByName);
	return c;
}

CIterator CLandRegister::ListByOwner(const string & owner) const
{
	int pos;
	CIterator c;


	pos = GetOwnerPos(owner);


	if (pos < 0)
	{
		c.Init(NULL);
	}
	else
	{
		COwner * o = _owners.at(pos);
		c.Init(o->GetOwned());
	}

	return c;
}


int CLandRegister::GetOwnerPos(const string & name) const
{
	int l = 0;
	int r = _ownersCount - 1;
	int middle;

	while (l <= r)
	{
		middle = (l + r) / 2;
		string ownerOrig = _owners.at(middle)->GetName();
		string ownerSearch = name;
		if (caseInsensitiveStringCompare(ownerOrig, ownerSearch))
		{
			return middle;
		}

		if (_owners.at(middle)->GetName() > name)
		{
			l = middle + 1;
		}
		else if (_owners.at(middle)->GetName() < name)
		{
			r = middle - 1;
		}
	}
	return -1;
}


void CLandRegister::SortByLocation()
{
	sort(_landByName.begin(), _landByName.end(), CompareCity);
}

void CLandRegister::SortById()
{
	sort(_landById.begin(), _landById.end(), CompareId);
}



#ifndef __PROGTEST__
void test0()
{
	CLandRegister x;
	string owner;

	assert(x.Add("Prague", "Thakurova", "Dejvice", 12345));
	assert(x.Add("Prague", "Evropska", "Vokovice", 12345));
	assert(x.Add("Prague", "Technicka", "Dejvice", 9873));
	assert(x.Add("Plzen", "Evropska", "Plzen mesto", 78901));
	assert(x.Add("Liberec", "Evropska", "Librec", 4552));
	CIterator i0 = x.ListByAddr();
	assert(!i0.AtEnd()
		&& i0.City() == "Liberec"
		&& i0.Addr() == "Evropska"
		&& i0.Region() == "Librec"
		&& i0.ID() == 4552
		&& i0.Owner() == "");

	i0.Next();
	assert(!i0.AtEnd()
		&& i0.City() == "Plzen"
		&& i0.Addr() == "Evropska"
		&& i0.Region() == "Plzen mesto"
		&& i0.ID() == 78901
		&& i0.Owner() == "");
	i0.Next();
	assert(!i0.AtEnd()
		&& i0.City() == "Prague"
		&& i0.Addr() == "Evropska"
		&& i0.Region() == "Vokovice"
		&& i0.ID() == 12345
		&& i0.Owner() == "");
	i0.Next();
	assert(!i0.AtEnd()
		&& i0.City() == "Prague"
		&& i0.Addr() == "Technicka"
		&& i0.Region() == "Dejvice"
		&& i0.ID() == 9873
		&& i0.Owner() == "");
	i0.Next();
	assert(!i0.AtEnd()
		&& i0.City() == "Prague"
		&& i0.Addr() == "Thakurova"
		&& i0.Region() == "Dejvice"
		&& i0.ID() == 12345
		&& i0.Owner() == "");
	i0.Next();
	assert(i0.AtEnd());

	assert(x.NewOwner("Librec", 4552, "Franta"));
	assert(x.NewOwner("Liberec", "Evropska", ""));
	//assert(x.NewOwner("Liberec", "Evropska", "   "));

	assert(x.Count("") == 5);
	CIterator i1 = x.ListByOwner("");
	assert(!i1.AtEnd()
		&& i1.City() == "Prague"
		&& i1.Addr() == "Thakurova"
		&& i1.Region() == "Dejvice"
		&& i1.ID() == 12345
		&& i1.Owner() == "");
	i1.Next();
	assert(!i1.AtEnd()
		&& i1.City() == "Prague"
		&& i1.Addr() == "Evropska"
		&& i1.Region() == "Vokovice"
		&& i1.ID() == 12345
		&& i1.Owner() == "");
	i1.Next();
	assert(!i1.AtEnd()
		&& i1.City() == "Prague"
		&& i1.Addr() == "Technicka"
		&& i1.Region() == "Dejvice"
		&& i1.ID() == 9873
		&& i1.Owner() == "");
	i1.Next();
	assert(!i1.AtEnd()
		&& i1.City() == "Plzen"
		&& i1.Addr() == "Evropska"
		&& i1.Region() == "Plzen mesto"
		&& i1.ID() == 78901
		&& i1.Owner() == "");
	i1.Next();
	assert(!i1.AtEnd()
		&& i1.City() == "Liberec"
		&& i1.Addr() == "Evropska"
		&& i1.Region() == "Librec"
		&& i1.ID() == 4552
		&& i1.Owner() == "");
	i1.Next();
	assert(i1.AtEnd());

	assert(x.Count("CVUT") == 0);
	CIterator i2 = x.ListByOwner("CVUT");
	assert(i2.AtEnd());

	assert(x.NewOwner("Prague", "Thakurova", "CVUT"));
	assert(x.NewOwner("Dejvice", 9873, "CVUT"));
	assert(x.NewOwner("Plzen", "Evropska", "Anton Hrabis"));
	assert(x.NewOwner("Librec", 4552, "Cvut"));
	assert(x.GetOwner("Prague", "Thakurova", owner) && owner == "CVUT");
	assert(x.GetOwner("Dejvice", 12345, owner) && owner == "CVUT");
	assert(x.GetOwner("Prague", "Evropska", owner) && owner == "");
	assert(x.GetOwner("Vokovice", 12345, owner) && owner == "");
	assert(x.GetOwner("Prague", "Technicka", owner) && owner == "CVUT");
	assert(x.GetOwner("Dejvice", 9873, owner) && owner == "CVUT");
	assert(x.GetOwner("Plzen", "Evropska", owner) && owner == "Anton Hrabis");
	assert(x.GetOwner("Plzen mesto", 78901, owner) && owner == "Anton Hrabis");
	assert(x.GetOwner("Liberec", "Evropska", owner) && owner == "Cvut");
	assert(x.GetOwner("Librec", 4552, owner) && owner == "Cvut");

	CIterator i3 = x.ListByAddr();
	assert(!i3.AtEnd()
		&& i3.City() == "Liberec"
		&& i3.Addr() == "Evropska"
		&& i3.Region() == "Librec"
		&& i3.ID() == 4552
		&& i3.Owner() == "Cvut");
	i3.Next();
	assert(!i3.AtEnd()
		&& i3.City() == "Plzen"
		&& i3.Addr() == "Evropska"
		&& i3.Region() == "Plzen mesto"
		&& i3.ID() == 78901
		&& i3.Owner() == "Anton Hrabis");
	i3.Next();
	assert(!i3.AtEnd()
		&& i3.City() == "Prague"
		&& i3.Addr() == "Evropska"
		&& i3.Region() == "Vokovice"
		&& i3.ID() == 12345
		&& i3.Owner() == "");
	i3.Next();
	assert(!i3.AtEnd()
		&& i3.City() == "Prague"
		&& i3.Addr() == "Technicka"
		&& i3.Region() == "Dejvice"
		&& i3.ID() == 9873
		&& i3.Owner() == "CVUT");
	i3.Next();
	assert(!i3.AtEnd()
		&& i3.City() == "Prague"
		&& i3.Addr() == "Thakurova"
		&& i3.Region() == "Dejvice"
		&& i3.ID() == 12345
		&& i3.Owner() == "CVUT");
	i3.Next();
	assert(i3.AtEnd());

	assert(x.Count("cvut") == 3);
	CIterator i4 = x.ListByOwner("cVuT");
	assert(!i4.AtEnd()
		&& i4.City() == "Prague"
		&& i4.Addr() == "Thakurova"
		&& i4.Region() == "Dejvice"
		&& i4.ID() == 12345
		&& i4.Owner() == "CVUT");
	i4.Next();
	assert(!i4.AtEnd()
		&& i4.City() == "Prague"
		&& i4.Addr() == "Technicka"
		&& i4.Region() == "Dejvice"
		&& i4.ID() == 9873
		&& i4.Owner() == "CVUT");
	i4.Next();
	assert(!i4.AtEnd()
		&& i4.City() == "Liberec"
		&& i4.Addr() == "Evropska"
		&& i4.Region() == "Librec"
		&& i4.ID() == 4552
		&& i4.Owner() == "Cvut");
	i4.Next();
	assert(i4.AtEnd());

	assert(x.NewOwner("Plzen mesto", 78901, "CVut"));
	assert(x.Count("CVUT") == 4);
	CIterator i5 = x.ListByOwner("CVUT");
	assert(!i5.AtEnd()
		&& i5.City() == "Prague"
		&& i5.Addr() == "Thakurova"
		&& i5.Region() == "Dejvice"
		&& i5.ID() == 12345
		&& i5.Owner() == "CVUT");
	i5.Next();
	assert(!i5.AtEnd()
		&& i5.City() == "Prague"
		&& i5.Addr() == "Technicka"
		&& i5.Region() == "Dejvice"
		&& i5.ID() == 9873
		&& i5.Owner() == "CVUT");
	i5.Next();
	assert(!i5.AtEnd()
		&& i5.City() == "Liberec"
		&& i5.Addr() == "Evropska"
		&& i5.Region() == "Librec"
		&& i5.ID() == 4552
		&& i5.Owner() == "Cvut");
	i5.Next();
	assert(!i5.AtEnd()
		&& i5.City() == "Plzen"
		&& i5.Addr() == "Evropska"
		&& i5.Region() == "Plzen mesto"
		&& i5.ID() == 78901
		&& i5.Owner() == "CVut");
	i5.Next();
	assert(i5.AtEnd());

	assert(x.Del("Liberec", "Evropska"));
	assert(x.Del("Plzen mesto", 78901));
	assert(!x.Del("Plzen mesto", 78901));
	assert(x.Count("cvut") == 2);
	CIterator i6 = x.ListByOwner("cVuT");
	assert(!i6.AtEnd()
		&& i6.City() == "Prague"
		&& i6.Addr() == "Thakurova"
		&& i6.Region() == "Dejvice"
		&& i6.ID() == 12345
		&& i6.Owner() == "CVUT");
	i6.Next();
	assert(!i6.AtEnd()
		&& i6.City() == "Prague"
		&& i6.Addr() == "Technicka"
		&& i6.Region() == "Dejvice"
		&& i6.ID() == 9873
		&& i6.Owner() == "CVUT");
	i6.Next();
	assert(i6.AtEnd());

	assert(x.Add("Liberec", "Evropska", "Librec", 4552));
}

void test1()
{
	CLandRegister x;
	string owner;

	assert(x.Add("Prague", "Thakurova", "Dejvice", 12345));
	assert(x.Add("Prague", "Evropska", "Vokovice", 12345));
	assert(x.Add("Prague", "Technicka", "Dejvice", 9873));
	assert(!x.Add("Prague", "Technicka", "Hradcany", 7344));
	assert(!x.Add("Brno", "Bozetechova", "Dejvice", 9873));
	assert(!x.GetOwner("Prague", "THAKUROVA", owner));
	assert(!x.GetOwner("Hradcany", 7343, owner));
	CIterator i0 = x.ListByAddr();
	assert(!i0.AtEnd()
		&& i0.City() == "Prague"
		&& i0.Addr() == "Evropska"
		&& i0.Region() == "Vokovice"
		&& i0.ID() == 12345
		&& i0.Owner() == "");
	i0.Next();
	assert(!i0.AtEnd()
		&& i0.City() == "Prague"
		&& i0.Addr() == "Technicka"
		&& i0.Region() == "Dejvice"
		&& i0.ID() == 9873
		&& i0.Owner() == "");
	i0.Next();
	assert(!i0.AtEnd()
		&& i0.City() == "Prague"
		&& i0.Addr() == "Thakurova"
		&& i0.Region() == "Dejvice"
		&& i0.ID() == 12345
		&& i0.Owner() == "");
	i0.Next();
	assert(i0.AtEnd());

	assert(x.NewOwner("Prague", "Thakurova", "CVUT"));
	assert(!x.NewOwner("Prague", "Thakurova", "CVUT"));
	assert(!x.NewOwner("Prague", "Thakurova", "CVuT"));
	assert(!x.NewOwner("Prague", "technicka", "CVUT"));
	assert(!x.NewOwner("prague", "Technicka", "CVUT"));
	assert(!x.NewOwner("dejvice", 9873, "CVUT"));
	assert(!x.NewOwner("Dejvice", 9973, "CVUT"));
	assert(!x.NewOwner("Dejvice", 12345, "CVUT"));
	assert(x.Count("CVUT") == 1);
	CIterator i1 = x.ListByOwner("CVUT");
	assert(!i1.AtEnd()
		&& i1.City() == "Prague"
		&& i1.Addr() == "Thakurova"
		&& i1.Region() == "Dejvice"
		&& i1.ID() == 12345
		&& i1.Owner() == "CVUT");
	i1.Next();
	assert(i1.AtEnd());

	assert(!x.Del("Brno", "Technicka"));
	assert(!x.Del("Karlin", 9873));
	assert(x.Del("Prague", "Technicka"));
	assert(!x.Del("Prague", "Technicka"));
	assert(!x.Del("Dejvice", 9873));
}
void test2()
{
	string owner;
	CLandRegister x;
}
int main()
{
	test2();
	test0();
	test1();

	return 0;
}

#endif /* __PROGTEST__ */

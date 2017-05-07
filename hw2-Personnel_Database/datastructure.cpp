// Definition of Datastructure class, hw2 of TIE-20100/TIE-20106
#include "datastructure.hpp"
#include <algorithm> 
#include <climits>

// For debug
#include <iostream>
using std::cerr;
using std::endl;


template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = rand() % range;
    return static_cast<Type>(start+num);
}




Datastructure::Datastructure()
{
	personnel_N = 0;
	salary_sorted = true;
	names_sorted = true;
}

Datastructure::~Datastructure()
{
	clear();

}

void Datastructure::add_person(string name, PersonID id, string title, Salary salary)
{
	

	
	salary_list.push_back(new Person(name, id, title, salary));
	name_list.push_back(salary_list[personnel_N]);
	id_table[id] = salary_list[personnel_N];

	personnel_N++;
	salary_sorted = false;
	names_sorted = false;

}

void Datastructure::remove_person(PersonID id)
{
	//eliminate associations and then remove from datastructures.

	Person* c_person;

	try {
		c_person = id_table.at(id);
	}
	catch(const std::out_of_range& oor){
		return;
	}

	string c_name = c_person->name;
	string c_title = c_person->title;

	

	if (c_person->bosses_n > 0)
	{
		PersonID c_boss = c_person->boss;
		Person* boss = id_table.at(c_boss);

		//first remove from list of underlings of the boss
		auto it1 = std::find(boss->underlings.begin(), boss->underlings.end(), c_person->id);
		boss->underlings.erase(it1);
		
		//give new boss to underlings of id, then add each one to the underlings of boss
		for (auto it = c_person->underlings.begin(); it != c_person->underlings.end(); it++)
		{
			id_table.at(*it)->boss = c_boss;
			boss->underlings.push_back(*it);
		}

		//bosses changed; update ranks
		update_ranks(c_boss);
	}
	else //if id doesn't have a boss:
		for (auto it = c_person->underlings.begin(); it != c_person->underlings.end(); it++)
		{
			id_table.at(*it)->boss = NO_ID;
			update_ranks(*it); //update ranks for each underling
		}
			
		id_table.erase(id);

		delete c_person;
		name_list.erase(find(name_list.begin(), name_list.end(), c_person));
		salary_list.erase(find(salary_list.begin(), salary_list.end(), c_person));

		
		personnel_N--;
		salary_sorted = false;
		names_sorted = false;
}

string Datastructure::get_name(PersonID id)
{
	return id_table.at(id)->name;
}

string Datastructure::get_title(PersonID id)
{	
	return id_table.at(id)->title;
}

Salary Datastructure::get_salary(PersonID id)
{

	return id_table.at(id)->salary;
}

vector<PersonID> Datastructure::find_persons(string name)
{
	std::vector<PersonID> names;

		for (auto i = salary_list.begin(); i != salary_list.end(); i++)
			if ((*i)->name == name)
				names.push_back((*i)->id);

	std::sort(names.begin(), names.end());

	return names;
}

vector<PersonID> Datastructure::personnel_with_title(string title)
{
	std::vector<PersonID> titles;
	
		for (auto i = salary_list.begin(); i != salary_list.end(); i++)
			if ((*i)->title == title)
				titles.push_back((*i)->id);

	std::sort(titles.begin(), titles.end());

	return titles;
}

void Datastructure::change_name(PersonID id, string new_name)
{
	id_table.at(id)->name=new_name;
	names_sorted = false;
	
}

void Datastructure::change_salary(PersonID id, Salary new_salary)
{
	id_table.at(id)->salary= new_salary;
	salary_sorted = false;
	
}

void Datastructure::add_boss(PersonID id, PersonID bossid)
{
	id_table.at(id)->boss=bossid;
	id_table.at(bossid)->underlings.push_back(id);

	update_ranks(id);
}

unsigned int Datastructure::size()
{	
	return personnel_N;
}

void Datastructure::clear()
{
	for (auto it = salary_list.begin(); it != salary_list.end(); it++)
		delete *it;

	salary_list.clear();
	name_list.clear();
	id_table.clear();

	personnel_N = 0;
}

vector<PersonID> Datastructure::underlings(PersonID id)
{
	vector<PersonID> und = id_table.at(id)->underlings;
	std::sort(und.begin(), und.end());
	return und;
}

vector<PersonID> Datastructure::personnel_alphabetically()
{
	//No point in storing PersonID. You'd still have to fetch the name for each one: n*const=linear + nlogn
	if (!names_sorted)
	{

		std::sort(name_list.begin(), name_list.end(), comp_names);
		names_sorted = true;

	}

	vector<PersonID> alpha(personnel_N);
	
	auto vIt = alpha.begin();
	auto lIt = name_list.begin();


	while (vIt!=alpha.end()) {
		*vIt++ = (*lIt++)->id;
	}

	
	return alpha;

	
}

vector<PersonID> Datastructure::personnel_salary_order()
{
	
	if (!salary_sorted)
	{
		std::sort(salary_list.begin(), salary_list.end(), comp_salary);
		salary_sorted = true;

	}


	vector<PersonID> sal(personnel_N);

	auto vIt = sal.begin();
	auto lIt = salary_list.begin();

	while (vIt != sal.end()) {
		*vIt++ = (*lIt++)->id;
	}

	return sal;

}

PersonID Datastructure::find_ceo()
{
	
	unsigned int ceos = 0;
	PersonID ceo;

	
	//look for people with no bosses. If more than one was found return NO_ID
	for (auto it = salary_list.begin(); it != salary_list.end(); it++)
		if ((*it)->bosses_n == 0)
		{
			ceos++;
			ceo = (*it)->id;
			
		}

	if (ceos == 1)
		return ceo;

	return NO_ID;

}

PersonID Datastructure::nearest_common_boss(PersonID id1, PersonID id2)
{
	
	
	Person *P1 = id_table.at(id1), *P2 =id_table.at(id2);
	PersonID mb1 = P1->boss, mb2 = P2->boss;

	//if the bosses of each id1 and id2 are not at the same rank, move the lower rank
	//up until they're at the same level. If at the same level , move up the id1 chain. Repeat until reached a NO_ID.
	while (mb1 != NO_ID && mb2 != NO_ID) {
		
		if (!mb1.compare(mb2)) //if equal strings (0 diff)
			return mb1;

		else if (P1->bosses_n < P2->bosses_n)
		{
			P2 = id_table.at(mb2);
			mb2 = P2->boss;
		}
		else 
		{
			P1 = id_table.at(mb1);
			mb1 = P1->boss;
		}
		
	}

	return NO_ID;
}

pair<unsigned int, unsigned int> Datastructure::higher_lower_ranks(PersonID id)
{
	
	//go through each person and if bosses<c_boss, add to first part; if >, add to second part.
	
	unsigned int c_bosses = id_table.at(id)->bosses_n;
	unsigned int high=0, low=0;
	
	for (auto it = salary_list.begin(); it != salary_list.end(); it++)
		if ((*it)->id != id)
		{

			if ((*it)->bosses_n > c_bosses)
				low++;
			else if ((*it)->bosses_n < c_bosses)
				high++; 

		}

	std::pair<unsigned int, unsigned int> rank_pair(high, low);
	return rank_pair;
  
}

PersonID Datastructure::min_salary()
{
	
	if (salary_sorted)
		return salary_list[0]->id;
	



	//else
	int min_salary = INT_MAX;
	int min_i = 0;

	for (int i = 0; i < personnel_N; i++)
	{
		if (salary_list[i]->salary < min_salary)
		{

			min_salary = salary_list[i]->salary;
			min_i = i;
		}

	}


	return salary_list[min_i]->id;
	
}

PersonID Datastructure::max_salary()
{
	


	if (salary_sorted)
		return salary_list[personnel_N - 1]->id;


	//else

	int max_salary = 0;
	int max_i = 0;

	for (int i = 0; i < personnel_N; i++)
	{
		if (salary_list[i]->salary > max_salary)
		{
			max_salary = salary_list[i]->salary;
			max_i = i;
		}

	}

	return salary_list[max_i]->id;
}

PersonID Datastructure::median_salary()
{
	

	if (!salary_sorted)
	{
		std::sort(salary_list.begin(), salary_list.end(), comp_salary);
		salary_sorted = true;
	}

	return salary_list[floor(personnel_N / 2)]->id;
}

PersonID Datastructure::first_quartile_salary()
{

	if (!salary_sorted)
	{
		std::sort(salary_list.begin(), salary_list.end(), comp_salary);
		salary_sorted = true;
	}

	return salary_list[floor(personnel_N / 4)]->id;
}

PersonID Datastructure::third_quartile_salary()
{

	if (!salary_sorted)
	{
		std::sort(salary_list.begin(), salary_list.end(), comp_salary);
		salary_sorted = true;
	}

	return salary_list[floor(3 * personnel_N / 4)]->id;
}

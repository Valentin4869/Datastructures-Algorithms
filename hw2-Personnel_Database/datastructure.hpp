// Definition of Datastructure class, hw2 of TIE-20100/TIE-20106
#ifndef DATASTRUCTURE_HPP
#define DATASTRUCTURE_HPP

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <unordered_map>
using std::unordered_map;
using std::unordered_multimap;

#include <utility>
using std::pair;

using PersonID = string;
PersonID const NO_ID = "";

using Salary = int;
Salary const NO_SALARY = -1;


struct Person
{
	
	PersonID id;
	Salary salary;
	string name;
	string title;
	PersonID boss;
	vector<PersonID> underlings;
	unsigned int bosses_n;
	Person(std::string Name, PersonID Id, string Title, Salary Salary) 
	{ name = Name;
	id = Id;
	title = Title;
	salary = Salary;
	boss = NO_ID;
	bosses_n = 0; 
	}
	//bool operator<(Person*);
};


class Datastructure
{
public:
	Datastructure();
	~Datastructure();

	void add_person(string name, PersonID id, string title, Salary salary);
	void remove_person(PersonID id);

	string get_name(PersonID id);
	string get_title(PersonID id);
	Salary get_salary(PersonID id);
	vector<PersonID> find_persons(string name);
	vector<PersonID> personnel_with_title(string title);

	void change_name(PersonID id, string new_name);
	void change_salary(PersonID id, Salary new_salary);

	void add_boss(PersonID id, PersonID bossid);

	unsigned int size();
	void clear();

	vector<PersonID> underlings(PersonID id);
	vector<PersonID> personnel_alphabetically();
	vector<PersonID> personnel_salary_order();

	PersonID find_ceo();
	PersonID nearest_common_boss(PersonID id1, PersonID id2);
	pair<unsigned int, unsigned int> higher_lower_ranks(PersonID id);

	PersonID min_salary();
	PersonID max_salary();

	PersonID median_salary();
	PersonID first_quartile_salary();
	PersonID third_quartile_salary();

private:

	unsigned int personnel_N;
	bool salary_sorted; 
	bool names_sorted;
	vector<Person*> name_list;
	vector<Person*> salary_list; //treated as primary copy

	unordered_map<string, Person*> id_table; //<id, person>


	
	inline void update_ranks(PersonID p) {
		Person* P = id_table.at(p); 
		if (P->boss == NO_ID)
			P->bosses_n = 0;
		else
		P->bosses_n = id_table.at(P->boss)->bosses_n + 1; //every underling has the same bosses as their bosses + 1

		//then update ranks of all underlings of this person
		for (auto it = P->underlings.begin(); it != P->underlings.end(); it++)
		update_ranks(*it);
	}


	static bool comp_names(Person* p1, Person* p2) {return (p1->name < p2->name);}
	static bool comp_salary(Person* p1, Person* p2) {return (p1->salary < p2->salary);}
};

#endif // DATASTRUCTURE_HPP

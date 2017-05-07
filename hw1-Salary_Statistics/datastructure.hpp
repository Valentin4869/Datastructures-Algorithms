// Definition of Datastructure class, hw1 of TIE-20100/TIE-20106

#ifndef DATASTRUCTURE_HPP
#define DATASTRUCTURE_HPP

#include <string>
using std::string;

#include <vector>
using std::vector;

struct Person
{
    string name;
    int salary;

	Person(std::string Name, int Salary) { name = Name; salary = Salary; }
	bool operator<( Person*);
};

class Datastructure
{
public:
    Datastructure();
    ~Datastructure();

    void add_person(string name, int salary);

    unsigned int size();
    void clear();

    vector<Person*> personnel_alphabetically();
    vector<Person*> personnel_salary_order();

    Person* min_salary();
    Person* max_salary();

    Person* median_salary();
    Person* first_quartile_salary();
    Person* third_quartile_salary();

private:

	
	bool salary_sorted;
	bool names_sorted;
	vector<Person*> personnel_ss;
	vector<Person*> personnel_ns;
	int personnel_N;

	
};

#endif // DATASTRUCTURE_HPP

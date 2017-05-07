// Definition of Datastructure class, hw3 of TIE-20100/TIE-20106
#include "datastructure.hpp"
// For debug
#include <iostream>
using std::cerr;
using std::endl;

#include <climits>
#include <cmath>

//compare names or salaries?
bool compmode = 0;

// Helper function to return a random (enough) number in range start-end
// I.e. random_in_range(1,4) produces a random number between 1, 2, 3, or 4.
template <typename Type>
Type random_in_range(Type start, Type end)
{
	auto range = end - start;
	++range;

	auto num = rand() % range;
	return static_cast<Type>(start + num);
}




bool Person::operator<(Person*p2) {
	if (compmode)
		return (name < p2->name);

	return (salary < p2->salary);

}

template<class T>
inline void merge(T low, T mid, T high) {
	
	T i = low;
	T j = mid;


	std::vector<Person*> sorted(std::distance(low, high));
	auto it = sorted.begin();

	//while neither subarrays reached the end
	while (i < mid && j < high) {
		if ((**j) < (*i))
			(*it++) = std::move(*j++);

		else
			(*it++) = std::move(*i++);

	}
	
	//Move leftover subarray.
	if (j == high)
		std::move(i, mid, it);
	else
		std::move(j, high, it);
	
	//move the sorted array back
	std::move(std::begin(sorted), std::end(sorted), low);
}


template<class T>
void merge_sort(T low, T high) {

	int size = std::distance(low, high);

	if (size <= 1)
		return;

	T mid(low);
	std::advance(mid, size / 2);

	merge_sort(low, mid);
	merge_sort(mid, high);
	merge(low, mid, high);

}


void merge_sort(vector<Person*> &plist) {

	merge_sort(std::begin(plist), std::end(plist));

}



Datastructure::Datastructure()
{
	personnel_N = 0;
	salary_sorted = true;
	names_sorted = true;

}

Datastructure::~Datastructure()
{

	for (auto it = personnel_ss.begin(); it != personnel_ss.end(); it++)
		delete *it;
}

void Datastructure::add_person(std::string name, int salary)
{
	personnel_N++;

	personnel_ss.push_back(new Person(name,salary));
	personnel_ns.push_back(personnel_ss[personnel_N - 1]);
	

	if (personnel_N > 1)
	{
		if (salary_sorted && salary < personnel_ss[personnel_N - 2]->salary)
				salary_sorted = false;
		

		if (names_sorted && name < personnel_ns[personnel_N - 2]->name)
				names_sorted = false;

	}
	

}

unsigned int Datastructure::size()
{
	return personnel_N;
}

void Datastructure::clear()
{
	
//only delete pointers in one array since the second array only holds the copies.
for (auto it = personnel_ss.begin(); it != personnel_ss.end(); it++)
		delete *it;

	personnel_N = 0;
	personnel_ss.clear();
	personnel_ns.clear();	
	salary_sorted = true;
	names_sorted = true;
}

vector<Person *> Datastructure::personnel_alphabetically()
{
	if (!names_sorted)
	{
		//change compare mode to names. And set it back to default value (salary comparison).
		compmode = 1;
		merge_sort(personnel_ns);
		names_sorted = true;
		compmode=0;
	}

	
	return personnel_ns;
}

vector<Person *> Datastructure::personnel_salary_order()
{
	
	if (!salary_sorted)
	{
		merge_sort(personnel_ss);
 		salary_sorted = true;
	}

	return personnel_ss;
}

Person* Datastructure::min_salary()
{

	
	if (salary_sorted)
		return personnel_ss[0];
	

	

	//else
	int min_salary = INT_MAX;
	int min_i = 0;
	
	for (int i = 0; i < personnel_N; i++)
	{
		if (personnel_ss[i]->salary < min_salary)
		{
			min_salary = personnel_ss[i]->salary;
			min_i = i;
		}
		
	}

	
	return personnel_ss[min_i];

}

Person* Datastructure::max_salary()
{

	if (salary_sorted)
		return personnel_ss[personnel_N-1];


	//else

	int max_salary = 0;
	int max_i = 0;

	for (int i = 0; i < personnel_N; i++)
	{
		if (personnel_ss[i]->salary > max_salary)
		{
			max_salary = personnel_ss[i]->salary;
			max_i = i;
		}

	}

	return personnel_ss[max_i];

}

Person* Datastructure::median_salary()
{
	if (!salary_sorted)
	{
		merge_sort(personnel_ss);
		salary_sorted = true;
	}
	
	return personnel_ss[floor(personnel_N/2)];
}


Person* Datastructure::first_quartile_salary()
{
	if (!salary_sorted)
	{
		merge_sort(personnel_ss);
		salary_sorted = true;
	}

	return personnel_ss[floor(personnel_N / 4)];
}

Person* Datastructure::third_quartile_salary()
{
	if (!salary_sorted)
	{
		merge_sort(personnel_ss);
		salary_sorted = true;
	}

	return personnel_ss[floor(3* personnel_N / 4)];
}

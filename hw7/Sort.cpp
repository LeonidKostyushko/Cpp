#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <fstream>
#include <string>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <algorithm>

using namespace std;

const int max_numbers = 200000;

void conc(const string &f_1, const string &f_2, const string &conc)
{
	uint64_t x, y;
	ifstream f1(f_1, ios::binary);
	ifstream f2(f_2, ios::binary);
	ofstream res(conc, std::ios::binary);

	f1.read(reinterpret_cast<char*>(&x), sizeof(x));
	f2.read(reinterpret_cast<char*>(&y), sizeof(y));

	while (!f1.eof() && !f2.eof())
	{
		if (x > y)
		{
			res.write(reinterpret_cast<char*>(&y), sizeof(y));
			f2.read(reinterpret_cast<char*>(&y), sizeof(y));
		}
		else
		{
			res.write(reinterpret_cast<char*>(&x), sizeof(x));
			f1.read(reinterpret_cast<char*>(&x), sizeof(x));
		}
	}
	ifstream* fin = &f1;
	if (f1.eof())
	{
		fin = &f2;
		swap(x, y);
	}
	while (!(*fin).eof())
	{
		res.write(reinterpret_cast<char*>(&x), sizeof(x));
		(*fin).read(reinterpret_cast<char*>(&x), sizeof(x));
	}
}

vector<uint64_t> read_numbers(ifstream &file, int amount)
{
	vector<uint64_t> arr;
	uint64_t tmp;
	for (int i = 0; i < amount; i++)
	{
		file.read(reinterpret_cast<char*>(&tmp), sizeof(tmp));
		if (file.eof()) return arr;
		arr.push_back(tmp);
	}
	return arr;
}

void p_sort(ifstream &file, mutex &mtx)
{
	thread::id thread_id = this_thread::get_id();
	int n = 0;

	ostringstream ss;
	ss << thread_id << "_";

	while (!file.eof())
	{
		unique_lock<std::mutex> lock(mtx);

		vector<uint64_t> arr = read_numbers(file, max_numbers);
		if (arr.empty()) break;
		lock.unlock();
		sort(arr.begin(), arr.end());

		ofstream out(ss.str() + to_string(++n), ios::binary);
		out.write(reinterpret_cast<char*>(arr.data()), arr.size() * sizeof(uint64_t));
		this_thread::yield();
	}

	while (n > 1)
	{
		unique_lock<mutex> lock(mtx);
		string f1 = ss.str() + to_string(n--);
		string f2 = ss.str() + to_string(n);
		string tmp = ss.str() + string("tmp");
		conc(ref(f1), ref(f2), ref(tmp));
		lock.unlock();
		remove(f1.c_str());
		remove(f2.c_str());
		rename(tmp.c_str(), f2.c_str());
		this_thread::yield();
	}
}

string f_name_get(thread::id id)
{
	ostringstream ss;
	ss << id << "_1";
	return ss.str();
}

bool f_check(const string &fileName)
{
	ifstream infile(fileName);
	return infile.good();
}

int main(int argc, char **argv)
{
	char* filename;
	if (argc == 2) filename = argv[1];
	else return -1;

	ifstream file(filename, ios::binary);
	mutex mtx;
	thread t1(p_sort, ref(file), ref(mtx));
	thread t2(p_sort, ref(file), ref(mtx));

	string first = f_name_get(t1.get_id());
	string second = f_name_get(t2.get_id());
	string res("result");

	t1.join();
	t2.join();

	if (f_check(first) && f_check(second))
	{
		conc(first, second, res);
		remove(first.c_str());
		remove(second.c_str());
	}
	else
	{
		if (f_check(first)) rename(first.c_str(), res.c_str());
		else rename(second.c_str(), res.c_str());
	}
	return 0;
}
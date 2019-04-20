// ConsoleApplication34.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream> 
#include <vector> 
#include <string>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <typeinfo>


using namespace std;



void deleteSpaces(string& str)
{
	std::string::iterator end_pos = std::remove(str.begin(), str.end(), ' ');
	str.erase(end_pos, str.end());
}

int isCorrect(string str)
{
	vector<string> numbers;

	if (str.length() == 0) return 1;

	for (int i = 0; i < 10; i++)
	{
		numbers.push_back(to_string(i));
	}


	for (int i = 0; i < str.length() - 1; i++)           // zeroDivision
	{
		if (str[i] == '/' && str[i + 1] == '0') return 1;
	}

	if (str[0] == '/' || str[0] == '*' || str[str.length() - 1] == '*' || str[str.length() - 1] == '/') return 1;  // binary operation became single operation

	int cnt;
	for (int i = 0; i < str.length(); i++)          // incorrect input
	{
		cnt = 0;
		if (str[i] != '/' && str[i] != '*' && str[i] != '+' && str[i] != '-')
		{
			for (int j = 0; j < 10; j++)
			{
				if (str[i] == numbers[j][0]) cnt++;
			}
			if (cnt == 0) return 1;
		}
	}






	return 0;
}


void buildTree(string& str, int vertice, vector<string>& stringTree)
{
	string leftSub;
	string rightSub;



	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == '+')
		{
			leftSub = str.substr(0, i);
			rightSub = str.substr(i + 1, str.size());

			stringTree[vertice] = str[i];
			buildTree(leftSub, 2 * vertice, stringTree);
			buildTree(rightSub, 2 * vertice + 1, stringTree);

			return;
		}

	}

	for (int i = str.length() - 1; i >= 1; i--)
	{
		if (str[i] == '-' && str[i - 1] != '-')
		{
			leftSub = str.substr(0, i);
			rightSub = str.substr(i + 1, str.size());

			stringTree[vertice] = str[i];
			buildTree(leftSub, 2 * vertice, stringTree);
			buildTree(rightSub, 2 * vertice + 1, stringTree);

			return;
		}

	}

	for (int i = str.length() - 1; i >= 0; i--)
	{

		if (str[i] == '*' || str[i] == '/')
		{
			leftSub = str.substr(0, i);
			rightSub = str.substr(i + 1, str.size());

			stringTree[vertice] = str[i];
			buildTree(leftSub, 2 * vertice, stringTree);
			buildTree(rightSub, 2 * vertice + 1, stringTree);

			return;
		}

	}

	stringTree[vertice] = str;

	return;


}

int countTree(vector<string>& stringTree)
{
	int b, a, res, answer;

	for (int i = stringTree.size() - 1; i >= 3; i--)
	{
		if (!stringTree[i].empty() && (i % 2 != 0))
		{
			b = stoi(stringTree[i]);
			a = stoi(stringTree[i - 1]);

			if (stringTree[(i - 1) / 2] == "+") res = a + b;
			else if (stringTree[(i - 1) / 2] == "-") res = a - b;
			else if (stringTree[(i - 1) / 2] == "/") res = a / b;
			else if (stringTree[(i - 1) / 2] == "*") res = a * b;

			stringTree[(i - 1) / 2] = to_string(res);


		}
	}

	answer = stoi(stringTree[1]);

	return answer;
}

int main(int argc, char* argv[])
{
	const int N = 1000;
	vector<string> stringTree(N);

	string str = "";
	int result;

	for (int i = 1; i < argc; ++i)
	{
		str += argv[i];
	}

	if (argc != 2)
	{
		cout << "error" << endl;
		return 0;
	}

	deleteSpaces(str);

	if (isCorrect(str) != 0)
	{
		cout << "error" << endl;
		return 1;
	}

	buildTree(str, 1, stringTree);



	result = countTree(stringTree);

	cout << result << endl;









	return 0;
}

#pragma once
#include <iostream>

using namespace std;
class Matrix
{
private:
	int *matr;
	int rows;
	int columns;

	class Proxy {
	private:
		int* arr;
		int size;
	public:
		Proxy(int* arr, int size) : arr(arr), size(size) {}
		int operator[](int ind) const {
			if (ind >= size)
				throw std::out_of_range("");
			return arr[ind];
		}

		int& operator[](int ind) {
			if (ind >= size)
				throw std::out_of_range("");
			return arr[ind];
		}
	};

public:
	Matrix(int rowsQuantity, int columnsQuantity) : rows(rowsQuantity), columns(columnsQuantity), matr(new int[rows * columns]) {}

	Proxy operator[](int ind) {
		if (ind >= rows)
			throw out_of_range("");
		return Proxy(matr + ind * columns, columns);
	}

	const Proxy operator[](int ind) const {
		if (ind >= rows)
			throw std::out_of_range("");
		return Proxy(matr + ind * columns, columns);
	}

	int getRows()
	{
		return rows;
	}

	int getColumns()
	{
		return columns;
	}

	Matrix operator+(const Matrix& b) const
	{
		int r = this->rows;
		int c = this->columns;
		Matrix a(r, c);


		for (int i = 0; i < r; i++)
		{
			for (int j = 0; j < c; j++)
			{
				a.matr[i*c + j] = this->matr[i*c + j] + b.matr[i*c + j];
			}
		}

		return a;

	}

	Matrix& operator*=(int val) {
		for (auto i = 0; i < rows * columns; i++)
			this->matr[i] *= val;
		return *this;
	}

	bool operator==(const Matrix& other) const
	{
		int c = this->columns;
		int r = this->rows;

		if (this == &other)
			return true;

		for (int i = 0; i < r * c; i++)
		{
			if (this->matr[i] != other.matr[i]) return false;
		}

		return true;
	}

	bool operator!=(const Matrix& other) const
	{
		return !(*this == other);
	}



};
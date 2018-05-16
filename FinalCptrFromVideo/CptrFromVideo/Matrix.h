#include "Header.h"


class matrix
{
	public:
		double **elements;
	    int N, M;

		// конструкторы матриц
		matrix (); // пустая матрица 3x3
		matrix (double a11, double a12, double a13, double a21, double a22, double a23, double a31, double a32, double a33); // матрица 3x3 с аргументами
		matrix (double a11, double a12, double a13); // матрица 1x3 с аргументами

		// копирующий конструктор
		matrix(const matrix &);

		// деструктор
	    ~matrix();

		// оператор присваивания
		matrix& operator=(const matrix&);

		// нахождение определителя
		double det();

		// деление на число матрицы
		matrix& del(double);

		// транспонирование матрицы
		matrix& transp();

		
};

// вывод матрицы в консоль
ostream& operator<<(ostream&, matrix&);

// нахождение обратной матрицы
matrix inv(matrix&);

// перемножение двух матриц
matrix operator* (matrix, matrix);

// построить матрицу преобразований
matrix convertMatrixCreating(Point, Point, Point, int);

// выбрать нужную матрицу
int objectMatrixChoosing(Point, int, int, int, int, Point, Point);

// найти новые координаты точек
Point objectsNewPositioning(Point, matrix&);

// определение знака числа
int sign(double);


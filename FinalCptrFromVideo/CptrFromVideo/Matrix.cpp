#include "Matrix.h"


// конструктор нулевой матрицы 3x3
matrix::matrix()
	{
		N = 3; 
		M = 3; 
		elements = new double *[N];

		for (int i = 0; i < N; ++ i)
		{
			elements[i] = new double[M];
			for (int j = 0; j < M; ++ j)
				elements[i][j] = 0;
		}
	}


// конструктор матрицы 3х3 с аргументами
matrix::matrix (double a11, double a12, double a13, double a21, double a22, double a23, double a31, double a32, double a33) 
	{
		N = 3; 
		M = 3; 
		elements = new double *[N];

		for (int i = 0; i < N; ++ i)
			elements[i] = new double[M];

		elements[0][0] = a11; 
		elements[0][1] = a12;
		elements[0][2] = a13;
		elements[1][0] = a21;
		elements[1][1] = a22;
		elements[1][2] = a23;
		elements[2][0] = a31;
		elements[2][1] = a32;
		elements[2][2] = a33;
	}

// конструктор матрицы 1х3 с аргументами
matrix::matrix (double a11, double a12, double a13) 
	{
		N = 1; 
		M = 3; 
		elements = new double *[N];

		for (int i = 0; i < N; ++ i)
			elements[i] = new double[M];

		elements[0][0] = a11; 
		elements[0][1] = a12;
		elements[0][2] = a13;
	}

// копирующий контруктор
 matrix::matrix(const matrix& mtr)
    {      
		N = mtr.N;
		M = mtr.M;

        elements = new double *[N];

		for (int i = 0; i < N; ++ i)
		{
			elements[i] = new double[M];

			for (int j = 0; j < M; ++j)
				elements[i][j] = mtr.elements[i][j];
		}

		
    }
    

// деструктор
matrix::~matrix()
	{


		for (int i = 0; i < N; i++)
			delete[] elements[i];

		delete[] elements;
	}

// транспонирование
matrix& matrix::transp()
	{

		for (int i = 0; i < N; ++ i)
			for (int j = i; j < M; ++ j)
				elements[i][j] = elements[j][i];

		return *this;
	}


// деление на число
matrix& matrix::del (double k)
	{
		for (int i = 0; i < N; ++ i)
			for (int j = 0; j < M; ++ j)
				elements[i][j] = elements[i][j] / k;

		return *this;
	}


// оператор присваивания
matrix& matrix::operator=(const matrix& mtr)
{

	for (int i = 0; i < N; i++)
			delete[] elements[i];

		delete[] elements;

	N = mtr.N;
	M = mtr.M;

	elements = new double *[N];

	for (int i = 0; i < N; ++ i)
	{
		elements[i] = new double[M];

		for (int j = 0; j < M; ++ j)
			elements[i][j] = mtr.elements[i][j];

	}
	return *this;
}

// оператор умножения матриц
matrix operator* (matrix m1, matrix m2)
	{
		matrix mtrOut = matrix ();

		for (int i = 0; i < m1.N; ++ i)
			for (int j = 0; j < m2.M; ++ j)
				for (int k = 0; k < m1.M; ++ k)
				mtrOut.elements[i][j] += m1.elements[i][k] * m2.elements[k][j];

		return matrix(mtrOut.elements[0][0], mtrOut.elements[0][1], mtrOut.elements[0][2], mtrOut.elements[1][0], mtrOut.elements[1][1], mtrOut.elements[1][2], mtrOut.elements[2][0], mtrOut.elements[2][1], mtrOut.elements[2][2]);
	}

// определитель
double matrix::det ()
	{
		if ((M != N ) || (M !=3 ))
			return 0;

		double t1 = elements[0][0]*elements[1][1]*elements[2][2] + elements[0][1]*elements[1][2]*elements[2][0] + elements[0][2]*elements[1][0]*elements[2][1];
		double t2 = elements[0][2]*elements[1][1]*elements[2][0] + elements[0][0]*elements[1][2]*elements[2][1] + elements[0][1]*elements[1][0]*elements[2][2];

		return t1-t2;

	}



// обратная матрица методом алгебраических дополнений
matrix inv(matrix& mtrIn)
	{
		double d = mtrIn.det();	

		//алгебраические дополнения
		double a11 = mtrIn.elements[1][1]*mtrIn.elements[2][2] - mtrIn.elements[1][2]*mtrIn.elements[2][1];
		double a12 = (-1)*(mtrIn.elements[1][0]*mtrIn.elements[2][2] - mtrIn.elements[1][2]*mtrIn.elements[2][0]);
		double a13 = mtrIn.elements[1][0]*mtrIn.elements[2][1] - mtrIn.elements[1][1]*mtrIn.elements[2][0];

		double a21 = (-1)*(mtrIn.elements[0][1]*mtrIn.elements[2][2] - mtrIn.elements[0][2]*mtrIn.elements[2][1]);
		double a22 = mtrIn.elements[0][0]*mtrIn.elements[2][2] - mtrIn.elements[0][2]*mtrIn.elements[2][0];
		double a23 = (-1)*(mtrIn.elements[0][0]*mtrIn.elements[2][1] - mtrIn.elements[0][1]*mtrIn.elements[2][0]);

		double a31 = mtrIn.elements[0][1]*mtrIn.elements[1][2] - mtrIn.elements[0][2]*mtrIn.elements[1][1];
		double a32 = (-1)*(mtrIn.elements[0][0]*mtrIn.elements[1][2] - mtrIn.elements[0][2]*mtrIn.elements[1][0]);
		double a33 = mtrIn.elements[0][0]*mtrIn.elements[1][1] - mtrIn.elements[0][1]*mtrIn.elements[1][0];

		matrix mtrOut = matrix(a11, a12, a13, a21, a22, a23, a31, a32, a33);
		mtrOut.transp();
		mtrOut.del(d);

		return matrix(mtrOut.elements[0][0], mtrOut.elements[0][1], mtrOut.elements[0][2], mtrOut.elements[1][0], mtrOut.elements[1][1], mtrOut.elements[1][2], mtrOut.elements[2][0], mtrOut.elements[2][1], mtrOut.elements[2][2]);
	}


// построить матрицу перехода
matrix convertMatrixCreating(Point p1, Point p2, Point p3, int number)
	{
		int width = 70;
		int length = 100;
		matrix tBefore = matrix(p1.x, p1.y, 1, p2.x, p2.y, 1, p3.x, p3.y, 1);

		Point tp1;
		Point tp2;
		Point tp3;

		switch (number)
		{
		case 1:
			tp1 = Point(0,width);
			tp2 = Point(length,width);
			tp3 = Point(length/2,width/2);
			break;

		case 2:
			tp1 = Point(length,width);
			tp2 = Point(length,0);
			tp3 = Point(length/2,width/2);
			break;

		case 3:
			tp1 = Point(length,0);
			tp2 = Point(0,0);
			tp3 = Point(length/2,width/2);
			break;

		case 4:
			tp1 = Point(0,0);
			tp2 = Point(0,width);
			tp3 = Point(length/2,width/2);
			break;
		}

		matrix tAfter = matrix(tp1.x, tp1.y, 1, tp2.x, tp2.y, 1, tp3.x, tp3.y, 1);

		matrix tBeforeInv = inv(tBefore);

		matrix T = tBeforeInv * tAfter;

		return matrix(T.elements[0][0], T.elements[0][1], T.elements[0][2], T.elements[1][0], T.elements[1][1], T.elements[1][2], T.elements[2][0], T.elements[2][1], T.elements[2][2]);
	}

// оператор вывода
ostream& operator<<(ostream& out, matrix& mtrIn)
	{
		out << "matrix " << mtrIn.N << "x" << mtrIn.M << endl;
		for (int i = 0; i < mtrIn.N; ++ i)
		{
			for (int j = 0; j < mtrIn.M; ++ j)
				out << mtrIn.elements[i][j] << " ";
			out << endl;
		}
		return out;
	}

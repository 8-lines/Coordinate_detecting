#include "Header.h"


class matrix
{
	public:
		double **elements;
	    int N, M;

		// ������������ ������
		matrix (); // ������ ������� 3x3
		matrix (double a11, double a12, double a13, double a21, double a22, double a23, double a31, double a32, double a33); // ������� 3x3 � �����������
		matrix (double a11, double a12, double a13); // ������� 1x3 � �����������

		// ���������� �����������
		matrix(const matrix &);

		// ����������
	    ~matrix();

		// �������� ������������
		matrix& operator=(const matrix&);

		// ���������� ������������
		double det();

		// ������� �� ����� �������
		matrix& del(double);

		// ���������������� �������
		matrix& transp();

		
};

// ����� ������� � �������
ostream& operator<<(ostream&, matrix&);

// ���������� �������� �������
matrix inv(matrix&);

// ������������ ���� ������
matrix operator* (matrix, matrix);

// ��������� ������� ��������������
matrix convertMatrixCreating(Point, Point, Point, int);

// ������� ������ �������
int objectMatrixChoosing(Point, int, int, int, int, Point, Point);

// ����� ����� ���������� �����
Point objectsNewPositioning(Point, matrix&);

// ����������� ����� �����
int sign(double);


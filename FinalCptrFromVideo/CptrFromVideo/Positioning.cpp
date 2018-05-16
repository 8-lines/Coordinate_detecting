#include "Matrix.h"


int objectsPositioning(Mat &imageObjects, Mat &imageColoredObjects, Point* &objectPosition)
{
	
	imageColoredObjects = Mat(imageObjects.rows, imageObjects.cols, CV_8UC3);
	imageColoredObjects = Scalar::all(0);

	Mat out[] = { imageColoredObjects };
	int from_to[] = { 0,0 };
	mixChannels( &imageObjects, 1, out, 1, from_to, 1 );
	

	/*imshow("Objects", imageObjects);
	imshow("ColoredObjects", imageColoredObjects);
	waitKey(0);*/


	Point seed;
	int lo = 0;
	int up = 100;
	int color = 0;
	Scalar newVal;

	for( int y=0; y<imageColoredObjects.rows; y++ ) {

		for( int x=0; x<imageColoredObjects.cols; x++ ) {

			if ( imageColoredObjects.at<Vec3b>(y,x) == Vec3b( 255,0,0 ) )
                {
					color++;
					seed = Point(x,y);
					if (color / 256 == 0)
						newVal = Scalar( 0,0,color );
					if (color / 256 == 1)
						newVal = Scalar( 0,color - 255,255 );
					if (color / 256 == 2)
						newVal = Scalar( color - 510,255,255 );
					floodFill( imageColoredObjects, seed, newVal, 0, Scalar(lo,lo,lo), Scalar(up,up,up), FLOODFILL_FIXED_RANGE);
                }

		}
	}

	int* objects = new int[color];
	for (int i = 0; i < color; i++)
		objects[i] = 0;

	objectPosition = new Point[color];

	for( int y=0; y<imageColoredObjects.rows; y++ ) {

		for( int x=0; x<imageColoredObjects.cols; x++ ) {

			if ( imageColoredObjects.at<Vec3b>(y,x) != Vec3b( 255,255,255 ) )
                {
					int a = imageColoredObjects.at<Vec3b>(y,x)[2];
					objects[a-1]++;
					objectPosition[a-1].x += x;
					objectPosition[a-1].y += y;
                }

		}
	}

	for( int i=0; i<color; i++) 
	{
		if (objects[i] > 10)
		{
			objectPosition[i].x /= objects[i];
			objectPosition[i].y /= objects[i];
		}
		else
		{
			objects[i] = 0;
			objectPosition[i].x = -1;
			objectPosition[i].y = -1;
		}
	}

	return color;

}

// определение нужной матрицы
int objectMatrixChoosing(Point pt, int k1, int b1, int k2, int b2, Point ptLT, Point ptRT)
{
	if (sign(k1*pt.x + b1 - pt.y) == sign(k1*ptLT.x + b1 - ptLT.y))
	{
		if (sign(k2*pt.x + b2 - pt.y) == sign(k2*ptRT.x + b2 - ptRT.y))
			return 1;
		else
			return 4;
	}
	else
		if (sign(k2*pt.x + b2 - pt.y) == sign(k2*ptRT.x + b2 - ptRT.y))
			return 2;
		else
			return 3;
}

// нахождение новых координат точки
Point objectsNewPositioning(Point p, matrix& T)
	{
		matrix m1 = matrix(p.x, p.y, 1);

		//cout << m1 << endl;

		double xNew = p.x*T.elements[0][0] + p.y*T.elements[1][0] + T.elements[2][0];
		double yNew = p.x*T.elements[0][1] + p.y*T.elements[1][1] + T.elements[2][1];

		//cout << xNew << " " << yNew << endl;
		return Point(xNew, yNew);
	}


// определение знака числа
int sign(double val) {
  if (val == 0)  return 0;
  if (val > 0)  return 1;
  else return -1;
}
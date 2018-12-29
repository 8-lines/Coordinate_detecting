#include "Header.h"

//
// ������������ ����������� � HSV
// � ��������� ��������� ���������
// Hmin, Hmax, Smin, Smax, Vmin, Vmax
// ��� ��������� ������� �������
//


// ��� �������� ������� HSV
Mat hsv;
Mat h_plane;
Mat s_plane;
Mat v_plane;
// ��� �������� ������� HSV ����� ��������������
Mat h_range;
Mat s_range;
Mat v_range;

// ��� �������� ��������� ��������
Mat hsv_and;

int Hmin = 0;
int Hmax = 255;

int Smin = 0;
int Smax = 255;

int Vmin = 0;
int Vmax = 255;

int HSVmax = 255;

//void myTrackbarVmax(int pos, void*) {
//        Vmax = pos;
//        inRange(v_plane, Scalar(Vmin), Scalar(Vmax), v_range);
//}

void coloursDetecting(Mat &imageArea, Mat &imageObjects, Point* &neededPixels, int numberOfPixels)
{
	/// ������������ � ����������� ������
	
	cvtColor( imageArea, hsv, CV_BGR2YUV );
	//imageArea.copyTo(hsv);

	// ������� ������������� �����������
	h_plane.create( imageArea.rows, imageArea.cols, CV_8UC1 );
	s_plane.create( imageArea.rows, imageArea.cols, CV_8UC1 );
	v_plane.create( imageArea.rows, imageArea.cols, CV_8UC1 );
	h_range.create( imageArea.rows, imageArea.cols, CV_8UC1 );
	s_range.create( imageArea.rows, imageArea.cols, CV_8UC1 );
	v_range.create( imageArea.rows, imageArea.cols, CV_8UC1 );
	hsv_and.create( imageArea.rows, imageArea.cols, CV_8UC1 );

    // ��������� �� ������
	Mat pics[] = { h_plane, s_plane, v_plane };
	split(hsv, pics);

    // ������������� ����������� � ������������ ��������
    // � ������� HSV
	// ����������� �������� (�������� �����������):
	// H: 2 - 33 (2 - 26, 26 - 33)
	// S: 203 - 255
	// V: 224 - 255
	// ����������� �������� (�������� �����������, ����� ����):
	// H: 22 - 136
	// S: 250 - 255
	// V: 121 - 255

	Hmin = 500;
    Hmax = 0;

    Smin = 500;
    Smax = 0;

    Vmin = 500;
    Vmax = 0;

	for (int i = 0; i < numberOfPixels; i++)
	{
		int x = neededPixels[i].x;
		int y = neededPixels[i].y;
		if ((int)hsv.at<Vec3b>(y,x)[0] == 0)
			imageArea.at<Vec3b>(y,x) = Vec3b( 255,255,255 );
		cout << x << " " << y << " " << (int)hsv.at<Vec3b>(y,x)[0] << " " << (int)hsv.at<Vec3b>(y,x)[1] << " " << (int)hsv.at<Vec3b>(y,x)[2] << endl;
	}

	for (int i = 0; i < numberOfPixels; i++)
	{
		int x = neededPixels[i].x;
		int y = neededPixels[i].y;
		if ((int)hsv.at<Vec3b>(y,x)[0] < Hmin) // Hmin
			Hmin = (int)hsv.at<Vec3b>(y,x)[0];
		if ((int)hsv.at<Vec3b>(y,x)[0] > Hmax) // Hmax
			Hmax = (int)hsv.at<Vec3b>(y,x)[0];
		if ((int)hsv.at<Vec3b>(y,x)[1] < Smin) // Smin
			Smin = (int)hsv.at<Vec3b>(y,x)[1];
		if ((int)hsv.at<Vec3b>(y,x)[1] > Smax) // Smax
			Smax = (int)hsv.at<Vec3b>(y,x)[1];
		if ((int)hsv.at<Vec3b>(y,x)[2] < Vmin) // Vmin
			Vmin = (int)hsv.at<Vec3b>(y,x)[2];
		if ((int)hsv.at<Vec3b>(y,x)[2] > Vmax) // Vmax
			Vmax = (int)hsv.at<Vec3b>(y,x)[2];
	}

	cout << "Y: " << Hmin << " " << Hmax << endl;
	cout << "U: " << Smin << " " << Smax << endl;
	cout << "V: " << Vmin << " " << Vmax << endl;

    inRange(h_plane, Scalar(Hmin), Scalar(Hmax), h_range);
	inRange(s_plane, Scalar(Smin), Scalar(Smax), s_range);
	inRange(v_plane, Scalar(Vmin), Scalar(Vmax), v_range);

    while(true){

        // ���������� ������������ �����������
		imshow("Area",imageArea);

		imshow("Y", h_plane);
		imshow("U", s_plane);
		imshow("V", v_plane);

		imshow("Y range", h_range);
		imshow("U range", s_range);
		imshow("V range", v_range);

        // ���������� ������������� �����������
		bitwise_and(h_range, s_range, hsv_and);
		bitwise_and(hsv_and, v_range, hsv_and);

		imshow("YUV and", hsv_and);

		char c = waitKey(33);
            if (c == 27)  // ���� ������ ESC - �������
                    break;
    }

	bitwise_and(h_range, s_range, hsv_and);
	bitwise_and(hsv_and, v_range, hsv_and);

	destroyAllWindows();

	//������� ����
	int iterations = 1;
	int size = 5;
	Mat element = getStructuringElement( MORPH_ELLIPSE, Size( size, size ), Point( -1 , 1 ) );
	morphologyEx( hsv_and, imageObjects, MORPH_CLOSE, element, Point(-1,-1), iterations );

	imshow("imageObjects", imageObjects);
	waitKey(0);

}
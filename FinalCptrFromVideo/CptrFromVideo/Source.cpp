#include "Matrix.h"
#include <ctime>

// изображения
Mat image;
Mat imageGray;
Mat imageSmooth;
Mat imageBorders;
Mat imageLines;
Mat imageField;
Mat imageArea;
Mat imageObjects;
Mat imageColoredObjects;
Mat mask;

// точки поля
Point ptLT;
Point ptRT;
Point ptLB;
Point ptRB;
Point ptMT;
Point ptMB;
Point ptMM;

// массив координат объектов
Point* objectPosition;

// количество пикселей нужного цвета
int numberOfPixels = 0;

// массив координат пикселей нужного цвета
Point* neededPixels;

// выбрал ли пользователь цвет
bool isClicked = false;


void onMouse( int event, int x, int y, int, void* )
{
    if( event != EVENT_LBUTTONDOWN )
        return;

	//isClicked = true;

    Point seed = Point(x,y);

	//Закрасим пиксели нужной области
	if ( !( (mask.at<Vec3b>(y,x)[0] == mask.at<Vec3b>(y,x)[1]) && (mask.at<Vec3b>(y,x)[0] == mask.at<Vec3b>(y,x)[2]) && (mask.at<Vec3b>(y,x)[0] == 100) ) )
	{
		//floodFill(mask, seed, Vec3b(255, 255, 255) ,0, Scalar(), Scalar(), 4 );
		numberOfPixels += floodFill(mask, seed, Vec3b(100, 100, 100) ,0, Scalar(), Scalar(), 4 );
	}

    cout << numberOfPixels << " pixels in total were repainted\n";
}

void pixelsMemorising(Mat &image)
{
	//Найдем и запомним координаты нужных пикселей
	neededPixels = new Point[numberOfPixels];
	int k = 0;

	for( int y=0; y<mask.rows; y++ ) 
	{
		for( int x=0; x<mask.cols; x++ ) 
		{
			if ( (mask.at<Vec3b>(y,x)[0] == mask.at<Vec3b>(y,x)[1]) && (mask.at<Vec3b>(y,x)[0] == mask.at<Vec3b>(y,x)[2]) && (mask.at<Vec3b>(y,x)[0] == 100) )
			{
				neededPixels[k] = Point(x,y);
				k++;
				//cout << x << " " << y << " " << (int)mask.at<Vec3b>(y,x)[0] << " " << (int)mask.at<Vec3b>(y,x)[1] << " " << (int)mask.at<Vec3b>(y,x)[2] << endl;
			}
		}
		if (k == numberOfPixels)
			break;
	}

	imshow("mask", mask);
	waitKey(0);

}

int main(int argc, char* argv[])
{
	
	//VideoCapture cap(0); // open the default camera
	//   if(!cap.isOpened())  // check if we succeeded
	//       return -1;
	//Mat image;
	//namedWindow("image",1);
	//
	//while (true)
	//{
	//	cap >> image;
	//	imshow("image", image);
	//	waitKey(0);
	//}

	//Mat image = imread( "Football-mid-adv.jpg", CV_LOAD_IMAGE_COLOR);
	Mat image = imread( "Football-real1.png", CV_LOAD_IMAGE_COLOR);


	// обрабатываем изображение
	ImagesFiltering(image, imageGray, imageSmooth, imageBorders );

	//Ищем линии
	ImagesLinesDrawing(image, imageBorders, imageLines);

	//Ищем ключевые точки
	ImagesDotsSearching(image, imageLines, imageField);

	//Выделяем нужную часть для распознавания
	ImagesAreaDetecting(image, imageField, imageArea);

	//Пользователь кликает на нужные маркеры
	Canny(imageArea, mask, 100, 200);
	cvtColor(mask, mask, COLOR_GRAY2BGR);
	imshow("Mask", mask);
	namedWindow( "ImageArea", 0 );
	setMouseCallback( "ImageArea", onMouse, 0 );

	while (isClicked == false)
    {
		
        imshow("ImageArea", imageArea);

        char c = (char)waitKey(0);
        if( c == 27 )
        {
            cout << "Exiting ...\n";
            break;
        }
	}
	// Запомним координаты нужных пикселей
	pixelsMemorising(image);
	

	clock_t timeStart = 0;
	clock_t timeStop = 0;
	timeStart = clock();

	//Распознаем объекты по цвету
	//objectsDetecting(imageArea, imageObjects);

	coloursDetecting(imageArea, imageObjects, neededPixels, numberOfPixels);

	/*imshow("Dst", imageArea);
	imshow("Objects", imageObjects);
	waitKey(0);*/

	// находим количество и координаты объектов
	int number = objectsPositioning(imageObjects, imageColoredObjects, objectPosition);
	cout << number << endl;

	//Строим матрицы преобразования
	matrix T1 = convertMatrixCreating(ptLT, ptRT, ptMM, 1);
	matrix T2 = convertMatrixCreating(ptRT, ptRB, ptMM, 2);
	matrix T3 = convertMatrixCreating(ptRB, ptLB, ptMM, 3);
	matrix T4 = convertMatrixCreating(ptLB, ptLT, ptMM, 4);
	//cout << number << endl;

	double k1 = (ptRT.y - ptLB.y)/(ptRT.x - ptLB.x);
	double b1 = k1*ptLB.x - ptLB.y;
	double k2 = (ptLT.y - ptRB.y)/(ptLT.x - ptRB.x);
	double b2 = k1*ptLT.x - ptLT.y;
	int k;

	//cout << endl << number << " objects:" << endl; // вывести количество всех найденных объектов
	for (int i = 0; i < number; i++)
	{
		if (objectPosition[i].x >= 0)
		{
			image.at<Vec3b>(objectPosition[i].y,objectPosition[i].x) = Vec3b(255, 255, 255); // пометить центр объекта белой точкой
																							 // для наглядности
			k = objectMatrixChoosing(objectPosition[i], k1, b1, k2, b2, ptLT, ptRT);
			cout << "Old position: " << objectPosition[i].x << " " << objectPosition[i].y << endl;
			switch (k)
			{
			case 1:
				objectPosition[i] = objectsNewPositioning(objectPosition[i], T1);
				break;
			case 2:
				objectPosition[i] = objectsNewPositioning(objectPosition[i], T2);
				break;
			case 3:
				objectPosition[i] = objectsNewPositioning(objectPosition[i], T3);
				break;
			case 4:
				objectPosition[i] = objectsNewPositioning(objectPosition[i], T4);
				break;

			}
			cout << "New position: " << objectPosition[i].x << " " << objectPosition[i].y << endl;
		}


	}

		timeStop = clock();
		cout << (double)(timeStop - timeStart)/(double)(CLOCKS_PER_SEC) << " Seconds elapsed" << endl;

	//int i = 0;
	//for( int x=0; x<mask.rows; x++ ) {

	//		for( int y=0; y<mask.cols; y++ ) {

	//			if ((x == neededPixels[i].x) && (y == neededPixels[i].y))
	//				{
	//					image.at<Vec3b>(y,x)[0] = 255; // B - синий
	//					image.at<Vec3b>(y,x)[1] = 255; // G - зелёный
	//					image.at<Vec3b>(y,x)[2] = 255; // R - красный
	//					i++;
	//				}

	//		}
	//		if (i == numberOfPixels)
	//			break;
	//	}

	// показываем картинки
	ImagesShowing(image, imageGray, imageSmooth, imageBorders, imageLines, imageField, imageArea, imageObjects, imageColoredObjects);

	// ждём нажатия клавиши
	waitKey(0);
	
	
	return 0;
}

#include "Matrix.h"

extern Point ptLT;
extern Point ptRT;
extern Point ptLB;
extern Point ptRB;
extern Point ptMT;
extern Point ptMB;
extern Point ptMM;


void ImagesFiltering(Mat &image, Mat &imageGray, Mat &imageSmooth, Mat&imageBorders)
{
	// преобразуем в градации серого
	cvtColor(image, imageGray, COLOR_RGB2GRAY);

	// сглаживаем
	GaussianBlur( imageGray, imageSmooth, Size( 5, 5 ), 0, 0 );

	// получаем границы
	Canny(imageSmooth, imageBorders, 100, 300, 3); //1:2 - 1:3

	// применяем операцию закрытия
	Mat imageBordersTemp;
	imageBorders.copyTo(imageBordersTemp);
	int iterations = 3;
	int size = 3;
	Mat element = getStructuringElement( MORPH_RECT, Size( size, size ), Point( -1 , 1 ) );
	morphologyEx( imageBordersTemp, imageBorders, MORPH_CLOSE, element, Point(-1,-1), iterations ); 

	////Преобразуем входное изображение в необходимый формат
	Mat imageTemp;
	imageTemp.create( image.size(), image.type() );
	imageTemp = Scalar::all(0);
	image.copyTo( imageTemp, imageBorders);

	/*imshow("Original", image);
	imshow("Borders", imageBorders);
	imshow("Smooth", imageSmooth);
	imshow( "Temp", imageTemp );

	waitKey(0);*/
}

void ImagesLinesDrawing(Mat &image, Mat &imageBorders, Mat &imageLines)
{
	//Создаем пустое изображение для рисования линий
	Mat imageTemp;
	imageTemp.create( image.size(), image.type() );
	imageTemp = Scalar::all(0);

	//Вектор для найденных линий
	vector<Vec4i> lines;

	//Ищем линии
	HoughLinesP(imageBorders, lines, 1, CV_PI/180, 50, 50, 10 );

		//Рисуем линии
		for( size_t i = 0; i < lines.size(); i++ )
		{
			Vec4i l = lines[i];
			int x1 = l[0];
			int y1 = l[1];
			int x2 = l[2];
			int y2 = l[3];
			line( imageTemp, Point(x1, y1), Point(x2, y2), Scalar(255,255,255), 1, LINE_AA);
		}

		//Убираем дыры
		int iterations = 3;
		int size = 3;
		Mat element = getStructuringElement( MORPH_RECT, Size( size, size ), Point( -1 , 1 ) );
		morphologyEx( imageTemp, imageLines, MORPH_CLOSE, element, Point(-1,-1), iterations ); 

		/*imshow( "Borders", imageBorders );
		imshow( "Lines", imageLines );
		waitKey(0);*/
	
}

void ImagesDotsSearching(Mat &image, Mat &imageLines, Mat &imageField)
{
	//Преобразуем входное изображение в необходимый формат
	Mat imageTemp2;
	cvtColor(imageLines, imageTemp2, COLOR_RGB2GRAY);
	imageTemp2.copyTo(imageLines);

	//Создаем пустое изображение для рисования линий
	imageField.create( image.size(), image.type() );
	imageField = Scalar::all(0);

	//Вектор для найденных линий
	vector<Vec4i> lines;

	/*imshow( "Lines", imageLines );
	imshow( "Field", imageField );
	imshow( "Temp", imageTemp );
	waitKey(0);*/

	//Ищем линии
	HoughLinesP(imageLines, lines, 2, CV_PI/180, 50, 50, 10 );

	ptLT = Point(5000,5000);
	ptRT = Point(0,5000);
	ptLB = Point(5000,0);
	ptRB = Point(0,0);

		for( size_t i = 0; i < lines.size(); i++ )
		{
			Vec4i l = lines[i];
			int x1 = l[0];
			int y1 = l[1];
			int x2 = l[2];
			int y2 = l[3];

			//Первая точка должна быть ближе к началу координат чем вторая
			if ( sqrt(x1*x1 + y1*y1) > sqrt(x2*x2 + y2*y2) )
			{
				int x,y;
				x = x1;
				y = y1;
				x1 = x2;
				y1 = y2;
				x2 = x;
				y2 = y;
			}			

			// Рисуем только длинные линии. Y-координаты определяются 
			// только горизонтальными линиями в случае, если на изображении только половина поля
			if ((x2-x1 >= (imageLines.cols)/2) || (y2-y1 >= (imageLines.rows)/2))
			{ 
				//Левая вертикальная линия
				if ( (abs(x2-x1)<=15) && ( x1<(imageLines.cols)/2) ) 
				{
					ptLT.x = min(ptLT.x,x1);
					//ptLT.y = min(ptLT.y,y1);
					ptLB.x = min(ptLB.x,x2);
					//ptLB.y = max(ptLB.y,y2);
				}

				//Правая вертикальная линия
				if ( (abs(x2-x1)<=15) && ( x1>(imageLines.cols)/2) )
				{
					ptRT.x = max(ptRT.x,x1);
					//ptRT.y = min(ptRT.y,y1);
					ptRB.x = max(ptRB.x,x2);
					//ptRB.y = max(ptRB.y,y2);
				}

				//Верхняя горизонтальная линия
				if ( (abs(y2-y1)<=15) && ( y1<(imageLines.rows)/2) ) 
				{
					ptLT.x = min(ptLT.x,x1);
					ptLT.y = min(ptLT.y,y1);
					ptRT.x = max(ptRT.x,x2);
					ptRT.y = min(ptRT.y,y2);
				}

				//Нижняя горизонтальная линия
				if ( (abs(y2-y1)<=15) && ( y1>(imageLines.rows)/2) )
				{
					ptLB.x = min(ptLB.x,x1);
					ptLB.y = max(ptLB.y,y1);
					ptRB.x = max(ptRB.x,x2);
					ptRB.y = max(ptRB.y,y2);
				}

			}
			 

		}

		//Находим точки для центральной линии
		ptMT.x = (ptLT.x + ptRT.x) / 2;
		ptMT.y = (ptLT.y + ptRT.y) / 2;
		ptMB.x = (ptLB.x + ptRB.x) / 2;
		ptMB.y = (ptLB.y + ptRB.y) / 2;

		//Находим центральную точку
		ptMM.x = (ptMT.x + ptMB.x) / 2;
		ptMM.y = (ptMT.y + ptMB.y) / 2;

		//Рисуем найденные линии
	    line( imageField, ptLT, ptRT, Scalar(255,255,255), 1, LINE_AA);
		line( imageField, ptLB, ptRB, Scalar(255,255,255), 1, LINE_AA);
		line( imageField, ptLT, ptLB, Scalar(255,255,255), 1, LINE_AA);
		line( imageField, ptRT, ptRB, Scalar(255,255,255), 1, LINE_AA);
		//line( imageField, ptMT, ptMB, Scalar(255,255,255), 1, LINE_AA); // центральная линия

		cout << "LT dot: " << ptLT.x << " " << ptLT.y << endl;
		cout << "RT dot: " << ptRT.x << " " << ptRT.y << endl;
		cout << "LB dot: " << ptLB.x << " " << ptLB.y << endl;
		cout << "RB dot: " << ptRB.x << " " << ptRB.y << endl;
		cout << "MM dot: " << ptMM.x << " " << ptMM.y << endl;

		/*imshow( "Field", imageField );
		waitKey(0);*/
		
}



void ImagesAreaDetecting(Mat &image, Mat &imageField, Mat &imageArea)
{
	Mat imageTemp;
	imageField.copyTo(imageTemp);
	Point seed = Point(imageTemp.cols/2,imageTemp.rows/2);

	int lo = 0;
	int up = 100;
	Scalar newVal = Scalar( 0,0,255 );
	floodFill( imageTemp, seed, newVal, 0, Scalar(lo,lo,lo), Scalar(up,up,up), FLOODFILL_FIXED_RANGE);
	image.copyTo(imageArea);

	//cvShowImage("Field",&(IplImage)imageField);
	//cvShowImage("Temp",&(IplImage)imageTemp);
	//cvShowImage("Dst",&(IplImage)imageArea);

	//waitKey(0);

	//Теперь наложим на основное изображение
	for( int y=0; y<imageArea.rows; y++ ) {
		for( int x=0; x<imageArea.cols; x++ ) {

			if ( imageTemp.at<Vec3b>(y,x)[2] != 255 )
                {
                    imageArea.at<Vec3b>(y,x)[0]= 0; // B - синий
                    imageArea.at<Vec3b>(y,x)[1]= 0; // G - зелёный
                    imageArea.at<Vec3b>(y,x)[2]= 0; // R - красный
                }

		}
	}

}



void ImagesShowing(Mat &image, Mat &imageGray, Mat &imageSmooth, Mat &imageBorders, Mat &imageLines, Mat &imageField, Mat &imageArea, Mat &imageObjects, Mat &imageColoredObjects)
{
	// показываем картинки

	imshow("Original", image);
	imshow("Gray", imageGray);
	imshow("Smooth", imageSmooth);
	imshow("Borders", imageBorders);
	imshow("Lines", imageLines);
	imshow("Field", imageField);	
	imshow("Dst", imageArea);	
	imshow("Objects", imageObjects);	
	imshow("ColoredObjects", imageColoredObjects);


}




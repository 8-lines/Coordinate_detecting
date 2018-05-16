#include "Header.h"

//
// конвертирует изображение в HSV
// и позвол€ет подобрать параметры
// Hmin, Hmax, Smin, Smax, Vmin, Vmax
// дл€ выделени€ нужного объекта
//


// дл€ хранени€ каналов HSV
Mat hsv;
Mat h_plane;
Mat s_plane;
Mat v_plane;
// дл€ хранени€ каналов HSV после преобразовани€
Mat h_range;
Mat s_range;
Mat v_range;

// дл€ хранени€ суммарной картинки
Mat hsv_and;

int Hmin = 0;
int Hmax = 255;

int Smin = 0;
int Smax = 255;

int Vmin = 0;
int Vmax = 255;

int HSVmax = 255;

//
// функции-обработчики ползунков
//
void myTrackbarHmin(int pos, void*) {
        Hmin = pos;
        inRange(h_plane, Scalar(Hmin), Scalar(Hmax), h_range);
}

void myTrackbarHmax(int pos, void*) {
        Hmax = pos;
        inRange(h_plane, Scalar(Hmin), Scalar(Hmax), h_range);
}

void myTrackbarSmin(int pos, void*) {
        Smin = pos;
        inRange(s_plane, Scalar(Smin), Scalar(Smax), s_range);
}

void myTrackbarSmax(int pos, void*) {
        Smax = pos;
        inRange(s_plane, Scalar(Smin), Scalar(Smax), s_range);
}

void myTrackbarVmin(int pos, void*) {
        Vmin = pos;
        inRange(v_plane, Scalar(Vmin), Scalar(Vmax), v_range);

}

void myTrackbarVmax(int pos, void*) {
        Vmax = pos;
        inRange(v_plane, Scalar(Vmin), Scalar(Vmax), v_range);
}

void objectsDetecting(Mat &image, Mat &imageObjects)
{
		/// конвертируем в HSV
		cvtColor( image, hsv, CV_BGR2HSV );

		// создаем одноканальные изображени€
		h_plane.create( image.rows, image.cols, CV_8UC1 );
		s_plane.create( image.rows, image.cols, CV_8UC1 );
		v_plane.create( image.rows, image.cols, CV_8UC1 );
		h_range.create( image.rows, image.cols, CV_8UC1 );
		s_range.create( image.rows, image.cols, CV_8UC1 );
		v_range.create( image.rows, image.cols, CV_8UC1 );
		hsv_and.create( image.rows, image.cols, CV_8UC1 );

        // разбиваем на каналы
		Mat pics[] = { h_plane, s_plane, v_plane };
		split(hsv, pics);

        // устанавливаем минимальное и максимальное значение
        // у каналов HSV
		// ѕодобранные значени€ (тестовое изображение):
		// H: 2 - 33 (2 - 26, 26 - 33)
		// S: 203 - 255
		// V: 224 - 255
		// ѕодобранные значени€ (реальное изображение):
		// H: 22 - 136
		// S: 250 - 255
		// V: 121 - 255
        Hmin = 22;
        Hmax = 136;

        Smin = 250;
        Smax = 255;

        Vmin = 121;
        Vmax = 255;

		myTrackbarHmin(Hmin, 0);
		myTrackbarHmax(Hmax, 0);
		myTrackbarSmin(Smin, 0);
		myTrackbarSmax(Smax, 0);
		myTrackbarVmin(Vmin, 0);
		myTrackbarVmax(Vmax, 0);


  //      //окна дл€ отображени€ картинки
  //      namedWindow("Original",1);
  //      namedWindow("H",1);
  //      namedWindow("S",1);
  //      namedWindow("V",1);
  //      namedWindow("H range",1);
  //      namedWindow("S range",1);
  //      namedWindow("V range",1);
  //      namedWindow("hsv and",1);

		////ползунки дл€ калибровки параметров H,S,V
  //      createTrackbar("Hmin", "H range", &Hmin, HSVmax, myTrackbarHmin);
  //      createTrackbar("Hmax", "H range", &Hmax, HSVmax, myTrackbarHmax);
  //      createTrackbar("Smin", "S range", &Smin, HSVmax, myTrackbarSmin);
  //      createTrackbar("Smax", "S range", &Smax, HSVmax, myTrackbarSmax);
  //      createTrackbar("Vmin", "V range", &Vmin, HSVmax, myTrackbarVmin);
  //      createTrackbar("Vmax", "V range", &Vmax, HSVmax, myTrackbarVmax);


   //     while(true){

   //             // показываем получившиес€ изображени€
			//	//cvShowImage("Original",&(IplImage)image);
   //             //
			//imshow("Original",image);

			//imshow("H", h_plane);
			//imshow("S", s_plane);
			//imshow("V", v_plane);

			//imshow("H range", h_range);
			//imshow("S range", s_range);
			//imshow("V range", v_range);

   //         // складываем одноканальные изображени€
			//bitwise_and(h_range, s_range, hsv_and);
			//bitwise_and(hsv_and, v_range, hsv_and);

			//imshow("hsv and", hsv_and);

			//char c = waitKey(33);
   //             if (c == 27)  // если нажата ESC - выходим
   //                     break;
   //     }

		bitwise_and(h_range, s_range, hsv_and);
		bitwise_and(hsv_and, v_range, hsv_and);


        //printf("\n[i] Results:\n" );
        //printf("[H] %d x %d\n", Hmin, Hmax );
        //printf("[S] %d x %d\n", Smin, Smax );
        //printf("[V] %d x %d\n", Vmin, Vmax );
		destroyAllWindows();

		//”бираем дыры
		int iterations = 3;
		int size = 3;
		Mat element = getStructuringElement( MORPH_ELLIPSE, Size( size, size ), Point( -1 , 1 ) );
		morphologyEx( hsv_and, imageObjects, MORPH_CLOSE, element, Point(-1,-1), iterations );

}
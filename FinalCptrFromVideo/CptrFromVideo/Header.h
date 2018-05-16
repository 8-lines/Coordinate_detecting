#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <math.h>
#include <iostream>

using namespace std;
using namespace cv;

void ImagesFiltering(Mat &image, Mat &imageGray, Mat &imageSmooth, Mat&imageBorders);

void ImagesLinesDrawing(Mat &image, Mat &imageSrc, Mat &imageDst);

void ImagesDotsSearching(Mat &image, Mat &imageSrc, Mat &imageDst);

void ImagesAreaDetecting(Mat &image, Mat &imageField, Mat &imageArea);

void objectsDetecting(Mat &imageArea, Mat &imageObjects);

int objectsPositioning(Mat &imageObjects, Mat &imageColoredObjects, Point* &objectPosition);

void ImagesShowing(Mat &image, Mat &imageGray, Mat &imageSmooth, Mat &imageBorders, Mat &imageLines, Mat &imageField, Mat &imageArea, Mat &imageObjects, Mat &imageColoredObjects);




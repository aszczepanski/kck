#include "image_processing.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace cv;
using namespace std;

static Mat src_gray;
static RNG rng(12345);

cv::Mat mergePictures(std::vector<cv::Mat>& images, int cols, int min_gap_size)
{
	// let's first find out the maximum dimensions
	int max_width = 0;
	int max_height = 0;
	for ( int i = 0; i < (int)images.size(); i++) {
		// check if type is correct 
		// you could actually remove that check and convert the image 
		// in question to a specific type
		if ( i > 0 && images[i].type() != images[i-1].type() ) {
			std::cerr << "WARNING:createOne failed, different types of images";
			return cv::Mat();
		}
		max_height = std::max(max_height, images[i].rows);
		max_width = std::max(max_width, images[i].cols);
	}
	// number of images in y direction
	int rows = std::ceil(images.size() / cols);

	// create our result-matrix
	cv::Mat result = cv::Mat::zeros(rows*max_height + (rows-1)*min_gap_size,
			cols*max_width + (cols-1)*min_gap_size, images[0].type());
	size_t i = 0;
	int current_height = 0;
	int current_width = 0;
	for ( int y = 0; y < rows; y++ ) {
		for ( int x = 0; x < cols; x++ ) {
			if ( i >= images.size() ) // shouldn't happen, but let's be safe
				return result;
			// get the ROI in our result-image
			cv::Mat to(result, cv::Range(current_height, current_height + images[i].rows),
					cv::Range(current_width, current_width + images[i].cols));
			// copy the current image to the ROI
			images[i++].copyTo(to);
			current_width += max_width + min_gap_size;
		}
		// next line - reset width and update height
		current_width = 0;
		current_height += max_height + min_gap_size;
	}
	return result;
}

void prepareImageWithContours(const cv::Mat& src, cv::Mat& dst, bool clearDst)
{
	/// Convert image to gray and blur it
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	const int blurSize = 5;
	blur(src_gray, src_gray, Size(blurSize,blurSize));

	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// erode
	const int erosion_elem = 0;
	const int erosion_size = 4;
	int erosion_type;
	if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
	else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
	else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

	Mat element1 = getStructuringElement( erosion_type,
			Size( 2*erosion_size + 1, 2*erosion_size+1 ),
			Point( erosion_size, erosion_size ) );

	/// Apply the erosion operation
	erode( src_gray, src_gray, element1 );


	/// Detect edges using canny
	Canny(src_gray, canny_output, 20, 100, 3);

	/// dilate
	const int dilation_elem = 0;
	const int dilation_size = 2;
	int dilation_type;
	if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
	else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
	else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

	Mat element = getStructuringElement( dilation_type,
			Size( 2*dilation_size + 1, 2*dilation_size+1 ),
			Point( dilation_size, dilation_size ) );
	/// Apply the dilation operation
	dilate( canny_output, canny_output, element );

	/// Find contours
	findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0,0));

	/// Get the moments
	vector<Moments> mu(contours.size());
	for (size_t i=0; i<contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
	}

	///  Get the mass centers:
	vector<Point2f> mc(contours.size());
	for (size_t i = 0; i < contours.size(); i++)
	{
		mc[i] = Point2f(mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
	}

	if (clearDst == true)
	{
		dst = Mat::zeros(canny_output.size(), CV_8UC3);
	}

	Scalar whiteColor(255,255,255);
	for (size_t i = 0; i< contours.size(); i++)
	{
		Scalar color(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
		drawContours(dst, contours, static_cast<int>(i), color, 2, 8, hierarchy, 0, Point());
		circle(dst, mc[i], 4, whiteColor, -1, 8, 0);
	}
}

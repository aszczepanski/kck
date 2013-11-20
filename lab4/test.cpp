#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>

using namespace cv;
using namespace std;

static Mat src;
static Mat src_gray;
static int thresh = 100;
static int max_thresh = 255;
static RNG rng(12345);
static int blurSize = 3;

static void thresh_callback(int, void*);

static cv::Mat mergePictures(std::vector<cv::Mat>& images, int cols, int min_gap_size = 0)
{
	// let's first find out the maximum dimensions
	int max_width = 0;
	int max_height = 0;
	for ( int i = 0; i < images.size(); i++) {
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
			cv::Mat to(result,
					cv::Range(current_height, current_height + images[i].rows),
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

static void prepareImageWithContours(const cv::Mat& src, cv::Mat& dst, int thresh, int blurSize, bool clearDst = true)
{
	/// Convert image to gray and blur it
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	blur(src_gray, src_gray, Size(blurSize,blurSize));

	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using canny
	//Canny(src_gray, canny_output, thresh, thresh*2, 3);
	//threshold(canny_output, canny_output, thresh, 255, THRESH_BINARY_INV);
	threshold(src_gray, canny_output, thresh, 255, THRESH_BINARY_INV);
	//Canny(canny_output, canny_output, thresh, thresh*2, 3);

	//namedWindow("xxx", WINDOW_AUTOSIZE);
	//imshow("xxx", canny_output);

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

int main(int, char* argv[])
{
	ifstream input;
	input.open("in1.in");

	if (input.good())
	{
		//cout << "opened" << endl;
		int n;
		input >> n;
		for (int i=0; i<n && !input.eof(); i++)
		{
			string line;
			getline(input, line);

			string imageName;
			int thresh, blurSize;
			input >> imageName >> thresh >> blurSize;

			//std::cout << imageName << " - " << thresh << " - " << blurSize << std::endl;

			/// Load source image
			src = imread(imageName, 1);

			pyrDown(src, src, Size(src.cols/2, src.rows/2));

			/// Create Window
			std::string source_window = "Picture";
			namedWindow(source_window, WINDOW_AUTOSIZE);
			//imshow(source_window, src);

			//createTrackbar( " Canny thresh:", "Source", &thresh, max_thresh, thresh_callback );
			//thresh_callback( 0, 0 );
			
			//vector<Mat> imgs(2);
			//imgs[0] = src.clone();
			//imgs[1] = src.clone();

			Mat dst = src.clone();
			prepareImageWithContours(src, dst, thresh, blurSize, false);

			//imgs[1] = dst.clone();

			//Mat ttt = mergePictures(imgs, 2, 2);
			//namedWindow("imgs", WINDOW_AUTOSIZE);
			//imshow("imgs", ttt);

			imshow(source_window, dst);

			waitKey(0);
		}

		input.close();
	}

	return 0;
}

void thresh_callback(int, void* )
{
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using canny
	Canny(src_gray, canny_output, thresh, thresh*2, 3);
	//threshold(canny_output, canny_output, thresh, 255, THRESH_BINARY_INV);
	//threshold(src_gray, canny_output, thresh, 255, THRESH_BINARY_INV);
	Mat tmpp;
	cvtColor(canny_output, tmpp, COLOR_BGR2GRAY);
	blur(canny_output, tmpp, Size(3,3));
	thresh = 127;
	Canny(canny_output, canny_output, thresh, thresh*2, 3);

	namedWindow("xxx", WINDOW_AUTOSIZE);
	imshow("xxx", canny_output);

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

	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (size_t i = 0; i< contours.size(); i++)
	{
		Scalar color(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
		drawContours(drawing, contours, static_cast<int>(i), color, 2, 8, hierarchy, 0, Point());
		circle(drawing, mc[i], 4, color, -1, 8, 0);
	}

	/// Calculate the area with the moments 00 and compare with the result of the OpenCV function
	printf("\t Info: Area and Contour Length \n");
	for (size_t i = 0; i< contours.size(); i++)
	{
		printf(" * Contour[%d] - Area (M_00) = %.2f - Area OpenCV: %.2f - Length: %.2f \n", static_cast<int>(i), mu[i].m00, contourArea(contours[i]), arcLength( contours[i], true ) );
		Scalar color(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
		drawContours(src, contours, i, color, 2, 8, hierarchy, 0, Point());
		circle(src, mc[i], 4, color, -1, 8, 0);
	}

	/// Show in a window
	namedWindow( "Contours", WINDOW_AUTOSIZE );
	//imshow( "Contours", drawing );
	imshow("Contours", src);

}

#include "image_processing.h"
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

int main(int, char* argv[])
{
	ifstream input;
	input.open("in1.in");

	if (input.good())
	{
		int n;
		input >> n;
		for (int i=0; i<n && !input.eof(); i++)
		{
			string imageName;
			input >> imageName;

			/// Load source image
			src = imread(imageName, 1);

			pyrDown(src, src, Size(src.cols/2, src.rows/2));

			//vector<Mat> imgs(2);
			//imgs[0] = src.clone();
			//imgs[1] = src.clone();

			Mat dst = src.clone();
			prepareImageWithContours(src, dst, false);

			//imgs[1] = dst.clone();

/*
			/// Create Window
			string source_window = "Picture";
			namedWindow(source_window, WINDOW_AUTOSIZE);
			imshow(source_window, dst);

			waitKey(0);
*/
			/// save pictures
			imwrite("xxx-" + imageName, dst);
		}

		input.close();
	}

	return 0;
}


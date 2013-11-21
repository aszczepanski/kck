#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <vector>

cv::Mat mergePictures(std::vector<cv::Mat>& images, int cols, int min_gap_size = 0);

void prepareImageWithContours(const cv::Mat& src, cv::Mat& dst, bool clearDst = true);

#endif // IMAGE_PROCESSING_H

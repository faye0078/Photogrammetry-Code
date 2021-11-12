#include "match.h"

void related_match(const Mat& right_img, const Mat& left_img, const int& window_size, const double& threshold,
	               const vector<Point>& right_points, const vector<Point>& left_points, vector<MatchPoint>& match_points){
	match_points.clear();

	Mat right_img_1, left_img_1;
	if (right_img.channels() != 1) {
		cvtColor(right_img, right_img_1, COLOR_BGR2GRAY);
	}
	else{
		right_img_1 = right_img.clone();
	}

	if (left_img.channels() != 1) {
		cvtColor(left_img, left_img_1, COLOR_BGR2GRAY);
	}
	else {
		left_img_1 = left_img.clone();
	}

	double r = window_size / 2;
	int number = 0;

	for (Point right_point : right_points) {
		number += 1;
		if (isTruePoint(right_img_1, right_point, window_size)) {

			Rect right_rect, left_rect;
			Mat right_window, left_window;
			right_rect = Rect(right_point.x - r, right_point.y - r, window_size, window_size);
			right_window = right_img_1(right_rect);

			double idex = 0.0, max_idex = 0.0;
			Point max_point;
			for (Point left_point : left_points) {
				if (isTruePoint(left_img_1, left_point, window_size)) {
					left_rect = Rect(left_point.x - r, left_point.y - r, window_size, window_size);
					left_window = left_img_1(left_rect);
					idex = computeCorrelationIdx(right_window, left_window);
					if (idex > max_idex)
					{
						max_idex = idex;
						max_point = left_point;
					}
				}
			}

			if (max_idex > threshold && fabs(right_point.y - max_point.y) < 50) {
				MatchPoint mpt;
				mpt.right_point = right_point;
				mpt.left_point = max_point;
				mpt.dist = max_idex;
				match_points.push_back(mpt);
			}
		}
	}

}

bool isTruePoint(const Mat& img, const Point& point, const int& window_size)
{
	double r = window_size / 2;
	if (point.x < r || point.y < r || point.x >= img.cols - r || point.y >= img.rows - r)
		return false;
	return true;
}

double computeCorrelationIdx(const Mat& right_window, const Mat& left_window){
	if (right_window.size != left_window.size) {
		cerr << "窗口大小不一致！" << endl;
		return 0;
	}

	double total_pixel = right_window.rows * left_window.cols;

	double right_avg = 0.0, left_avg = 0.0;
	for (int i = 0; i < right_window.rows; ++i) {
		for (int j = 0; j < right_window.cols; ++j) {
			right_avg += right_window.at<uchar>(i, j);
			left_avg += left_window.at<uchar>(i, j);
		}
		right_avg /= total_pixel;
		left_avg /= total_pixel;
	}

	double numerator = 0.0;
	double denominator_right = 0.0;
	double denominator_left = 0.0;

	for (int i = 0; i < right_window.rows; i++)
		for (int j = 0; j < right_window.cols; j++)
		{
			numerator += (right_window.at<uchar>(i, j) - right_avg) * (left_window.at<uchar>(i, j) - left_avg);
			denominator_right += pow((right_window.at<uchar>(i, j) - right_avg), 2);
			denominator_left += pow((left_window.at<uchar>(i, j) - left_avg), 2);
		}

	double denominator = sqrt(denominator_right * denominator_left);
	return abs(numerator / denominator);


}

void drawMatches(const Mat& right_img, const Mat& left_img, Mat& outputImg, vector<MatchPoint>& matches){
	outputImg.create(Size(right_img.cols + left_img.cols, max(right_img.rows, left_img.rows)), CV_8UC3);

	left_img.copyTo(outputImg(cv::Rect(0, 0, left_img.cols, left_img.rows)));
	right_img.copyTo(outputImg(cv::Rect(left_img.cols, 0, right_img.cols, right_img.rows)));

	cv::Point pt1, pt2;

	for (MatchPoint match : matches)
	{
		cv::Scalar color(192, 23, 67);

		pt1 = match.left_point;
		pt2 = cv::Point(match.right_point.x + left_img.cols, match.right_point.y);

		cv::circle(outputImg, pt1, 5, color, 2);
		cv::circle(outputImg, pt2, 5, color, 2);
		cv::line(outputImg, pt1, pt2, color, 2);
	}
}

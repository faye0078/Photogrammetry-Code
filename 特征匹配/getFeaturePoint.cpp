#include "getFeaturePoint.h"
#include<math.h>

vector<Point> get_moravec(const int& moravec_thre, const int& moravec_size, const Mat& img) {

	vector<Point> points;

    int moravec_size_1 = moravec_size;
	if (moravec_size_1 % 2 == 0) {
		moravec_size_1 += 1;
	}

	Mat img_0 = img.clone();

	if (img_0.channels() != 1) {
		cvtColor(img_0, img_0, COLOR_RGB2GRAY);
	}

	//GaussianBlur(img_0, img_0, Size(moravec_size, moravec_size), 0, 0);

	Mat value_img = Mat::zeros(img_0.size(), CV_32FC1);

	int s = moravec_size_1 / 2;

	for (int i = s; i < img_0.rows - s; ++i) {
		for (int j = s; j < img_0.cols - s; ++j) {
			double value[4] = { 0.0 };
			double min_value = 0.0;

			for (int k = -s; k < s; k++) {
				value[0] += pow(img_0.at<uchar>(i + k, j) - img_0.at<uchar>(i + k + 1, j), 2);
				value[1] += pow(img_0.at<uchar>(i, j + k) - img_0.at<uchar>(i, j + k + 1), 2);
				value[2] += pow(img_0.at<uchar>(i + k, j + k) - img_0.at<uchar>(i + k + 1, j + k + 1), 2);
				value[3] += pow(img_0.at<uchar>(i + k, j - k) - img_0.at<uchar>(i + k + 1, j - k - 1), 2);
			}

			min_value = min(min(value[0], value[1]), min(value[2], value[3]));
			value_img.at<float>(i, j) = min_value;
		}
	}

    int max_value;
    Point point;
    s = 50;
    for (int i = s; i < img.rows - s;)
    {
        for (int j = s; j < img.cols - s;)
        {
            point.x = -1;
            point.y = -1;
            max_value = 0;
            for (int m = -s; m < s; m++)
            {
                for (int n = -s; n < s; n++)
                {
                    if (value_img.at<float>(i + m, j + n) > max_value)
                    {
                        max_value = value_img.at<float>(i + m, j + n);
                        point.y = i + m;
                        point.x = j + n;
                    }
                }
            }
            if (max_value > moravec_thre)
            {
                points.push_back(point);
            }
            j += 101;
        }
        i += 101;
    }

    return points;

}

void draw_points(const cv::Mat& srcImg, cv::Mat& outputImg, std::vector<cv::Point> corners)
{
    outputImg = srcImg.clone();

    for (cv::Point corner : corners)
    {
        //cv::Scalar color(u(e), u(e), u(e));
        cv::Scalar color(121, 32, 184);

        cv::circle(outputImg, corner, 5, color, 2);
    }
}
#pragma once
#include<opencv2/opencv.hpp>
#include<math.h>
using namespace std;
using namespace cv;

struct MatchPoint
{
    cv::Point2d right_point;
    cv::Point2d left_point; 
    double dist;       
};

void related_match(const Mat& right_img, const Mat& left_img, const int& window_size, const double& threshold,
    const vector<Point>& right_points, const vector<Point>& left_points,
    vector<MatchPoint>& match_points);

// 判断点能不能计算相关系数
bool isTruePoint(const Mat& img, const Point& point, const int& window_size);

// 计算相关系数
double computeCorrelationIdx(const Mat& right_window, const Mat& left_window);

// 绘制匹配后的图像
void drawMatches(const Mat& right_img, const Mat& left_img, Mat& outputImg, vector<MatchPoint>& matches);

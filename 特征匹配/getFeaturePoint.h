#pragma once
#include<opencv2/opencv.hpp>
#include<vector>
#include<string>

using namespace std;
using namespace cv;

vector<Point> get_moravec(const int& moravec_thre, const int& moravec_size, const Mat& img);

// 绘制含特征点的图像
void draw_points(const Mat& srcImg, Mat& outputImg, vector<Point> corners);
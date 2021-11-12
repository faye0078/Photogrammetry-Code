#include<iostream>
#include<string>
#include<vector>

#include <opencv2/opencv.hpp>

#include"getFeaturePoint.h"
#include"match.h"

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {

    const char* rimagename = "D:/大三上/摄影测量学/作业二/编程练习/u0367_panRight.bmp";
    const char* limagename = "D:/大三上/摄影测量学/作业二/编程练习/u0369_panLeft.bmp";

    //从文件中读入图像
    Mat right_img = imread(rimagename, 1);
    Mat left_img = imread(limagename, 1);

    //如果读入图像失败
    if (right_img.empty()||left_img.empty()) {
        fprintf(stderr, "Can not load image successfully");
        return -1;
    }
   
    //imshow("right_image", right_img);
    //imshow("left_img", left_img);

    vector<Point> right_points, left_points;

    // Moravec角点阈值,窗口大小
    int moravec_thre = 1000;
    int moravec_size = 5;

    // 计算角点
    right_points = get_moravec(moravec_thre, moravec_size, right_img);
    left_points = get_moravec(moravec_thre, moravec_size, left_img);

    // 绘制角点
    cv::Mat img_1_points, img_2_points;
    draw_points(right_img, img_1_points, right_points);
    draw_points(left_img, img_2_points, left_points);

    // 保存并显示
    const char* img_1_points_path = "D:/大三上/摄影测量学/作业二/编程练习/img_right_points.bmp";
    const char* img_2_points_path = "D:/大三上/摄影测量学/作业二/编程练习/img_left_points.bmp";
    cv::imwrite(img_1_points_path, img_1_points);
    cv::imwrite(img_2_points_path, img_2_points);
    //cv::imshow("img_right 角点检测后图像", img_1_points);
    //cv::imshow("img_left 角点检测后图像", img_2_points);
    //cv::waitKey(0);

    // 相关系数匹配
    int corr_window_size = 25;
    double corr_threshold = 0.90;

    vector<MatchPoint> match_points;
    related_match(right_img, left_img, corr_window_size, corr_threshold, right_points, left_points, match_points);

    // 显示并保存
    Mat img_match;
    const char* img_match_path = "D:/大三上/摄影测量学/作业二/编程练习/img_match.bmp";
    drawMatches(right_img, left_img, img_match, match_points);
    imwrite(img_match_path, img_match);
    imshow("匹配结果：", img_match);
    waitKey(0);
    return 0;
}
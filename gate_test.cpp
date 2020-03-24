#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <utility>
#include <iostream>

int main(int argc, char** argv)
{
    cv::Mat img;
    std::vector<std::vector<cv::Point>> contours, good_contours_;
    std::vector<cv::Vec4i> hierarchy;

    //Thresholding
    cv::GaussianBlur( img, img, cv::Size(3,3), 0, 0 );
    cv::cvtColor( img, img, CV_BGR2HSV);
    cv::inRange( img, cv::Scalar(100,110,110), cv::Scalar(179,255,255),img);
    //cv::inRange(img, cv::Scalar(0,0,0), cv::Scalar(30,30,30), img);
    cv::dilate( img, img, cv::Mat(), cv::Point(-1,-1), 2, 1, 1);
    cv::erode( img ,img, cv::Mat(), cv::Point(-1,-1), 2, 1, 1);


    //find Good contours
    cv::Canny(img , img, 200, 300 , 3);
    cv::findContours(img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
        cv::imshow("canny", img);
        cv::waitKey(0);
        cv::destroyWindow("canny");
    
    std::vector<std::vector<cv::Point>>::iterator ptr = contours.begin();
    std::vector<std::vector<cv::Point>>::iterator end = contours.end();
    std::cout<<contours.size()<<std::endl;

    for(ptr;ptr!=end;++ptr)
    {
        if(cv::contourArea(*ptr)>250){
            good_contours_.push_back(*ptr);
        }
    } 
    cv::Mat test = cv::Mat::zeros(img.size(), CV_8UC3);
    int size = good_contours_.size();
    int i=0;
    for(i;i<size;i+=1){
        cv::drawContours(test,good_contours_, i,cv::Scalar(255,0,0) );
    }
    cv::imshow("contour",test);
    cv::waitKey(0);
    cv::destroyWindow("contour");

    //Frame Frame Centre

    int size, length, x, y, k;
    std::vector<cv::Point> approx;
    std::cout<<good_contours_.size()<<std::endl;
    for( i=0; i<good_contours_.size(); i+=1)
    {
        cv::approxPolyDP(good_contours_[i],approx, 0.02*cv::arcLength(good_contours_[i], true), true);
        size = approx.size();
        std::cout<<size<<std::endl;
        length =0;
        if(1)
        {
            x=0;
            y=0;
            std::cout<<"good"<<i<<std::endl;
            for(k=0;k<size;k+=1)
            {
                std::cout<<"Inside good"<<i<<std::endl;
                x = x + approx[k].x;
                y = y + approx[k].y;
                cv::circle(img,cv::Point(approx[k].x, approx[k].y),4,cv::Scalar(255, 255, 255),-1);
                cv::putText(img,std::to_string(i),approx[k], CV_FONT_HERSHEY_PLAIN,4,cv::Scalar(255,255,255));
            }
            x = x/size;
            y = y/size;
            length = abs(approx[0].x - approx[1].x);
            std::cout<<i<<" "<<x<<","<<y<<std::endl;
        }
    }
    return 0;
}
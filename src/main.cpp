#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>

using namespace cv;

int main(){
    VideoCapture cam;
    for (size_t i = 0; i < 9; i++)
    {
        cam.open(i);
        if (cam.isOpened())
        {
            std::cout<<"opening cam detected at "<<i<<"\n";
            break;
        }
        else if (!cam.isOpened() && i==8)
        {
            std::cout<<"no working cams detected";
            return 1;
        }
        cam.release();
    }
    Mat img,imghsv;
    int width=static_cast<int>(cam.get(CAP_PROP_FRAME_WIDTH)),height=static_cast<int>(cam.get(CAP_PROP_FRAME_HEIGHT));
    int sum=0,columnRatio=9,rowRatio=16;
    std::cout<<"\033[2J\033[3J\033[H"<<std::flush;
    std::cout << "\033[?25l";
    std::string outstr;
    while (true)
    {
        cam.read(img);
        int key = waitKey(10);
        if (key == 27 || key == 'q'){
            std::cout << "\033[?25h";
            break;
        }
        if (img.empty())
        {
            continue;
        }
        cvtColor(img,imghsv,COLOR_BGR2HSV);
        outstr="";
        for (size_t i = 0; i < height; i+=rowRatio)
        {
            for (size_t j = 0; j < width; j+=columnRatio)
            {
                sum=0;
                size_t jlimit= (width-j < columnRatio)? width-j: columnRatio;
                size_t ilimit= (height-i < rowRatio)? height-i: rowRatio;
                for (size_t k = 0; k < ilimit; k++)
                {
                    for (size_t l = 0; l < jlimit; l++)
                    {
                        sum+=imghsv.at<Vec3b>(k+i,l+j)[1]+(int)(0.05*imghsv.at<Vec3b>(k+i,l+j)[0]);
                    }
                }
                int avg=sum/(jlimit*ilimit);
                if (avg>128)
                {
                    outstr+="M";
                }
                else if (avg>100 && avg<=128)
                {
                    outstr+="#";
                }
                else if (avg>50 && avg<=100)
                {
                    outstr+="x";
                }
                else if (avg>30 && avg<=50)
                {
                    outstr+="*";
                }
                else
                {
                    outstr+=" ";
                }
            }
            outstr+="\n";
        }
        std::cout<<outstr;
        std::cout<<"\033[H";
        imshow("webcam", img);
        if (getWindowProperty("webcam", WND_PROP_VISIBLE) < 1){
            std::cout << "\033[?25h";
            break;
        }
    }
    return 0;
}
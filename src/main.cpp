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
    int sum=0,columnRatio=5,rowRatio=10,option;
    std::cout<<"Tile sizes available:\n1)9x16\n2)5x10\n3)3x6\nEnter tile size you want:";
    std::cin>>option;
    switch (option)
    {
    case 1:
        columnRatio=9;rowRatio=16;
        break;
    case 2:
        columnRatio=5;rowRatio=10;
        break;
    case 3:
        columnRatio=3;rowRatio=6;
        break;
    default:
        columnRatio=9;rowRatio=16;
        break;
    }
    std::cout<<"\033[2J\033[3J\033[H"<<std::flush;
    std::cout << "\033[?25l";
    std::string outstr;
    float hueweight=1,satweight=0.1;
    int boundaryParam=0,prevBoundaryParam=0;
    bool autoAdjust=true;
    while (true)
    {
        cam.read(img);
        int key = waitKey(10);
        int  maskedKey= key & 0xFF;
        if (key == 27 || maskedKey== 'q'){
            std::cout << "\033[?25h";
            break;
        }
        switch (maskedKey)
        {
        case 'y':
            hueweight+=0.05;
            break;
        case 'h':
            hueweight=(hueweight-0.05>0) ? hueweight-0.05 : hueweight;
            break;
        case 'a':
            autoAdjust=(autoAdjust) ? false : true;
            break;
        case 'u':
            autoAdjust=false;
            satweight += 0.01;
            break;
        case 'j':
            autoAdjust=false;
            satweight = (satweight - 0.01 > 0.01) ? satweight - 0.01 : satweight;
            break;
        case 'r':
            hueweight=1; satweight=0.1;
            break;
        }
        if (img.empty())
        {
            continue;
        }
        int Param=(int)((satweight*255+hueweight*179*1.422)/((satweight+hueweight)*(2)*6));
        cvtColor(img,imghsv,COLOR_BGR2HSV);
        outstr="";
        boundaryParam=0;
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
                        Vec3b pxl =imghsv.at<Vec3b>(k+i,l+j);
                        sum+=(int)(((satweight*pxl[1])+((hueweight*pxl[0]*(1.422))))/(hueweight+satweight));
                    }
                }
                int avg=sum/(jlimit*ilimit);
                if (avg>6*Param)
                {
                    if (!outstr.empty() && outstr.back()!='M' && outstr.back()!='\n')
                    {
                        boundaryParam++;
                    }
                    if (!outstr.empty() && outstr.back()!='M' && outstr.back()==' ')
                    {
                        boundaryParam++;
                    }
                    outstr+="M";
                }
                else if (avg>5*Param && avg<=6*Param)
                {
                    if (!outstr.empty() && outstr.back()!='#' && outstr.back()!='\n')
                    {
                        boundaryParam++;
                    }
                    if (!outstr.empty() && outstr.back()!='#' && outstr.back()==' ')
                    {
                        boundaryParam++;
                    }
                    outstr+="#";
                }
                else if (avg>4*Param && avg<=5*Param)
                {
                    if (!outstr.empty() && outstr.back()!='@' && outstr.back()!='\n')
                    {
                        boundaryParam++;
                    }
                    if (!outstr.empty() && outstr.back()!='@' && outstr.back()==' ')
                    {
                        boundaryParam++;
                    }
                    outstr+="@";
                }
                else if (avg>3*Param && avg<=4*Param)
                {
                    if (!outstr.empty() && outstr.back()!='x' && outstr.back()!='\n')
                    {
                        boundaryParam++;
                    }
                    if (!outstr.empty() && outstr.back()!='x' && outstr.back()==' ')
                    {
                        boundaryParam++;
                    }
                    outstr+="x";
                }
                else if (avg>2*Param && avg<=3*Param)
                {
                    if (!outstr.empty() && outstr.back()!='*' && outstr.back()!='\n')
                    {
                        boundaryParam++;
                    }
                    if (!outstr.empty() && outstr.back()!='*' && outstr.back()==' ')
                    {
                        boundaryParam++;
                    }
                    outstr+="*";
                }
                else if (avg>Param && avg<=2*Param)
                {
                    if (!outstr.empty() && outstr.back()!='-' && outstr.back()!='\n')
                    {
                        boundaryParam++;
                    }
                    if (!outstr.empty() && outstr.back()!='-' && outstr.back()==' ')
                    {
                        boundaryParam++;
                    }
                    outstr+="-";
                }
                else
                {
                    if (!outstr.empty() && outstr.back()!=' ' && outstr.back()!='\n')
                    {
                        boundaryParam+=2;
                    }
                    outstr+=" ";
                }
            }
            outstr+="\n";
        }
        std::cout<<outstr;
        std::cout<<"\nhue weighting:"<<hueweight<<"\033[K"<<" \nsaturation weighting: "<<satweight<<"\033[K";
        std::cout<<"\ny/h hue  u/j sat  a auto  q/esc quit r reset hue and saturation"<<"\033[K"<<" autoAdjust:"<<(autoAdjust?"on":"off")<<"\033[K";
        std::cout<<"\033[H";
        if (boundaryParam-50>prevBoundaryParam && prevBoundaryParam!=0 && autoAdjust)
        {
            satweight+=0.01*(boundaryParam-prevBoundaryParam)*(boundaryParam-prevBoundaryParam);
        }
        else if(prevBoundaryParam-50>boundaryParam && prevBoundaryParam!=0 && autoAdjust && satweight>0)
        {
            satweight=(satweight-0.01*(boundaryParam-prevBoundaryParam)*(boundaryParam-prevBoundaryParam)>0) ? satweight-0.01*(boundaryParam-prevBoundaryParam)*(boundaryParam-prevBoundaryParam) : satweight;
        }
        prevBoundaryParam=boundaryParam;
        imshow("webcam", img);
        if (getWindowProperty("webcam", WND_PROP_VISIBLE) < 1){
            std::cout << "\033[?25h";
            break;
        }
    }
    return 0;
}
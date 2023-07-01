
#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "face.h"

using namespace std;
using namespace cv;
using namespace aip;

int main()
{
    VideoCapture cap;
    cap.open(0);
    if (!cap.isOpened()) {
        cout << "ERROR! Unable to open camera"<<endl;
        return 0;
    }
    cout << "Camera open success!"<<endl;

    CascadeClassifier Classfiler("/usr/share/opencv/haarcascades/haarcascade_frontalface_alt2.xml");	//实例化级联分类器类加载人脸模型

    // 设置APPID/AK/SK
    std::string app_id = "35147801";
    std::string api_key = "c309qPjzCvfbH35F3x2k6udz";
    std::string secret_key = "8Voqc2XSFieMBS5dgt9PCmBsQwISX6bI";

    aip::Face client(app_id, api_key, secret_key);	//新建一个client对象

    Mat Colorframe;	//实例化mat对象，mat是用于存储图像类
    Mat Processframe;
    Mat MatFace;
    vector<Rect> Allface;
    vector<uchar> JpgFace;
    string base64face;
    Json::Value result;
    Json::Value options;
    time_t sec;

    cout << "Start grabbing" << endl;
    for (;;)
    {
   	cap.read(Colorframe);	//摄像头照相并且存放到frame中
        if (Colorframe.empty()) {
    	 cout << "ERROR! blank frame grabbed"<<endl;
            break;
        }
	cvtColor(Colorframe,Processframe,COLOR_BGR2GRAY);	//彩色转换成灰度图
	equalizeHist(Processframe,Processframe);	//直方图均匀化，储存在Processframe对象中以节省内存
	Classfiler.detectMultiScale(Processframe,Allface);	//检测人脸并用矩形框起来
	if(Allface.size())
	{
	    rectangle(Processframe,Allface[0],Scalar(255,255,0));	//画出人脸框
	    rectangle(Colorframe,Allface[0],Scalar(255,255,0));
	    MatFace = Processframe(Allface[0]);	//截取人脸
	    imencode(".jpg",MatFace,JpgFace);	//转变为jpg格式并存在JpgFace容器中

	    base64face = base64_encode((char *)JpgFace.data(), JpgFace.size());	//jpg转成base
	    result = client.face_search_v3(base64face,"BASE64","testing",options);	// 调用人脸搜索

	   if(!result["result"].isNull())
	    {
		if(result["result"]["user_list"][0]["score"].asInt() > 80)
		{
		    cout << result["result"]["user_list"][0]["user_id"] << endl;
		    sec = time(NULL);
		    cout << ctime(&sec) << endl;
		    putText(Colorframe,result["result"]["user_list"][0]["user_id"].asString(),Point(30,50),FONT_HERSHEY_SIMPLEX,1,Scalar(255,255,0));
		    putText(Colorframe,ctime(&sec),Point(0,100),FONT_HERSHEY_SIMPLEX,1,Scalar(255,255,0));
		}
	    }
	}
 	imshow("video", Colorframe);	//打开video名的窗口来显示frame帧
        waitKey(40);
    }
    return 0;
}

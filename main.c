//
//  main.c
//  OpencvTest
//
//  Created by takeda on 12/01/08.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include <opencv/cv.h>
#include <opencv/highgui.h>

IplImage* doCvGray(IplImage* in);
IplImage* doCanny(IplImage* in, double lowThresh, double highThresh, double aperture);
void init_edge_threshold(int* heght, int* low);

int main (int argc, const char * argv[])
{
    IplImage    *frame = 0;             // 動画の1
    IplImage    *outGray = 0;
    IplImage    *outEdge = 0;
    int         inputChar = 0;
    int         heightEdge = 0;
    int         lowEdge = 0;
    
    cvNamedWindow ("Capture", CV_WINDOW_AUTOSIZE);
    cvNamedWindow ("Edge", CV_WINDOW_AUTOSIZE);
    
    // カメラから画像をキャプチャ
    CvCapture *capture = cvCreateCameraCapture(0);
    assert(capture != NULL);
    
    // Edge閾値初期化
    init_edge_threshold(&heightEdge, &lowEdge);    
    
    // ESCが押されるで続ける
    while (1) {
        // カメラやファイルから一つのフレームを取り出す
        frame = cvQueryFrame (capture);
        
        // グレーに色空間の変換
        outGray = doCvGray(frame);
        
        //　グレーに変換したものをエッジ検出
        outEdge = doCanny(outGray, lowEdge, heightEdge, 3);
        
        // 画像の表示
        cvShowImage("Capture", frame);
        cvShowImage("Gray", outGray);
        cvShowImage("Edge", outEdge);
        
         
        // キーイベントを待つ(2ms)
        inputChar = cvWaitKey (2);
        // 「esc」：終了するためループから抜ける
        if( inputChar == '\x1b'){
            break;
        }
        
        
        switch (inputChar) {
            //「+」:ボタンを押したらエッジ強調を強めにする
            case 43:
                heightEdge++;
                break;

            //「-」:ボタンを押したらエッジ強調を弱めにする
            case 45:
                heightEdge--;
                break;
 
            //「c」:Edgeの初期化
            case 99:
                init_edge_threshold(&heightEdge, &lowEdge);
                break;

            //「s」or「S」：画像の保存
            case 83:
            case 115:
                // 画像の保存場所の指定
                cvSaveImage("/Users/tkd55/Desktop/xxxx.jpg", outEdge, 0);
                break;
                
            default:
                break;
        }
        //　画像データの解放
        cvReleaseImage(&outGray);
    }
    
    // CvCapture 構造体を解放
    cvReleaseCapture (&capture);
    cvDestroyWindow ("Capture");
    cvDestroyWindow ("Edge");
    
    return 0;
}

IplImage* doCvGray(IplImage* colorImage){
    IplImage* grayImage;
    
    // 画像領域のメモリ確保
    grayImage = cvCreateImage(cvGetSize(colorImage), IPL_DEPTH_8U, 1);
    
    // 画像の色空間の変換（カラー　→　グレー）
    cvCvtColor(colorImage, grayImage, CV_BGR2GRAY);
    
    return grayImage;
};

IplImage* doCanny(IplImage* grayImage, double lowThresh, double highThresh, double aperture){
    IplImage* edgeImage;

    // 画像領域のメモリ確保
    edgeImage = cvCreateImage(cvGetSize(grayImage), IPL_DEPTH_8U, 1);
    
    // エッジ検出
    cvCanny(grayImage, edgeImage, lowThresh, highThresh, aperture);
    return edgeImage;
};

void init_edge_threshold(int* heightEdge, int* lowEdge)
{
    *heightEdge = 100;
    *lowEdge = 10;
}
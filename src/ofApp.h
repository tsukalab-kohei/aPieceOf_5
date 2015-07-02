#pragma once

#include "ofMain.h"
#include "ofxOpenCV.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include <gl/ofGLRenderer.cpp>
#include "ofxCv.h"
#include "Utilities.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    //custom
    void showEarth();
    void matching();
    void showMatchImage();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    int cameraSize_w;
    int cameraSize_h;
    int cameraSmallSize_w;
    int cameraSmallSize_h;
    int riversImgSize_w;
    int riversImgSize_h;
    int riversSmallSize_w;
    int riversSmallSize_h;
    
    int threshold;
    
    int count;
    int dx, dy;
    
    double minVal;
    double maxVal;
    double mostmaxVal; //6つの中の最大類似度
    int mostmaxIndex; //6つの中の最大類似度を持つ要素
    
    float cut_x;
    float cut_y;
    float cut_w;
    float cut_h;
    
    ofRectangle cutFrame;
    
    string areaName[6] = {"South America", "Europe", "Africa", "Australia", "North America", "Asia"};
    
    ofSoundPlayer captureSound;
    ofVideoGrabber videoGrabber;
    
    ofImage capImage;
    ofImage capBg;
    ofImage riversImage[6];
    ofImage resultImage[6];
    
    ofxCvColorImage colorBg;
    ofxCvColorImage videoImage;
    
    ofxCvColorImage colorRivers[6];
    ofxCvGrayscaleImage grayImage;
    ofxCvGrayscaleImage grayRivers[6];
    ofxCvGrayscaleImage grayRivers_cut; //for test
    ofxCvGrayscaleImage grayBg;
    ofxCvGrayscaleImage grayDiff;
    ofxCvGrayscaleImage edgeImage;
    ofxCvGrayscaleImage edgeRivers[6];
    ofxCvGrayscaleImage edgeRivers_cut; // for test
    ofxCvGrayscaleImage capturedEdge;
    ofxCvGrayscaleImage showRiver;
    ofxCvGrayscaleImage ofImageEdge;
    
    //globe
    ofImage earth;
    ofImage earth2;
    GLUquadricObj *quadric;
    
    ofxCvContourFinder contourFinder;
    
    cv::Mat matImg;
    cv::Mat matImg2;
    cv::Mat cut_img;
//    cv::Mat result[6];
//    cv::Mat result;
    IplImage *result;
    
    cv::Ptr<cv::FeatureDetector> detector1;
    cv::Ptr<cv::FeatureDetector> detector2;
    
    cv::Ptr<cv::DescriptorExtractor> extractor1;
    cv::Ptr<cv::DescriptorExtractor> extractor2;
    
    std::vector<cv::KeyPoint> keypoints1;
    std::vector<cv::KeyPoint> keypoints2;
    
    cv::Ptr<cv::DescriptorMatcher> matcher;
    std::vector<cv::DMatch> dmatch;
    
    bool flagDrawimage;
    bool isMatchCompleted;
};
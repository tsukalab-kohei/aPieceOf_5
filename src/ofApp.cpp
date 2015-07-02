#include "ofApp.h"
//#include "ofxCv.h"
//#include "Utilities.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //音声ファイル
    captureSound.loadSound("capture.wav");
    
    flagDrawimage = false;
    isMatchCompleted = false;
    
    threshold = 24;
    
    count = 0;
    dx = 0;
    dy = 0;

    cameraSize_w = 400;
    cameraSize_h = 300;
    cameraSmallSize_w = cameraSize_w;
    cameraSmallSize_h = cameraSize_h;
    riversImgSize_w = 1920;
    riversImgSize_h = 1080;
    riversSmallSize_w = riversImgSize_w / 1;
    riversSmallSize_h = riversImgSize_h / 1;
    
    cut_x = 100;
    cut_y = 50;
    cut_w = 150;
    cut_h = 150;
    
    
    //領域確保
    videoImage.allocate(cameraSmallSize_w, cameraSmallSize_h);
    grayImage.allocate(cameraSmallSize_w, cameraSmallSize_h);
    edgeImage.allocate(cameraSmallSize_w, cameraSmallSize_h);
    grayDiff.allocate(cameraSmallSize_w, cameraSmallSize_h);
    
    for(int i=0; i<6; i++){
        colorRivers[i].allocate(riversSmallSize_w, riversSmallSize_h);
        grayRivers[i].allocate(riversImgSize_w, riversImgSize_h);
        edgeRivers[i].allocate(riversSmallSize_w, riversSmallSize_h);
    }
    capturedEdge.allocate(cameraSmallSize_w, cameraSmallSize_h);
    showRiver.allocate(riversImgSize_w, riversImgSize_h);
    
    //video
    videoGrabber.initGrabber(cameraSize_w, cameraSize_h);
    
    //河川画像, エッジ検出
    for (int i=0; i<6; i++) {
        string strNum = ofToString(i);
        string loadStr = "./rivers/rivers";
        string loadStr2 = ".png";
        loadStr += strNum;
        loadStr += loadStr2;
//        ofLog(OF_LOG_NOTICE, loadStr);
        
        riversImage[i].loadImage(loadStr);
        riversImage[i].resize(riversSmallSize_w, riversSmallSize_h);
        riversImage[i].setImageType(OF_IMAGE_COLOR);
        colorRivers[i].setFromPixels(riversImage[i].getPixels(), riversSmallSize_w, riversSmallSize_h);
        
        riversImage[i].setImageType(OF_IMAGE_GRAYSCALE);
        grayRivers[i].setFromPixels(riversImage[i].getPixels(), riversSmallSize_w, riversSmallSize_h);
        
        cvCanny(grayRivers[i].getCvImage(), edgeRivers[i].getCvImage(), 30, 70);
    }
    
    //背景との差分
    //    grayDiff.absDiff(grayBg, grayImage);
    //    grayDiff.threshold(threshold);
    
    //エッジ
    //    cvCanny(grayImage.getCvImage(), edgeImage.getCvImage(), 30, 70);
    
    //特徴点検出器
    //    detector1 = cv::FeatureDetector::create("SURF");
    //    detector2 = cv::FeatureDetector::create("SURF");
    
    //特徴量検出器
    //    extractor1 = cv::DescriptorExtractor::create("SURF");
    //    extractor2 = cv::DescriptorExtractor::create("SURF");
    
    //マッチング
    //    matcher = cv::DescriptorMatcher::create("BruteForce");
    
    //テンプレートマッチング
    //    for(int i=0; i<6; i++){
    //        tmach[i] = *cvCreateImage(cvSize(riversSmallSize_w, riversSmallSize_h), 32, 1);
    //    }
    
    //地球
    ofSetVerticalSync(true);
    
    glEnable(GL_DEPTH_TEST); //enable depth comparisons and update the depth buffer
    ofDisableArbTex(); //needed for textures to work with gluSphere
    //    earth.loadImage("palm.jpg");
    earth.loadImage("earth5.jpg");
    
    //prepare quadric for sphere
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    gluQuadricNormals(quadric, GLU_SMOOTH);
}

//--------------------------------------------------------------
void ofApp::update(){
    //video
    videoGrabber.update();
    
    if(videoGrabber.isFrameNew()){
//        cv::Mat frame(videoGrabber.getHeight(), videoGrabber.getWidth(), CV_8UC3, videoGrabber.getPixels());
//        cvtColor(frame, frame, CV_RGB2BGR);
        
        videoImage.setFromPixels(videoGrabber.getPixels(), cameraSmallSize_w, cameraSmallSize_h);
        grayImage = videoImage;
        edgeImage = videoImage;
        //grayImage.mirror(false, true);
        //エッジ抽出
        cvSmooth(grayImage.getCvImage(), grayImage.getCvImage());
        cvCanny(grayImage.getCvImage(), edgeImage.getCvImage(), 15, 70);
    }
    
    //特徴点
    //    matImg = cv::Mat(edgeImage.getCvImage());
    //    matImg2 = cv::Mat(edgeRivers.getCvImage());
    //    detector1->detect(matImg, keypoints1);
    //    detector2->detect(matImg2, keypoints2);
    
    //特徴量
    //    cv::Mat descriptor1;
    //    extractor1->compute(matImg, keypoints1, descriptor1);
    //
    //    cv::Mat descriptor2;
    //    extractor2->compute(matImg2, keypoints2, descriptor2);
    
    //マッチング
    //    matcher->match(descriptor1, descriptor2, dmatch);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 0);
    
    ofSetColor(200, 10, 200);
    ofNoFill();
    ofRect(20+cut_x, 20+cameraSmallSize_h+100+cut_y, cut_w, cut_h);
    
    ofSetColor(0, 191, 255);
    edgeImage.draw(20, 20+cameraSmallSize_h+100);
    
    //背景との差分
    ofSetHexColor(0xffffff);
    
    //エッジ
    if(!flagDrawimage){
        
    }else{
        //blobs
//        for (int i = 0; i < contourFinder.nBlobs; i++) {
//            ofSetColor(255);
//            contourFinder.blobs[i].draw(400-dx, 100);
//        }
        
//        ofImageEdge.draw(400-dx, 100);
        grayRivers_cut.draw(400-dx, 100);
        
        ofSetHexColor(0xffffff);
        stringstream captureStr;
        captureStr << "Capturing is success." << endl;
        ofDrawBitmapString(captureStr.str(), 500, 50);
        
        count++;
        if(count > 40){
            
            //キャプチャ後のアニメーション
            if(dx < 380){
                dx = dx + 20;
                
            }else{
                if(count < 200){
                    if(count % 2 == 0){
                        ofSetHexColor(0xffffff);
                        stringstream captureStr;
                        captureStr << "Searching..." << endl;
                        ofDrawBitmapString(captureStr.str(), 500, 50);
                    }
                    showEarth();
                }else{
                    if(isMatchCompleted){
                        showMatchImage();
                        ofSetHexColor(0xffffff);
                        stringstream name;
                        name << "matched " << areaName[mostmaxIndex] << endl;
                        ofDrawBitmapString(name.str(), 450, 150);
                    }
                }
            }
        }else{
        }
    }
    
    //特徴点
    //掌画像
    for (vector<cv::KeyPoint>::iterator itk = keypoints1.begin(); itk != keypoints1.end(); itk++) {
        ofSetColor(31, 63, 255);
        ofCircle(20+cameraSmallSize_w + itk->pt.x, 20 + itk->pt.y, 2);
    }
    //河川画像
    for (vector<cv::KeyPoint>::iterator itk = keypoints2.begin(); itk != keypoints2.end(); itk++) {
        //        ofCircle(20+cameraSmallSize_w*2 + itk->pt.x, 20 + itk->pt.y, 2);
    }
}

void ofApp::showEarth(){
    //地球
    
    //change origin to center
    ofTranslate(ofGetWidth()/2 + 200, ofGetHeight()/2, 0);
    
    //rotate sphere over time
    ofRotateY(ofGetFrameNum());
    ofRotateX(-90); //north pole facing up
    
    //bind and draw texture
    earth.getTextureReference().bind();
    gluSphere(quadric, 150, 200, 200);
}

void ofApp::matching(){
    
    //画像切り出し
    ofImageEdge = capturedEdge;
    capturedEdge.setROI(cut_x, cut_y, cut_w, cut_h);
    ofImageEdge.setFromPixels(capturedEdge.getRoiPixelsRef());
    
    cutFrame.x = grayRivers[0].width/2;
    cutFrame.y = grayRivers[0].height/2;
    cutFrame.width = 100;
    cutFrame.height = 100;
    grayRivers_cut.allocate(cutFrame.width, cutFrame.height);
    grayRivers[0].setROI(cutFrame.x, cutFrame.y, cutFrame.width, cutFrame.height);
    grayRivers_cut = grayRivers[0];
    grayRivers[0].resetROI();
    

    //blobs
    contourFinder.findContours(ofImageEdge, 20, (cut_w*cut_h), 10, true);
    
    for (int i=0; i<1; i++) {
        
//        cv::Mat mat1 = cv::cvarrToMat(edgeRivers[i].getCvImage());
//        cv::Mat mat2 = cv::cvarrToMat(ofImageEdge.getCvImage());
//        cv::Mat mat1 = edgeRivers[i].getCvImage();
//        cv::Mat mat2 = ofImageEdge.getCvImage();
//32
//        ofLog(OF_LOG_NOTICE, "mat1 width:%lf, height:%lf", mat1.cols, mat1.rows);
//        ofLog(OF_LOG_NOTICE, "mat2 width:%lf, height:%lf", mat2.cols, mat2.rows);
        
//        result = edgeRivers[i].getCvImage();
        
//        cv::Mat mat1 = edgeRivers[i].getCvImage();
//        cv::Mat mat2 = ofImageEdge.getCvImage();
        
//        cv::Mat mat1;
//        mat1 = ofxCv::toCv(edgeRivers[i]);
//        cv::Mat mat2;
//        mat2 = ofxCv::toCv(ofImageEdge);
//        cv::Mat mat1 = cv::Mat(riversSmallSize_w, riversSmallSize_h, CV_LOAD_IMAGE_GRAYSCALE, edgeRivers[i].getPixels());
//        cv::Mat mat2 = cv::Mat(riversSmallSize_w, riversSmallSize_h, CV_LOAD_IMAGE_GRAYSCALE, ofImageEdge.getPixels());
    
//        ofLog(OF_LOG_NOTICE, "mat1 width:%lf, height:%lf", mat1.cols, mat1.rows);
//        ofLog(OF_LOG_NOTICE, "mat2 width:%lf, height:%lf", mat2.cols, mat2.rows);
//        ofLog(OF_LOG_NOTICE, "getCvImage width:%lf, height:%lf", edgeRivers[i].getCvImage()->width, edgeRivers[i].getCvImage()->height);
//         ofLog(OF_LOG_NOTICE, "getCvImage width:%lf, height:%lf", ofImageEdge.getCvImage()->width, ofImageEdge.getCvImage()->height);
        
        result = cvCreateImage(cvSize(grayRivers[0].width - grayRivers_cut.width + 1, grayRivers[0].height - grayRivers_cut.height + 1), 32, 1);
        cvMatchTemplate(grayRivers[0].getCvImage(), grayRivers_cut.getCvImage(), result, CV_TM_CCORR_NORMED);
        
//        cv::matchTemplate(mat1, mat2, result, CV_TM_CCOEFF);
        //    cv::matchTemplate(mat1, mat2, result, CV_TM_SQDIFF_NORMED);
        //    cv::matchTemplate(mat1, mat2, result, CV_TM_CCORR_NORMED);
        //    cv::minMaxLoc(result, &minVal, NULL);
//            cv::minMaxLoc(result, NULL, &maxVal);
        CvPoint minLoc, maxLoc;
        cvMinMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, 0);
        
        //    ofLog(OF_LOG_NOTICE, "minVal: %lf", minVal);
        ofLog(OF_LOG_NOTICE, "%d番目のminVal: %lf", i, minVal);
        ofLog(OF_LOG_NOTICE, "%d番目のmaxVal: %lf", i, maxVal);
        ofLog(OF_LOG_NOTICE, "%d番目のminLoc: (%lf,%lf)", i, minLoc.x, minLoc.y);
        ofLog(OF_LOG_NOTICE, "%d番目のmaxLoc: (%lf,%lf)", i, maxLoc.x, maxLoc.y);
        if(mostmaxVal < maxVal){
            mostmaxVal = maxVal;
            mostmaxIndex = i;
        }
    }
    ofLog(OF_LOG_NOTICE, "mostmaxVal:%lf, index:%d", mostmaxVal, mostmaxIndex);
    isMatchCompleted = true;
}

void ofApp::showMatchImage(){
    showRiver = grayRivers[mostmaxIndex];
    showRiver.resize(riversSmallSize_w/3, riversSmallSize_h/3);
    showRiver.draw(450, 200);
    
//    showRiver = edgeRivers[mostmaxIndex];
//    showRiver.resize(riversSmallSize_w/3, riversSmallSize_h/3);
//    showRiver.draw(450, 200);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key){
        case OF_KEY_RETURN:
            capturedEdge = edgeImage;
            flagDrawimage = true;
            
            //マッチング
            matching();
            captureSound.play();
            break;
            
        case OF_KEY_BACKSPACE:
            isMatchCompleted = false;
            flagDrawimage = false;
            count = 0;
            dx = 0;
            break;
        
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}

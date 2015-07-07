#include "ofApp.h"
//#include "ofxCv.h"
//#include "Utilities.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableAlphaBlending();
//    ofSetFullscreen(true);
//    ofHideCursor();
    
    //音声ファイル
    captureSound.loadSound("capture.wav");
    
    flagDrawimage = false;
    isMatchCompleted = false;
    
    //背景差分
//    threshold = 24;
//    bgImage.loadImage("./background.jpg");
//    testImage.setImageType(OF_IMAGE_GRAYSCALE);
//    grayBg.setFromPixels(bgImage.getPixels(), bgImage.width, bgImage.height);
//    grayDiff.allocate(bgImage.width/2, bgImage.height/2);
    
    //counter
    count = 0;
    count_mapshowing = 0;
    
    dx = 0;
    dy = 0;

    cameraSize_w = 400;
    cameraSize_h = 300;
    cameraSmallSize_w = cameraSize_w;
    cameraSmallSize_h = cameraSize_h;
    riversImgSize_w = 1440;
    riversImgSize_h = 1080;
    riversSmallSize_w = riversImgSize_w;
    riversSmallSize_h = riversImgSize_h;
    
    cut_x = 100;
    cut_y = 50;
    cut_w = 150;
    cut_h = 150;
    
    
    //領域確保
    videoImage.allocate(cameraSmallSize_w, cameraSmallSize_h);
    grayImage_hide.allocate(cameraSmallSize_w, cameraSmallSize_h); //カメラ掌画像
    grayImage_show.allocate(cameraSmallSize_w, cameraSmallSize_h); //カメラ掌画像
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
        string loadStr = "./rivers/clipimages/rivers";
        string loadStr2 = ".jpg";
        loadStr += strNum;
        loadStr += loadStr2;
        
        riversImage[i].loadImage(loadStr);
        riversImage[i].resize(riversSmallSize_w, riversSmallSize_h);
        riversImage[i].setImageType(OF_IMAGE_COLOR);
        colorRivers[i].setFromPixels(riversImage[i].getPixels(), riversSmallSize_w, riversSmallSize_h);
        
        riversImage[i].setImageType(OF_IMAGE_GRAYSCALE);
        grayRivers[i].setFromPixels(riversImage[i].getPixels(), riversSmallSize_w, riversSmallSize_h);
        
        cvCanny(grayRivers[i].getCvImage(), edgeRivers[i].getCvImage(), 30, 70);
    }
    
    //河川画像，
    rivborImage.loadImage("./rivers/riversborder.gif");
    rivborImage.setImageType(OF_IMAGE_GRAYSCALE);
    grayRivBor.setFromPixels(rivborImage.getPixels(), 2041, 1216);
    
    //地球
    ofSetVerticalSync(true);
    
    glEnable(GL_DEPTH_TEST); //enable depth comparisons and update the depth buffer
    ofDisableArbTex(); //needed for textures to work with gluSphere
    earth.loadImage("earth6_blue.jpg");
    
    //prepare quadric for sphere
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    gluQuadricNormals(quadric, GLU_SMOOTH);
    
    //Serial
    ar.setup(1, 9600);
//    isSetupArduino = false;
//    arduino.connect("/dev/cu.usbmodem1421");
//    ofAddListener(arduino.EInitialized, this, &ofApp::setupArduino);
    
    //Google Map
    gMapView.setup();
    currentArea_index = 0;
    currentArea_name = areaList[currentArea_index]; //南米スタート

}

//--------------------------------------------------------------
void ofApp::update(){
    //video
    videoGrabber.update();
    if(videoGrabber.isFrameNew()){
        captureImage();
    }
    
    //Serial
    if ((ofGetFrameNum() >= 60*5)&&(ofGetFrameNum() % 600 == 0)) {
            ar.update();
        if (ar.isRotated) {
            shift();
            ar.isRotated = false;
        }
        //    arduino.update();
    }
    
    //Google Map
    gMapView.update();
}

void ofApp::setupArduino(const int &version) {
    ofRemoveListener(arduino.EInitialized, this, &ofApp::setupArduino);
//    arduino.sendAnalogPinReporting(2, ARD_ANALOG);
    arduino.sendDigitalPinMode(2, ARD_INPUT);
    isSetupArduino = true;
    ofLog(OF_LOG_NOTICE, "setupArduino");
}

void ofApp::updateArduino() {
    arduino.update();
    
    if(!isSetupArduino) {
        ofDrawBitmapString("arduino no ready", 500, 600);
    }else {
//        ofDrawBitmapString(ofToString(arduino.getAnalog(2)), 500, 600);
        ofDrawBitmapString(ofToString(arduino.getDigital(1)), 500, 600);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 0);
    ofSetHexColor(0xffffff);
    
//    ofSetColor(200, 10, 200);
//    ofNoFill();
//    ofRect(20+cut_x, 20+cameraSmallSize_h+100+cut_y, cut_w, cut_h);
//
    //camera
//    edgeImage.draw(20, 20+cameraSmallSize_h+100);
    
    //エッジ
    if(!flagDrawimage){
        showEarth(ofGetWidth()/2, ofGetHeight()/2, 0, 150);
    }else{
        
//        ofImageEdge.draw(400-dx, 100);
//        grayRivers_cut.draw(400-dx, 100);
        
        count++;
        if(count > 0){
            
            //キャプチャ後のアニメーション
            if(dx < 460){
                showEarth(ofGetWidth()/2, ofGetHeight()/2, dx, 150);
                dx = dx + 10;
                
            }else{
                if(count < 200){
                    
                    if(count % 2 == 0){
                        ofSetHexColor(0xffffff);
                        stringstream captureStr;
                        captureStr << "Searching..." << endl;
                        ofDrawBitmapString(captureStr.str(), 500, 50);
                    }
                }else{
                    if(isMatchCompleted){
                        count_mapshowing++;
                        if(count_mapshowing < 60*3) {
                            showMatchImage();
                            ofSetHexColor(0xffffff);
//                            stringstream name;
//                            name << "matched " << areaList[mostmaxIndex] << endl;
//                            ofDrawBitmapString(name.str(), 450, 150);
                        }else {
                            gMapView.showMap();
                        }
                    }
                    
                    //矩形
                    ofSetColor(200, 10, 200);
                    ofNoFill();
                    //テスト用
//                    float w = grayTestImage.width/3;
//                    ofRect(50 + grayTestImage.width/2 - w/2, 50 + grayTestImage.height/2 - w/2, w, w);
                    float w = grayImage_show.width/3;
                    ofRect(50 + grayImage_show.width/2 - w/2, 50 + grayImage_show.height/2 - w/2, w, w);
                    
                    //掌全体
                    ofSetColor(0xffffff);
                    //テスト用
//                    grayTestImage.resize(2448/10, 3264/10);
//                    grayTestImage.draw(50, 50);
                    grayImage_show.draw(50, 50);
                    
                    
                    //
                    //            //blobs
                    //            ofLog(OF_LOG_NOTICE, "nBlobs:%d", contourTest.nBlobs);
                    //            for (int i = 0; i < contourTest.nBlobs; i++) {
                    //                ofSetColor(255);
                    //                contourTest.blobs[i].draw(100, 50+dx);
                    //            }
                    
                    //            //切り抜き画像
                    //            ofSetColor(0xffffff);
                    //            grayCutImage.draw(400-dx, 50);
                    
                    showEarth(200, ofGetHeight()/2 + 150, 0, 80);
                }
            }
        }else{
            
        }
    }
}

void ofApp::showEarth(float x, float y, float z, float r){
    //球
    earth.getTextureReference().unbind();
    sphere.set(r, 20);
    sphere.setPosition(x, y, z);
    sphere.rotate(ofGetFrameNum(), 0, 1.0, 0);
    sphere.drawWireframe();
    
    //地球
    ofTranslate(x, y, z);
    ofRotateY(ofGetFrameNum());
    ofRotateX(-90);
    
    earth.getTextureReference().bind();
    gluSphere(quadric, r, 200, 200);
}

//
void ofApp::captureImage() {
    videoImage.setFromPixels(videoGrabber.getPixels(), cameraSmallSize_w, cameraSmallSize_h);
    grayImage_hide = videoImage;
    edgeImage = videoImage;
    
    //エッジ抽出
    cvSmooth(grayImage_hide.getCvImage(), grayImage_hide.getCvImage());
    cvCanny(grayImage_hide.getCvImage(), edgeImage.getCvImage(), 15, 70);
}

//背景差分
void ofApp::diffCapturedImage() {
    //背景との差分
    ofLog(OF_LOG_NOTICE, "grayBg(%d,%d)\n", grayBg.width, grayBg.height);
    ofLog(OF_LOG_NOTICE, "grayTestImage(%d,%d)\n", grayTestImage.width, grayTestImage.height);
    grayDiff.absDiff(grayBg, grayTestImage);
    grayDiff.threshold(threshold);
}

//切り抜き
void ofApp::cutCapturedImage() {
    grayImage_show = grayImage_hide;
    
    //テスト用
//    testImage.loadImage("./palm.jpg");
//    testImage.setImageType(OF_IMAGE_GRAYSCALE);
//    grayTestImage.allocate(testImage.width, testImage.height);
//    grayTestImage.setFromPixels(testImage.getPixels(), testImage.width, testImage.height);
//    ofLog(OF_LOG_NOTICE, "grayTestImage(%d,%d)", grayTestImage.width, grayTestImage.height);
    
    //テスト用
//    cutW = 2448/3;
    cutW = cameraSize_w/3;
    cutH = cutW;
    grayCutImage.allocate(cutW, cutH);
    
    //テスト用
//    grayTestImage.setROI(grayTestImage.width/2 - cutW/2, grayTestImage.height/2 - cutH/2, cutW, cutH);
//    grayCutImage = grayTestImage;
//    grayTestImage.resetROI();
    grayImage_show.setROI(grayImage_show.width/2 - cutW/2, grayImage_show.height/2 - cutH/2, cutW, cutH);
    grayCutImage = grayImage_show;
    grayImage_show.resetROI();
    
    grayCutImage.resize(grayCutImage.width/4, grayCutImage.height/4);
    
    //輪郭抽出
    grayCutEdge.allocate(grayCutImage.width, grayCutImage.height);
    ofLog(OF_LOG_NOTICE, "grayCutImage(%d,%d)", grayCutImage.width, grayCutImage.height);
    grayCutEdge = grayCutImage;
    cvSmooth(grayCutImage.getCvImage(), grayCutImage.getCvImage());
    cvCanny(grayCutImage.getCvImage(), grayCutEdge.getCvImage(), 15, 70);
    
    //blobs
    contourTest.findContours(grayCutEdge, 10, (grayCutImage.width*grayCutImage.height)/2, 10, true);
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
        
        result = cvCreateImage(cvSize(grayRivers[currentArea_index].width - grayRivers_cut.width + 1, grayRivers[currentArea_index].height - grayRivers_cut.height + 1), 32, 1);
        grayCutImage.resize(100, 100);
    
        //template matching
        cvMatchTemplate(grayRivers[currentArea_index].getCvImage(), grayCutImage.getCvImage(), result, CV_TM_CCORR_NORMED);
        
        CvPoint minLoc, maxLoc;
        cvMinMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, 0);
        
        subjectLocation.x = maxLoc.x;
        subjectLocation.y = maxLoc.y;
        
        //load the google map
        int indexOfHtml = gMapView.classifyDetectedPoint(subjectLocation.x, subjectLocation.y);
        gMapView.loadMap(currentArea_name, indexOfHtml);
        
//        ofLog(OF_LOG_NOTICE, "%d番目のminVal: %lf", i, minVal);
//        ofLog(OF_LOG_NOTICE, "%d番目のmaxVal: %lf", i, maxVal);
        ofLog(OF_LOG_NOTICE, "%d番目のminLoc: (%d,%d)", currentArea_index, minLoc.x, minLoc.y);
        ofLog(OF_LOG_NOTICE, "%d番目のmaxLoc: (%d,%d)", currentArea_index, maxLoc.x, maxLoc.y);
//        if(mostmaxVal < maxVal){
//            mostmaxVal = maxVal;
//            mostmaxIndex = i;
//        }
    
    ofLog(OF_LOG_NOTICE, "mostmaxVal:%lf, index:%d", mostmaxVal, mostmaxIndex);
    isMatchCompleted = true;
}

void ofApp::showMatchImage(){
    
    //マッチングポイント描画
    if(ofGetFrameNum()%60 <= 30) {
        ofSetColor(10, 200, 200);
    }else {
        ofSetColor(200, 10, 200);
    }
    ofNoFill();
    ofRect(450+subjectLocation.x/3, 200+subjectLocation.y/3, cutFrame.width/3, cutFrame.height/3);
    
    ofSetHexColor(0xffffff);
//    showRiver = grayRivers[currentArea_index];
    showRiver.resize(riversSmallSize_w/3, riversSmallSize_h/3);
//    showRiver.draw(450, 200);
    grayRivers[currentArea_index].draw(450, 200, riversSmallSize_w/3, riversSmallSize_h/3);
    
//    grayRivBor.resize(grayRivBor.width/3, grayRivBor.height/3);
//    grayRivBor.draw(450, 200);
}

void ofApp::reset() {
    isMatchCompleted = false;
    flagDrawimage = false;
    count = 0;
    count_mapshowing = 0;
    dx = 0;
}

//検索範囲変更
void ofApp::shift() {
    reset();
    flagDrawimage = true;
    
    //切り抜き
    cutCapturedImage();
    
    //検索範囲変更
    currentArea_index++;
    if(currentArea_index >= 6) {
        currentArea_index = currentArea_index % 6;
    }
    currentArea_name = areaList[currentArea_index];
    ofLog(OF_LOG_NOTICE, "KeyPressed currentArea_index: %d", currentArea_index);
    
    //マッチング
    matching();
    captureSound.play();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key){
        case OF_KEY_RETURN:
            capturedEdge = edgeImage;
            flagDrawimage = true;
            
            //切り抜き
            cutCapturedImage();
            
            //マッチング
            matching();
            captureSound.play();
            break;
            
        case OF_KEY_BACKSPACE:
            reset();
            break;
            
        case OF_KEY_SHIFT:
            reset();
            flagDrawimage = true;
            
            //切り抜き
            cutCapturedImage();
            
            //検索範囲変更
            currentArea_index++;
            if(currentArea_index >= 6) {
                currentArea_index = currentArea_index % 6;
            }
            currentArea_name = areaList[currentArea_index];
            ofLog(OF_LOG_NOTICE, "KeyPressed currentArea_index: %d", currentArea_index);
            
            //マッチング
            matching();
            captureSound.play();
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

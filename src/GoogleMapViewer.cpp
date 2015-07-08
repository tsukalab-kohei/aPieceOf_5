//
//  GoogleMapViewer.cpp
//  aPieceOf_5
//
//  Created by 池田昂平 on 2015/07/06.
//
//

#include "GoogleMapViewer.h"

GoogleMapViewer::GoogleMapViewer() {
}

void GoogleMapViewer::setup() {
    browser.setup(ofGetWidth(), ofGetHeight());
    
    edgeImage.allocate(ofGetWidth(), ofGetHeight());
    
    loadMap("southAmerica", 0);
}

void GoogleMapViewer::update() {
    ofxAwesomium::updateCore();
    browser.update();
}

void GoogleMapViewer::loadMap(string area, int num) {
    string urlStr1 = "file:///Users/kohei/Documents/of_v0.8.4_osx_release/apps/myApps/aPieceOf_5/bin/data/html/";
    urlStr1 += area;
    urlStr1 += "/";
    urlStr1 += area;
    urlStr1 += ofToString(num);
    urlStr1 += ".html";
//    browser.loadURL("file:///Users/kohei/Documents/of_v0.8.4_osx_release/apps/myApps/aPieceOf_5/bin/data/html/southAmerica/southAmerica0.html");
    browser.loadURL(urlStr1);
    
    createEdgeImage();
}

void GoogleMapViewer::createEdgeImage() {
    edgeImage.setFromPixels(browser.frame.getPixels(), ofGetWidth(), ofGetHeight());
    cvCanny(edgeImage.getCvImage(), edgeImage.getCvImage(), 15, 70);
}

void GoogleMapViewer::showMap() {
    if(browser.getIsLoading()) {
        ofSetColor(0);
        ofDrawBitmapString("Loading...", 10, 15);
    }
    
//    ofSetColor(255, 255, 255, 200);
//    edgeImage.draw(0, 0, browser.frame.getWidth(), browser.frame.getHeight());
    
//    ofColor c;
//    c.setHsb(128, 200, 255);
//    ofSetColor(c);
    ofSetColor(255, 255, 255, 255);
    browser.frame.draw(0, 0, browser.frame.getWidth(), browser.frame.getHeight());
}

//地図を12分割→マッチングポイントの分類
int GoogleMapViewer::classifyDetectedPoint(float locX, float locY) {
    int indexOfHtml = 0;
    
    if (locY < 360) {
        if (locX < 360) {
            indexOfHtml = 0;
        }else if (locX < 720) {
            indexOfHtml = 1;
        }else if (locX < 1080) {
            indexOfHtml = 2;
        }else {
            indexOfHtml = 3;
        }
        
    }else if (locY < 720) {
        if (locX < 360) {
            indexOfHtml = 4;
        }else if (locX < 720) {
            indexOfHtml = 5;
        }else if (locX < 1080) {
            indexOfHtml = 6;
        }else {
            indexOfHtml = 7;
        }
        
    }else {
        if (locX < 360) {
            indexOfHtml = 8;
        }else if (locX < 720) {
            indexOfHtml = 9;
        }else if (locX < 1080) {
            indexOfHtml = 10;
        }else {
            indexOfHtml = 11;
        }
    }
    return indexOfHtml;
}

//
//  GoogleMapViewer.h
//  aPieceOf_5
//
//  Created by 池田昂平 on 2015/07/06.
//
//

#ifndef __aPieceOf_5__GoogleMapViewer__
#define __aPieceOf_5__GoogleMapViewer__

#include <stdio.h>
#include "ofxAwesomium.h"
#include "ofxOpenCv.h"

class GoogleMapViewer {
    
public:
    GoogleMapViewer();
    void setup();
    void update();
    void loadMap(string area, int num);
    void createEdgeImage();
    void showMap();
    int classifyDetectedPoint(float locX, float locY);
    
    ofxAwesomium browser;
    ofxCvGrayscaleImage edgeImage;
private:
    
};

#endif /* defined(__aPieceOf_5__GoogleMapViewer__) */

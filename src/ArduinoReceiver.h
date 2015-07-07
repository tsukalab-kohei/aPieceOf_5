//
//  ArduinoReceiver.h
//  aPieceOf_5
//
//  Created by 池田昂平 on 2015/07/03.
//
//

#ifndef __aPieceOf_5__ArduinoReceiver__
#define __aPieceOf_5__ArduinoReceiver__

#include <stdio.h>
#include "ofMain.h"

class ArduinoReceiver {
    
public:
    ArduinoReceiver();
    void setup(int modem, int rate);
    void update();
    
    int data;
    int data_str;
    unsigned char bytes[8];
    bool isRotated;
private:
    ofSerial serial;
};


#endif /* defined(__aPieceOf_5__ArduinoReceiver__) */

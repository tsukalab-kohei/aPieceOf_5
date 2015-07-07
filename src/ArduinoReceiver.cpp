
//
//  ArduinoReceiver.cpp
//  aPieceOf_5
//
//  Created by 池田昂平 on 2015/07/03.
//
//

#include "ArduinoReceiver.h"

ArduinoReceiver::ArduinoReceiver() {
    data = 0;
}

void ArduinoReceiver::setup(int modem, int rate) {
    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
//    serial.setup("/dev/cu.usbmodem1421", rate);
    serial.setup(1, rate);
    
    isRotated = false;
}

void ArduinoReceiver::update() {
    
        if(serial.isInitialized()) {
            
            if(serial.available() > 0) {
                data = serial.readByte();
                if(data == 1) {
                    if(!isRotated) {
                        ofLog(OF_LOG_NOTICE, "data:"+ofToString(data));
                        isRotated = true;
                    }
                }else {
                    isRotated = false;
                }
            }
    }
}

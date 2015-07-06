
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
//    serial.setup(modem, rate);
    serial.setup("/dev/cu.usbmodem1421", rate);
}

void ArduinoReceiver::update() {
    int bytesRequired = 8;
    int bytesRemaining = bytesRequired;
    
    while(bytesRemaining > 0) {
        if(serial.isInitialized()) {
            int bytesArrayOffset = bytesRequired - bytesRemaining;
            
            if(serial.available() > 0) {
                data = serial.readByte();
                if(data <= 1) {
                    ofLog(OF_LOG_NOTICE, "data:"+ofToString(data));
                }
//                int result = serial.readBytes(&bytes[bytesArrayOffset], bytesRemaining);
//                
//                if (result == OF_SERIAL_ERROR) {
//                    ofLog( OF_LOG_ERROR, "unrecoverable error reading from serial" );
//                    break;
//                }else if (result == OF_SERIAL_NO_DATA) {
//                    //try again
//                }else {
//                    //足りない文字数
//                    bytesRemaining = bytesRemaining - result;
//                }
            }
        }
    }
//    ofLog(OF_LOG_NOTICE, "%s", bytes);
}

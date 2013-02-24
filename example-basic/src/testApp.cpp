#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    w = 320;
    h = 240;
    vid.initGrabber(w, h);
    ofBackground(30);
}

//--------------------------------------------------------------
void testApp::update(){
    vid.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    vid.draw(0,0);
    ws.draw(0, 0);
    ofDrawBitmapString("capture image w/ spacebar \nmark areas with your mouse \npress 'w' to segment the image", 10, vid.getHeight() + 10);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch(key)  {
        case ' ':
            ws.setup(vid.getPixels(), vid.width, vid.height);
            break;
        case 'w':
            ws.segment();
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    ws.addToCurrentMarker(x, y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    ws.finishCurrentMarker();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
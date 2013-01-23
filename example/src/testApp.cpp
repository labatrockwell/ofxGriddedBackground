#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofSetFrameRate(30);
    ofSetWindowShape(1500, 800);
    
    bg.setDrawDebug(true);
    
    bg.setup( 1200,
              800,
              "images/bg_x_y.jpg",
              60,
              30 * 10,
              0,
              true );
    
    //bg.setReverseDirection();
}

//--------------------------------------------------------------
void testApp::update(){
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofPoint anchor;
    ofRectangle world;
    
    anchor = ofPoint(100, 0);
    world = ofRectangle( 100, 0,
                         500, 500 );
    
    bg.draw(world,
            anchor,
            0,
            ofGetElapsedTimeMillis());
    
    
    // mini gap
    anchor = ofPoint(600, 0);
    
    world = ofRectangle( 600, 0,
                         500, 500 );
    
    bg.draw(world,
            anchor,
            0,
            ofGetElapsedTimeMillis());
    
    ofSetColor(255, 255, 255, 100);
    ofRect(100, 0,
           1000, 500);
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

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
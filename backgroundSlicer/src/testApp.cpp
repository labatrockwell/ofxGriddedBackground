#include "testApp.h"

ofImage test;

//--------------------------------------------------------------
void testApp::setup(){
    cur.setUseTexture(false);
}

//--------------------------------------------------------------
void testApp::update(){
    if ( toLoad != "" && bDoSlice ){
        bDoSlice = false;
        bool bLoaded = cur.loadImage(toLoad);
        if ( !bLoaded ){
            ofLogError("Error, image did not load correctly.");
        } else {
            ofLog(OF_LOG_NOTICE, "Successfully loaded image");
        }
        int wSub = 10;
        int hSub = 4;
        
        stringstream ss;
        ss << "Splitting image into " << wSub << " x " << hSub << " parts.... wait for it";
        ofLog(OF_LOG_NOTICE, ss.str());
        
        int height = cur.height / hSub;
        int width  = cur.width / wSub;
        
        for ( int h=0; h<hSub; h++){
            for ( int w=0; w<wSub; w++){
                ofImage clone(cur);
                clone.update();
                clone.crop( w * width, h * height, width, height );
                ofSaveImage(clone, "bg_"+ofToString(w)+"_"+ofToString(h)+".jpg");
            }
        }
        
        
        bDoSlice = false;
        ofLog(OF_LOG_NOTICE, "Splitting complete.");
    }
}

//--------------------------------------------------------------
void testApp::draw(){

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

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
    toLoad = dragInfo.files[0];
    bDoSlice = true;
}
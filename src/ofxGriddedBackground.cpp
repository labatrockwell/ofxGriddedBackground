//
//  ofxGriddedBackground.cpp
//  Blooms
//
//  Created by Lucas Vickers on 12/4/12.
//
//

#include "ofxGriddedBackground.h"
#include <float.h>

ofxGriddedBackground::ofxGriddedBackground() :

_imageRootString("bg_x_y.jpg"),
_worldHeight(0),
_worldWidth(0),
_canvasWidth(0),
_canvasHeight(0),
_gridX(0),
_gridY(0),
_imageWidth(0),
_imageHeight(0),
_startTime(0),
_secondsBackgroundShift(60),
_direction(1),
_bPreloadTest(true)

{
    
}

ofxGriddedBackground::~ofxGriddedBackground()
{
    clearPanels();
}

void ofxGriddedBackground::clearPanels()
{
    map<int, panel>::iterator it;
    for( it = _backgroundPanels.begin(); it != _backgroundPanels.end(); ++it ) {
        if( it->second.image != NULL) {
            delete it->second.image;
        }
    }
    _backgroundPanels.clear();
}

bool ofxGriddedBackground::setup( int worldWidth,
                                  int worldHeight,
                                  string imageRootString,
                                  int secondsBackgroundShift,
                                  int updatesBeforeFlush,
                                  unsigned long long startTime,
                                  bool preloadTest)
{
    clearPanels();
    
    _worldHeight = worldHeight;
    _worldWidth = worldWidth;
    _imageRootString = imageRootString;
    _secondsBackgroundShift = secondsBackgroundShift;
    _startTime = startTime;
    _bPreloadTest = preloadTest;
    _updatesBeforeFlush = updatesBeforeFlush;
    
    if( imageRootString.length() == 0 || imageRootString.find("_x_y.") == string::npos ) {
        stringstream ss;
        ss << "ofxGriddedBackground requires image root name similar to background_x_y.jpg, given " << imageRootString;
        ofLog(OF_LOG_ERROR, ss.str());
        return false;
    }
    
    string dirStr =
     imageRootString.find("/") == string::npos ?
     "" : imageRootString.substr(0, imageRootString.find_last_of("/"));
    
    ofDirectory dir(dirStr);
    dir.allowExt( imageRootString.substr(imageRootString.find_last_of("_x_y.") + 1) );
    //cout << "setting extension to " << imageRootString.substr(imageRootString.find_last_of("_x_y.") + 1) << endl;
    
    dir.listDir();
    vector<ofFile> files = dir.getFiles();
    
    _gridX = 0;
    _gridY = 0;
    
    vector<unsigned int> ignores;
    for(int i=0; i<files.size(); ++i) {
        
        int start = imageRootString.find_last_of("/") + 1;
        int end = imageRootString.find_last_of("_x_y.") - strlen("_x_y.") + 1;
        
        string searchStr = imageRootString.substr( start, end - start );

        if( files[i].getFileName().find( searchStr ) == string::npos ) {
            ignores.push_back(i);
            stringstream ss;
            ss << "ofxGriddedBackground ignoring file " << files[i].getFileName();
            ofLog(OF_LOG_NOTICE, ss.str() ) ;
        } else {
            // _x_y
            string name = files[i].getFileName();
            
            int y = name.find_last_of("_");
            int x = name.find_last_of("_", y - 1);
            int period = name.find_last_of(".");
            
            int xInt = atoi( name.substr(x+1, y-x -1).c_str() );
            int yInt = atoi( name.substr(y+1, period-y -1).c_str() );
            
            _gridX = xInt > _gridX ? xInt : _gridX;
            _gridY = yInt > _gridY ? yInt : _gridY;
            
            if(_drawDebug) {
                stringstream ss;
                ss << "ofxGriddedBackground pushing out " << files[i].getFileName();
                ofLog(OF_LOG_NOTICE, ss.str());
            }
            
        }
    }
 
    // zero relative
    ++_gridX;
    ++_gridY;

    
    for(int i=ignores.size()-1; i>=0; --i) {
        files.erase(files.begin() + ignores[i]);
    }
    
    if( files.size() == 0 ) {
        ofLog(OF_LOG_ERROR, "ofxGriddedBackground found no background panels");
        return false;
    }
    
    bool first = true;
    
    for(int i=0; i<files.size(); ++i) {
        
        string name = files[i].getFileName();
        
        int y = name.find_last_of("_");
        int x = name.find_last_of("_", y - 1);
        int period = name.find_last_of(".");
        
        int xInt = atoi( name.substr(x+1, y-x -1).c_str() );
        int yInt = atoi( name.substr(y+1, period-y -1).c_str() );
        
        ofImage temp;
        
        if( _bPreloadTest || first ) {
            if( ! temp.loadImage(files[i]) ) {
                stringstream ss;
                ss << "ofxGriddedBackground failed to load image " << files[i].getFileName();
                ofLog(OF_LOG_ERROR, ss.str());
            }
        }

        if( first ) {
            first = false;
            _canvasWidth = temp.width * _gridX;
            _canvasHeight = temp.height * _gridY;
            _imageWidth = temp.width;
            _imageHeight = temp.height;
        }
        
        int index = yInt * _gridX + xInt;
        
        _backgroundPanels[index] = panel( files[i] );
    }
    
    return true;
}


void ofxGriddedBackground::setForwardDirection()
{
    _direction = 1;
}

void ofxGriddedBackground::setReverseDirection()
{
    _direction = -1;
}

void ofxGriddedBackground::draw( ofRectangle stencilWindow,
                                 ofPoint anchorCorner,
                                 float rotate,
                                 unsigned long long timeMS )
{
    
    if(_backgroundPanels.size() == 0) {
        return;
    }
 
    ofPushStyle();
    ofSetColor(255, 255, 255, 255);
    
    int startX;
    int startY;
    int endX;
    int endY;
    
    float shiftX;
    
    
    for(int i=0; i<_backgroundPanels.size(); ++i) {
        _backgroundPanels[i].drawSinceUse += 1;
        if( _backgroundPanels[i].drawSinceUse > _updatesBeforeFlush && _backgroundPanels[i].loaded ) {
            
            // clear doesn't seem to be working very well so delete image each time
            _backgroundPanels[i].image->clear();
            delete _backgroundPanels[i].image;
            _backgroundPanels[i].image = NULL;
            _backgroundPanels[i].loaded = false;
            
            if(_drawDebug) {
                stringstream ss;
                ss << "unloaded " << _backgroundPanels[i].file.getFileName();
                ofLog(OF_LOG_NOTICE, ss.str());
            }
        }
    }
    
    long double sec = (timeMS - _startTime);
    
    long double shiftPct = sec / 1000.0f / _secondsBackgroundShift;

    //cout << shiftPct << endl;
    
    shiftPct -= floor(shiftPct);
    shiftPct *= _direction;
    
    stencilWindow.x += shiftPct * _canvasWidth;
    //cout << "Shifting stencil window over by " << (shiftPct * _canvasWidth) << endl;
    
    // start off w/ a basic range.  floor so that it works with negative movements
    startX = floor(stencilWindow.getMinX() / _imageWidth);
    startY = floor(stencilWindow.getMinY() / _imageHeight);

    endX = stencilWindow.getMaxX() / _imageWidth;
    endY = stencilWindow.getMaxY() / _imageHeight;
    
    float xAdjust = - (stencilWindow.getMinX() / _imageWidth)  / _gridX * _canvasWidth;
    float yAdjust = - (stencilWindow.getMinY() / _imageHeight) / _gridY * _canvasHeight;
    
    for( int xx=startX; xx<=endX; ++xx ) {
        for(int yy=startY; yy<=endY; ++yy) {
            
            int drawX = xx % _gridX;
            int drawY = yy % _gridY;
            
            if( drawX < 0 ) {
                drawX += _gridX;
            }

            if( drawY < 0 ) {
                drawY += _gridY;
            }
            
            float offsetX = ((float) xx) / _gridX * _canvasWidth;
            float offsetY = ((float) yy) / _gridY * _canvasHeight;
            
            int index = drawY * _gridX + drawX;
            
            drawImage(index,
                      offsetX + xAdjust + anchorCorner.x,
                      offsetY + yAdjust + anchorCorner.y);
        }
    }
    
        
    ofPopStyle();
}


void ofxGriddedBackground::drawImage( int index,
                                      float x,
                                      float y )
{
    
    if( ! _backgroundPanels[index].loaded ) {
        try {
            _backgroundPanels[index].image = new ofImage();
            
            if(_drawDebug) {
                stringstream ss;
                ss << "loading " << _backgroundPanels[index].file.getFileName();
                ofLog(OF_LOG_NOTICE, ss.str());
            }
            
            _loader.loadFromDisk(_backgroundPanels[index].image,
                                 _backgroundPanels[index].file.getAbsolutePath() );
            
            _backgroundPanels[index].loaded = true;
            _backgroundPanels[index].drawSinceUse = 0;
        } catch (exception& e) {
            stringstream ss;
            ss << "Unable to create new ofImage, out of memory: " << e.what();
            ofLog( OF_LOG_ERROR, ss.str() );
            _backgroundPanels[index].image = NULL;
        }
    }
    
    if( _backgroundPanels[index].image != NULL ) {
        
        _backgroundPanels[index].image->draw(x, y, _imageWidth, _imageHeight);
        _backgroundPanels[index].drawSinceUse = 0;
        
    }
    // else you could draw a default
}

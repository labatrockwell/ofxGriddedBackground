//
//  ofxGriddedBackground.h
//  Blooms
//
//  Created by Lucas Vickers on 12/4/12.
//
//

#ifndef __Blooms__ofxGriddedBackground__
#define __Blooms__ofxGriddedBackground__

#include <iostream>
#include "ofMain.h"

#include "ofxThreadedImageLoader.h"


/*
 * This class will take a grid of images (can make up a larger background)
 * and will line them up to make an overall background.  If you provide
 * a stencil view we will only draw the backgrounds that fall within the
 * given draw space
 *
 * TO USE:
 *
 * imageRootString = name base for your background panels. 
 *  ie background_x_y.jpg or images/background_x_y.jpg
 *  _x_y is the x and y grid, starting from 0_0
 *
 * Don't use _x_y in the name, so
 *
 * background_x_y.png   = OK
 * x_y_bg_x_y.jpg       = NOT OK
 * bg/bg_x_y.jpg        = OK
 *
 * Don't use the root such as bg_ in the name so
 *
 * bg_x_y.jpg and bg-scaled.jpg can not be in same dir
 *
 *
 *
 * Also this class will scroll the background forward or backward.  This 
 * will cause the image to wrap on the horizontal.
 *
 */

class ofxGriddedBackground
{
    
private:
    
    /**
     * panel class
     *
     * Helpder class for ofxGriddedBackground
     */
    class panel {
    public:
        
        panel() :
            loaded(false),
            image(NULL),
            file(),
            drawSinceUse(0),
            index(0)
        { }
        
        panel(ofFile f) :
            loaded(false),
            image(NULL),
            file(f),
            drawSinceUse(0),
            index(0)
        { }
        
        ~panel()
        { }
        
        bool loaded;
        
        // use a pointer so it doesn't matter if we are coppied in vector
        ofImage* image;
        ofFile file;
        int index;
        
        unsigned int drawSinceUse;
        
    };
    
public:
    
    /**
     * ofxGriddedBackground constructor
     */
    ofxGriddedBackground();
    
    ~ofxGriddedBackground();
    
    /**
     * Set up the gridded background that will be used
     *
     * @param worldWidth        The width of the world you are working in.  If just one monitor this
     *                          is probably the size of your screen.  If multiple monitors/computers
     *                          this gets much larger
     * @param worldHeight       The height of the world.  See worldWidth for additional info.
     * @param imageRootString   Such as "bg/bg_x_y.jpg", this is the naming convention of your images
     * @param secondsBackgroundShift    Seconds it takes for us for the entire image (all grids) to pass
     *                                  along the screen.  Optional.
     * @param updatesBeforeFlush        The number of updates (FPS * seconds) we wait after not using an image
     *                                  before we flush it from memory
     * @param startTime         The start time used to figure out how much we should shift the images
     *                          you will need to sync this if working across multiple machines.  Optional.
     * @param preloadTest       If true, all images will first be loaded and unloaded to expose file
     *                          problems early on.  Optional
     *
     */
    bool setup( int worldWidth,
                int worldHeight,
                string imageRootString,
                int secondsBackgroundShift = 0,
                int updatesBeforeFlush = 30 * 60,
                unsigned long long startTime = 0,
                bool preloadTest = true);
    
    /**
     * Draw the section of the background image that is visible. 
     *
     * @param stencilWindow     The section of the world you need to see.   This will
     *                          usually be something like ofRectangle(0, 0, ofGetWidth(), ofGetHeight()) 
     *                          unless working with multiple screens.
     * @param anchorCorner      The point you want the background to start drawing.  For most purposes this will
                                be (0,0) for top left, but mabye you want to move things around.
     * @param rotate            At what degree you want the screen (view) rotated.  CURRENTLY NOT FUNCTIONAL.
     *                          Optional.
     * @param timeMS            Current milleseconds.  This will be used in conjunction with startTime
     *                          to decide how much we should shift our image
     
     */
    void draw( ofRectangle stencilWindow,
               ofPoint anchorCorner,
               float rotate = 0,
               unsigned long long timeMS = 0 );
    
    /**
     * Sets the background to shift left (move right to left)
     */
    void setForwardDirection();

    /**
     * Sets the background to shift right (move left to right)
     */
    void setReverseDirection();
    
    /**
     * Enable or disable debug information
     * @param val       If true, enable debug trace.  If false, disable debug trace.
     */    
    void setDrawDebug(bool val) {
        _drawDebug = val;
    }
    
private:
    
    void clearPanels();
    
    // draws an image, taking care of all logic such as counting draws, etc.
    void drawImage( int index,
                    float x,
                    float y );
    
    map<int, panel>     _backgroundPanels;
    
    string              _imageRootString;

    // size of the window we are drawing
    int                 _worldHeight;
    int                 _worldWidth;
    
    // the size of the entire background image once all the grid pieces are collected.
    int                 _canvasWidth;
    int                 _canvasHeight;
    
    // the number of grid pieces.
    int                 _gridX;
    int                 _gridY;
    
    // the image height.  Should be the same across all images.
    int                 _imageWidth;
    int                 _imageHeight;
    
    unsigned long long  _startTime;
    
    int                 _secondsBackgroundShift;
    int                 _updatesBeforeFlush;
    
    int                 _direction;

    bool                _bPreloadTest;
    
    ofxThreadedImageLoader   _loader;
    
    bool                _drawDebug;
};


#endif /* defined(__Blooms__ofxGriddedBackground__) */

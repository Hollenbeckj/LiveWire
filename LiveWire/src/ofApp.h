#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofVideoGrabber grabber; //VideoGrabber for cam input
    
    ofxCvColorImage image;      //CV ColorImage for our cam input
    ofxCvGrayscaleImage greyImage;  //CV GreyscaleImage for conversion of our cam
    ofxCvGrayscaleImage greyBackground; //CV GreyscaleImage to store the background
    ofxCvGrayscaleImage greyDiff;       //CV GreyscaleImage to store the difference between the greyImage and greyBackground aka bk removal
    ofxCvGrayscaleImage greyProcessed;  //CV GreyscaleImage that is preprocessed prior to blob detection
    
    bool learnBackground;   //bool to set whether or not to look for a new background plate
    void bkToggler();       //function to toggle the background
    
    ofxCvContourFinder contourFinder;   //object for handling contour finding aka blobs
    
    /*
    //GUI Stuff
    ofxPanel gui;           //ofxPanel object for gui objects
    ofxButton lBKButton;    //learn background Button
    ofxToggle showProcessed;    //toggle to show our RGB or Processed Image
    ofxToggle blurToggle;   //toggle for blur
    ofxIntSlider blurSlider;    //slider to control our blur amount
    ofxToggle errodeToggle;     //errosion toggle
    ofxToggle dilateToggle; //toggle dilation
    ofxToggle threshToggle;     //thresholding toggle
    ofxIntSlider threshSlider;  //slider for setting threshold amount
    
    bool guiStatus;     //bool to toggle the gui draw
     */
    
    vector <ofPoint> blobCenters;   //ofPoints vector for our Blob centroids
    int maxBlobs;   //maximum number of blobs
};

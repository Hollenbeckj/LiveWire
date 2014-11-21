#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxGui.h"
#include "ofxFlowTools.h"

#define USE_PROGRAMMABLE_GL

using namespace flowTools;

class ofApp : public ofBaseApp{
    
public:
    //functions
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    
    //audio functions
    void audioIn(float * input, int bufferSize, int nChannels);
    
    //base variables
    bool drawPoint;//booean for telling the program to draw the point when it finds something
    vector <ofPoint> blobCenters;   //ofPoints vector for our Blob centroids
    int maxBlobs;   //number of blobs allowed to track

    //webcam/ video stuff
    ofVideoGrabber grabber; //VideoGrabber for cam input
    
    //OpenCV variables
    ofxCvColorImage image;      //CV ColorImage for our cam input
    ofxCvGrayscaleImage greyImage;  //CV GreyscaleImage for conversion of our cam
    ofxCvGrayscaleImage greyBackground; //CV GreyscaleImage to store the background
    ofxCvGrayscaleImage greyDiff;       //CV GreyscaleImage to store the difference between the greyImage and greyBackground
    ofxCvGrayscaleImage greyProcessed;  //CV GreyscaleImage that is preprocessed prior to blob detection
    ofxCvContourFinder contourFinder;   //object for handling contour finding aka blobs
    ofPoint blobLocation;
    
    //visuals
        
    //Audio and FFT stuff
    ofSoundStream micInput;
    
    //vector <float> left;
    vector <float> audioFreq;
    vector <float> volHistory;
    float smoothedVol;
    float scaledVol;
    int bufferCounter;
    int 	drawCounter;
    

};

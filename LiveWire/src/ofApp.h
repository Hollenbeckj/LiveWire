#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxGui.h"
#include "particle.h"

class ofApp : public ofBaseApp{
    
public:
    //functions
    void setup();
    void update();
    void draw();
    
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
    
    //VISUALS BEGIN///////////////////////////////////////////////////////////////////////////////////////

    //FBO object
    ofFbo screenFbo;
    
    //visuals setup
    void checkParticles();
    void createParticles();
    void createAlphaTrail();
    public: ofPoint particleLoc;
    int particleR;
    int particleG;
    int particleB;
    int maxParticles;
    int fboTimer;
    
    // physics forces
    ofPoint wind;//of Point for wind
    ofPoint gravity;//of point for gravity
    
    //vectors
    vector<particle> particles;
    //VISUALS END////////////////////////////////////////////////////////////////////////////////////////
    
    //Audio and FFT stuff///////////////////////////////////////////////////////////////////////////////
    
    void checkFreq();
    ofSoundStream micInput;
    vector <float> audioFreq;
    vector <float> volHistory;
    float smoothedVol;
    float scaledVol;
    int scaledVolInt;
    int bufferCounter;
    int drawCounter;
    //Audio and FFT end////////////////////////////////////////////////////////////////////////////////

};

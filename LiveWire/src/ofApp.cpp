#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowShape(1280, 720);
    ofSetFrameRate(60);
    ofSetBackgroundAuto(true);
    ofBackground(0);
    
    grabber.setDeviceID(0);             //grab our first cam device
    grabber.setDesiredFrameRate(30);    //set framerate for cam
    grabber.initGrabber(1280, 720);      //set our incoming size
    
    image.allocate(grabber.width, grabber.height);  //allocate our CV ColorImage with w/h based off cam
    greyImage.allocate(grabber.width, grabber.height);  //same for other CV Images
    greyBackground.allocate(grabber.width, grabber.height); //same
    greyProcessed.allocate(grabber.width,grabber.height);   //same
    
    learnBackground = false;    //intially set to false
    
    /*
    lBKButton.addListener(this, &ofApp::bkToggler);    //setup a listener on our learn background button to toggle it's state momentarily
    guiStatus = true;   //set gui status to true
    
    gui.setup();    //setup the ofxPanel
    gui.add(lBKButton.setup("Learn Background"));
    gui.add(showProcessed.setup("Processed Image",true));
    gui.add(blurToggle.setup("Blur",false));
    gui.add(blurSlider.setup("Blur Amount",9,0,100));
    gui.add(errodeToggle.setup("Erode",false));
    gui.add(dilateToggle.setup("Dilate",false));
    gui.add(threshToggle.setup("Threshold",false));
    gui.add(threshSlider.setup("Threshold Amount",128,0,255));
     */
    
    maxBlobs = 1;   //number of blobs allowed for the program to detect 
    
    blobCenters.resize(maxBlobs);   //set the size or our vector based off the numbe of blobs
}

//--------------------------------------------------------------
void ofApp::update(){
    
    grabber.update();   //pull our next frame of video
    
    image.setFromPixels(grabber.getPixelsRef());    //set our CV Color Image to have the same data as the current frame
    
    greyImage = image;  //convert our color image to grayscale
    
    //change the background comparision baseline if learnBackground is true
    if(learnBackground){
        greyBackground = greyImage;     //set the background to the current frame
        learnBackground = false;        //toggle back to false
    }
    
    greyDiff.absDiff(greyBackground, greyImage);    //do a comparison between our current background ref and current frame and store result in greyDiff
    
    greyProcessed = greyDiff;  //set our greyProcessed to be our greyDiff
    
    //toggle our various processing passes on and off
    //if(blurToggle){
        //greyProcessed.blur(blurSlider); //blur
    //}
    //if(errodeToggle){
        //greyProcessed.erode();          //errode
    //}
    //if (dilateToggle) {
       // greyProcessed.dilate();         //dilate
    //}
    //if(threshToggle){
        greyProcessed.threshold(234);   //threshold our image
    //}
    
    //finding our contours on our processed image with a min size of 10 and max of half our screen size, looking for 1 blob, and looking for holes
    contourFinder.findContours(greyProcessed, 40, (grabber.width*grabber.height)/2, maxBlobs, true);
    
    //iterate through our blobs and pull the centroid and store as ofPoint
    for(int i = 0; i < contourFinder.nBlobs; i++){
        blobCenters[i] = contourFinder.blobs[i].centroid;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofSetColor(255,255,255);
    
    //if (showProcessed) {
        image.draw(0,0);    //draw our color image
    //}else{
       // greyProcessed.draw(0,0);    //draw our processed greyscale image
    //}
    
    
    for(int i = 0; i < contourFinder.nBlobs; i++){ // go through the amount of blobs specified to find and draw them
        contourFinder.blobs[i].draw(0,0);
    }
    
    for(int i = 0; i < blobCenters.size(); i++){
        ofSetColor(0, 255, 0, 150);
        ofFill();
        ofCircle(blobCenters[i], 40);
    }
    
    //toggle gui
    //if(guiStatus){
        //gui.draw();     //draw our gui if toggled on
    //}
}

//function connected to the lBKToggle button
void ofApp::bkToggler(){
    learnBackground = true;     //if mouse is pressed then grab a new background plate
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    //if (key =='h') {
        //guiStatus = !guiStatus; //flip our guistate
    //}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

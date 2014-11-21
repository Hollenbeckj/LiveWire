#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetWindowShape(1280, 720);
    ofSetFrameRate(60);
    ofSetBackgroundAuto(true);
    ofBackground(0);
    drawPoint = false; // tell the program to not draw the point at first
    
    //camera setup
    grabber.setDeviceID(0);             //grab our first cam device
    grabber.setDesiredFrameRate(60);    //set framerate for cam
    grabber.initGrabber(1280, 720);      //set our incoming size
    
    //OpenCV setup
    image.allocate(grabber.width, grabber.height);  //allocate our CV ColorImage with w/h based off cam
    greyImage.allocate(grabber.width, grabber.height);  //same for other CV Images
    greyBackground.allocate(grabber.width, grabber.height); //same
    greyProcessed.allocate(grabber.width,grabber.height);   //same
    maxBlobs = 1;   //number of blobs allowed for the program to detect
    blobCenters.resize(maxBlobs);   //set the size or our vector based off the numbe of blobs
    blobLocation = ofPoint(0,0);
    
    // visuals setup
    
    //fluid particles setup
        
    //draw Forces
    
    // audio setup
    int bufferSize = 400;//size of the audio buffer
    micInput.setup(this, 0, 2, 44100, bufferSize, 4);//initialize the program to read mic input
    micInput.listDevices();//list the known mic devices upon runnign the program
    audioFreq.assign(bufferSize, 0.0);
	volHistory.assign(400, 0.0);
    bufferCounter	= 0;
	drawCounter		= 0;
	smoothedVol     = 0.0;
	scaledVol		= 0.0;


    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //openCV stuff
    grabber.update();   //pull our next frame of video
    image.setFromPixels(grabber.getPixelsRef());    //set our CV Color Image to have the same data as the current frame
    greyImage = image;  //convert our color image to grayscale
    
    //change the background comparision baseline if learnBackground is true

    greyDiff.absDiff(greyBackground, greyImage);    //do a comparison between our current background ref and current frame and store result in greyDiff
    greyProcessed = greyDiff;  //set our greyProcessed to be our greyDiff
    
    greyProcessed.threshold(250);   //threshold for finding light this is set high so that it can find the brightest light possible
    contourFinder.findContours(greyProcessed, 40, (grabber.width*grabber.height)/2, maxBlobs, true);
    
    
    if(contourFinder.nBlobs == 1)
    {
        drawPoint = true;
        //cout<<"drawing point ";
    }
    else
    {
        drawPoint = false;
        //cout<<"cant draw point ";
    }
    
    //iterate through our blobs and pull the centroid and store as ofPoint
    for(int i = 0; i < contourFinder.nBlobs; i++)
    {
        blobCenters[i] = contourFinder.blobs[i].centroid;
    }
    
    //audio stuff
    scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    volHistory.push_back( scaledVol );
    
    if( volHistory.size() >= 400 )
    {
		volHistory.erase(volHistory.begin(), volHistory.begin()+1);
	}


}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //cout<<scaledVol;
    
    if(drawPoint)
    {
    ofBackground(0);
    ofSetColor(255,255,255);
    image.draw(0,0);//draw the webcam feed
        
    //interate through the loop and draw the found blobs
    for(int i = 0; i < contourFinder.nBlobs; i++)
        { 
            contourFinder.blobs[i].draw(0,0);
        }
    //iterate through the amount of blobs found and draw a circle on the blob
    for(int i = 0; i < blobCenters.size(); i++)
        {
            blobLocation = ofPoint(blobCenters[i]);
            ofSetColor(255, 0, 0, 150);
            ofFill();
            ofCircle(blobLocation, scaledVol * 190.0f);
            //cout<<blobLocation;
            
        }
    }
    else // if it hasnt found a light dont draw anything but the webcam feed
    {
        ofBackground(0);
        ofSetColor(255,255,255);
        image.draw(0,0);    //draw our color image
    }
    
}

void ofApp::audioIn(float * input, int bufferSize, int nChannels){
	
	float curVol = 0.0;
	
	// samples are "interleaved"
	int numCounted = 0;
    
	//go through each sample and calculate the root mean square which is a rough way to calculate volume
	for (int i = 0; i < bufferSize; i++){
		audioFreq[i]= input[i*2]*0.5;
		curVol += audioFreq[i] * audioFreq[i];
		numCounted+=1;
        //audioFreq.clear();
	}
	
	//this is how we get the mean of rms :)
	curVol /= (float)numCounted;
	
	// this is how we get the root of rms :)
	curVol = sqrt( curVol );
	
	smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;
	
	bufferCounter++;
	
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetWindowShape(1280, 720);
    //ofSetFullscreen(true);
    screenFbo.allocate(1280, 720);
    ofSetFrameRate(120);
    ofSetBackgroundAuto(false);
    ofSetBackgroundAuto(true);
    ofBackground(0);
    drawPoint = false; // tell the program to not draw the point at first

    //CAMERA setup////////////////////////////////////////////////////////////////////////////////////////////////////////////
    grabber.setDeviceID(0);             //grab our first cam device
    grabber.setDesiredFrameRate(120);    //set framerate for cam
    grabber.initGrabber(1280, 720);      //set our incoming size
    //CAMERA end/////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    //OPENCV setup////////////////////////////////////////////////////////////////////////////////////////////////////////////
    image.allocate(grabber.width, grabber.height);  //allocate our CV ColorImage with w/h based off cam
    greyImage.allocate(grabber.width, grabber.height);  //same for other CV Images
    greyBackground.allocate(grabber.width, grabber.height); //same
    greyProcessed.allocate(grabber.width,grabber.height);   //same
    maxBlobs = 1;   //number of blobs allowed for the program to detect
    blobCenters.resize(maxBlobs);   //set the size or our vector based off the numbe of blobs
    blobLocation = ofPoint(0,0);
    //OPENCV end/////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // VISUALS setup//////////////////////////////////////////////////////////////////////////////////////////////////////////
    particleLoc = ofPoint(0,0);
    maxParticles = 1000;//max number of particles allowed in the vector
    fboTimer = 4;//set the FBO clear time to 4
    //particles setup
    particle p = particle();
    particles.push_back(p);
    //VISUALS end////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // AUDIO setup///////////////////////////////////////////////////////////////////////////////////////////////////////////
    int bufferSize = 400;//size of the audio buffer
    micInput.setup(this, 0, 2, 44100, bufferSize, 4);//initialize the program to read mic input
    micInput.setDeviceID(3);
    audioFreq.assign(bufferSize, 0.0);
	volHistory.assign(400, 0.0);
    bufferCounter = 0;
	drawCounter	= 0;
	smoothedVol = 0.0;
	scaledVol = 0.0;//scaled volume in float numbers
    scaledVolInt = 0;// scaled volume in integers
    //AUDIO end/////////////////////////////////////////////////////////////////////////////////////////////////////////////


    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //openCV update/////////////////////////////////////////////////////////////////////////////////////////////////////////
    grabber.update();   //pull our next frame of video
    
    image.setFromPixels(grabber.getPixelsRef());    //set our CV Color Image to have the same data as the current frame
    
    greyImage = image;  //convert our color image to grayscale

    greyDiff.absDiff(greyBackground, greyImage);//compare the difference of the background to a greyed version
    
    greyProcessed = greyDiff;//set our greyProcessed to the greyDiff
    
    greyProcessed.threshold(250);//threshold for finding light this is set high so that it can find fine light points
    
    contourFinder.findContours(greyProcessed, 40, (grabber.width*grabber.height)/2, maxBlobs, true);//using the graber find the contours of the blob
    if(contourFinder.nBlobs == 1) // if the number of detected blobs is 1
    {
        drawPoint = true;//set draw point to be true to allow drawing
        
        createAlphaTrail();
        
        if(particles.size()<maxParticles)//if the size of the vector of particles is less than the max allowed particles
        {
            createParticles();//create particles
            checkParticles();//check the size of the vector
        }
    }
    else
    {
        drawPoint = false;//if none of the above is true then remain false to avoid drawing when nothing is detected
    }
    
    for(int i = 0; i < contourFinder.nBlobs; i++)//go throught the blobs and send the information to a location point
    {
        blobCenters[i] = contourFinder.blobs[i].centroid;
    }
    //openCV end////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    //audio update//////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);//map the smoothed volume to a sensible set of integers
    
    volHistory.push_back( scaledVol );//push the oldest volume recording in the vector
    
    if( volHistory.size() >= 400 )//if the volume history hits the cap of 400 erase the oldest volume recording
    {
		checkFreq();//check the recorded amount of frequencies
	}
    
    //audio end////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    //visuals update///////////////////////////////////////////////////////////////////////////////////////////////////////
    
    for(int i = 0; i < particles.size(); i++)
    {
        particles[i].applyForce(wind);//apply the wind force to the big circle particles
        particles[i].applyForce(gravity);//apply the gravity force to the big circle particles
        particles[i].changeAlpha();//change the particle alpha
        particles[i].particleFlow();//run the particle flow function on every particle
        particles[i].particleMass = scaledVolInt;//set the current particles mass to be the current volume for variation
        particles[i].update();//update particles
    }
    
    particleLoc = blobLocation;//particle location is equal to the location of the blob found using the camera (must update)
    
    if(drawPoint)
        {
            screenFbo.begin();//start the FBO
            if(fboTimer == 0)//if the timer is 0 clear the FBO to give it a delayed alpha trail effect
            {
                ofClear(255, 255, 255);//clear the screen of the old particles
                fboTimer = 4;//after clearing reset the timer
            }
            screenFbo.end();//end the fbo
    
            screenFbo.begin();
            ofEnableAlphaBlending();
            for(int i = 0; i < particles.size(); i++)//loop through and draw the particles
            {
                particles[i].draw();//draw the particles
                particles[i].particleFlow();//call the flow function on the particles
            }
            screenFbo.end();
        }
    //VISUALS end/////////////////////////////////////////////////////////////////////////////////////////////////////////
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    if(drawPoint)
    {
    ofBackground(0);
    ofSetColor(255,255,255);//set the color of webcam feed to let in all colors
    image.draw(0,0);//draw the webcam feed
    
    for(int i = 0; i < contourFinder.nBlobs; i++) //go through and find blobs on the screen
        { 
            //contourFinder.blobs[i].draw(0,0);
        }

    for(int i = 0; i < blobCenters.size(); i++) //draw a circle around any blobs it finds
        {
            blobLocation = ofPoint(blobCenters[i]);//set blob location to the light point and update it every frame
        }
        
        screenFbo.draw(0,0);//draw the trailed aprticles to the screen
        
        }
    else // if it hasnt found a light dont draw anything but the webcam feed
        {
            ofBackground(0);
            ofSetColor(255,255,255);
            image.draw(0,0);
        }
    
}

void ofApp::audioIn(float * input, int bufferSize, int nChannels){
	
	float curVol = 0.0;//current volume recorded
	
	int numCounted = 0;
    
	for (int i = 0; i < bufferSize; i++)//go through the recorded samples and calculate the volume
    {
		audioFreq[i]= input[i*2]*0.5;
		curVol += audioFreq[i] * audioFreq[i];
		numCounted+=1;
	}
	
	curVol /= (float)numCounted;//calculate the mean
	
	curVol = sqrt( curVol );//calculate the square root of the current volume
	
	smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;
	
	bufferCounter++;
	
}

void ofApp::checkFreq()
{
    if( volHistory.size() >= 400 )//of the vector of volumes is greate than or is equal to 400
    {
		volHistory.erase(volHistory.begin());//erase the oldest volume recording
	}

}

void ofApp::checkParticles()
{
    particle p = particle();
    
    if(particles.size() == maxParticles)//if the size of the Vector is equal to the maximum set size then erase the oldest
    {
        particles.erase(particles.begin());//erase the oldest
    }

    if(p.particleAlpha == 0)//if particle alpha is 0
    {
        particles.erase(particles.begin());//delete the particles that have had their alpha reduced to zero
    }

}

void ofApp::createParticles()
{
    particle p = particle(); //set particles to be represented as tthe variable "p"
    p.changeAlpha();//call the change alpha function in the class
    p.particleLocation = blobLocation;//set the particle location to the blob location and pass it to the class
    p.particleRadius = scaledVol*30.0f;//set the radius of the particles to be what the recorded volume level is
    scaledVolInt = (int) p.particleRadius;//convert the float numbers of volume to be ints (makes it easier to work with)
    p.particleVelocity.x = scaledVolInt*(ofRandom(-2,2));//set the x velocity to be the currect scalled volume times 2 or -2
    p.particleVelocity.y = scaledVolInt*(ofRandom(2,-2));//set the y velocity to be the currect scalled volume times -2 or 2
    particles.push_back(p);
}

void ofApp::createAlphaTrail()
{
    fboTimer--;
    if(fboTimer==0)
    {
        fboTimer = 0;
    }
}

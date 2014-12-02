//
//  particle.h
//  LiveWire
//
//  Created by Joshua Hollenbeck on 11/24/14.
//
//

#ifndef __LiveWire__particle__
#define __LiveWire__particle__

#include <iostream>

#include "ofMain.h"

class particle
{
public:
    //constructor
    particle();
    
    //variables
    ofPoint particleLocation;       //ofPoint to describe location
    ofPoint particleVelocity;       //ofPoint to describe velocity
    ofPoint particleAcceleration;   //ofPoint to describe acceleration
    
    float particleMass;                 //mass of our particle aka a scalar
    int particleScaler;                  //scalar for our mass
    int particleAlpha;
    //ofPoint spinForce;
    float particleRadius;
    
    
    //functions
    void update();          //update method for movement calculations
    void draw();         //draw method
    void applyForce(ofPoint);      //receives a force as a ofPoint and applies it to our acceleration
    void changeAlpha();
    void particleFlow();
    
};


#endif /* defined(__LiveWire__particle__) */

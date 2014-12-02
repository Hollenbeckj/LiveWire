//
//  particles.h
//  LiveWire
//
//  Created by Joshua Hollenbeck on 11/23/14.
//
//

#ifndef ___ParticlesWithGui__particle__
#define ___ParticlesWithGui__particle__

#include <iostream>
#include "ofMain.h"

class particles
{
public:
    //constructor
    particles();
    
    //variables
    ofPoint particleLocation;       //ofPoint to describe location
    ofPoint particleVelocity;       //ofPoint to describe velocity
    ofPoint particleAcceleration;   //ofPoint to describe acceleration
    
    float particleMass;
    int particleScaler;
    
    //functions
    void update();          //update method for movement calculations
    void draw();         //draw method
    void applyForce(ofPoint);      //receives a force as a ofPoint and applies it to our acceleration
    
};
#endif /* defined(___ParticlesWithGui__particle__) */

//
//  particles.cpp
//  LiveWire
//
//  Created by Joshua Hollenbeck on 11/23/14.
//
//

#include "particles.h"
//#include "ofApp.h"

particles::particles()
{
    //basic variables
    particleLocation = ofPoint(500,500);
    //particleLocation =
    
    //physics
    particleVelocity = ofPoint(ofRandom(0,1),ofRandom(0,-5));
    particleAcceleration = ofPoint(0.0,0.0);
    particleScaler = 2;
    particleMass = 2;
    
}
void particles::update()
{
    particleVelocity = particleVelocity + particleAcceleration;
    particleVelocity.limit(10);
    particleLocation = particleLocation+particleVelocity;
    particleAcceleration*=0;
    
}

void particles::draw()
{
    
    ofCircle(particleLocation, 5);
}

void particles::applyForce(ofPoint f)
{
    ofPoint force = f/particleMass;
    
    particleAcceleration+=force;
    
}

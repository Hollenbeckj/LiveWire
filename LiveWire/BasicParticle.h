//
//  BasicParticle.h
//  vectors_t1
//
//  Created by Emery Martin on 9/23/14.
//
//
#pragma once

#ifndef __vectors_t1__BasicParticle__
#define __vectors_t1__BasicParticle__

//#include <iostream>
#include "ofMain.h"

class BasicParticle{
    
public:     //public vars
    BasicParticle();        //default constructor
    
    ofPoint location;       //ofPoint to describe location
    ofPoint velocity;       //ofPoint to describe velocity
    ofPoint acceleration;   //ofPoint to describe acceleration
    
    float accelerationScaler;   //used to scale our direciton unit vector to increase/decrease steering capability
    float mass;                 //mass of our particle aka a scalar
    int particleAlpha;
    
    int s;                  //scalar for our mass
    
    void update();          //update method for movement calculations
    void display();         //draw method
    void applyForce(ofPoint);      //receives a force as a ofPoint and applies it to our acceleration
    void reduceAlpha();
};

#endif /* defined(__vectors_t1__BasicParticle__) */

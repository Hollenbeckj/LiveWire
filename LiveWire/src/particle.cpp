//
//  particle.cpp
//  LiveWire
//
//  Created by Joshua Hollenbeck on 11/24/14.
//
//

#include "particle.h"
#include "ofMain.h"

particle::particle()
{
    //basic variables
    particleLocation = ofPoint(0,0);
    
    //physics
    particleVelocity = ofPoint(ofRandom(1,-1),ofRandom(1,-1));
    particleAcceleration = ofPoint(0.0,0.0);
    particleScaler = 5;
    particleMass = 10;
    particleRadius = 5;
    particleAlpha = 255;
    
}


void particle::update()
{
    particleVelocity = particleVelocity + particleAcceleration;//set velocity equal to the velocity plus acceleration
    particleVelocity.limit(ofRandom(10,20));//raandomize the limit of velocity between 10 and 20
    particleLocation = particleLocation+particleVelocity;//set location to the current location plus acceleration
    
}

void particle::draw()
{
    ofColor color = ofColor::red;//set the color to be red at first
    float hue = ofMap( 1, 1, 255, particleAlpha, 255 );//map the hue to the particle alpha
    color.setHue(hue);//set the color to the hue
    ofSetColor(color,particleAlpha);//set the particle color to the new color and set its alpha to particle alpha
    ofFill();//fill it with the color

    ofCircle(particleLocation, particleRadius);//draw a particle
    
}

void particle::applyForce(ofPoint f)
{
    ofPoint force = f/particleMass;// set a point for the force to take effect on
    
    particleAcceleration+=force;//asign the force to the particle
    
}
void particle::changeAlpha()
{
    particleAlpha-=3;//reduce the alpha by 3 every frame
    
    if(particleAlpha < 0)//check if the particle alpha is lower than 0
        {
            particleAlpha = 0;//if the alpha is lower than zero set it to zero
        }
}

void particle::particleFlow()
{
    for(int i = 0; i < 255;i++);//loop through the particles and add controlled burst of acceleration to the particles
    {
        particleAcceleration = ofPoint(ofRandom(-2,2),ofRandom(2,-2));//variate the acceleration to give a scrammble effect
    }

}

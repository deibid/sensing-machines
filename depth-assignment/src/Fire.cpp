//
//  Fire.cpp
//  depth-assignment
//
//  Created by David on 10/21/19.
//

#include "Fire.h"
#include "ofMain.h"


void Fire::setPosition(int x_, int y_){
    x = x_;
    y = y_;
    
    std::cout <<"Puse posición";
}

void Fire::update(bool swing){
    
    if(swing){
        cout<<"cool";
//        fireSize = fireSize - 10.0f;
        fireSize = fireSize - coolFactor;
    }else{
//        fireSize = fireSize + 0.3f;
        fireSize = fireSize + burnFactor;
    }
    
    if(fireSize <= 0){
        won = true;
    }
    
    
}

void Fire::draw(){
    
    if(!won){
        ofPushStyle();
        ofSetColor(200, 20, 20);
        ofDrawCircle(x,y,fireSize);
        ofPopStyle();
        
    }else{
        //super dirty way of managing a win state. This should be done outside of this class
        ofPushStyle();
        ofSetColor(250);
        ofDrawBitmapStringHighlight("YOU WIN", ofGetWindowWidth()/2,ofGetWindowHeight()/2);
        ofPopStyle();
    }
    
}



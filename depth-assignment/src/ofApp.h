#pragma once

#include "ofMain.h"
#include "Fire.h"


#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxGui.h"
#include "ofxKinect.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

    ofxKinect kinect;
    ofImage thresholdImg;
    
    ofParameter<float> nearThreshold;
    ofParameter<float> farThreshold;
    
    ofxPanel guiPanel;
    ofxCv::ContourFinder contourFinder;
    
    
    Fire fire;
    
    //flag to track if a body was inside the band during the previous update call
    int prevBodyInBand = 0;
		
};

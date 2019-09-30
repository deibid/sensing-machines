#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "daColorOption.h"
#include "daDrawPoint.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void mousePressed(int x, int y, int button);
    
    
    ofVideoGrabber videoGrabber;
    ofxPanel guiPanel;
    
    ofImage processImg;
    
    ofxCv::ContourFinder contourFinder;
    ofParameter<ofColor> targetColor;
    ofParameter<int> colorThreshold;
    ofParameter<float> minAreaTarget;
    ofParameter<float> maxAreaTarget;
    ofParameter<int> blurRadius;
    ofParameter<int> dilateIterations;
    ofParameter<int> persistanceFrames;
    ofParameter<float> maxDistanceToTrack;
    
    ofParameter<bool> debugMode;
    ofParameter<bool> drawingMode;
    
    ofColor colorUnderMouse;
    
    
    bool fingersDetected;
    ofVec2f finger1;
    ofVec2f finger2;
    bool isClicked;
        
    int prevContourNumber;
    
    
    //drawing vars
    ofColor drawingColor;
    
    cv::Point2f hoverCursor;
    std::vector<cv::Rect> openFingerRects;
    cv::Rect *closedFingerRect;
    
    std::vector<daDrawPoint> drawPoints;
    
    
    daColorOption colorOptions[4];
    
    
    
    
    
    
    private:
    
        void calculateContourRatio(std::vector<cv::Point> contour);
    
    
		
};

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    ofSetWindowShape(1280,900);
    ofSetCircleResolution(50);

    
    nearThreshold.set("Near Threshold", 0.01f, 0.0f, 0.1f);
    farThreshold.set("Far Threshold", 0.02f, 0.0f, 0.1f);
    
    guiPanel.setup("Depth Threshold","settings.json");
    guiPanel.add(nearThreshold);
    guiPanel.add(farThreshold);

    contourFinder.setUseTargetColor(true);
    
    //Empieza el registro de la camara
    kinect.setRegistration(true);
    kinect.init();
    kinect.open();
    
    fire.setPosition(ofGetWindowWidth()/2, ofGetWindowHeight()/2);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    kinect.update();
    
//    fire.update(ofRandom(0,100)>50);
    
    ofFloatPixels rawDepthData = kinect.getRawDepthPixels();
    ofFloatPixels near,far,result;
    
    ofxCv::threshold(rawDepthData, near, nearThreshold);
    ofxCv::threshold(rawDepthData, far, farThreshold, true);
    ofxCv::bitwise_and(near,far,result);
    
    thresholdImg.setFromPixels(result);
    
    contourFinder.getTracker().setPersistence(30);
    contourFinder.findContours(thresholdImg);
    
    if(contourFinder.size() > 0 && prevBodyInBand == 0){
        fire.update(true);
        prevBodyInBand = contourFinder.size();
    }else if(contourFinder.size() > 0 && prevBodyInBand != 0){
            //hand is hovering in frame
    }else if(contourFinder.size() == 0){
        prevBodyInBand =0 ;
        fire.update(false);
    }
    
    

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
//    kinect.getDepthTexture().draw(0,0);
//    float distAtMouse = kinect.getDistanceAt(ofGetMouseX(), ofGetMouseY());
//    ofDrawBitmapStringHighlight(ofToString(distAtMouse), ofGetMouseX(),ofGetMouseY());
    
    
    thresholdImg.draw(0,0);
    
    contourFinder.draw();
    guiPanel.draw();
    fire.draw();

}

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    
    ofSetWindowShape(640,480);
    videoGrabber.setup(640,480);
    ofScale( -1, 1, 1 );
    
    
    
    colorOptions[0].fillColor = ofColor(255,186,67);
    colorOptions[0].x =10;
    colorOptions[0].y =ofGetHeight()-100;
    colorOptions[0].width =80;
    colorOptions[0].height =80;
    
    
    colorOptions[1].fillColor = ofColor(77,96,167);
    colorOptions[1].x =100;
    colorOptions[1].y =ofGetHeight()-100;
    colorOptions[1].width =80;
    colorOptions[1].height =80;
    
    colorOptions[2].fillColor = ofColor(255);
    colorOptions[2].x =190;
    colorOptions[2].y =ofGetHeight()-100;
    colorOptions[2].width =80;
    colorOptions[2].height =80;
    
    colorOptions[3].fillColor = ofColor(194,19,74);
    colorOptions[3].x =280;
    colorOptions[3].y =ofGetHeight()-100;
    colorOptions[3].width =80;
    colorOptions[3].height =80;
    
    
    
    targetColor.set("Target Color", ofColor(0,255,0));
    colorThreshold.set("Color Threshold",5,0,40);
    minAreaTarget.set("Min Area Target",0.01f,0,0.25f);
    maxAreaTarget.set("Max Area Target", 0.5f,0.f,0.9f);
    blurRadius.set("Blur Radius",4,0,25);
    persistanceFrames.set("Persistance Frames",5,0,60);
    maxDistanceToTrack.set("Max Distance to Track",0.2f,0.0f,1.0f);
    dilateIterations.set("Dilate Amount",1,0,20);
    debugMode.set("Debug Mode",true);
    drawingMode.set("Drawing Mode", true);
    
    guiPanel.setup("Object Tracker", "settings.json");
    guiPanel.add(targetColor);
    guiPanel.add(colorThreshold);
    guiPanel.add(minAreaTarget);
    guiPanel.add(maxAreaTarget);
    guiPanel.add(blurRadius);
    guiPanel.add(dilateIterations);
    guiPanel.add(persistanceFrames);
    guiPanel.add(maxDistanceToTrack);
    guiPanel.add(debugMode);
    guiPanel.add(drawingMode);
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    videoGrabber.update();
    
    if(videoGrabber.isFrameNew()){
        colorUnderMouse = videoGrabber.getPixels().getColor(mouseX,mouseY);
    
        processImg.setFromPixels(videoGrabber.getPixels());
        ofxCv::GaussianBlur(processImg, blurRadius);
        ofxCv::erode(processImg,dilateIterations.get());
        processImg.update();
        
        contourFinder.setTargetColor(targetColor,ofxCv::TRACK_COLOR_HSV);
        contourFinder.setThreshold(colorThreshold);
        contourFinder.setMinAreaNorm(minAreaTarget);
        contourFinder.setMaxAreaNorm(maxAreaTarget);
        contourFinder.getTracker().setPersistence(persistanceFrames);
        contourFinder.getTracker().setMaximumDistance(maxDistanceToTrack);
        contourFinder.findContours(processImg);
        
        
        
        //detectar rects
        //ver la distancia entre ellos
        //ver sus proporciones
        //determinar si son los dedos o no
        
        
        //Middle filtering between found contours and my desired targets.
        //In order to make detection more robust, the tests here need to grow
//        std::vector<cv::Rect> fingerRects;
        openFingerRects.clear();
        closedFingerRect = NULL;
        
        for(int i =0; i<contourFinder.size(); i++){
            cv::Rect bRect = boundingRect(contourFinder.getContour(i));
            float aspectRatio = (float)bRect.height/(float)bRect.width;
            cout<<"Aspect ratio : "<< aspectRatio <<endl;
            if(aspectRatio < 1.15 && aspectRatio > 0.85) {
                openFingerRects.push_back(bRect);
                if(openFingerRects.size()!=2)cout<<"Found finger :  "<<bRect <<"\n";
            }
            
            if(aspectRatio < 2.4 && aspectRatio > 1.6){
                closedFingerRect = &bRect;
//                daDrawPoint drawSpot = daDrawPoint(closedFingerRect->x+closedFingerRect->width/2,
//                                                        closedFingerRect->y+closedFingerRect->height/2);
                daDrawPoint drawSpot;
                drawSpot.x = closedFingerRect->x+closedFingerRect->width/2;
                drawSpot.y = closedFingerRect->y+closedFingerRect->height/2;
                drawSpot.fillColor = drawingColor;
                
                drawPoints.push_back(drawSpot);
                //very dirty conversion. Having a hard time hanlding so many structs
                hoverCursor = cv::Point2f(drawSpot.x,drawSpot.y);
                
            }
        }
        
        for(daColorOption co:colorOptions){
            if(co.inside(hoverCursor.x,hoverCursor.y)){
                drawingColor = co.fillColor;
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    
    ofPushStyle();
    
    if(!drawingMode){
        if(debugMode){
            processImg.draw(0, 0, ofGetWidth(), ofGetHeight());
        }else{
            videoGrabber.draw(videoGrabber.getWidth(),0,-videoGrabber.getWidth(),videoGrabber.getHeight());
        }
    }
    
    contourFinder.draw();
    
    for(int i =0; i<drawPoints.size();i++){
        ofSetColor(drawPoints[i].fillColor);
        ofDrawCircle(drawPoints[i].x, drawPoints[i].y, 5);
    }
    
    
    for(daColorOption c:colorOptions){
        ofSetColor(c.fillColor);
        ofDrawRectangle(c.getX(), c.getY(), c.getWidth(), c.getHeight());
    }
    
    if(openFingerRects.size()==2){
        
        cv::Rect finger1 = openFingerRects[0];
        cv::Rect finger2 = openFingerRects[1];
    
        ofSetColor(150,17,163);
        ofDrawRectangle(finger1.x, finger1.y, finger1.width, finger1.height);
        ofDrawRectangle(finger2.x, finger2.y, finger2.width, finger2.height);
        
        ofSetColor(228,38,246);
        
        ofVec2f centerFinger1 = ofVec2f(finger1.x+finger1.width/2,finger1.y+finger1.height/2);
        ofVec2f centerFinger2 = ofVec2f(finger2.x+finger2.width/2,finger2.y+finger2.height/2);
        
        ofDrawCircle(centerFinger1.x, centerFinger1.y, 5);
        ofDrawCircle(centerFinger2.x, centerFinger2.y, 5);
        
        ofVec2f midPoint = centerFinger1;
        midPoint.middle(centerFinger2);
        
//        float midX = abs(centerFinger1.x+(centerFinger1.x-centerFinger2.x)/2);
//        float midY = abs(centerFinger1.y+(centerFinger1.y-centerFinger2.y)/2);
        
        hoverCursor.x = midPoint.x;
        hoverCursor.y = midPoint.y;
        
        //draw cursor is pending
        ofSetColor(10);
        ofDrawCircle(midPoint.x,midPoint.y,10);
        
    }
    
    
    ofSetColor(drawingColor);
    ofDrawRectangle(ofGetWidth()-50,ofGetHeight()-50,40,40);
    
    
    
    
    ofPopStyle();
    guiPanel.draw();
    
}


void ofApp::mousePressed(int x, int y, int button){
    
    if(guiPanel.getShape().inside(x,y)) return;
    targetColor = colorUnderMouse;
    
}


void calculateContourRatio(std::vector<cv::Point> contour){
 
    ofLog() << "The Contour Is:   " << contour;

}

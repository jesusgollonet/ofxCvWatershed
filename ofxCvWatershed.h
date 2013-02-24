//
//  ofxCvWatershed.h
//  watershedTest
//
//  Created by Jesus Gollonet on 5/4/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

class ofxCvWatershed {
public:
    void setup(unsigned char * px, int _w, int _h);
    // not sure about this. we should be able to pass in any shape, not just draw
    void addToCurrentMarker(int _x, int _y);
    void finishCurrentMarker();
    void segment();
    void draw(int _x, int _y);
    
    vector<ofxCvBlob> blobs; // after the contourFinder
    int state;
private:
    void reset();
    vector <ofPoint>  currentMarkers;

    bool bCurrentlyMarking;
    
    CvMemStorage * storage;
    CvSeq        * contours;
    CvMat        * colors;
    
    IplImage     * iplTargetImg;
    IplImage     * iplMarkers32sImg;
    IplImage     * iplMarkersTempImg;
    
    // just for drawing
    ofxCvColorImage         targetImg;
    ofxCvGrayscaleImage     markersImg;
    ofxCvColorImage         watershedImg;
    ofxCvGrayscaleImage         watershedGrayImg;
    
    ofxCvContourFinder      contourFinder;
    
    bool bCanDraw;
    
    
};

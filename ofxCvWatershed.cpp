//
//  ofxCvWatershed.cpp
//  watershedTest
//
//  Created by Jesus Gollonet on 5/4/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "ofxCvWatershed.h"

void ofxCvWatershed::setup(unsigned char * px, int _w, int _h){

    bCurrentlyMarking = false;
    storage = cvCreateMemStorage(1000);
    contours = 0;    
    
    iplTargetImg        = cvCreateImage(cvSize(_w, _h), IPL_DEPTH_8U,  3);
    iplMarkersTempImg   = cvCreateImage(cvSize(_w, _h), IPL_DEPTH_8U,  1);    
    iplMarkers32sImg    = cvCreateImage(cvSize(_w, _h), IPL_DEPTH_32S, 1);
    
    cvZero(iplMarkersTempImg);
    
    memcpy( iplTargetImg->imageData,  px, _w*_h*3);
    
    
    targetImg.allocate(_w,_h); 
    
    targetImg.setFromPixels(px, _w, _h);
    markersImg.allocate(_w,_h);
    watershedImg.allocate(_w,_h);
    watershedGrayImg.allocate(_w,_h);
    markersImg.set(0);
    watershedGrayImg.set(0);
    
    bCanDraw = true;
}

void ofxCvWatershed::addToCurrentMarker(int _x, int _y) {
    if (!bCurrentlyMarking) { 
        bCurrentlyMarking = true;
        currentMarkers.clear();
        currentMarkers.push_back(ofPoint(_x, _y));
    }

    CvPoint prevPt      = cvPoint(currentMarkers.back().x, currentMarkers.back().y);
    CvPoint currentPt   = cvPoint(_x, _y);
    
    cvLine( iplMarkersTempImg, prevPt, currentPt, cvScalarAll(255), 5, 8, 0 );
    markersImg = iplMarkersTempImg;
    currentMarkers.push_back(ofPoint(_x, _y));
}

void ofxCvWatershed::finishCurrentMarker() {
    bCurrentlyMarking = false;    
}

void ofxCvWatershed::segment() {
    reset();
    int nContours = cvFindContours( iplMarkersTempImg, storage, &contours, sizeof(CvContour),
                                   CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
    
    int i, j, compCount = 0;
    
    cvZero( iplMarkers32sImg );
    for( ; contours != 0; contours = contours->h_next, compCount++ ) {
        cvDrawContours( 
                       iplMarkers32sImg, 
                       contours, 
                       cvScalarAll(compCount+1),
                       cvScalarAll(compCount+1), 
                       -1, -1, 8, 
                       cvPoint(0,0) 
                       );
    }
    
    CvRNG rng = cvRNG(-1);
    colors = cvCreateMat( 1, compCount, CV_8UC3 );
    for( i = 0; i < compCount; i++ ) {
        uchar* ptr = colors->data.ptr + i*3;
        // no colors for now.
        ptr[0] = (uchar)0;//(cvRandInt(&rng)%180 + 50);
        ptr[1] = (uchar)0;//(cvRandInt(&rng)%180 + 50);
        ptr[2] = (uchar)0;//(cvRandInt(&rng)%180 + 50);
    }
    
    cvWatershed( iplTargetImg, iplMarkers32sImg );
    
    // paint the watershed image
    for( i = 0; i < iplMarkers32sImg->height; i++ ) {
        for( j = 0; j < iplMarkers32sImg->width; j++ ) {
            int idx     =  CV_IMAGE_ELEM( iplMarkers32sImg, int, i, j );
            uchar* dst  = &CV_IMAGE_ELEM( iplTargetImg, uchar, i, j*3 );
            if( idx == -1 ) {
                dst[0] = dst[1] = dst[2] = (uchar)255;
            } else if( idx <= 0 || idx > compCount ){
                dst[0] = dst[1] = dst[2] = (uchar)0; // should not get here
            }else {
                uchar* ptr = colors->data.ptr + (idx-1)*3;
                dst[0] = ptr[0]; dst[1] = ptr[1]; dst[2] = ptr[2];
            }
        }
    }
    //cvAddWeighted( watershed, 0.5, colorImg.getCvImage(), 0.5, 0, watershed );
    watershedImg     = iplTargetImg;
    watershedGrayImg = watershedImg;
    watershedGrayImg.threshold(140);
    //watershedGrayImg.invert();
    
    printf("contorus %i", contourFinder.findContours( watershedGrayImg, 10, 
                               (watershedImg.width * watershedImg.height)/ 2.f,
                               20, true));
}

void ofxCvWatershed::draw(int _x, int _y) {
    if(bCanDraw) {
        if(targetImg.bAllocated)        targetImg.draw(_x, _y);
        if(markersImg.bAllocated)       markersImg.draw(_x + targetImg.width, _y);
        if(watershedImg.bAllocated)     watershedImg.draw(_x + targetImg.width * 2, _y);
        if(watershedGrayImg.bAllocated) watershedGrayImg.draw(_x + targetImg.width * 2, _y + targetImg.height);
        contourFinder.draw(_x + targetImg.width * 2, _y + targetImg.height);
    }
}


void ofxCvWatershed::reset() {
    
    
//    cvReleaseImage( &cvImage );
//    cvReleaseImage( &cvImageTemp );
    
}





#pragma once

#include "ofMain.h"

class ofxSelfOrganizingMap {
    
public:
    ofxSelfOrganizingMap();
	~ofxSelfOrganizingMap();

    void setInitialLearningRate(float initialLearningRate);
    void setNumIterations(int numIterations);
    void setFeaturesRange(int numFeatures1, double *minInstance1, double *maxInstance1);
    void setMapSize(int dimx, int dimy, int dimz=1);
    void setup();    
    void updateMap(double *instance);
    ofPoint findBestMatchingCell(double *instance) const;
    double * getMapAt(int i, int j, int k=0) const { return map[i][j][k]; }
    int getCurrentIteration() { return t; }
    int getNumIterations() { return numIterations; }
    
    double * minInstance, * maxInstance;
    
    
protected:
    double **** map;
    double *** distLUT;
    int dimx, dimy, dimz, numFeatures;
    
    int t, numIterations;
    float initialRadius, radius;
    float initialLearningRate, learningRate;
    float lambda;
};
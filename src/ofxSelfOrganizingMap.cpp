#include "ofxSelfOrganizingMap.h"

//-------------------------------------------
ofxSelfOrganizingMap::ofxSelfOrganizingMap() {
    // initialize all defaults
    dimx = dimy = 256;
    dimz = 1;
    
    numFeatures = 3;
    minInstance = new double[3];
    maxInstance = new double[3];
    for (int i=0; i<3; i++) {
        minInstance[i] = 0;
        maxInstance[i] = 1;
    }
    
    numIterations = 3000;
    initialLearningRate = 0.1;
}

//-------------------------------------------
void ofxSelfOrganizingMap::setInitialLearningRate(float initialLearningRate) {
    this->initialLearningRate = initialLearningRate;
}

//-------------------------------------------
void ofxSelfOrganizingMap::setNumIterations(int numIterations){
    this->numIterations = numIterations;
}

void ofxSelfOrganizingMap::setMapSize(int dimx, int dimy, int dimz) {
    this->dimx = dimx;
    this->dimy = dimy;
    this->dimz = dimz;
}

//-------------------------------------------
void ofxSelfOrganizingMap::setFeaturesRange(int numFeatures1, double *minInstance1, double *maxInstance1) {
    
    delete [] minInstance;
    delete [] maxInstance;
    
    numFeatures = numFeatures1;
    minInstance = new double[numFeatures];
    maxInstance = new double[numFeatures];
    
    for (int i=0; i<numFeatures; i++) {
        minInstance[i] = minInstance1[i];
        maxInstance[i] = maxInstance1[i];        
    }
}

//-------------------------------------------
void ofxSelfOrganizingMap::setup() {
    // initialize map
    map = new double***[dimx];
    for (int i = 0 ; i< dimx; i++) {
        map[i] = new double**[dimy];
        for (int j = 0; j < dimy; j++) {
            map[i][j] = new double*[dimz];
            for (int k = 0; k < dimz; k++) {
                map[i][j][k] = new double[numFeatures];
                for (int z = 0; z < numFeatures; z++) {
                    map[i][j][k][z] = ofRandom(minInstance[z], maxInstance[z]);
                }
            }
        }
    }    
    // initialize distance lookup
    distLUT = new double**[dimx];
    for (int i = 0; i < dimx; i++) {
        distLUT[i] = new double*[dimy];
        for (int j = 0; j < dimy; j++) {
            distLUT[i][j] = new double[dimz];
            for (int k = 0; k < dimz; k++) {
                distLUT[i][j][k] = sqrt(i*i + j*j + k*k);
            }
        }
    }  
    // parameters
    initialRadius = max(dimx, dimy) / 2;
    lambda = numIterations / log(initialRadius);
    t = 0;    
}

//--------------------------------------------------------------
ofPoint ofxSelfOrganizingMap::findBestMatchingCell(double *instance) const {
 
    ofPoint minDistPt(0,0,0);
    double instanceDist, minDist = numFeatures;
    
    for (int i = 0; i < dimx; i++) {
        for (int j = 0; j < dimy; j++) {
            for (int k = 0; k < dimz; k++) {
                instanceDist = 0;
                for (int z = 0; z < numFeatures; z++)
                    instanceDist += pow(ofMap(map[i][j][k][z] - instance[z], minInstance[z], maxInstance[z], 0.0, 1.0), 2);
                if (instanceDist < minDist) {
                    minDistPt.set(i, j, k);
                    minDist = instanceDist;
                }
            }
        }
    }
    return minDistPt;
}

//--------------------------------------------------------------
void ofxSelfOrganizingMap::updateMap(double *instance) {
    if (t < numIterations) {
        radius = initialRadius * exp( -t / lambda );
        learningRate = initialLearningRate * exp( -t / lambda );        
        ofPoint c = findBestMatchingCell(instance);    
        for (int i = 0; i < dimx; i++) {
            for (int j = 0; j < dimy; j++) {
                for (int k = 0; k < dimz; k++) {  
                    if (distLUT[ (int)abs(i-c.x) ][ (int)abs(j-c.y) ][ (int)abs(k-c.z) ] < radius) {
                        float phi = exp(-pow( distLUT[(int)abs(i-c.x)][(int)abs(j-c.y)][ (int)abs(k-c.z) ], 2) / ( 2 * pow(radius, 2)));                        
                        for (int z = 0; z < numFeatures; z++) {
                            map[i][j][k][z] += phi * learningRate * (instance[z] - map[i][j][k][z]);
                        }   
                    }
                }
            }
        }
        t++;
    }
}

//-------------------------------------------
ofxSelfOrganizingMap::~ofxSelfOrganizingMap() {
    // cleanup map
    for (int i = 0 ; i< dimx; i++) {
        for (int j = 0; j < dimy; j++) {
            for (int k = 0; k < dimz; k++) {
                delete [] map[i][j][k];
            }
            delete [] map[i][j];
        }
        delete [] map[i];
    }
    delete [] map;
    
    // cleanup distLUT
    for (int i = 0 ; i < dimx; i++) {        
        for (int j = 0 ; j < dimy; j++) {        
            delete [] distLUT[i][j];
        }
        delete [] distLUT[i];
    }
    delete [] distLUT;
    
    delete [] minInstance;
    delete [] maxInstance;
}

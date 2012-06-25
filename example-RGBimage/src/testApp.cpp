#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {  
    
    // In this example, we are making an SOM from an image file.  We are
    // using the image's RGB values as the features, and will be creating
    // a 2D map of the color distribution
    for (int i = 0; i < 5; i++) {
        sourcePaths[i] = "flower" + ofToString(i+1) + ofToString(".jpg");
        sourceImgThumbs[i].loadImage(sourcePaths[i]);
    }
    sourceImg.loadImage(sourcePaths[0]);
    somImg.allocate(256, 256, OF_IMAGE_COLOR);
    
    // Before setting up the SOM, you must tell it the size of the
    // feature vector and the minimum and maximum values of each element
    double minInstance[3] = { 0, 0, 0 };
    double maxInstance[3] = { 255, 255, 255 };
    som.setFeaturesRange(3, minInstance, maxInstance);

    // Optional step: the initial learning rate is 0.1 by default, and the 
    // number of iterations is 3000 by default.  In most cases, you 
    // won't need to change this. 
    // The initial learning rate controls how aggressive the map
    // update process is and the number of iterations is self-explanatory.
    // By setting the former higher and the latter lower, your map will
    // converge faster, but it may not converge on the most optimal
    // solution.  A lower learning rate with more iterations will usually
    // create a smoother map.
    // If you are seeing fuzziness or noisiness in any region of the map, 
    // it means that you should increase either the rate or number of iterations
    // because the SOM did not have enough time to smooth out the noise.
    som.setInitialLearningRate(0.1);
    som.setNumIterations(3000);

    // After you are with options, you initialize the SOM with setup
    som.setup();
}

//--------------------------------------------------------------
void testApp::update() {
    
    // To train the SOM, we have to randomly sample instances from the
    // data we are modeling, in this case an RGB image.
    // The updateMap(instance) method will run exactly numIterations times, 
    // after which the method stops affecting the map.
    
    // randomly sample a color from the source image and update map
    ofColor samp = sourceImg.getColor( ofRandom(sourceImg.getWidth()), ofRandom(sourceImg.getHeight()));
    double instance[3] = { samp.r, samp.g, samp.b };
    som.updateMap(instance);

    // At any time, the map's value vector at a given point can be accessed
    // with the getMapAt method. In this example, we set somImg's pixels to those
    // vectors to see how the colors are distributed on the map.
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            double * c = som.getMapAt(i,j);
            ofColor col(c[0], c[1], c[2]);
            somImg.setColor(i, j, col);
        }
    }
    somImg.update();
}

//--------------------------------------------------------------
void testApp::draw() {
    ofBackground(0);
    ofDrawBitmapString("Source image", 30, 20);
    ofDrawBitmapString("Self-organizing map: Iteration " + ofToString(som.getCurrentIteration()) + "/" + ofToString(som.getNumIterations()), 530, 20);
    sourceImg.draw(20, 30, 480, 480);
    somImg.draw(520, 30, 480, 480);
    
    // draw thumbnails
    ofDrawBitmapString("Click one of the thumbnails or drag another image file into the application to change the source", 120, 540);
    for (int i = 0; i < 5; i++)
        sourceImgThumbs[i].draw(25 + 195*i, 550, 185, 185);    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    for (int i = 0; i < 5; i++) {
        if (x>25+195*i && x<210+195*i && y>540 && y<725) {
            sourceImg.loadImage(sourcePaths[i]);
            som.setup();
        }
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo info){
	if (info.files.size()>0) {
        sourceImg.loadImage(info.files[0]);
        som.setup();
	}
}
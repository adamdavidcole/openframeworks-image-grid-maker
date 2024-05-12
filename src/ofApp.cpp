#include "ofMain.h"
#include "ofApp.h"



void ofApp::setup(){
    cellWidth = 896;
    cellHeight = 512;
    
    numRows = 7;
    numColumns = 26;
    
    
    
    canvasWidth = cellWidth * numRows; // Set your canvas width
    canvasHeight = cellHeight * numColumns; // Set your canvas height
    
    fbo.allocate( canvasWidth, canvasHeight);


    gridWidth = cellWidth;
    gridHeight = cellHeight;
    
    gui.setup(); // Set up the GUI

    // Set up the slider
   // Initial value, min value, max value
    gui.add(skipSlider.setup("Skip", 1, 1, 10));
    skipSlider.addListener(this, &ofApp::skipSliderChanged);
    skip = 1; // Initial skip value
    
    gui.add(startFrameSlider.setup("Start frame", 0, 0, 1000, 1));
    startFrameSlider.addListener(this, &ofApp::startFrameSliderChanged);
    startFrame = 0; // Initial skip value
    
    gui.add(endFrameSlider.setup("End frame", 0, 0, 1000, 1));
    endFrameSlider.addListener(this, &ofApp::endFrameSliderChanged);
    endFrame = 0; // Initial skip value
    
    ofDirectory dir("images/");
    dir.listDir();

   // Add directories to the dropdown menu
   for(auto & d : dir.getFiles()){
       if(d.isDirectory()){
           ofParameter<string> dirParam;
           dirParam.set(d.getBaseName(), d.getBaseName());
           ofLog() << "directory name: " << d.getBaseName();
           
           dirParam.addListener(this, &ofApp::dirParamChanged);
           
           dirGroup.add(dirParam);
       }
   }
    gui.add(dirGroup);

    
    
    loadImages("images/controlnet-02-openpose");
    createImage();
}

void ofApp::dirParamChanged(string & value) {
    ofLog() << "dirParamChanged: " << value;
    loadImages("images/" + value);
}

void ofApp::loadImages(string path) {
    ofLog() << "loadImages from: " << path;
    dir.listDir(path); // Set your directory
    dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order

    //allocate the vector to have as many ofImages as files
    if( dir.size() ){
        images.assign(dir.size(), ofImage());
    }
    
    totalImageCount = (int)dir.size();

    // you can now iterate through the files and load them into the ofImage vector
    for(int i = 0; i < totalImageCount; i++){
        images[i].load(dir.getPath(i));
    }
    
    ofLog() << "images loaded";
    
    createImage();
}

void ofApp::update(){
}

void ofApp::createImage() {
    // Create a large FBO (Frame Buffer Object)

    fbo.begin();
    ofClear(255,255,255, 0); // Clear the FBO


    int imageCount = 0;
    for (int j = 0; j < numColumns; j++) {
        for (int i = 0; i < numRows; i++) {
            if (imageCount > totalImageCount - endFrame) imageCount = 0;
            if (imageCount == 0) imageCount += startFrame;
            
            int x = i * cellWidth;
            int y = j * cellHeight;
        
            images[imageCount % totalImageCount].draw(x, y, cellWidth, cellHeight);
            
    
            
            imageCount += skip;
            
            
        }
    }
    
    fbo.end();
}

void ofApp::draw(){
    gui.draw(); // Draw the GUI
    
    // Draw the large image (scaled to maintain aspect ratio)
   ofSetColor(255);
   float width = ofGetWidth();
   float height = ofGetHeight();
   float imageAspect = fbo.getWidth() / fbo.getHeight();
   float windowAspect = width / height;

   if (windowAspect > imageAspect) {
       // Window is wider than image, scale by height
       width = height * imageAspect;
   } else {
       // Window is taller than image, scale by width
       height = width / imageAspect;
   }

    fbo.draw((ofGetWidth() - width) / 2, (ofGetHeight() - height) / 2, width, height);
}

void ofApp::skipSliderChanged(int & value) {
    skip = value;
    
    ofLog() << "skip count is " << value;
    createImage();

}


void ofApp::startFrameSliderChanged(int & value) {
    startFrame = value;
    
    ofLog() << "start frame is " << startFrame;
    createImage();
}

void ofApp::endFrameSliderChanged(int & value) {
    endFrame = value;
    
    ofLog() << "end frame is " << endFrame;
    createImage();
}


//#include "ofApp.h"
//
////--------------------------------------------------------------
//void ofApp::setup(){
//
//}
//
////--------------------------------------------------------------
//void ofApp::update(){
//
//}
//
////--------------------------------------------------------------
//void ofApp::draw(){
//
//}
//
////--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 's') {
        ofPixels pixels;
        fbo.readToPixels(pixels);
        ofSaveImage(pixels, "controlnet-print-" + ofGetTimestampString() + ".png");
    }
}
//
////--------------------------------------------------------------
//void ofApp::keyReleased(int key){
//
//}
//
////--------------------------------------------------------------
//void ofApp::mouseMoved(int x, int y ){
//
//}
//
////--------------------------------------------------------------
//void ofApp::mouseDragged(int x, int y, int button){
//
//}
//
////--------------------------------------------------------------
//void ofApp::mousePressed(int x, int y, int button){
//
//}
//
////--------------------------------------------------------------
//void ofApp::mouseReleased(int x, int y, int button){
//
//}
//
////--------------------------------------------------------------
//void ofApp::mouseEntered(int x, int y){
//
//}
//
////--------------------------------------------------------------
//void ofApp::mouseExited(int x, int y){
//
//}
//
////--------------------------------------------------------------
//void ofApp::windowResized(int w, int h){
//
//}
//
////--------------------------------------------------------------
//void ofApp::gotMessage(ofMessage msg){
//
//}
//
////--------------------------------------------------------------
//void ofApp::dragEvent(ofDragInfo dragInfo){ 
//
//}

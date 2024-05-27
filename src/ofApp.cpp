#include "ofMain.h"
#include "ofApp.h"



void ofApp::setup(){
    scale = 0.1;
    cellWidth = 896 * scale;
    cellHeight = 512 * scale;
    
    numRows = 7;
    numColumns = 20;
    
    animationStartTime = -1;
    animationTotalDuration = 10.0;
    
    canvasWidth = cellWidth * numRows; // Set your canvas width
    canvasHeight = cellHeight * numColumns; // Set your canvas height
    
    fbo.allocate( canvasWidth, canvasHeight);


    gridWidth = cellWidth;
    gridHeight = cellHeight;
    
    gui.setDefaultWidth(300);
    gui.setup(); // Set up the GUI

    // Set up the slider
   // Initial value, min value, max value
    gui.add(skipSlider.setup("Skip", 1, 1, 15));
    skipSlider.addListener(this, &ofApp::skipSliderChanged);
    skip = 1; // Initial skip value
    
    gui.add(startFrameSlider.setup("Skip Start frame", 0, 0, 100, 1));
    startFrameSlider.addListener(this, &ofApp::startFrameSliderChanged);
    startFrame = 0; // Initial skip value
    
    endFrame = 2; // Initial skip value
    gui.add(endFrameSlider.setup("Skip End frame", endFrame, 0, 100, 1));
    endFrameSlider.addListener(this, &ofApp::endFrameSliderChanged);

    gui.add(flipHorizontalCheckbox.setup("Flip horizontal", false));
    shouldFlipHorizontal = false;
    flipHorizontalCheckbox.addListener(this, &ofApp::flipHorizontalToggleChanged);
    
    
    shiftStartFrame = 0; // Initial skip value
    gui.add(shiftStartFrameSlider.setup("Shift Start Frame", shiftStartFrame, -300, 300, 1));
    shiftStartFrameSlider.addListener(this, &ofApp::shiftStartFrameSliderChanged);

    
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

    
    
    loadImages("images/ControlNetWrestle_004");
    createImage();
}

void ofApp::dirParamChanged(string & value) {
    ofLog() << "dirParamChanged: " << value;
    loadImages("images/" + value);
}

void ofApp::flipHorizontalToggleChanged(bool & value) {
    ofLog() << "flipHorizontalToggleChanged: " << value;
    shouldFlipHorizontal = value;
    
    createImage();
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

    int imageCount = shiftStartFrame;
    if (imageCount < 0) {
        imageCount = totalImageCount + shiftStartFrame;
    }
    for (int j = 0; j < numColumns; j++) {
        for (int i = 0; i < numRows; i++) {
            if (imageCount > totalImageCount - endFrame) imageCount = 0;
            if (imageCount == 0) imageCount += startFrame;
            
            ofImage img = images[imageCount % totalImageCount];
            
            int x = i * cellWidth;
            int y = j * cellHeight;
            
            img.mirror(false, shouldFlipHorizontal);
            
        
            img.draw(x, y, cellWidth, cellHeight);
    
            
            imageCount += skip;
            
            
        }
    }
    
    fbo.end();
}

float easeOut(float t) {
    return 1 - pow(1 - t, 3);
}

void ofApp::draw(){
   gui.draw(); // Draw the GUI
    
    if (animationStartTime > 0) {
        float t = (ofGetElapsedTimef() - animationStartTime) / animationTotalDuration;
        if (t > 1) {
            t = 1;
            animationStartTime = -1;
        }
        
        float easedT = easeOut(t);
        
        ofLog() << easedT << "\n";
        
        int startFrame = ofMap(easedT, 0, 1, 0, totalImageCount);
        shiftStartFrame = startFrame;
        createImage();
    }
    
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

void ofApp::shiftStartFrameSliderChanged(int & value) {
    shiftStartFrame = value;
    
    ofLog() << "shiftStartFrame " << shiftStartFrame;
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
    
    if (key == 'm') {
        animationStartTime = ofGetElapsedTimef();
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

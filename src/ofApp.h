#pragma once

#include "ofMain.h"
#include "ofxGui.h"


class ofApp : public ofBaseApp{
    public:
    void setup();
    void update();
    void draw();
    void createImage();

    ofxPanel gui;
    ofxIntSlider skipSlider;
    int skip;
    void skipSliderChanged(int & value);
    
    ofxIntSlider startFrameSlider;
    int startFrame;
    void startFrameSliderChanged(int & value);
    
    ofxIntSlider shiftStartFrameSlider;
    int shiftStartFrame;
    void shiftStartFrameSliderChanged(int & value);
    
    ofxIntSlider endFrameSlider;
    int endFrame;
    void endFrameSliderChanged(int & value);
    
    ofxToggle flipHorizontalCheckbox;
    bool shouldFlipHorizontal;
    void flipHorizontalToggleChanged(bool & value);
    
    ofParameterGroup dirGroup;
    void dirParamChanged(string & value);
    
    
    int canvasWidth;
    int canvasHeight;
    
    float scale;
    int cellWidth;
    int cellHeight;
    int numRows;
    int numColumns;
    int totalImageCount;
    
    float animationStartTime;
    float animationTotalDuration;
    

    int gridWidth;
    int gridHeight;
    vector<ofImage> images;
    ofDirectory dir;

    ofFbo fbo;
    ofImage largeImage;
    
    void keyPressed(int key);
    void loadImages(string path);

};

//class ofApp : public ofBaseApp{
//
//	public:
//		void setup();
//		void update();
//		void draw();
//
//		void keyPressed(int key);
//		void keyReleased(int key);
//		void mouseMoved(int x, int y );
//		void mouseDragged(int x, int y, int button);
//		void mousePressed(int x, int y, int button);
//		void mouseReleased(int x, int y, int button);
//		void mouseEntered(int x, int y);
//		void mouseExited(int x, int y);
//		void windowResized(int w, int h);
//		void dragEvent(ofDragInfo dragInfo);
//		void gotMessage(ofMessage msg);
//		
//};

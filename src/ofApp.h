#pragma once

#include "ofMain.h"
#include "ofxKinectForWindows2.h"
#include "Menu.h"

class ofApp : public ofBaseApp{

public:
	typedef map<JointType, ofVec2f> jointMap2f;

	void setup();
	void update();
	void draw();

	int getCentralBodyIdx();
	void mirrorJoints2f(jointMap2f& jointMap, int w=1920);
	void drawHandState(HandState state, ofVec2f pos, int radius=50);

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	ofxKFW2::Device kinect;
	bool bStreams;
	ofImage kColImg;
	ICoordinateMapper* coordinateMapper;

	jointMap2f joints2dMirror;
	ofColor jointColor = ofColor::green;

	int numTracked = 0, bodyIdx = -1;
	
	Menu menu;
};

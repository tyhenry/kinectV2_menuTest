#pragma once

#include "ofMain.h"
#include "ofxKinectForWindows2.h"
#include "ofxGui.h"
#include "User.h"
#include "Menu.h"

class ofApp : public ofBaseApp{

public:
	//typedef map<JointType, ofVec2f> jointMap2f;

	void setup();
	void update();
	void draw();

	int getCentralBodyIdx(float depthThreshFar = 100.f);
	//void mirrorJoints2f(jointMap2f& jointMap, int w=1920);
	//void drawHandState(HandState state, ofVec2f pos, int radius=50);

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

	//jointMap2f joints2dMirror;
	//ofColor jointColor = ofColor::green;
	//HandState leftHandState;
	//HandState rightHandState;

	int numTracked = 0, bodyIdx = -1;
	User user;
	Menu menu;

	ofxPanel gui;
	bool bDrawGui = false;
	bool bDrawVertScreen = false;

	ofVec2f btn2pos;
	ofVec2f btn3posDown; ofVec2f btn3posUp;
	bool scrollDown = true;

	bool handsUp = false;
};

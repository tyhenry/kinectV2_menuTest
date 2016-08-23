#pragma once
#include "ofMain.h"
#include "ofxKinectForWindows2.h"

class User {
protected:
	struct HandStates {
		HandState left = HandState_Unknown;
		HandState right = HandState_Unknown;
	};
	struct Joint2fData {
		ofVec2f pos = ofVec2f(0, 0);
		TrackingState state = TrackingState_NotTracked;
	};

	typedef map<JointType, Joint2fData> JointMap2f;
	typedef ofxKFW2::Data::Body kBody;

public:

	User(ICoordinateMapper* coordinateMapper)
		: _coordMapper(coordinateMapper) {}

	void setCoordinateMapper(ICoordinateMapper* coordinateMapper)
		{ _coordMapper = coordinateMapper;  }

	bool setBody(const kBody* body); // returns false if same body
	bool update(); // false is _bodyPtr is nullptr

	bool jointExists(JointType type) { return (_joints2d.find(type) != _joints2d.end()); }
	ofVec2f getJoint2dPos(JointType type);
	TrackingState getTrackingState(JointType type);
	HandState getLeftHandState();
	HandState getRightHandState();

	void draw(ofVec2f pos, float width, float height);
	void draw() { draw(ofVec2f(0, 0), 1920, 1080); }
	void drawJoints2d(ofVec2f pos, float width, float height);
	void drawJoints2d() { drawJoints2d(ofVec2f(0, 0), 1920, 1080); }
	void drawHandStates(ofVec2f pos, float width, float height);
	void drawHandStates() { drawHandStates(ofVec2f(0, 0), 1920, 1080); }
	void drawHandState(JointType hand, ofVec2f pos, float width, float height);
	void drawHandState(JointType hand) { drawHandState(hand, ofVec2f(0, 0), 1920, 1080); }

protected:

	const kBody* _bodyPtr;
	ICoordinateMapper* _coordMapper;
	JointMap2f _joints2d; // joints positions in 2d colorcam space
	JointMap2f _pJoints2d; // previous frame
	HandStates _handStates; // left, right
	HandStates _pHandStates; // previous frame
};

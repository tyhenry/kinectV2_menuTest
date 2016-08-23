#include "User2.h"

bool User::setBody(const kBody* body) {
	if (body == _bodyPtr) {
		_bodyPtr = body;
		return true;
	}
	return false;
}

bool User::update() {
	if (_bodyPtr == nullptr) return false;
	auto& joints = _bodyPtr->joints;

	// save previous joint positions
	_pJoints2d = _joints2d;
	// calc new joint positions (world > color coords)
	_joints2d.clear();
	for (auto& joint : joints) {
		ofVec2f& p = _joints2d[joint.second.getType()].pos
			= ofVec2f();

		TrackingState& state = _joints2d[joint.second.getType()].state
			= joint.second.getTrackingState();

		if (state == TrackingState_NotTracked) continue;

		p.set(joint.second.getProjected(_coordMapper));
	}
	// save previous hand states
	_pHandStates = _handStates;
	// get new hand states
	_handStates.left = _bodyPtr->leftHandState;
	_handStates.right = _bodyPtr->rightHandState;
	return true;
}

ofVec2f User::getJoint2dPos(JointType type) {
	return _joints2d[type].pos;
}
TrackingState User::getTrackingState(JointType type) {
	return _joints2d[type].state;
}
HandState User::getLeftHandState() {
	return _handStates.left;
}
HandState User::getRightHandState() {
	return _handStates.right;
}

void User::draw(ofVec2f pos, float width, float height) {
	drawJoints2d(pos, width, height);
	drawHandStates(pos, width, height);
}

void User::drawJoints2d(ofVec2f pos, float width, float height) {
	// draw joints
	for (auto& joint : _joints2d) {

		const JointType& jType = joint.first;
		ofVec2f& jPos = joint.second.pos;
		TrackingState& jState = joint.second.state;

		if (jState == TrackingState_NotTracked) continue;

		int rad = jState == TrackingState_Inferred ? 2 : 8;
		ofSetColor(ofColor::green);
		ofPushMatrix();
		ofTranslate(pos);
		ofScale(width / 1920, height / 1080);
		ofDrawCircle(jPos, rad);
		ofPopMatrix();
	}
}

void User::drawHandStates(ofVec2f pos, float width, float height) {
	drawHandState(JointType_HandLeft, pos, width, height);
	drawHandState(JointType_HandRight, pos, width, height);
}

void User::drawHandState(JointType hand, ofVec2f pos, float width, float height) {
	HandState hState; ofVec2f hPos;

	// determine hand
	if (hand == JointType_HandLeft) hState = _handStates.left;
	else if (hand == JointType_HandRight) hState == _handStates.right;
	else return;

	// get pos
	if (jointExists(hand)) {
		hPos = _joints2d[hand].pos;
	}
	else return;

	// determine state
	ofColor col;
	switch (hState) {
	case HandState_Unknown: case HandState_NotTracked:
		return;
	case HandState_Open:
		col = ofColor(0, 255, 0, 80);
		break;
	case HandState_Closed:
		col = ofColor(255, 255, 0, 80);
		break;
	case HandState_Lasso:
		col = ofColor(0, 255, 255, 80);
		break;
	}
	// draw
	int radius = 50;
	ofPushMatrix();
	ofTranslate(pos);
	ofScale(width / 1920, height / 1080);
	ofPushStyle();
	ofEnableAlphaBlending();
	ofSetColor(col);
	ofDrawCircle(pos, radius);
	ofDisableAlphaBlending();
	ofPopStyle();
	ofPopMatrix();
}


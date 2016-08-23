#include "User.h"

bool User::setBody(kBody* body) {
	if (_bodyPtr != body) {
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

bool User::jointExists(JointType type, bool prev) {
	if (prev) return (_pJoints2d.find(type) != _pJoints2d.end());
	return (_joints2d.find(type) != _joints2d.end());
}

ofVec2f User::getJoint2dPos(JointType type, bool prev) {
	ofVec2f pos(0, 0);
	if (prev && jointExists(type,true)) pos = _pJoints2d[type].pos;
	else if (!prev && jointExists(type)) pos = _joints2d[type].pos;
	return pos;
}
TrackingState User::getTrackingState(JointType type, bool prev) {
	TrackingState state = TrackingState_NotTracked;
	if (prev && jointExists(type, true)) state = _pJoints2d[type].state;
	else if (!prev && jointExists(type)) state = _joints2d[type].state;
	return state;
}
HandState User::getLeftHandState(bool prev) {
	return prev ? _pHandStates.left : _handStates.left;
}
HandState User::getRightHandState(bool prev) {
	return prev ? _pHandStates.right : _handStates.right;
}

bool User::isRightHandUp() {
	if (!jointExists(JointType_HandRight) || !jointExists(JointType_Head)
		|| _bodyPtr == nullptr)
		return false;
	if (getJoint2dPos(JointType_HandRight).y < getJoint2dPos(JointType_Head).y)
		return true;
	return false;
}

bool User::isLeftHandUp() {
	if (!jointExists(JointType_HandLeft) || !jointExists(JointType_Head)
		|| _bodyPtr == nullptr)
		return false;
	if (getJoint2dPos(JointType_HandLeft).y < getJoint2dPos(JointType_Head).y)
		return true;
	return false;
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
	HandState hState = HandState_Unknown; ofVec2f hPos(0,0);

	// determine hand
	if (hand == JointType_HandLeft) hState = _handStates.left;
	else if (hand == JointType_HandRight) hState = _handStates.right;
	else {
		ofLogError("User::drawHandState") << "JointType parameter not a hand!";
		return;
	}
	// get pos
	if (jointExists(hand)) hPos = _joints2d[hand].pos;
	else return;

	// determine state
	ofColor col;
	switch (hState) {
	case HandState_NotTracked:
		return;
	case HandState_Unknown: 
		col = ofColor(255,0,0,80);
		break;
		//return;
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
	ofDrawCircle(hPos, radius);
	ofDisableAlphaBlending();
	ofPopStyle();
	ofPopMatrix();
}

void User::clear() {
  	_bodyPtr = nullptr;
	_joints2d.clear();
	_pJoints2d.clear();
	_handStates = HandStates();
	_pHandStates = HandStates();
}


#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofBackground(0);
	ofSetWindowShape(1920, 1080);
	
	kinect.open();
	kinect.initColorSource();
	kinect.initBodySource();

	if (kinect.getSensor()->get_CoordinateMapper(&coordinateMapper) < 0) {
		ofLogError("setup") << "couldn't aquire coordinate mapper!";
	}
	else {
		ofLogNotice("setup") << "aquired coordinate mapper";
	}
	
	bStreams = false;
	kColImg.allocate(1920, 1080, OF_IMAGE_COLOR);
	ofSetColor(255);

	menu = Menu("Karla-Bold.ttf", 16);
	int nBtns = 4;
	float btnW = 200;
	float btnH = 80;
	float spaceH = 40;
	float btnX = ofGetWidth()*0.75f;
	float btnY = ofGetHeight()*0.5f - (nBtns*btnH + (nBtns - 1)*spaceH)*0.5f;

	for (int i = 0; i < nBtns; i++) {
		string label = "button " + ofToString(i+1);
		menu.addButton(label, ofVec2f(btnX, btnY), btnW, btnH, ofColor::fromHsb((int)ofRandom(255), 255, 100));
		btnY += btnH + spaceH;
	}
}

//--------------------------------------------------------------
void ofApp::update(){

	kinect.update();
	ofPixels& colPix = kinect.getColorSource()->getPixels();
	if (!colPix.size()) {
		bStreams = false;
		ofLogVerbose("update") << "not reading streams from kinect yet";
		return;
	}
	else {
		kColImg.setFromPixels(colPix); // is mirrored from kinect!
		bStreams = true;
	}

	numTracked = 0;
	for (auto& body : kinect.getBodySource()->getBodies()) {
		if (body.tracked) numTracked++;
	}
	bodyIdx = -1;
	if (numTracked > 0) {
		// get centermost body
		bodyIdx = getCentralBodyIdx();

		if (bodyIdx >= 0) {

			auto& joints = kinect.getBodySource()->getBodies()[bodyIdx].joints;

			// convert world to screen
			joints2dMirror.clear();
			for (auto& joint : joints) {
				ofVec2f& p = joints2dMirror[joint.second.getType()] = ofVec2f();

				TrackingState state = joint.second.getTrackingState();
				if (state == TrackingState_NotTracked) continue;

				p.set(joint.second.getProjected(coordinateMapper));
			}
			// mirror joint positions along x
			// mirrorJoints2f(joints2dMirror); // already mirrored!
		}

		menu.mousePress(joints2dMirror[JointType_HandRight]);
	}

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetColor(255);
	/* draw vertical crop
	ofTexture& tex = kColImg.getTexture();
	float sW = kColImg.getHeight() / 1.77777778f;
	float sX = kColImg.getWidth()*0.5f - sW*0.5f;
	tex.drawSubsection(sX, 0, sW, kColImg.getHeight(), sX, 0, sW, kColImg.getHeight());
	*/
	kColImg.draw(0, 0, 1920, 1080);

	if (numTracked > 0 && bodyIdx >= 0) {

		auto& body = kinect.getBodySource()->getBodies()[bodyIdx];

		// draw joints
		for (auto& joint : joints2dMirror) {

			const JointType& type = joint.first;
			ofVec2f& pos = joint.second;

			TrackingState state = body.joints.at(type).getTrackingState();
			if (state == TrackingState_NotTracked) continue;

			int rad = state == TrackingState_Inferred ? 2 : 8;
			ofSetColor(jointColor);
			ofDrawCircle(pos, rad);
		}

		// draw hand states
		drawHandState(body.leftHandState, joints2dMirror[JointType_HandLeft]);
		drawHandState(body.rightHandState, joints2dMirror[JointType_HandRight]);
	}

	menu.draw();

	stringstream infoSs;
	infoSs << "num tracked: " << ofToString(numTracked) << endl
		<< "body idx: " << ofToString(bodyIdx) << endl
		<< "fps: " << ofToString(ofGetFrameRate()) << endl;
	ofDrawBitmapStringHighlight(infoSs.str(), 10, 10);

}

//--------------------------------------------------------------
int ofApp::getCentralBodyIdx() {

	auto & bodies = kinect.getBodySource()->getBodies();

	int closestIdx = -1;
	float closestDist = 99999.f;
	for (int i = 0; i < bodies.size(); i++) {
		if (!bodies[i].tracked) continue; // skip untracked body
		float dist = abs(bodies[i].joints.at(JointType_SpineBase).getPosition().x);
		if (dist < closestDist) {
			closestIdx = i;
			closestDist = dist;
		}
	}
	return closestIdx;
}

//--------------------------------------------------------------
void ofApp::mirrorJoints2f(jointMap2f& jointMap, int w) {

	for (auto& joint : jointMap) {
		joint.second.x = w - joint.second.x;
	}
}

//--------------------------------------------------------------
void ofApp::drawHandState(HandState state, ofVec2f pos, int radius) {

	ofColor col; bool tracked = true;
	switch (state) {
	case HandState_Unknown: case HandState_NotTracked:
		tracked = false;
		break;
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
	if (tracked) {
		ofPushStyle();
		ofEnableAlphaBlending();
		ofSetColor(col);
		ofDrawCircle(pos, radius);
		ofDisableAlphaBlending();
		ofPopStyle();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

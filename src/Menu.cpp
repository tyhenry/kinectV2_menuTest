#include "Menu.h"

Menu::Menu(string pathToFont, int fontSize) {
	font.load(ofToDataPath(pathToFont), fontSize);
	stringFbo.allocate(800, 400);
}

int Menu::addButton(string label, ofVec2f pos, float w, float h,
	ofColor bgColor, ofColor labelColor, ofColor highlightColor, ofColor pressColor) {

	mButtons.push_back(Button());
	Button& btn = mButtons.back();
	btn.label = label;
	btn.rect = ofRectangle(pos, w, h);
	btn.bgColor = bgColor;
	btn.labelColor = labelColor;
	btn.highlightColor = highlightColor;
	btn.pressColor = pressColor;

	return (mButtons.size() - 1);
}

bool Menu::setButtonPos(int idx, ofVec2f pos) {

	if (idx >= mButtons.size() || idx < 0) return false;
	mButtons[idx].rect.position = pos; return true;
}

bool Menu::setButtonSize(int idx, float w, float h) {

	if (idx >= mButtons.size() || idx < 0) return false;
	mButtons[idx].rect.setSize(w, h); return true;
}

bool Menu::setButtonLabel(int idx, string label) {
	if (idx >= mButtons.size() || idx < 0) return false;
	mButtons[idx].label = label; return true;
}

bool Menu::setButtonBgColor(int idx, ofColor bgColor) {
	if (idx >= mButtons.size() || idx < 0) return false;
	mButtons[idx].bgColor = bgColor; return true;
}

bool Menu::setButtonLabelColor(int idx, ofColor labelColor) {
	if (idx >= mButtons.size() || idx < 0) return false;
	mButtons[idx].labelColor = labelColor; return true;
}

bool Menu::setButtonPressColor(int idx, ofColor pressColor) {
	if (idx >= mButtons.size() || idx < 0) return false;
	mButtons[idx].pressColor = pressColor; return true;
}

bool Menu::setButtonHighlightColor(int idx, ofColor highlightColor) {
	if (idx >= mButtons.size() || idx < 0) return false;
	mButtons[idx].highlightColor = highlightColor; return true;
}

ofVec2f Menu::getButtonPos(int idx) {
	if (idx >= mButtons.size() || idx < 0) return ofVec2f(0, 0);
	return mButtons[idx].rect.position;
}

bool Menu::getButton(int idx, Button& btn) {
	if (idx >= mButtons.size() || idx < 0) return false;
	btn = mButtons[idx]; return true;
}

int Menu::hover(ofVec2f pos) {
	if (mHoveredBtn == hitTestBtns(pos)) { // still hovering
		mHoverSecs = ofGetElapsedTimef() - mHoverStart;
	}
	else {
		mHoveredBtn = hitTestBtns(pos); // get hover idx
		if (mHoveredBtn != -1) { // new button hover
			mHoverStart = ofGetElapsedTimef();
			mHoverSecs = 0;
		}
		else { // no hover button
			mHoverStart = 0;
			mHoverSecs = 0;
		}
	}

	mPressedBtn = -1;
	mPressOffset = ofVec2f(0, 0);
	return mHoveredBtn;
}

int Menu::press(ofVec2f pos) {
	mPressedBtn = hitTestBtns(pos);
	if (mPressedBtn != -1) mPressOffset = pos - mButtons[mPressedBtn].rect.getPosition();
	else mPressOffset = ofVec2f(0, 0);
	return mPressedBtn;
}

int Menu::drag(ofVec2f pos) {
	if (mPressedBtn == -1) {
		mDragPos = ofVec2f(0, 0);
		return -1;
	}
	mDragPos = pos;
	return mPressedBtn;
}

int Menu::release() {
	int tmp = mPressedBtn;
	mPressedBtn = -1;
	return tmp;
}

int Menu::hitTestBtns(ofVec2f pos) {
	int idx = -1;
	for (int i=mButtons.size()-1; i>=0; i--){ // last to first (i.e. reverse draw order)
		if (mButtons[i].rect.inside(pos)) {
			idx = i;
			break;
		}
	}
	return idx;
}

bool Menu::deleteButton(int idx) {
	if (idx >= mButtons.size() || idx < 0) return false;
	mButtons.erase(mButtons.begin() + idx); return true;
}

void Menu::draw() {
	for (int i = 0; i < mButtons.size(); i++) {
		drawButton(i);
	}
}

bool Menu::drawButton(int idx) {
	if (idx >= mButtons.size() || idx < 0) return false;
	Button& btn = mButtons[idx];
	ofPushStyle();
	// draw highlight
	if (mHoveredBtn == idx) {
		ofSetColor(btn.highlightColor);
		ofNoFill();
		ofSetLineWidth(7);
		ofDrawRectangle(btn.rect.getPosition() - ofVec2f(5, 5), btn.rect.width + 5, btn.rect.height + 5);
		ofFill();
	}
	// draw box
	ofColor& bgCol = mPressedBtn == idx ? btn.pressColor : btn.bgColor;
	ofSetColor(bgCol);
	ofDrawRectangle(btn.rect);
	// draw label
	ofSetColor(btn.labelColor);
	drawStringInBox(btn.label, btn.rect);
	ofPopStyle();
	return true;
}

void Menu::drawStringInBox(string s, ofRectangle& box) {
	float sW = font.stringWidth(s);
	float sH = font.stringHeight(s);

	float baseline = box.height*0.5f + sH*0.5f; // center on y
	float left = 5; // left align if text larger than box
	if (sW < box.width - 10.f) { // or if can be centered
		left = box.width*0.5f - sW*0.5f;
	}
	stringFbo.begin();
	{
		ofClear(255,0);
		font.drawString(s, left, baseline);
	}
	stringFbo.end();
	ofTexture& tex = stringFbo.getTexture();
	tex.drawSubsection(box.x,box.y+2,box.width-5,box.height-2,0,2,box.width-5,box.height-2);
}


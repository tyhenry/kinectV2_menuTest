#pragma once
#include "ofMain.h"

class Menu {
public:
	struct Button {
		ofRectangle rect;
		string label;
		ofColor bgColor;
		ofColor labelColor;
		ofColor pressColor;
	};

	Menu(string pathToFont="Karla-Bold.ttf", int fontSize = 11);
	
	int addButton(string label, ofVec2f pos, float w, float h,
					ofColor bgColor = ofColor::black,
					ofColor labelColor = ofColor::white,
					ofColor pressColor = ofColor::grey); // returns button idx
	bool setButtonPos(int idx, ofVec2f pos);
	bool setButtonSize(int idx, float w, float h);
	bool setButtonLabel(int idx, string label);
	bool setButtonBgColor(int idx, ofColor bgColor);
	bool setButtonLabelColor(int idx, ofColor labelColor);
	bool setButtonPressColor(int idx, ofColor pressColor);

	int getNumButtons() { return mButtons.size(); }
	bool getButton(int idx, Button& btn); 

	int mousePress(ofVec2f pos); // return idx of button pressed, -1 if none
	int mouseDrag(ofVec2f pos); // return idx of button pressed & dragged, -1 if none
	int mouseRelease(ofVec2f pos); // return idx of button depress, -1 if none

	bool isPressed(int idx) { return (idx == mPressedBtn); }
	int getPressed() { return mPressedBtn; } // -1 if none pressed

	int hitTestBtns(ofVec2f pos); // -1 if none
	bool deleteButton(int idx); // -1 if none
	void clear() { mButtons.clear(); } // clears all buttons

	void draw();
	bool drawButton(int idx);

protected:

	void drawStringInBox(string s, ofRectangle& box);
	
	vector<Button> mButtons;
	int mPressedBtn = -1;
	ofVec2f mDragPos = ofVec2f(0, 0);
	ofTrueTypeFont font;
	ofFbo stringFbo;
};
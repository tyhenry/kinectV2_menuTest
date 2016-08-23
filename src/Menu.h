#pragma once
#include "ofMain.h"

class Menu {
public:
	struct Button {
		ofRectangle rect;
		string label;
		ofColor bgColor;
		ofColor labelColor;
		ofColor highlightColor;
		ofColor pressColor;
	};

	Menu(string pathToFont="Karla-Bold.ttf", int fontSize = 11);
	
	int addButton(string label, ofVec2f pos, float w, float h,
					ofColor bgColor = ofColor::black,
					ofColor labelColor = ofColor::white,
					ofColor highlightColor = ofColor::white,
					ofColor pressColor = ofColor::grey); // returns button idx
	bool setButtonPos(int idx, ofVec2f pos);
	bool setButtonSize(int idx, float w, float h);
	bool setButtonLabel(int idx, string label);
	bool setButtonLabelColor(int idx, ofColor labelColor);
	bool setButtonBgColor(int idx, ofColor bgColor);
	bool setButtonPressColor(int idx, ofColor pressColor);
	bool setButtonHighlightColor(int idx, ofColor highlightColor);

	int getNumButtons() { return mButtons.size(); }
	bool getButton(int idx, Button& btn);

	int hover(ofVec2f pos); // return idx of button hovered, -1 if none
	int press(ofVec2f pos); // return idx of button pressed, -1 if none
	int drag(ofVec2f pos); // return idx of button pressed & dragged, -1 if none
	int release(); // return idx of button depress, -1 if none

	bool isPressed(int idx) { return (idx == mPressedBtn); }
	int getPressed() { return mPressedBtn; } // -1 if none pressed

	ofVec2f getDragPosMinusOffset() { return mDragPos - mPressOffset;  }

	int hitTestBtns(ofVec2f pos); // -1 if none
	bool deleteButton(int idx); // -1 if none
	void clear() { mButtons.clear(); } // clears all buttons

	void draw();
	bool drawButton(int idx);

protected:

	void drawStringInBox(string s, ofRectangle& box);
	
	vector<Button> mButtons;
	int mHoveredBtn = -1;
	int mPressedBtn = -1;
	ofVec2f mDragPos = ofVec2f(0, 0);
	ofVec2f mPressOffset = ofVec2f(0, 0); // distance from btn TL corner to press pos

	ofTrueTypeFont font;
	ofFbo stringFbo;
};
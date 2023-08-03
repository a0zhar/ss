#pragma once
#ifndef _MENU_H_
#define _MENU_H_
#include "types.h"
#include <utility>

#define MaxSubmenuLevels 20

typedef void(*Function)();
typedef void(*KeyboardHandler)(char*);

class Menu {
private:
	Function mainMenu;    // The main menu (first displayed when opening the in-game menu)
	Function currentMenu; // The current menu being displayed, either a submenu or another menu.
	// Array to store the last submenu at each level (not 100% sure)
	Function lastSubmenu[MaxSubmenuLevels];
	int submenuLevel;     // The current submenu level

	char* tipText;        // Text for displaying tips/instructions
	int currentOption;    // The current menu option being hovered over
	int optionCount;      // The number of options inside the menu
	// Array to store the last option at each level (not 100% sure)
	int lastOption[MaxSubmenuLevels];

	// PS4 Controller Interaction Events
	bool upPress;     // Was the upper D-pad button clicked?
	bool downPress;   // Was the lower D-pad button clicked?
	bool leftPress;   // Was the left D-pad button clicked?
	bool rightPress;  // Was the right D-pad button clicked?
	bool leftHold;    // Is the left D-pad button being held down?
	bool rightHold;   // Is the right D-pad button being held down?
	bool optionPress; // Was the option button clicked?
	bool squarePress; // Was the Square button clicked?

	int instructionCount;   // Count of instructions
	bool setupIntructionsThisFrame; // Indicator for setting up instructions in the current frame
	bool xInstruction; 	    // Instruction related to the 'X' button (not clear from the code)
	bool squareInstruction; // Instruction related to the 'Square' button (not clear from the code)
	bool lrInstruction;     // Instruction related to the left and right buttons (not clear from the code)

	bool keyboardActive;             // Is the keyboard active for input?
	KeyboardHandler keyboardHandler; // Function pointer to handle keyboard input

	template <typename T>
	struct scrollData {
		T* var;
		T min, max;
		int decimals;
	};
	static scrollData<int> intScrollData;
	static scrollData<float> floatScrollData;

	static void intScrollKeyboardHandler(char* text);
	static void floatScrollKeyboardHandler(char* text);

	void playSound(char* sound, char* ref = "HUD_PLAYER_MENU");

	// Enum for text alignment when drawing text (left, center, right)
	enum Alignment {
		Left,     // Align the text to the left inside the parent element
		Center,   // Center the text inside the parent element
		Right     // Align the text to the right inside the parent element
	};

	void drawText(char* text, Vector2 pos, int size, Font font, char* color, char* alignment, bool outline);
	void positionMenuText(char* text, float xPos, Alignment alignment);

	bool fastScrolling; 		  // Indicates whether fast scrolling is enabled
	bool colorEditing; 			  // Indicates whether color editing is enabled
	bool editingAlpha; 		  	  // Indicates whether alpha channel editing is enabled

	Color* colorToEdit;           // Pointer to the color being edited
	Function colorChangeCallback; // Callback function for color change
	void colorEditor();

public:
	bool open; 		            // Indicates whether the menu is open
	bool sounds; 	            // Indicates whether sounds are enabled
	bool instructions;          // Indicates whether instructions are enabled

	char* title;                // Title of the menu
	int maxOptions;             // Maximum number of options in the menu

	Color bannerColor;          // The Background Color of Banner (where the Menu Title Text is displayed)
	Color bannerTextColor;      // The Foreground Color of the Menu Title Text
	Color bodyColor;            // The color of the Body of our Menu (not title part)
	Color optionsActiveColor;   // The color of an enabled menu feature
	Color optionsInactiveColor; // The color of an disabled menu feature
	Color scrollerColor;
	Color indicatorColor;
	Color instructionsColor;

	Menu();
	Menu(Menu& menu);
	Menu(Function main);

	static void drawCenterNotification(char* text, int duration = 3000);
	static void drawFeedNotification(char* text, char* subtitle, char* title = "Menu Base");

	void monitorButtons();
	void run();

	void changeSubmenu(Function submenu);
	void openKeyboard(KeyboardHandler handler, int maxLength, char* defaultText = "");

	void banner(char* text);

	bool hovered();
	bool pressed();
	bool scrolled();

	Menu& option(char* text);
	void spacer(char* text);
	Menu& data(char* text);
	Menu& data(bool b);
	Menu& data(int i);
	Menu& data(float f, int decimalPlaces);

	Menu& scroller(int* i, int min, int max, bool fast, bool keyboard);
	Menu& scroller(float* f, float min, float max, float increment, int decimalPlaces, bool fast, bool keyboard);
	Menu& scroller(char** textArray, int* index, int numItems, bool fast);
	Menu& scroller(Font* font);

	Menu& toggle(bool* b);
	Menu& tip(char* text);
	Menu& submenu(Function sub);
	Menu& keyboard(KeyboardHandler handler, int maxLength, char* defaultText = "");
	Menu& editColor(Color* color, bool editAlpha, Function callback = nullptr);

	template<typename F, typename... Args> 
	Menu& call(F func, Args&&... args) {
		if (pressed()) func(std::forward<Args>(args)...);
		return *this;
	}
	Hash vehicleToSpawn;
	Menu& vehicleSpawn(Hash vehicleHash);
};

#endif
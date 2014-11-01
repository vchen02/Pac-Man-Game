#ifndef _USERINTERFACE_H_
#define _USERINTERFACE_H_

#include <string>

const int PAC_SOUND_SMALL_EAT = 0;
const int PAC_SOUND_BIG_EAT   = 1;
const int PAC_SOUND_START     = 2;
const int PAC_SOUND_DIE       = 3;
const int MON_EAT_YOU_UP	  = 4;
const int EPIC_FAIL			  = 5;
const int PSYCHO_CHASE		  = 6;
const int SCREAMING			  = 7;
const int SAD_TROMBONE		  = 8;
const int YEAH_BABY			  = 9;

const char ARROW_UP    = '8';
const char ARROW_DOWN  = '2';
const char ARROW_LEFT  = '4';
const char ARROW_RIGHT = '6';
const char USE_POWER   = '\r';


///////////////////////////////////////////////////////////
// Screen -- Visual Output
///////////////////////////////////////////////////////////

class Screen
{
public:
	Screen(int width, int height);
	~Screen();
	void updateStatus(std::string status);

private:
	int m_width;
	int m_height;
	std::string m_status;
};

///////////////////////////////////////////////////////////
// SoundFX -- Audio Output
///////////////////////////////////////////////////////////

class SoundFX
{
public:
	static void playNachManSound(int sound);
	static void playClip(std::string ClipFile);
	static void playTone(int hertz, int milliseconds);
};

///////////////////////////////////////////////////////////
// Functions for Keyboard Input
///////////////////////////////////////////////////////////

bool getCharIfAny(char& ch);
void waitForEnter();

///////////////////////////////////////////////////////////
// Functions for Timing
///////////////////////////////////////////////////////////

void delay();

#endif // _USERINTERFACE_H

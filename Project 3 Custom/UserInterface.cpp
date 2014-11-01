#include "UserInterface.h"

#include <windows.h>
#include "BGIgraphics.h"

#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

///////////////////////////////////////////////////////////
// Screen -- Visual Output
///////////////////////////////////////////////////////////

const int STATUS_X = 10;
const int STATUS_Y = 460;

  // Correct the bad interfaces in BGIgraphics.h
inline int textwidth(const char* s) { return textwidth(const_cast<char*>(s)); }
inline int textheight(const char* s) { return textheight(const_cast<char*>(s)); }
inline void outtextxy(int x, int y, const char* s) { outtextxy(x, y, const_cast<char*>(s)); }

Screen::Screen(int width, int height)
 : m_width(width), m_height(height) 
{
		// request autodetection, initialize graphics
	int gdriver = VGA;  // Detect
	int gmode = VGAHI;
	initgraph(&gdriver, &gmode, "");
	int errorcode = graphresult();
	if (errorcode != grOk)  // error
	{
		cout << "Error initializing graphics: " << grapherrormsg(errorcode) << endl;
		abort();
	}
}

Screen::~Screen()
{
	closegraph();
}

void Screen::updateStatus(string status)
{
	m_status = status;
}

///////////////////////////////////////////////////////////
// SoundFX -- Audio Output
///////////////////////////////////////////////////////////

const int SMALL_EAT_DELAY = 250; // msec
const int BIG_EAT_DELAY   = 520; // msec

void SoundFX::playNachManSound(int sound)
{
	static ULONGLONG endSuppressSmallEatSoundTime = 0;
	ULONGLONG now = GetTickCount64();

	switch (sound)
	{
	  case PAC_SOUND_SMALL_EAT:
		if (now >= endSuppressSmallEatSoundTime)
		{
			playClip("paceatsmall.wav");
			endSuppressSmallEatSoundTime = now  + SMALL_EAT_DELAY;
		}
		break;
	  case PAC_SOUND_BIG_EAT:
		playClip("paceatbig.wav");
		endSuppressSmallEatSoundTime = now  + BIG_EAT_DELAY;
		break;
	  case PAC_SOUND_START:
		playClip("pacstart.wav");
		break;
	  case PAC_SOUND_DIE:			
		playClip("pacdie.wav");
		break;
	  case MON_EAT_YOU_UP:			//Used by Stinky when NachMan is within 5 (x, y) of Stink
		playClip("Ieatyouup.wav");
		break;
	  case EPIC_FAIL:				//Used when player dies and have no more lifes
		playClip("epic_fail.wav");
		break;
	  case PSYCHO_CHASE:			//Used by Inky if ChaseCounter is set .
		playClip("psycho_chase.wav");
		break;
	  case SCREAMING:				//Used by Clyde in Vulerable State . 
		playClip("screaming.wav");
		break;
	  case SAD_TROMBONE:			//Used when player die and still have remaining life
		playClip("sad_trombone.wav" );
		break;
	  case YEAH_BABY:
		playClip("yeah_baby.wav" );
		break;
	}
}

void SoundFX::playClip(std::string ClipFile)
{
	if( ClipFile.c_str() == "sad_trombone.wav" ||  ClipFile.c_str() == "epic_fail.wav" || ClipFile.c_str() == "yeah_baby.wav" )
	{
		PlaySound(NULL, 0, 0);
		PlaySound(ClipFile.c_str(), NULL, SND_NOSTOP|SND_SYNC);
		return;
	}

	if ( ClipFile.c_str() == "screaming.wav" )
	{	
		PlaySound(NULL, 0, 0);
		PlaySound(ClipFile.c_str(), NULL, SND_SYNC|SND_NOSTOP);
		return;
	}
	if ( ClipFile.c_str() == "Ieatyouup.wav" )
	{	
		PlaySound(NULL, 0, 0);
		PlaySound(ClipFile.c_str(), NULL, SND_ASYNC|SND_NOSTOP);
		return;
	}
	else
	{
		PlaySound(ClipFile.c_str(), NULL, SND_ASYNC);
	}
}

void SoundFX::playTone(int hertz, int milliseconds)
{
	Beep(hertz, milliseconds);
}

///////////////////////////////////////////////////////////
// Functions for Keyboard Input
///////////////////////////////////////////////////////////

// If the user has hit a key, set ch to that character and return true;
//   otherwise, leave ch unchanged and return false.
bool getCharIfAny(char& ch)
{
    if (kbhit())
    {
		int c = getch();
		if (c != 0xE0  &&  c != 0x00)  // first of the two sent by arrow keys
            ch = c;
        else
        {
            c = getch();
            switch (c)
            {
              case 'K':  ch = ARROW_LEFT;  break;
              case 'M':  ch = ARROW_RIGHT; break;
              case 'H':  ch = ARROW_UP;    break;
              case 'P':  ch = ARROW_DOWN;  break;
              default:   ch = '?';         break;
            }
        }
        return true;
    }
    return false;
}

void waitForEnter()
{
	char ch;
	while (ch = getch(), ch != '\n' && ch != '\r')
		;
}

///////////////////////////////////////////////////////////
// Functions for Timing
///////////////////////////////////////////////////////////

// delay - sleep for 20 milliseconds (env. variable NACHDELAY overrides)

void delay()
{
    static unsigned long amt = 0;
	if (amt == 0)
    {
#pragma warning(push)
#pragma warning(disable : 4996)
        const char* p = std::getenv("NACHDELAY");
#pragma warning(pop)
        if (p == NULL  ||  (amt = std::strtoul(p, NULL, 0)) == 0)
            amt = 20;
    }
    Sleep(amt);
}

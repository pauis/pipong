#include <string>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "pongcolor.h"
#include "sysio.h"

#ifdef POSIX
#include <termios.h>
#endif
#ifdef LINUX_GPIO_RPI
#include <wiringPi.h>
#endif

using std::string;
using pong::PColor;
using pong::Rect;
using pong::PRect;

namespace pong { namespace sys
{
	void SOut::GotoPos(int x, int y)
	{
		#ifdef POSIX
		printf("\033[%d;%df", y, x);
		#endif
	}

	void SOut::GotoPos(Point pos)
	{
		GotoPos(pos.GetXpos(), pos.GetYpos());
	}

	void SOut::PrintColorString(string str, PColor colornum)
	{
		#ifdef POSIX
		printf("\033[%dm%s\033[0m", colornum.GetNum(), str.c_str());
		fflush(stdout);
		#endif
	}

	SOut& SOut::Clear(void)
	{
		#ifdef POSIX
		system("clear");
		#endif

		return *this;
	}

	int SOut::GetLength(void)
	{
		return 100;
	}

	int SOut::GetWidth(void)
	{
		return 40;
	}

	SOut& SOut::EraseWrite(PRect drect, PRect wrect)
	{
		static SOut sout;
		drect.SetColor(PColor::BLACK);

		sout << drect << wrect;

		return *this;
	}

	SOut& SOut::operator<<(PRect rect)
	{
		for(int width=0; width<rect.GetWidth(); width++)
		{
			GotoPos((rect.GetSpoint()).GetXpos(), (rect.GetSpoint()).GetYpos() + width);

			for(int length=0; length<rect.GetLength(); length++)
			{
				PrintColorString("█", rect.GetColor());
			}
		}

		return *this;
	}

	SOut& SOut::operator<<(PString str)
	{
		GotoPos(str.GetSpoint());
		PrintColorString(str.GetString(), str.GetColor());

		return *this;
	}

	void SIn::ClearBuf(void)
	{
		while (getchar() != EOF);
	}

	void SIn::operator>>(int& target)
	{
		#ifdef POSIX
		target = getchar();
		// ClearBuf();
		#endif
	}

	SIn::SIn(void)
	{
		// empty
	}

	SIn::~SIn()
	{

		// empty
	}

	SInInitial::SInInitial(void)
	{
		#ifdef POSIX
		regulartset = new struct termios;
		newtset = new struct termios;

		tcgetattr(0, regulartset);
		*newtset = *regulartset;
		(*newtset).c_lflag &= ~ICANON;
		(*newtset).c_lflag &= ~ECHO;
		(*newtset).c_cc[VTIME] = 0;
		(*newtset).c_cc[VMIN] = 0;

		tcsetattr(0, TCSANOW, newtset);
		#endif
	}

	SInInitial::~SInInitial()
	{
		#ifdef POSIX
		tcsetattr(0, TCSANOW, regulartset);

		delete regulartset;
		delete newtset;
		#endif
	}

	SCurrent& SCurrent::DelayMsec(long msec)
	{
		#ifdef POSIX
		static struct timespec tim;
		static int ini = 0;

		if (ini == 0)
		{
			tim.tv_sec = 0;
			tim.tv_nsec = (msec * 1000000L);
		}

		nanosleep(&tim, NULL);
		#endif

		return *this;
	}

	bool SCurrent::CycleTick(int cyclenum)
	{
		static int cycount = 0;

		if (cycount == 0)
		{
			cycount++;
			return true;
		}
		else
		{
			cycount++;
		}

		if (cycount >= cyclenum)
			cycount = 0;

		return false;
	}
}}

#ifndef __SYSIO_H__
#define __SYSIO_H__

#include <string>
#include <ctime>
#include "pongcolor.h"
#include "pongstring.h"

#ifdef POSIX
#include <termios.h>
#endif

namespace pong { namespace sys
{
	class SOut
	{
	private:
		static int objnum;
		void GotoPos(int x, int y);
		void GotoPos(pong::Point pos);
		void PrintColorString(std::string str, pong::PColor colornum);
	public:
		SOut(void);
		SOut& Refresh(void);
		SOut& Clear(void);
		int GetLength(void);
		int GetWidth(void);
		SOut& EraseWrite(pong::PRect drect, pong::PRect wrect);
		SOut& operator<<(pong::PRect rect);
		SOut& operator<<(pong::PString str);
		~SOut();
	};

	class SIn
	{
	private:
		#ifdef POSIX
		static struct termios* regulartset;
		static struct termios* newtset;
		#endif
		static int objnum;
		void ClearBuf(void);
	public:
		SIn(void);
		void operator>>(int& target);
		~SIn();
	};

	class SCurrent
	{
	public:
		SCurrent& DelayMsec(long msec);
		bool CycleTick(int cyclenum);
	};
}}

#endif

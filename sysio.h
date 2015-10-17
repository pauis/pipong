#ifndef __SYSIO_H__
#define __SYSIO_H__

#include <string>
#include "pongcolor.h"
#include "pongstring.h"

using std::string;
using pong::PString;
using pong::PColor;

namespace pong { namespace sys
{
	class STtyOut
	{
	private:
		void GotoXy(int x, int y);
		void PrintColorString(string str, PColor colornum);
	public:
		STtyOut& operator<<(PRect& rect);
		STtyOut& operator<<(PString& str);
	};

	class STtyIn
	{
	public:

	};

	class SGpio
	{
	private:
		int gpionum;
	protected:
		void SetupBasic(void);
	public:
		enum
		{
			P1SWITCH1 = 0,
			P1SWITCH2 = 1,
			P1LED1    = 4,
			P1LED2    = 5,
			P2SWITCH1 = 6,
			P2SWITCH2 = 7,
			P2LED1    = 12,
			P2LED2    = 13
		};
		virtual bool CheckWrite(void) = 0;
		virtual ~SGpio();
	};
}}

#endif

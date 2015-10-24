#include <string>
#include "pongobj.h"
#include "pongstring.h"
#include "sysio.h"
#include "pongproperties.h"
#include "pongtrigger.h"

using pong::PString;
using pong::PRect;
using pong::PKProperty;
using pong::PGProperty;
using pong::PGTrigger;
using pong::sys::SOut;
using pong::sys::SInInitial;
using pong::sys::SCurrent;

int main(void)
{
	SOut sout;
	SInInitial sin;
	SCurrent scurrent;
	int keyinput;
	int terminal_length = sout.GetLength();
	int terminal_width = sout.GetWidth();
	PRect boundary_outer = PRect(1, 1, terminal_length, terminal_width, PColor(PColor::BLUE));
	PRect boundary_inner = PRect(2, 2, terminal_length-2, terminal_width-2, PColor(PColor::BLACK));

	bool signal_terminate = false;
	PGTrigger gmode_event = PGTrigger::LOBBY;
	PGTrigger gmode_stage = PGTrigger::NONE;

	while (signal_terminate != true)
	{
		sin >> keyinput;

		if (gmode_event == PGTrigger(PGTrigger::LOBBY))
		{
			for (; gmode_stage != PGTrigger(PGTrigger::LOBBY); gmode_stage.Set(PGTrigger::LOBBY))
			{
				sout.Clear();

				boundary_outer.SetColor(PColor(PColor::BLUE));
				sout << boundary_outer;
				sout << boundary_inner;

				sout << PString("Pipong - Classic Table Tennis Game", PColor(PColor::CYAN), Point(terminal_length/2-16, terminal_width/2-2));
				sout << PString("Press 's' to start game", PColor(PColor::DEFAULT), Point(terminal_length/2-13, terminal_width/2));
				sout << PString("Press 'q' to exit the game", PColor(PColor::DEFAULT), Point(terminal_length/2-13, terminal_width/2+1));
			}

			if (keyinput == PKProperty::PSTART)
				gmode_event.Set(PGTrigger::INGAME);
			else if (keyinput == PKProperty::PEXIT)
				signal_terminate = true;
		}
		else if (gmode_event == PGTrigger(PGTrigger::INGAME))
		{
			for (; gmode_stage != PGTrigger(PGTrigger::INGAME); gmode_stage.Set(PGTrigger::INGAME))
			{
				sout.Clear();

				boundary_outer.SetColor(PColor(PColor::BROWN));
				sout << boundary_outer;
				sout << boundary_inner;
			}

			if (keyinput == PKProperty::PEXIT)
				gmode_event.Set(PGTrigger::LOBBY);
		}

		scurrent.DelayMsec(PGProperty::PCYCLEDELAY);
	}
	return 0;
}

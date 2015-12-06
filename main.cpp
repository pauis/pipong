#include <string>
#include "pongobj.h"
#include "pongstring.h"
#include "sysio.h"
#include "pongproperties.h"
#include "pongtrigger.h"
#include "mainautomation.h"

using std::string;
using pong::PString;
using pong::PColor;
using pong::Point;
using pong::PRect;
using pong::PKProperty;
using pong::PGProperty;
using pong::PGTrigger;
using pong::sys::SOut;
using pong::sys::SIn;
using pong::sys::SCurrent;

int main(void)
{
	// System Object
	SOut sout;
	SIn ssin;
	SCurrent scurrent;
	int keyinput;

	// System Setting
	int terminal_length = sout.GetLength();
	int terminal_width = sout.GetWidth();

	// Game Object
	Point upmovevector(0, -1);
	Point downmovevector(0, 1);
	PRect boundary_up(3, 2, terminal_length-4, 1);
	PRect boundary_down(3, terminal_width-1, terminal_length-4, 1);
	PRect boundary_left(2, 2, 1, terminal_width-2);
	PRect boundary_right(terminal_length-1, 2, 1, terminal_width-2);
	PRect boundary_court(3, 3, terminal_length-4, terminal_width-4);
	PRect lcursor(2, terminal_width/2-2, 1, 8);
	PRect rcursor(terminal_length-1, terminal_width/2-2, 1, 8);
	PRect ball(terminal_length/2, terminal_width/2, 1, 1);
	PRect pbuf;
	string appdesc = "Pipong - Classic Table Tennis Game";
	string startdesc = "Press 's' to start game";
	string quitdesc = "Press 'q' to exit the game";
	string detaildesc = "See more details on https://github.com/pauis/pipong.";

	// Game Setting
	Point ballmovevector(-1, 1);
	bool signal_terminate = false;
	PGTrigger gmode_event = PGTrigger::LOBBY;
	PGTrigger gmode_stage = PGTrigger::NONE;
	PGTrigger gmode_lobby = PGTrigger::LOBBY;
	PGTrigger gmode_ingame = PGTrigger::INGAME;

	// Game Logic
	while (signal_terminate != true)
	{
		ssin >> keyinput;

		if (gmode_event == gmode_lobby)
		{
			for (; gmode_stage != gmode_lobby; gmode_stage.Set(PGTrigger::LOBBY))
			{
				sout.Clear();

				boundary_up.SetColor(PColor(PColor::BLUE));
				boundary_down.SetColor(PColor(PColor::BLUE));
				sout << boundary_up << boundary_down;

				sout << PString(appdesc, PColor(PColor::CYAN), Point(terminal_length/2-20, terminal_width/2-2));
				sout << PString(startdesc, Point(terminal_length/2-12, terminal_width/2));
				sout << PString(quitdesc, Point(terminal_length/2-12, terminal_width/2+1));
				sout << PString(detaildesc, Point(terminal_length/2-20, terminal_width/2+3));
			}

			if (keyinput == PKProperty::PSTART)
				gmode_event.Set(PGTrigger::INGAME);
			else if (keyinput == PKProperty::PEXIT)
				signal_terminate = true;
		}
		else if (gmode_event == gmode_ingame)
		{
			for (; gmode_stage != gmode_ingame; gmode_stage.Set(PGTrigger::INGAME))
			{
				sout.Clear();

				boundary_up.SetColor(PColor(PColor::CYAN));
				boundary_down.SetColor(PColor(PColor::CYAN));
				lcursor.SetSypos(terminal_width/2-4);
				rcursor.SetSypos(terminal_width/2-4);
				ball.SetSxpos(terminal_length/2);
				ball.SetSypos(terminal_width/2);
				sout << boundary_up << boundary_down << lcursor << rcursor << ball;
			}

			if (keyinput == PKProperty::PEXIT)
				gmode_event.Set(PGTrigger::LOBBY);
			else if (keyinput == PKProperty::PP1UP)
				MainAM::PRectMove(sout, lcursor, upmovevector, boundary_left);
			else if (keyinput == PKProperty::PP1DOWN)
				MainAM::PRectMove(sout, lcursor, downmovevector, boundary_left);
			else if (keyinput == PKProperty::PP2UP)
				MainAM::PRectMove(sout, rcursor, upmovevector, boundary_right);
			else if (keyinput == PKProperty::PP2DOWN)
				MainAM::PRectMove(sout, rcursor, downmovevector, boundary_right);

			if (scurrent.CycleTick(PGProperty::PBALLFREQ))
			{
				pbuf = MainAM::PRectMove(sout, ball, ballmovevector, boundary_court);

				if (boundary_up.CheckInclude(pbuf) || boundary_down.CheckInclude(pbuf))
					ballmovevector.SetYpos(-(ballmovevector.GetYpos()));

				if (boundary_left.CheckInclude(pbuf) || boundary_right.CheckInclude(pbuf))
				{
					if (lcursor.CheckInclude(pbuf) || rcursor.CheckInclude(pbuf))
						ballmovevector.SetXpos(-(ballmovevector.GetXpos()));
					else
						gmode_event.Set(PGTrigger::LOBBY);
				}
			}
		}

		sout.Refresh();
		scurrent.DelayMsec(PGProperty::PCYCLEDELAY);
	}

	return 0;
}

#include "stdafx.h"
#include "chest.h"


/*
	Constructor of the chest.
	Also set the color of the chest.
	You either borned black or white. Or EMPTY
*/
Chest::Chest(wchar_t _name)
{
	icon = _name;
	switch (icon) {
	case WK:
	case WQ:
	case WR:
	case WB:
	case WH:
	case WP:
		color = WHITE;
		break;

	case BK:
	case BQ:
	case BR:
	case BB:
	case BH:
	case BP:
		color = BLACK;
		break;
	default:
		color = EMPTY;
		break;
	}

}

/*
	== overload operator, for comparing the chest with wchar_t const value in constant.h
	it save about 10 seconds for each ==
*/
bool Chest::operator==(wchar_t const & _icon) const
{
	return wcsncmp(&_icon, &icon,1)==0;					//wcscmp not working, since the value of those _icon and icon is different
														//either compare the 1st characster of those widestring using wcsncmp or just ==
}


/*
	Return the color of the chest (color code in int)
*/

int Chest::GetColor() const
{
	return color;
}

/*
	This is for print out the chest piece to board.
	That why we use wcout
*/
wostream& operator<<(wostream& out, const Chest& chest) {	
	return out << chest.icon;
}

/*
	This is for save the chest piece to a file.
	
	Save a unicode character to a file is a pain in the ass.
	Not only we need to save it using wostream, the file we save it also be able to store unicode character.
	Even worse, we also need wide istream to read those unicode character, and we need to make sure the unicode character won't broken.
	Since it sound so complicated and I tested for a while without both of them working properly, the best and lazy method
	is to save the file as normal char, and we can load the file as normal char.

	We can do the conversion from wchar_t <-> char easily.

	MAYBE WE CAN HAVE A TEMPLATE TO DO 2 WAYS CONVERSION, INSTEAD OF DOING IT LIKE THIS.
*/
ostream & operator<<(ostream & out, const Chest & chest)
{
	switch (chest.icon) {
	case WK:
		return out << CWK;
	case WQ:
		return out << CWQ;
	case WR:
		return out << CWR;
	case WB:
		return out << CWB;
	case WH:
		return out << CWH;
	case WP:
		return out << CWP;

	case BK:
		return out << CBK;
	case BQ:
		return out << CBQ;
	case BR:
		return out << CBR;
	case BB:
		return out << CBB;
	case BH:
		return out << CBH;
	case BP:
		return out << CBP;
	default:
		return out << CET;	
	}
}

/*
	This is for load the chest piece from a file.

	As mentined in the ostream above, this is a lazy and effecient method
*/
istream & operator>>(istream & in, Chest & chest)
{
	char chestChar;
	in >> chestChar;

	switch (chestChar) {
	case CWK:
		chest = WK;
		break;
	case CWQ:
		chest = WQ;
		break;
	case CWR:
		chest = WR;
		break;
	case CWH:
		chest = WH;
		break;
	case CWB:
		chest = WB;
		break;
	case CWP:
		chest = WP;
		break;

	case CBK:
		chest = BK;
		break;
	case CBQ:
		chest = BQ;
		break;
	case CBR:
		chest = BR;
		break;
	case CBH:
		chest = BH;
		break;
	case CBB:
		chest = BB;
		break;
	case CBP:
		chest = BP;
		break;
	case CET:
		chest = ET;
		break;
	default:
		chest = ET;
		break;
	}
	return in;
}


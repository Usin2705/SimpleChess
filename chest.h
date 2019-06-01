// Contain the Chest object for the chestboard.
// Chest have two attritube, color and icon.
// Color have 3 value (easier when check the validity of chest's move) BLACK, WHITE and EMPTY
// Icon have 13 value, they are all available chest types. Icon is wchar_t so remember to use wcout to print out.
// 

#pragma once

#include <iostream>
#include <string>
#include "constant.h"

using namespace std;

class Chest {
	friend wostream& operator<<(wostream& out, const Chest& chest);
	friend ostream& operator<<(ostream &out, const Chest& chest);			// for saving chest game file, because Unicode is hard
	friend istream& operator>>(istream &in, Chest& chest);					// for loading chest game file
public:
	Chest(wchar_t _name = ET);
	bool operator==(wchar_t const &_icon) const;
	int GetColor() const;
private:
	wchar_t icon;
	int color;
};
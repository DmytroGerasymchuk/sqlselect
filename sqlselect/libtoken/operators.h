#pragma once

#include "token_part.h"
#include "token.h"

namespace libtoken
{
	ostream& operator << (ostream& os, const token_part& tp);
	ostream& operator << (ostream& os, const token& t);
	ostream& operator << (ostream& os, const vector<token>& line);
}
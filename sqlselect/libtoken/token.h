#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "token_part.h"

namespace libtoken
{
	using namespace std;

	struct token
	{
		char parts_separator = '\x00';

		vector<token_part> parts;

		void clear();

		friend ostream& operator << (ostream& os, const token& t);
	};
}
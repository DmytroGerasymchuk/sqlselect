#pragma once

#include <string>
#include <iostream>

namespace libtoken
{
	using namespace std;

	struct token_part
	{
		string body;
		char qualified_by = '\x00';

		void clear();

		friend ostream& operator << (ostream& os, const token_part& tp);
	};
}
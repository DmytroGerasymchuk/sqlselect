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
		enum class Type { Undefined, Atomic, Multipart, Text, EOL };

		char parts_separator = '\x00';
		vector<token_part> parts;

		void clear(char new_parts_separator = '\x00');
		Type get_type() const;

		friend ostream& operator << (ostream& os, const token& t);
	};
}
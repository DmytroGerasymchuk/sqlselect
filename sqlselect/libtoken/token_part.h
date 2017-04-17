#pragma once

#include <string>
#include <iostream>

namespace libtoken
{
	using namespace std;

	struct token_part
	{
		enum class Type { General, Text, EOL };

		Type type = Type::General;
		char qualified_by = '\x00';
		string body;

		void clear();

		friend ostream& operator << (ostream& os, const token_part& tp);
	};
}
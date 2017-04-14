#include "token.h"

namespace libtoken
{
	void token::clear(char new_parts_separator)
	{
		parts_separator = new_parts_separator;

		parts.clear();
	}

	ostream& operator << (ostream& os, const token& t)
	{
		bool first_part = true;

		for (const auto& tp : t.parts)
		{
			if (first_part)
				first_part = false;
			else
				if (t.parts_separator)
					cout << t.parts_separator;

			os << "[" << tp << "]";
		}

		return os;
	}
}
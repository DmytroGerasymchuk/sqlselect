#include "operators.h"

namespace libtoken
{
	// output of one token_part

	ostream& operator << (ostream&os, const token_part& tp)
	{
		if (tp.qualified_by)
			os << tp.qualified_by;

		if (tp.type == token_part::Type::EOL)
			os << "\\n";
		else
			os << tp.body;

		if (tp.qualified_by)
			os << tp.qualified_by;

		return os;
	}

	// output of complete token

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

	// output of vector of tokens

	ostream& operator << (ostream& os, const vector<token>& line)
	{
		bool first_token = true;

		for (const auto& t : line)
		{
			if (first_token)
				first_token = false;
			else
				os << ' ';

			os << t;
		}

		return os;
	}
}
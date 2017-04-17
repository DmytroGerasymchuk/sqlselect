#include "token_part.h"

namespace libtoken
{
	void token_part::clear()
	{
		type = Type::General;
		qualified_by = '\x00';
		body.clear();
	}

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
}
#include "token_part.h"

namespace libtoken
{
	void token_part::clear()
	{
		body.clear();
		qualified_by = '\x00';
	}

	ostream& operator << (ostream&os, const token_part& tp)
	{
		os << tp.body;

		return os;
	}
}
#include "stmt_part_stream.h"

namespace libsql
{
	stmt_part_stream& stmt_part_stream::operator >> (vector<token>& stmt_parts)
	{
		if (!state)
			throw statement_error("this statement_part_stream is not readable any more", ts.get_buffer());

		stmt_parts.clear();

		token t;
		string end_of_stmt{ sql_token_stream_settings::end_of_stmt };

		while (ts >> t)
		{
			if (t.get_type() == token::Type::Atomic)
				if (t.parts[0].body == end_of_stmt)
					break;

			stmt_parts.push_back(t);
		}

		stmt_assert(ts && (stmt_parts.size() == 0), "empty statement encountered");

		state = (stmt_parts.size() > 0);

		return *this;
	}
}
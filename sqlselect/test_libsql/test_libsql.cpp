#include <fstream>

#include "../libsql/libsql.h"

using namespace std;

void dump_stmt_parts()
{
	using namespace libsql;

	cout << "stmt_part_stream test: read and dump SQL statements as token vectors" << endl << endl;

	ifstream ifs{ "../test_data/libsql_stmt_part_test.txt" };
	if (!ifs)
		throw std::exception("ifs.open failed!");

	stmt_part_stream sps{ ifs };
	vector<token> stmt_parts;

	while (sps >> stmt_parts)
		cout << "Got statement parts: " << stmt_parts << endl;

	cout << endl;
}

int main()
{
	try
	{
		dump_stmt_parts();

		cout << "End-of-program." << endl;
	}
	catch (const libtoken::libtoken_exception& ex)
	{
		cerr << "LIBTOKEN EXCEPTION: " << typeid(ex).name() << ": " << ex.what() << " at source line " << ex.cur_line_no << " pos " << ex.cur_line_pos << std::endl;
	}
	catch (const libsql::stmt_part_stream::statement_error& ex)
	{
		cerr << "STATEMENT ERROR: " << ex.what() << " at source line " << ex.cur_line_no << " pos " << ex.cur_line_pos << std::endl;
	}
	catch (const libsql::libsql_exception& ex)
	{
		cerr << "LIBSQL EXCEPTION: " << typeid(ex).name() << ": " << ex.what() << std::endl;
	}
	catch (const std::exception& ex)
	{
		cerr << "EXCEPTION: " << typeid(ex).name() << ": " << ex.what() << std::endl;
	}

	return 0;
}

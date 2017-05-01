#include <fstream>

#include "../libtoken/libtoken.h"
#include "../libsql/libsql.h"

using namespace std;

void dump_raw_chars()
{
	using namespace libtoken;

	cout << "line_buffer test: read whole file and dump raw characters" << endl << endl;

	ifstream ifs{ "../test_data/libtoken_test.txt" };
	if (!ifs)
		throw std::exception("ifs.open failed!");

	line_buffer line_buf{ ifs };

	cout << "[";

	while (char c = line_buf.getch())
		if (c == '\n')
			cout << "\\n";
		else
			cout << c;

	cout << "]" << endl << endl;
}

void dump_tokens()
{
	using namespace libtoken;

	cout << "token_stream test: read whole file and dump tokens using SQL-style stream settings" << endl << endl;

	ifstream ifs{ "../test_data/libtoken_test.txt" };
	if (!ifs)
		throw std::exception("ifs.open failed!");

	token_stream ts{ libsql::sql_token_stream_settings(), ifs };

	token t;

	while (ts >> t)
		cout << t << " ";

	cout << endl << endl;
}

int main()
{
	try
	{
		dump_raw_chars();
		dump_tokens();

		cout << "End-of-program." << endl;
	}
	catch (const libtoken::libtoken_exception& ex)
	{
		cerr << "LIBTOKEN EXCEPTION: " << typeid(ex).name() << ": " << ex.what() << " at source line " << ex.cur_line_no << " pos " << ex.cur_line_pos << std::endl;
	}
	catch (const std::exception& ex)
	{
		cerr << "EXCEPTION: " << typeid(ex).name() << ": " << ex.what() << std::endl;
	}

    return 0;
}

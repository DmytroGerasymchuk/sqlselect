#include <fstream>

#include "../libtabular/libtabular.h"

using namespace std;

void dump_tabular_onefile()
{
	using namespace libtabular;

	cout << "tabular_stream test: read and dump CSV file with inside-header" << endl << endl;

	ifstream ifs{ "../test_data/tabstream_onefile.txt" };
	if (!ifs)
		throw std::exception("ifs.open failed!");

	tabular_stream_settings tss;
	tabular_stream ts{ tss, ifs, ifs };

	cout << ts.header << endl;

	vector<token> line;

	while (ts >> line)
		cout << line << endl;

	cout << endl;
}

void dump_tabular_sepfiles()
{
	using namespace libtabular;

	cout << "tabular_stream test: read and dump space-separated file with separate header" << endl << endl;

	ifstream ifs_h{ "../test_data/tabstream_sep_header.txt" };
	if (!ifs_h) throw std::exception("ifs_h.open failed!");
	ifstream ifs_d{ "../test_data/tabstream_sep_data.txt" };
	if (!ifs_d) throw std::exception("ifs_d.open failed!");

	tabular_stream_settings tss;
	tss.field_separator = '\x00';
	tabular_stream ts{ tss, ifs_h, ifs_d };

	cout << ts.header << endl;

	vector<token> line;

	while (ts >> line)
		cout << line << endl;

	cout << endl;
}

int main()
{
	try
	{
		dump_tabular_onefile();
		dump_tabular_sepfiles();

		cout << "End-of-program." << endl;
	}
	catch (const libtoken::libtoken_exception& ex)
	{
		cerr << "LIBTOKEN EXCEPTION: " << typeid(ex).name() << ": " << ex.what() << " at source line " << ex.cur_line_no << " pos " << ex.cur_line_pos << std::endl;
	}
	catch (const libtabular::tabular_stream::tabularization_error& ex)
	{
		cerr << "TABULARIZATION ERROR: " << ex.what() << " at source line " << ex.cur_line_no << " pos " << ex.cur_line_pos << std::endl;
	}
	catch (const libtabular::libtabular_exception& ex)
	{
		cerr << "LIBTABULAR EXCEPTION: " << typeid(ex).name() << ": " << ex.what() << std::endl;
	}
	catch (const std::exception& ex)
	{
		cerr << "EXCEPTION: " << typeid(ex).name() << ": " << ex.what() << std::endl;
	}

	return 0;
}

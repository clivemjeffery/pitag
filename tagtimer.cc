//
// Raspberry Running tagtimer program.
// Runs on the R-Pi to time laps with RFID tags.
//
// Usage: tagtimer &
//		The program will loop indefinitely reading tags and
//		writing times to standard output and a file.
//		Use Ctrl-C to stop. Note that the & disconnects the program
//		from the terminal so that you can issue a tail -f command
//		on the output, if you want to see what's happening. 
//
#include <stdlib.h>
#include <vector>
#include <map>
#include "CTagReader.h"
#include "CTagData.h"

using namespace std;
ofstream fout;
const char* OUTFILE = "tagtimes.txt";

class CTimerReader : public CTagReader
{
	public:
		CTagData tags;
		
		// maps ids to last lap so that the next lap can be worked out
		typedef std::map<std::string, time_t> LapMapType;
		LapMapType laps;
		
		CTimerReader();
		void on_tag();
		void keep_lap();
};

CTimerReader::CTimerReader() {
	tags.read();
};

void CTimerReader::keep_lap() {
	laps[get_id()] = get_seconds();
};

void CTimerReader::on_tag() {

	fout.open(OUTFILE, ios_base::app);
	
	fout << get_id() << '\t';
	if (tags.find(get_id())) {
		fout << tags.found_name();
	} else {
		fout << "*UNKNOWN*";
	}	
	fout << '\t' << get_seconds() << '\t' << (get_seconds() - laps[get_id()]) << endl;
	keep_lap();
	
	fout.close();
};
 
int main()
{
	CTimerReader reader;
	
	fout.open(OUTFILE);	// rewrite
	fout << "RFID Tag\tRunner Name\tTime\tLap" << endl;
	fout.close();
	
	cout << "Ready to time laps for " << reader.tags.count() << " known runners." << endl;
	cout << "To see what's happening use tail -f " << OUTFILE << endl;
	
    //reader.signal_detected.connect( sigc::mem_fun(reader, &CTimerReader::on_tag) );
    reader.run();
    
    return 0;
}

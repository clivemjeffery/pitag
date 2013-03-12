//
// Raspberry Running tagtimer program.
// Runs on the R-Pi to time laps with RFID tags.
//
// Usage: tagtimer
//		The program will loop indefinitely reading tags and
//		writing times to standard output and a file.
//		Use Ctrl-C to stop. 
//
#include <stdlib.h>
#include "CTagReader.h"
#include "CTagData.h"

using namespace std;
CRFIDtagreader reader;
CTagData tags;
ofstream fout;
const char* OUTFILE = "tagtimes.txt";

void on_tag() {
	string new_name;
	
	fout.open(OUTFILE, ios_base::app);
	
	if (tags.find(reader.get_id())) {
		cout << tags.found_name();
		fout << tags.found_name();	// todo: make some sort ot tee
	} else {
		cout << reader.get_id();
		fout << reader.get_id();
	}	
	cout << '\t' << reader.get_seconds() << endl;
	fout << '\t' << reader.get_seconds() << endl;
	
	fout.close();
}
 
int main()
{
	tags.read();
	
	fout.open(OUTFILE);	// rewrite
	fout.close();
	
	cout << "Ready to time laps for " << tags.count() << " runners." << endl;
	
    reader.signal_detected.connect( sigc::ptr_fun(on_tag) );
    reader.run();
    
    return 0;
}

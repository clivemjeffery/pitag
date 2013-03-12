//
// Raspberry Running tagmap program.
// Runs on the R-Pi to say who owns which tag.
//
// Usage: tagmap
//		The program will report how many tags are known and then loop
//		indefinitely read to read tags and put out the current owner,
//		allowing the user to enter a new name if desired. 
//
#include <stdlib.h>
#include "CTagReader.h"
#include "CTagData.h"

using namespace std;
CRFIDtagreader reader;
CTagData tags;

void on_tag() {
	string new_name;
	
	cout << "This tag [" << reader.get_id() << "] ";
	
	if (tags.find(reader.get_id())) {
		cout << "belongs to " << tags.found_name() << endl;
	} else {
		cout << "does not belong to anyone yet" << endl;
	}	
	cout << "enter a new name, Q to quit or C to continue: " << flush;
	getline(cin, new_name);
	if (new_name == "C" ) {
		cout  << "No changes made";
	} else if (new_name == "Q") {
		exit(0); // this is probably bad form, alternative?
	} else {
		tags.set_name(reader.get_id(), new_name);
		cout << "Tag now belongs to " << new_name << flush;
	}
	cout << ". Scan another tag..." << endl;
}
 
int main()
{
	tags.read();
	
	cout << "There are " << tags.count() << "tags in the database." << endl;
	cout << "Scan tags to see who they belong to. You can also update them." << endl;
	cout << "Scan the first tag..." << endl;
	
    reader.signal_detected.connect( sigc::ptr_fun(on_tag) );
    reader.run();
    
    tags.write();
    return 0;
}

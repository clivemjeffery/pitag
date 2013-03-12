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
CTagReader reader;
CTagData tags;

string last_id;

void on_tag() {
	
	if (reader.get_id() != last_id) { // skip the same id read twice in a row
		last_id = reader.get_id();
			
		cout << "This tag [" << reader.get_id() << "] ";
		if (tags.find(reader.get_id())) {
			cout << "belongs to " << tags.found_name() << "." << endl;
		} else {
			cout << "does not belong to anyone yet." << endl;
		}	
		cout << "Enter a new name, Q to quit or hit RETURN to continue: " << flush;
		
		string new_name;
		getline(cin, new_name);
		if (new_name.length() == 0 ) {
			cout << "Scan a different tag..." << endl;
		} else if (new_name == "Q") {
			reader.stop();
		} else {
			tags.set_name(reader.get_id(), new_name);
			cout << "Tag now belongs to " << new_name << ". Scan another..." << endl;
		}
	}
}
 
int main()
{
	tags.read();
	
	cout << "There are " << tags.count() << " tags in the database." << endl;
	cout << "Scan tags to see who they belong to. You can also update them." << endl;
	cout << "Scan the first tag..." << endl;
	
    reader.signal_detected.connect( sigc::ptr_fun(on_tag) );
    reader.run();
    
    tags.write();
    return 0;
}

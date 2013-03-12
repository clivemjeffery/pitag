//
// This class is the Tag Database mapping tag identities to names
// in the running club. In the future it needs to map them to
// website identities.
//
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility> // make_pair

class CTagData {
	private:
		typedef std::map<std::string, std::string> TagMapType;
		
		TagMapType name_map;// map to people's names
		
		static const char* CTagData_FILENAME;	// filename in which name_map is stored
		
		std::string id;		// the last id searched for with find
		std::string name;	// the name associated with id if it was found
		
	public:
		CTagData() { };
		std::string found_id() { return id; };
		std::string found_name() { return name; };

		bool find(std::string id);	// returns true if id is found
		
		void set_name(std::string id, std::string name);
		
		void read();	// read known tags from file
		void write();						// write tags out to file used for read
				
		size_t count() { return name_map.size(); };
};

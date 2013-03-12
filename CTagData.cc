#include "CTagData.h"

const char* CTagData::CTagData_FILENAME = "tag_name.map";

void CTagData::read() {
	std::ifstream fin(CTagData_FILENAME);
    if (fin.is_open()) {
    	while (fin.good()) {
			std::string line;
			std::string id;
			std::string name;

			std::getline(fin,line);
			
			// split line at the tab character (if there isn't one...)
    		size_t tab = line.find('\t');
    		id = line.substr(0,tab);
    		name = line.substr(tab+1,line.length()-(tab+1));
    		
    		if (id.size() > 0)
	    		name_map[id] = name;
    	}
    	fin.close();
    }
};

void CTagData::write() {
	std::ofstream fout;
  	fout.open(CTagData_FILENAME);
    TagMapType::const_iterator end = name_map.end(); 
    for (TagMapType::const_iterator it = name_map.begin(); it != end; ++it) {
        fout << it->first << '\t' << it->second << '\n';
    }
    fout.close();
};

void CTagData::set_name(std::string id, std::string name) {
	name_map[id] = name;
	write();	// because otherwise we'll have to handle SIGINT
};

bool CTagData::find(std::string id) {
	TagMapType::iterator iter = name_map.begin();
    iter = name_map.find(id);
    if (iter != name_map.end() ) {
		id = iter->first;
        name = iter->second;
        return true;
	} else {
    	return false;
    }
};

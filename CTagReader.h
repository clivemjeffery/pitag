//
// A class to read RFID tags and raise a signal when one come in.
// Uses libsigc++ for signal behaviour.
//
#include <string>
#include <time.h>
#include <sigc++/sigc++.h>

class CTagReader
{	 
	private:
		bool ok;			// status
		int fd;				// file descriptor for the serial device
		time_t start;		// start time: 0 on construction and after reset_timer()
		time_t seconds;		// seconds after start
		std::string id;		// identity read by last read_tag call
		std::string error;	// description of last error
	
	public:
		CTagReader(); // open serial ports, starts timer
		void reset_timer();
		void run();
		void stop();
		
		bool is_ok() { return ok; };
		std::string get_id() { return id; };		// identity read
		time_t get_seconds() { return seconds; };	// time when readseconds after start
		
		std::string get_error();	// description of last error
		
		sigc::signal<void> signal_detected;
};

//
// A class to read RFID tags and raise a signal when one come in.
// Uses libsigc++ for signal behaviour.
//
#include <wiringSerial.h>
#include <string.h>
#include <errno.h>

#include "CTagReader.h"

CRFIDtagreader::CRFIDtagreader()
{
	char dev[13] = "/dev/ttyAMA0";
	if ((fd = serialOpen (dev, 9600)) < 0) {
		error = strerror(errno);
		ok = fd;
	} else {
		start = time(NULL);
		seconds = 0;
		ok = 0;
	}
};

void CRFIDtagreader::reset_timer()
{
	start = time(NULL);
	seconds = 0;
};

void CRFIDtagreader::run()
{
	const struct timespec delay = {0, 250000}; // {tv_sec, tv_nsec}
	
	for (;;) { // loop, getting each ID presented
		
		// Data sent is 14 bytes, including a 02 as first and 03 as last (to be stripped)
		std::string rx(14,'X');
		
		// Can't seem to read it all in one go, probably 9200 baud rate is too slow.
		// So try to sleep for a bit...
		int d = serialDataAvail(fd);
		while (d < 14) {
			nanosleep(&delay, NULL);
			d = serialDataAvail(fd);
		}
		
		int len = read(fd, (void*)rx.data(), 14);
		if (len < 0) { error = strerror(errno); }
		else if (len == 0) { 
			// no data after waiting, so keep waiting
		}
		else
		{
			std::string new_id(rx.substr(1,12));	// The id read
			time_t t = time(NULL);					// at this time...
			t = t - start;							// ...after start
			
			if (new_id == id) {				// same id as last read
				if (t < (seconds + 2)) {	// at nearly the same time
					// this is a double read of the same tag
					seconds = t;
				} else {
					seconds = t;
					signal_detected.emit();
				}
			} else {
				// this is definitely a new tag
				id = new_id;
				seconds = t;
				signal_detected.emit();
			}
			

		}
	}
};

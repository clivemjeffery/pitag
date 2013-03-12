//
// A class to read RFID tags and raise a signal when one come in.
// Uses libsigc++ for signal behaviour.
//
#include <wiringSerial.h>
#include <string.h>
#include <errno.h>

#include "CTagReader.h"

CTagReader::CTagReader()
{
	char dev[13] = "/dev/ttyAMA0";
	if ((fd = serialOpen (dev, 9600)) < 0) {
		error = strerror(errno);
		ok = false;
	} else {
		start = time(NULL);
		seconds = 0;
		ok = true;
	}
};

void CTagReader::reset_timer()
{
	start = time(NULL);
	seconds = 0;
};

void CTagReader::stop()
{
	ok = false;
};

void CTagReader::run()
{
	const struct timespec delay = {0, 250000}; // {tv_sec, tv_nsec}
	
	while (ok) { // loop, getting each ID presented
		
		// Data sent is 14 bytes, including a 02 as first and 03 as last (to be stripped)
		std::string rx(14,'X');
		
		// Can't seem to read it all in one go, probably 9200 baud rate is too slow.
		// So sleep for a bit and get the rest
		int d = serialDataAvail(fd);
		while (d < 14) {
			nanosleep(&delay, NULL);
			d = serialDataAvail(fd);
		}
		
		int len = read(fd, (void*)rx.data(), 14);
		if (len < 0) {
			error = strerror(errno);
			ok = false;
		}
		else if (len == 0) { 
			// no data after waiting, so keep waiting
		}
		else
		{
			id = rx.substr(1,12);	
			time_t t = time(NULL);
			seconds = t - start;
			signal_detected.emit();
		}
	}
};

#ifndef __AUDIOEVENT_H__
#define __AUDIOEVENT_H__

#include <string>

using namespace std;

class AudioEvent
{
public:

	AudioEvent();

	string name;
	unsigned int id = 0;

	int event_id = 0L;			// If event is playing, is different from 0L

};

#endif
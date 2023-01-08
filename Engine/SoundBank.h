#ifndef __SOUNDBANK_H__
#define __SOUNDBANK_H__

#include <string>
#include <vector>

using namespace std;

class AudioEvent;

class SoundBank
{

public:

	vector<AudioEvent*> events;

	string name;
	string path;
	unsigned int id = 0;
};

#endif //__SOUNDBANK_H__
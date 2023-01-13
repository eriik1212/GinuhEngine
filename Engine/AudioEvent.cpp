#include "AudioEvent.h"

AudioEvent::AudioEvent()
{
}

bool AudioEvent::IsPlaying() const
{
    return event_id != 0L;

}

// ------------------------------------------------- Unnecessary

void AudioEvent::Unload()
{
    unload = true;
}

void AudioEvent::Unloaded()
{
    unload = false;
}

bool AudioEvent::IsReadyToUnload() const
{
    return unload && !IsPlaying();
}

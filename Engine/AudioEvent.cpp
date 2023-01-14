#include "AudioEvent.h"

AudioEvent::AudioEvent()
{
}

bool AudioEvent::IsPlaying() const
{
    return event_id != 0L;

}
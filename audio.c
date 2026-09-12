#include "audio.h"
#include <stddef.h>

void StopAllMusic(Music *musicTracks, int count) {
    for (int i = 0; i < count; i++) {
        if (musicTracks[i].stream.buffer != NULL) {
            StopMusicStream(musicTracks[i]);
        }
    }
}

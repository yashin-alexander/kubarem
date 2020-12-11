#include "audio/audio.h"


_Sound::_Sound(SoLoud::Soloud & _soloud_core,
               const glm::vec3 & source_position,
               const glm::vec3 & listener_position,
               const glm::vec3 & listener_look_at):
    _soloudCore(_soloud_core),
    _sourcePosition(source_position),
    _listenerPosition(listener_position),
    _listenerLookAt(listener_look_at)
{}


_Sound::~_Sound()
{};


void _Sound::UpdatePositioning(){
    _soloudCore.set3dListenerPosition(
        this->_listenerPosition.x,
        this->_listenerPosition.y,
        this->_listenerPosition.z
    );
    _soloudCore.set3dListenerAt(
        this->_listenerLookAt.x,
        this->_listenerLookAt.y,
        this->_listenerLookAt.z
    );
    _soloudCore.set3dSourcePosition(
        this->_soundHandler,
        this->_sourcePosition.x,
        this->_sourcePosition.y,
        this->_sourcePosition.z
    );
}


AudioPositioned::AudioPositioned(SoLoud::Soloud & soloud_core,
                             const char * file_path,
                             const glm::vec3 & source_position,
                             const glm::vec3 & listener_position,
                             const glm::vec3 & listener_look_at):
    _Sound(soloud_core, source_position, listener_position, listener_look_at)
{
    this->sample.load(file_path);
}


void AudioPositioned::RunPlayback(){
    this->_soundHandler = this->_soloudCore.play3d(
            this->sample,
            this->_sourcePosition.x,
            this->_sourcePosition.y,
            this->_sourcePosition.z
    );
}


void AudioPositioned::StopPlayback(){
    this->_soloudCore.stop(this->_soundHandler);
}


AudioBackground::AudioBackground(SoLoud::Soloud & soloud_core, const char * file_path):
    AudioPositioned(soloud_core, file_path, glm::vec3(0),glm::vec3(0),glm::vec3(0))
{}


void AudioBackground::RunPlayback() {
    this->_soundHandler = _soloudCore.play(this->sample);
}

AudioSpeech::AudioSpeech(SoLoud::Soloud &soloud_core,
                         const char *text_to_speak,
                         unsigned int frequency,
                         float speed,
                         float declination,
                         int wave_form):
_Sound(soloud_core, glm::vec3(0),glm::vec3(0),glm::vec3(0))
{
    speech.setText(text_to_speak);
    speech.setParams(frequency, speed, declination, wave_form);

//    speech.
}

void AudioSpeech::RunPlayback() {
    this->_soundHandler = this->_soloudCore.play(speech);
}

void AudioSpeech::StopPlayback() {
    this->_soloudCore.stop(this->_soundHandler);
}
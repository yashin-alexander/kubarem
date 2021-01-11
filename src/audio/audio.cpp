#include "audio/audio.h"
#include "log.h"


_Sound::_Sound(SoLoud::Soloud * _soloud_core):
    _soloudCore(_soloud_core)
{}


_Sound::~_Sound()
{};


void _Sound::UpdatePositioning(glm::vec3 source_position, glm::vec3 listener_position, glm::vec3 listener_look_at){
    _soloudCore->set3dListenerPosition(
        listener_position.x,
        listener_position.y,
        listener_position.z
    );
    _soloudCore->set3dListenerAt(
            listener_look_at.x,
            listener_look_at.y,
            listener_look_at.z
    );
    _soloudCore->set3dSourcePosition(
        this->_soundHandler,
        source_position.x,
        source_position.y,
        source_position.z
    );
}


AudioPositioned::AudioPositioned(SoLoud::Soloud * soloud_core,
                             const char * file_path):
    _Sound(soloud_core)
{
    this->file_name = file_path;
    this->sample.load(file_path);
}


void AudioPositioned::SetSoundName(std::string sound_name) {
    this->file_name = sound_name;
    log_dbg("New sound name = %s", sound_name.c_str());
}


std::string AudioPositioned::GetSoundName() {
    return this->file_name;
}


void AudioPositioned::RunPlayback(glm::vec3 source_position){
    this->StopPlayback();
    this->sample = SoLoud::Wav();
    this->sample.load(file_name.c_str());
    log_dbg("Exec sound name = %s", file_name.c_str());
    this->_soundHandler = this->_soloudCore->play3d(
            this->sample,
            source_position.x,
            source_position.y,
            source_position.z
    );
}


void AudioPositioned::StopPlayback(){
    this->_soloudCore->stopAll();
}


AudioBackground::AudioBackground(SoLoud::Soloud * soloud_core, const char * file_path):
    AudioPositioned(soloud_core, file_path)
{}


void AudioBackground::RunPlayback() {
    this->StopPlayback();
    this->sample = SoLoud::Wav();
    this->sample.load(file_name.c_str());
    this->_soundHandler = _soloudCore->play(this->sample);
}

AudioSpeech::AudioSpeech(SoLoud::Soloud *soloud_core,
                         const char *text_to_speak,
                         unsigned int frequency,
                         float speed,
                         float declination,
                         int wave_form):
_Sound(soloud_core)
{
    this->SetTextToSpeak(text_to_speak);
    speech.setParams(frequency, speed, declination, wave_form);
}

void AudioSpeech::RunPlayback() {
    log_dbg("Run text speech = %s", this->text_to_speak.c_str());
    this->_soundHandler = this->_soloudCore->play(speech);
}

void AudioSpeech::StopPlayback() {
    this->_soloudCore->stop(this->_soundHandler);
}

std::string AudioSpeech::GetTextToSpeak() {
    return this->text_to_speak;
}

void AudioSpeech::SetTextToSpeak(const char * text_to_speak) {
    speech.stop();
    this->text_to_speak = std::string(text_to_speak);
    speech.setText(text_to_speak);
}
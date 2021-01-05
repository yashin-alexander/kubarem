#pragma once

#include <cstdint>
#include <soloud.h>
#include <soloud_wav.h>
#include <soloud_speech.h>

#include <glm/glm.hpp>


class _Sound{
protected:
    uint8_t _soundHandler;
    SoLoud::Soloud * _soloudCore;

public:
    _Sound(SoLoud::Soloud * soloud_core);
    virtual ~_Sound();
    virtual void RunPlayback() = 0;
    virtual void StopPlayback() = 0;
    virtual void UpdatePositioning(glm::vec3 source_position, glm::vec3 listener_position, glm::vec3 listener_look_at);
};


class AudioPositioned: public _Sound{
protected:
    SoLoud::Wav sample;
public:
    AudioPositioned(SoLoud::Soloud * soloud_core, const char * file_path);
    void RunPlayback() override {};
    void RunPlayback(glm::vec3 source_position);
    void StopPlayback() override;
};

class AudioBackground: AudioPositioned{
public:
    AudioBackground(SoLoud::Soloud * soloud_core, const char * file_path);
    void RunPlayback() override;
    void UpdatePositioning(glm::vec3 source_position, glm::vec3 listener_position, glm::vec3 listener_look_at) override {};
};


class AudioSpeech: _Sound{
protected:
    SoLoud::Speech speech;
public:
    AudioSpeech(SoLoud::Soloud * soloud_core,
                const char * text_to_speak,
                unsigned int frequency,
                float speed,
                float declination,
                int wave_form);
    void RunPlayback() override;
    void StopPlayback() override;
};

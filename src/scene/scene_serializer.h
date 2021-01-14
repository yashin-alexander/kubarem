#pragma once

#include <soloud.h>
#include "scene.h"
#include "audio/audio.h"


namespace kubarem {
    class SceneSerializer
    {
    public:
        SceneSerializer(Scene * scene);

        void Serialize(const std::string& filepath);
        void SerializeRuntime(const std::string& filepath);

        bool Deserialize(const std::string& filepath);
        bool DeserializeRuntime(const std::string& filepath);
    private:
        Scene * scene;
        AudioPositioned * audioPositioned;
        AudioBackground * audioBackground;
        AudioSpeech * audioSpeech;
    };
}

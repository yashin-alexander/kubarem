#pragma once

#include <soloud.h>
#include "scene.h"


namespace kubarem {
    class SceneSerializer
    {
    public:
        SceneSerializer(Scene * scene);

        void Serialize(const std::string& filepath);
        void SerializeRuntime(const std::string& filepath);

        bool Deserialize(const std::string& filepath, SoLoud::Soloud * soloud_core);  // TODO: remove soloud pass
        bool DeserializeRuntime(const std::string& filepath);
    private:
        Scene * scene;
    };
}

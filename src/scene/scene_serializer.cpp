#include "scene_serializer.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

#include "entity.h"
#include "components.h"


namespace YAML {
    template<>
    struct convert<glm::vec3> {
        static Node encode(const glm::vec3 &rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node &node, glm::vec3 &rhs) {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec4> {
        static Node encode(const glm::vec4 &rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            return node;
        }

        static bool decode(const Node &node, glm::vec4 &rhs) {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };
}


namespace kubarem {
    YAML::Emitter &operator<<(YAML::Emitter &out, const glm::vec3 &v) {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter &operator<<(YAML::Emitter &out, const glm::vec4 &v) {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return out;
    }

    SceneSerializer::SceneSerializer(Scene *scene)
            : scene(scene) {
    }

    static void SerializeEntity(YAML::Emitter &out, Entity entity) {
        auto &uuid = entity.getComponent<kubarem::UuidComponent>().uuid;

        out << YAML::BeginMap; // Entity
        out << YAML::Key << "entity" << YAML::Value << uuid.c_str(); // TODO: Entity ID goes here

        if (entity.hasComponent<TagComponent>()) {
            auto &c = entity.getComponent<kubarem::TagComponent>();

            out << YAML::Key << "TagComponent";
            out << YAML::BeginMap;
            out << YAML::Key << "tag" << YAML::Value << c.tag;
            out << YAML::EndMap;
        }

        if (entity.hasComponent<TransformComponent>()) {
            auto &c = entity.getComponent<TransformComponent>();

            out << YAML::Key << "TransformComponent";
            out << YAML::BeginMap;
            out << YAML::Key << "position" << YAML::Value << c.position;
            out << YAML::Key << "size" << YAML::Value << c.size;
            out << YAML::EndMap;
        }

        if (entity.hasComponent<ShaderProgramComponent>()) {
            auto &c = entity.getComponent<ShaderProgramComponent>();

            out << YAML::Key << "ShaderProgramComponent";
            out << YAML::BeginMap;
            out << YAML::Key << "v_shader_path" << YAML::Value << c.v_shader_path.c_str();
            out << YAML::EndMap;
        }

        if (entity.hasComponent<ModelComponent>()) {
            auto &c = entity.getComponent<ModelComponent>();

            out << YAML::Key << "ModelComponent";
            out << YAML::BeginMap;
            out << YAML::Key << "model_path" << YAML::Value << c.model_path.c_str();
            out << YAML::EndMap;
        }

        if (entity.hasComponent<CameraComponent>()) {
            auto &c = entity.getComponent<CameraComponent>();

            out << YAML::Key << "CameraComponent";
            out << YAML::BeginMap;
            out << YAML::Key << "is_tpc" << YAML::Value << c.is_tpc;
            out << YAML::EndMap;
        }

        if (entity.hasComponent<IlluminateCacheComponent>()) {
            auto &c = entity.getComponent<IlluminateCacheComponent>();

            out << YAML::Key << "IlluminateCacheComponent";
            out << YAML::Flow;
            out << YAML::BeginSeq;
            for (auto const& value: c.light_sources){
                out << value;
            }
            out << YAML::EndSeq;
        }

        if (entity.hasComponent<ThirdPersonCharacterComponent>()) {
            auto &c = entity.getComponent<ThirdPersonCharacterComponent>();

            out << YAML::Key << "ThirdPersonCharacterComponent";
            out << YAML::BeginMap;
            out << YAML::Key << "is_third_person_char" << YAML::Value << c.is_third_person_char;
            out << YAML::EndMap;
        }

        if (entity.hasComponent<CubeObjectComponent>()) {
            auto &c = entity.getComponent<CubeObjectComponent>();

            out << YAML::Key << "CubeObjectComponent";
            out << YAML::BeginMap;
            out << YAML::Key << "texture_path" << YAML::Value << c.texture_path;
            out << YAML::EndMap;
        }

        if (entity.hasComponent<ParticlesComponent>()) {
            auto &c = entity.getComponent<ParticlesComponent>();

            out << YAML::Key << "ParticlesComponent";
            out << YAML::BeginMap;
            out << YAML::Key << "position" << YAML::Value <<  c.controller.referenceParameters.position;
            out << YAML::Key << "color" << YAML::Value << c.controller.referenceParameters.color;
            out << YAML::Key << "life_length" << YAML::Value << c.controller.referenceParameters.life_length;
            out << YAML::Key << "rotation" << YAML::Value << c.controller.referenceParameters.rotation;
            out << YAML::Key << "velocity" << YAML::Value << c.controller.referenceParameters.velocity;
            out << YAML::Key << "scale" << YAML::Value << c.controller.referenceParameters.scale;
            out << YAML::Key << "gravity_effect" << YAML::Value << c.controller.referenceParameters.gravity_effect;
            out << YAML::EndMap;
        }

        if (entity.hasComponent<AudioPositionedComponent>()) {
            auto &c = entity.getComponent<AudioPositionedComponent>();

            out << YAML::Key << "AudioPositionedComponent";
            out << YAML::BeginMap;
            out << YAML::Key << "sound_name" << YAML::Value << c.audio.GetSoundName();
            out << YAML::EndMap;
        }

        if (entity.hasComponent<AudioBackgroundComponent>()) {
            auto &c = entity.getComponent<AudioBackgroundComponent>();

            out << YAML::Key << "AudioBackgroundComponent";
            out << YAML::BeginMap;
            out << YAML::Key << "sound_name" << YAML::Value << c.audio.GetSoundName();
            out << YAML::EndMap;
        }

        if (entity.hasComponent<AudioSpeechComponent>()) {
            auto &c = entity.getComponent<AudioSpeechComponent>();

            out << YAML::Key << "AudioSpeechComponent";
            out << YAML::BeginMap;
            out << YAML::Key << "text_to_speak" << YAML::Value << c.audio.GetTextToSpeak();
            out << YAML::EndMap;
        }

       /*
        * TODO: Think about macro
        *
        if (entity.hasComponent<>()) {
            auto &c = entity.getComponent<>();

            out << YAML::Key << "";
            out << YAML::BeginMap;
            out << YAML::Key << "" << YAML::Value << c.;
            out << YAML::EndMap;
        }
        */

        out << YAML::EndMap;
    }

    void SceneSerializer::Serialize(const std::string &filepath) {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "scene" << YAML::Value << "Unnamed";
        out << YAML::Key << "entities" << YAML::Value << YAML::BeginSeq;
        scene->registry.each([&](auto entityID) {
            Entity entity = {entityID, scene};
            SerializeEntity(out, entity);
        });
        out << YAML::EndSeq;
        out << YAML::EndMap;

        std::ofstream output_stream(filepath);
        output_stream << out.c_str();
    }

    void SceneSerializer::SerializeRuntime(const std::string &filepath) {
        log_err("Not implemented!");
        assert(false);
    }

    bool SceneSerializer::Deserialize(const std::string &filepath) {
        return true;
    }

    bool SceneSerializer::DeserializeRuntime(const std::string &filepath) {
        // Not implemented
        log_err("Not implemented");
        return false;
    }

}
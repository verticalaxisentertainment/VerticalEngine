#pragma once

namespace YAML {
	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3) {
				return false;
			}

			rhs.x = node[0].as<double>();
			rhs.y = node[1].as<double>();
			rhs.z = node[2].as<double>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs) {
			if (!node.IsSequence() || node.size() != 4) {
				return false;
			}

			rhs.x = node[0].as<double>();
			rhs.y = node[1].as<double>();
			rhs.z = node[2].as<double>();
			rhs.w = node[3].as<double>();
			return true;
		}
	};
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& vec3)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << vec3.x << vec3.y << vec3.z << YAML::EndSeq;
	return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& vec2)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << vec2.x << vec2.y << YAML::EndSeq;
	return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& vec4)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << vec4.r << vec4.g << vec4.b << vec4.a << YAML::EndSeq;
	return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const RigidBody2DComponent::Type& type)
{
	switch (type)
	{
	case RigidBody2DComponent::Type::Static: out << "Static"; break;
	case RigidBody2DComponent::Type::Dynamic: out << "Dynamic"; break;
	}
	return out;
}

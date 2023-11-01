#include "pch.h"
#include "SceneSerializer.h"
#include "Entity.h"

#include <yaml-cpp/yaml.h>
#include "Utils/YAMLOverloading.h"




void SceneSerializer::Create(const std::string& name)
{
	std::ofstream fout(name + ".yaml");
	YAML::Emitter out;

	//out << YAML::BeginSeq << "Scene";
	//out << YAML::EndSeq;

	out << YAML::BeginMap;
	out << YAML::Key << "Scene";
	out << YAML::Value << name;
	out << YAML::Key << "Entities";
	out << YAML::Value;
	/*out	<< YAML::BeginSeq;
	out << YAML::BeginMap;
	out << YAML::Key << "Entity" << YAML::Value << "1235";;
	out << YAML::Block << YAML::Key << "Name" << YAML::Value << "rifki";
	out << YAML::EndMap;*/
	out << YAML::EndMap;



	fout << out.c_str();

	/*if (config["username"]) {

		INFO("Username is: {}\n", config["username"].as<std::string>());
	}

	const std::string username = config["username"].as<std::string>();*/
}

void SceneSerializer::InitEntity(Entity& entity,const std::string& name)
{
	YAML::Node scene = YAML::LoadFile(name + ".yaml");
	std::fstream file;
	file.open(name + ".yaml");

	

	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Entity" << YAML::Value << entity.GetComponent<IDComponent>().ID.id();
	if (entity.HasComponent<TransformComponent>() && !entity.HasComponent<UITransformComponent>())
	{
		auto transform = entity.GetComponent<TransformComponent>();
		out << YAML::Key<<"TransformComponent";
		out << YAML::BeginMap;
		out << YAML::Block << YAML::Key << "Translition" << YAML::Value << transform.Translation;
		out << YAML::Block << YAML::Key << "Scale" << YAML::Value << transform.Scale;
		out << YAML::Block << YAML::Key << "Rotation" << YAML::Value << transform.Rotation;
		out << YAML::EndMap;
	}
	if (entity.HasComponent<UITransformComponent>())
	{
		auto uiTransform = entity.GetComponent<UITransformComponent>();
		out << YAML::Key << "UITransformComponent";
		out << YAML::BeginMap;
		out << YAML::Block << YAML::Key << "Translition" << YAML::Value << uiTransform.Translation;
		out << YAML::Block << YAML::Key << "Scale" << YAML::Value << uiTransform.Scale;
		out << YAML::Block << YAML::Key << "Rotation" << YAML::Value << uiTransform.Rotation;
		out << YAML::EndMap;
	}
	if (entity.HasComponent<SpriteRendererComponent>())
	{
		auto sprite = entity.GetComponent<SpriteRendererComponent>();
		out << YAML::Key << "SpriteRendererComponent";
		out << YAML::BeginMap;
		out << YAML::Block << YAML::Key << "Color" << YAML::Value << sprite.Color;
		if (sprite.Texture)
			out << YAML::Block << YAML::Key << "Texture" << YAML::Value << sprite.Texture->GetPath();
		else
			out << YAML::Block << YAML::Key << "Texture" << YAML::Value << "NULL";

		out << YAML::EndMap;
	}
	if (entity.HasComponent<CircleRendererComponent>())
	{
		auto circle = entity.GetComponent<CircleRendererComponent>();
		out << YAML::Key << "CircleRendererComponent";
		out << YAML::BeginMap;
		out << YAML::Block << YAML::Key << "Color" << YAML::Value << circle.Color;
		out << YAML::Block << YAML::Key << "Fade" << YAML::Value << circle.Fade;
		out << YAML::Block << YAML::Key << "Thickness" << YAML::Value << circle.Thickness;
		out << YAML::EndMap;
	}
	if (entity.HasComponent<RigidBody2DComponent>())
	{
		auto rb = entity.GetComponent<RigidBody2DComponent>();
		out << YAML::Key << "RigidBody2DComponent";
		out << YAML::BeginMap;
		out << YAML::Block << YAML::Key << "Type" << YAML::Value << rb.BodyType;
		out << YAML::EndMap;
	}
	if (entity.HasComponent<BoxCollider2DComponent>())
	{
		auto bCollider = entity.GetComponent<BoxCollider2DComponent>();
		out << YAML::Key << "BoxCollider2DComponent";
		out << YAML::BeginMap;
		out << YAML::Block << YAML::Key << "Offset" << YAML::Value << bCollider.Offset;
		out << YAML::Block << YAML::Key << "Size" << YAML::Value << bCollider.Size;
		out << YAML::Block << YAML::Key << "Density" << YAML::Value << bCollider.Density;
		out << YAML::Block << YAML::Key << "Friction" << YAML::Value << bCollider.Friction;
		out << YAML::EndMap;
	}
	if (entity.HasComponent<CircleCollider2DComponent>())
	{
		auto cCollider = entity.GetComponent<CircleCollider2DComponent>();
		out << YAML::Key << "BoxCollider2DComponent";
		out << YAML::BeginMap;
		out << YAML::Block << YAML::Key << "Offset" << YAML::Value << cCollider.Offset;
		out << YAML::Block << YAML::Key << "Radius" << YAML::Value << cCollider.Radius;
		out << YAML::Block << YAML::Key << "Density" << YAML::Value << cCollider.Density;
		out << YAML::Block << YAML::Key << "Friction" << YAML::Value << cCollider.Friction;
		out << YAML::EndMap;
	}
	if (entity.HasComponent<TextRendererComponent>())
	{
		auto text = entity.GetComponent<TextRendererComponent>();
		out << YAML::Key << "TextRendererComponent";
		out << YAML::BeginMap;
		out << YAML::Block << YAML::Key << "Text" << YAML::Value << text.Text;
		out << YAML::Block << YAML::Key << "Color" << YAML::Value << text.Color;
		out << YAML::Block << YAML::Key << "Scale" << YAML::Value << text.Scale;
		out << YAML::EndMap;
	}

	out << YAML::EndMap;

	YAML::Node node = YAML::Load(out.c_str());

	scene["Entities"].push_back(node);
	file <<scene;

	/*for (YAML::const_iterator it = scene.begin(); it != scene.end(); it++)
	{
		const YAML::Node& entityNode = *it;

		std::cout << entityNode;
	}*/
}

void SceneSerializer::Read(const std::string& sceneFile,const std::shared_ptr<Scene>& scene)
{
	YAML::Node file = YAML::LoadFile(sceneFile + ".yaml");

	//YAML::Node entities = file["Entities"];
	const YAML::Node& entities = file["Entities"];
	for (YAML::const_iterator it = entities.begin(); it != entities.end(); it++)
	{
		const YAML::Node& entityNode = *it;
		auto entity = scene->CreateEntityWithUUID(entityNode["Entity"].as<uint64_t>());
		//INFO("Entity: {}", entity["TransformComponent"].as<std::string>());
		//auto test = entityNode["TransformComponent"]["Translition"].as<glm::vec3>();

		if (entityNode["TransformComponent"].IsDefined())
		{
			auto& transform = entity.GetComponent<TransformComponent>();
			transform.Translation = entityNode["TransformComponent"]["Translition"].as<glm::vec3>();
			transform.Scale = entityNode["TransformComponent"]["Scale"].as<glm::vec3>();
			transform.Rotation = entityNode["TransformComponent"]["Rotation"].as<glm::vec3>();
		}
		if (entityNode["UITransformComponent"].IsDefined())
		{
			auto& transform = entity.AddComponent<UITransformComponent>();
			transform.Translation = entityNode["UITransformComponent"]["Translition"].as<glm::vec3>();
			transform.Scale = entityNode["UITransformComponent"]["Scale"].as<glm::vec3>();
			transform.Rotation = entityNode["UITransformComponent"]["Rotation"].as<glm::vec3>();
		}

		if (entityNode["RigidBody2DComponent"].IsDefined())
		{
			entity.AddComponent<BoxCollider2DComponent>();
			entity.AddComponent<RigidBody2DComponent>();
			auto& rb = entity.GetComponent<RigidBody2DComponent>();
			if (entityNode["RigidBody2DComponent"]["Type"].as<std::string>() == "Static")
			{
				rb.BodyType = RigidBody2DComponent::Type::Static;
			}
			else
				rb.BodyType = RigidBody2DComponent::Type::Dynamic;
		}

		if (entityNode["SpriteRendererComponent"].IsDefined())
		{
			if (entityNode["SpriteRendererComponent"]["Texture"].as<std::string>() == "NULL")
				entity.AddComponent<SpriteRendererComponent>().Color = entityNode["SpriteRendererComponent"]["Color"].as<glm::vec4>();
			else
				entity.AddComponent<SpriteRendererComponent>().Texture.reset(Texture2D::Create(entityNode["SpriteRendererComponent"]["Texture"].as<std::string>()));
		}

		if (entityNode["TextRendererComponent"].IsDefined())
		{
			auto& text = entity.AddComponent<TextRendererComponent>();
			text.Text = entityNode["TextRendererComponent"]["Text"].as<std::string>();
			text.Color = entityNode["TextRendererComponent"]["Color"].as<glm::vec4>();
			text.Scale = entityNode["TextRendererComponent"]["Scale"].as<float>();
		}

		if (entityNode["CircleRendererComponent"].IsDefined())
		{
			auto& circle = entity.AddComponent<CircleRendererComponent>();
			circle.Color = entityNode["CircleRendererComponent"]["Color"].as<glm::vec4>();
			circle.Fade = entityNode["CircleRendererComponent"]["Fade"].as<float>();
			circle.Thickness = entityNode["CircleRendererComponent"]["Thickness"].as<float>();
		}

		//std::cout << entity.GetComponent<TransformComponent>().Translation.x;
	}

	//std::cout << (entities.IsSequence() ? "yes" : "no");

}

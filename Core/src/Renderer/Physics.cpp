#include "pch.h"
#include "Physics.h"
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_body.h>
#include <box2d/b2_world.h>

#include "box2d/b2_fixture.h"
#include "Renderer/Renderer.h"


struct Drawable
{
	b2Body* Body;
	glm::vec2 Size;
};

struct PhysicsObject
{
	b2World World;
	b2BodyDef BodyDef;

	std::vector<Drawable> bodies;

	PhysicsObject()
		:World(b2Vec2(0,-10)),BodyDef()
	{
		for (auto body : bodies)
		{
			body.Body = nullptr;
			body.Size =glm::vec2(0.0f);
		}
	}
};

static PhysicsObject s_Object;


void Physics::CreateStaticBody(const glm::vec3& position,const glm::vec2& scale)
{
	s_Object.BodyDef.type = b2_staticBody;
	s_Object.BodyDef.position.Set(position.x, position.y);

	b2Body* body = s_Object.World.CreateBody(&s_Object.BodyDef);

	b2PolygonShape groundBox;
	groundBox.SetAsBox(scale.x/2, scale.y/2);

	body->CreateFixture(&groundBox, 0.0f);

	s_Object.bodies.push_back({ body,scale });

}

void Physics::CreateDynamicBody(const glm::vec3& position, const glm::vec2& scale)
{
	s_Object.BodyDef.type = b2_dynamicBody;
	s_Object.BodyDef.position.Set(position.x, position.y);

	b2Body* body = s_Object.World.CreateBody(&s_Object.BodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(scale.x/2, scale.y/2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	body->CreateFixture(&fixtureDef);

	s_Object.bodies.push_back({ body,scale });
}

void Physics::Clear()
{
	for(int i=0;i<s_Object.bodies.size();i++)
	{
		if(s_Object.bodies[i].Body->GetType() == b2_dynamicBody)
		{
			s_Object.World.DestroyBody(s_Object.bodies[i].Body);
			s_Object.bodies.erase(s_Object.bodies.begin()+i);
		}
	}
	INFO(s_Object.bodies.size());
}

void Physics::Simulate(const float& timestep)
{
	
		s_Object.World.Step(timestep, velocityIterations, positionIterations);
	
		for(auto body:s_Object.bodies)
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, glm::vec3(body.Body->GetPosition().x, body.Body->GetPosition().y, 1.0f))* glm::scale(glm::mat4(1.0f), { body.Size.x,body.Size.y,1.0f });
			model = glm::rotate(model, body.Body->GetAngle(), glm::vec3(0.0f, 0.0f, 1.0f));
			
			//printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
			Renderer::DrawQuad(model, { 1.0f,1.0f,1.0f,1.0f });
		}
}

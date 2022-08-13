#pragma once
#include "Layer/Layer.h"
#include "Renderer/VertexArray.h"
#include "Events/KeyEvent.h"

#include <glad/glad.h>

#define BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)

class GameLayer :public Layer
{
public:
	GameLayer()
		:Layer("GameLayer")
	{
		m_VertexArray.reset(VertexArray::Create());

		vertices = {
			// positions   // texCoords
			-0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  1.0f, 0.0f,

			-0.5f,  0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  1.0f, 1.0f
		};

		indices = { 0,1,2,3,4,5 };

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create((float*)&vertices, sizeof(vertices)));


		BufferLayout layout = {
			{ShaderDataType::Float2, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"}
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create((uint32_t*)&indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

	}

	void OnUpdate() override
	{
		m_VertexArray->Bind();
		glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OnEvent(Event& e) override
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(GameLayer::onKeyPressed));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(GameLayer::onKeyReleased));
		e.Handled |= e.IsInCategory(EventCategoryMouse);
	}


private:
	std::array<float, 24> vertices;
	std::array<float, 12> trianglevertices;
	std::array<uint32_t, 6> indices;
	unsigned int VBO;

	/*std::shared_ptr<VertexBuffer> m_VertexBuffer;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;*/
	std::shared_ptr<VertexArray> m_VertexArray;

	bool onKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == GLFW_KEY_C)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		return true;
	}

	bool onKeyReleased(KeyReleasedEvent& e)
	{
		if (e.GetKeyCode() == GLFW_KEY_C)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		return true;
	}
};

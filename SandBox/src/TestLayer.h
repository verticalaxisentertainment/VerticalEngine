#pragma once
#include "DLL.h"

#define BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)

class TestLayer:public Layer
{
public:
	TestLayer()
		:Layer("TestLayer") {
	}

	void OnAttach() override
	{
		auto& window = Application::Get().GetWindow();

		FrameBufferSpecification fbSpec;
		fbSpec.Width = window.GetWidth();
		fbSpec.Height = window.GetHeight();
		m_FrameBuffer.reset(FrameBuffer::Create(fbSpec));

		m_Texture.reset(Texture2D::Create(m_FrameBuffer.get()));
		Application::Get().GetFrameBuffer() = m_FrameBuffer;
	}

	void OnUpdate() override
	{

		static float alpha = 1.0f;
		RenderCommand::SetDepthTest(true);
		auto& app = Application::Get();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		m_FrameBuffer->Bind();
		RenderCommand::Clear();
		Renderer::BeginScene();
		Renderer::RenderText(text, { -130.0f,-10.0f,1.0f }, 0.7f, { 1.0f,1.0f,1.0f,1.0f });
		Renderer::DrawQuad({ 0.0f,0.0f,0.0f }, { 300.0f,100.0f }, { 0.0f,0.0f,1.0f,alpha },31);
		if((glm::sin(Math::Time()*4) + 1) / 2>0.5f)
			Renderer::DrawLine({ text.length()*22.5f-125.0f,-25.0f,1.0f}, { text.length() * 22.5f-125.0f,25.0f,1.0f}, {1.0f,1.0f,1.0f,1.0f });
		Renderer::EndScene();

		INFO(UIRenderer::GetPickedID());

		//textfield stuff
		if (UIRenderer::GetPickedID() == 31)
		{
			app.GetWindow().SetCursor(Cursor::IBEAM);

			if (Input::IsMouseButtonPressed(Mouse::Button0))
			{
				INFO("basti");
				focused = true;
				alpha = 0.3f;
			}
		}
		else
		{
			if (Input::IsMouseButtonPressed(Mouse::Button0))
			{
				focused = false;
				alpha = 1.0f;
			}
			app.GetWindow().SetCursor(Cursor::ARROW);
		}

		m_FrameBuffer->UnBind();

		RenderCommand::Clear();
		Renderer::DrawFrameBuffer(m_FrameBuffer);
		
		/*Renderer::BeginScene();
		RenderCommand::SetDepthTest(false);
		glm::mat4 model(1.0f);
		model = glm::translate(model, { 0.0f,0.0f,1.0f });
		model = glm::scale(model, { 1200.0f,720.0f,1.0f });
		Renderer::DrawQuad(model, m_Texture);
		Renderer::EndScene();*/
	}

	void OnEvent(Event& e) override
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(TestLayer::onKeyPressed));
	}

	static char m_Text[128];
private:
	std::shared_ptr<FrameBuffer> m_FrameBuffer;
	std::shared_ptr<Texture2D> m_Texture;
	std::string text;
	bool focused = false;

	bool onKeyPressed(KeyPressedEvent& e)
	{
		if (focused)
		{
			ERROR(e.GetKeyCode());
			switch (e.GetKeyCode())
			{
				case Key::A:text += "A"; break;
				case Key::B:text += "B"; break;
				case Key::C:text += "C"; break;
				case Key::D:text += "D"; break;
				case Key::E:text += "E"; break;
				case Key::F:text += "F"; break;
				case Key::G:text += "G"; break;
				case Key::H:text += "H"; break;
				case Key::Space:text += " "; break;
				case Key::Backspace:if(text.length())text.pop_back(); break;
				default:break;
			}
		}

		return true;
	}
};

char TestLayer::m_Text[128];


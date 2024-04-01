#include <iostream>

// std
#include <stack>

// SFML
#include <SFML/Graphics.hpp>

// SKY
#include <sky/AppMan.h>
#include <sky/Debug.h>
#include <sky/Input.h>

// imgui
#include <imgui.h>
#include <imgui-SFML.h>

namespace sky
{
	sf::CircleShape circle(20, 6);
	namespace misc
	{
		void skinImGui()
		{
			// Deep Dark style by janekb04 from ImThemes (modified)
			ImGuiStyle& style = ImGui::GetStyle();

			style.Alpha = 0.8;
			style.DisabledAlpha = 0.6000000238418579;
			style.WindowPadding = ImVec2(8.0, 8.0);
			style.WindowRounding = 3.0;
			style.WindowBorderSize = 1.0;
			style.WindowMinSize = ImVec2(32.0, 32.0);
			style.WindowTitleAlign = ImVec2(0.0, 0.5);
			style.WindowMenuButtonPosition = ImGuiDir_Left;
			style.ChildRounding = 4.0;
			style.ChildBorderSize = 1.0;
			style.PopupRounding = 4.0;
			style.PopupBorderSize = 1.0;
			style.FramePadding = ImVec2(5.0, 2.0);
			style.FrameRounding = 3.0;
			style.FrameBorderSize = 1.0;
			style.ItemSpacing = ImVec2(6.0, 6.0);
			style.ItemInnerSpacing = ImVec2(6.0, 6.0);
			style.CellPadding = ImVec2(6.0, 6.0);
			style.IndentSpacing = 25.0;
			style.ColumnsMinSpacing = 6.0;
			style.ScrollbarSize = 15.0;
			style.ScrollbarRounding = 9.0;
			style.GrabMinSize = 10.0;
			style.GrabRounding = 3.0;
			style.TabRounding = 4.0;
			style.TabBorderSize = 1.0;
			style.TabMinWidthForCloseButton = 0.0;
			style.ColorButtonPosition = ImGuiDir_Right;
			style.ButtonTextAlign = ImVec2(0.5, 0.5);
			style.SelectableTextAlign = ImVec2(0.0, 0.0);

			style.Colors[ImGuiCol_Text] = ImVec4(1.0, 1.0, 1.0, 1.0);
			style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4980392158031464, 0.4980392158031464, 0.4980392158031464, 1.0);
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09803921729326248, 0.09803921729326248, 0.09803921729326248, 1.0);
			style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0, 0.0, 0.0, 0.0);
			style.Colors[ImGuiCol_PopupBg] = ImVec4(0.1882352977991104, 0.1882352977991104, 0.1882352977991104, 0.9200000166893005);
			style.Colors[ImGuiCol_Border] = ImVec4(0.1882352977991104, 0.1882352977991104, 0.1882352977991104, 0.2899999916553497);
			style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0, 0.0, 0.0, 0.239999994635582);
			style.Colors[ImGuiCol_FrameBg] = ImVec4(0.0470588244497776, 0.0470588244497776, 0.0470588244497776, 0.5400000214576721);
			style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1882352977991104, 0.1882352977991104, 0.1882352977991104, 0.5400000214576721);
			style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2000000029802322, 0.2196078449487686, 0.2274509817361832, 1.0);
			style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0, 0.0, 0.0, 1.0);
			style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.05882352963089943, 0.05882352963089943, 0.05882352963089943, 1.0);
			style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0, 0.0, 0.0, 1.0);
			style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1372549086809158, 0.1372549086809158, 0.1372549086809158, 1.0);
			style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0470588244497776, 0.0470588244497776, 0.0470588244497776, 0.5400000214576721);
			style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3372549116611481, 0.3372549116611481, 0.3372549116611481, 0.5400000214576721);
			style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.4000000059604645, 0.4000000059604645, 0.4000000059604645, 0.5400000214576721);
			style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.5568627715110779, 0.5568627715110779, 0.5568627715110779, 0.5400000214576721);
			style.Colors[ImGuiCol_CheckMark] = ImVec4(0.3294117748737335, 0.6666666865348816, 0.8588235378265381, 1.0);
			style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.3372549116611481, 0.3372549116611481, 0.3372549116611481, 0.5400000214576721);
			style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.5568627715110779, 0.5568627715110779, 0.5568627715110779, 0.5400000214576721);
			style.Colors[ImGuiCol_Button] = ImVec4(0.0470588244497776, 0.0470588244497776, 0.0470588244497776, 0.5400000214576721);
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1882352977991104, 0.1882352977991104, 0.1882352977991104, 0.5400000214576721);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.2000000029802322, 0.2196078449487686, 0.2274509817361832, 1.0);
			style.Colors[ImGuiCol_Header] = ImVec4(0.0, 0.0, 0.0, 0.5199999809265137);
			style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.0, 0.0, 0.0, 0.3600000143051147);
			style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.2000000029802322, 0.2196078449487686, 0.2274509817361832, 0.3300000131130219);
			style.Colors[ImGuiCol_Separator] = ImVec4(0.2784313857555389, 0.2784313857555389, 0.2784313857555389, 0.2899999916553497);
			style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.4392156898975372, 0.4392156898975372, 0.4392156898975372, 0.2899999916553497);
			style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.4000000059604645, 0.4392156898975372, 0.4666666686534882, 1.0);
			style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.2784313857555389, 0.2784313857555389, 0.2784313857555389, 0.2899999916553497);
			style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.4392156898975372, 0.4392156898975372, 0.4392156898975372, 0.2899999916553497);
			style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.4000000059604645, 0.4392156898975372, 0.4666666686534882, 1.0);
			style.Colors[ImGuiCol_Tab] = ImVec4(0.0, 0.0, 0.0, 0.5199999809265137);
			style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1372549086809158, 0.1372549086809158, 0.1372549086809158, 1.0);
			style.Colors[ImGuiCol_TabActive] = ImVec4(0.2000000029802322, 0.2000000029802322, 0.2000000029802322, 0.3600000143051147);
			style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0, 0.0, 0.0, 0.5199999809265137);
			style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1372549086809158, 0.1372549086809158, 0.1372549086809158, 1.0);
			style.Colors[ImGuiCol_PlotLines] = ImVec4(1.0, 0.0, 0.0, 1.0);
			style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0, 0.0, 0.0, 1.0);
			style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.0, 0.0, 0.0, 1.0);
			style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0, 0.0, 0.0, 1.0);
			style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.0, 0.0, 0.0, 0.5199999809265137);
			style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.0, 0.0, 0.0, 0.5199999809265137);
			style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2784313857555389, 0.2784313857555389, 0.2784313857555389, 0.2899999916553497);
			style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0, 0.0, 0.0, 0.0);
			style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0, 1.0, 1.0, 0.05999999865889549);
			style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2000000029802322, 0.2196078449487686, 0.2274509817361832, 1.0);
			style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.3294117748737335, 0.6666666865348816, 0.8588235378265381, 1.0);
			style.Colors[ImGuiCol_NavHighlight] = ImVec4(1.0, 0.0, 0.0, 1.0);
			style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0, 0.0, 0.0, 0.699999988079071);
			style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.0, 0.0, 0.0, 0.2000000029802322);
			style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.0, 0.0, 0.0, 0.3499999940395355);
		}
	}
	namespace time
	{
		sf::Time deltaTime;
		sf::Clock deltaClock;
		sf::Clock appClock;

		int frameSamples = 30;
		int samplesRemaining = frameSamples;
		float sampleTime;

		constexpr int fpsHistoryLen = 120;
		std::array<float, fpsHistoryLen> fpsHistory;
		int currentSampleIndex = 0;
		float sampleFps;

		int truncInterval = 20;
		int truncCount = truncInterval;
		float lerpFps, lerpFpsTrunc;

		void deltaReset()
		{
			deltaTime = time::deltaClock.restart();

			--samplesRemaining;
			if (samplesRemaining < 0)
			{
				samplesRemaining = frameSamples;
				sampleFps = 1000.f * frameSamples / sampleTime;
				fpsHistory[currentSampleIndex] = sampleFps;
				if (++currentSampleIndex >= fpsHistoryLen) currentSampleIndex = 0;
				sampleTime = 0.f;

				if (--truncCount < 0)
				{
					truncCount = truncInterval;
					lerpFpsTrunc = lerpFps;
				}
			}

			sampleTime += deltaTime.asMicroseconds() * 0.001f;
			lerpFps = std::lerp(lerpFps, sampleFps, 0.3f);
		}
	}
	namespace render
	{
		std::unique_ptr<sf::RenderWindow> windowPtr;
		std::unique_ptr<sf::RenderTexture> bufferPtr;
		sf::Sprite bufferSprite;
		sf::View windowView;

		void init()
		{
			windowPtr = std::make_unique<sf::RenderWindow>(sf::VideoMode(pixelWidth * scaleFactor, pixelHeight * scaleFactor), "SkyEngine");
			windowView = sf::View(sf::Vector2f(pixelWidth / 2.f, pixelHeight / 2.f), sf::Vector2f(pixelWidth, pixelHeight));
			windowPtr->setView(windowView);

			// Init imgui
			ImGui::SFML::Init(*windowPtr);
			ImGuiIO& ImGuiIO = ImGui::GetIO();
			ImGuiIO.FontDefault = ImGuiIO.Fonts->AddFontFromFileTTF("common/font/IBMPlexMono-Regular.ttf", 20.f);
			ImGui::SFML::UpdateFontTexture();
			misc::skinImGui();

			// RenderTexture Buffer + Sprite
			sf::ContextSettings settings;
			settings.antialiasingLevel = render::aaLevel;
			bufferPtr = std::make_unique<sf::RenderTexture>();
			bufferPtr->create(pixelWidth, pixelHeight, settings);

			bufferSprite = sf::Sprite(bufferPtr->getTexture()); // <- stable ref to renderTexture result buffer
			bufferSprite.setOrigin(pixelWidth / 2.f, pixelHeight / 2.f);
		}

		void draw()
		{
			// clear & set views
			windowPtr->clear(sf::Color(0, 0, 0, 255));
			bufferPtr->clear(sf::Color(0, 0, 0, 255));
			windowPtr->setView(windowView);
			bufferPtr->setView(windowView);

			// entity drawing
			//windowPtr->draw(circle);
			bufferPtr->draw(circle);

			ImGui::Begin("Debug");
			if (ImGui::CollapsingHeader("FPS"))
			{
				ImGui::Text(fmt::format("[{:.2f}]", time::lerpFpsTrunc).c_str());
				ImGui::PlotLines("", time::fpsHistory.data(), time::fpsHistoryLen, time::currentSampleIndex, 0, 30, 3.4028235E38F, ImVec2(ImGui::GetContentRegionAvail().x, 100));
			}
			ImGui::End();
			in::imgui();

			// display to window
			bufferPtr->display();
			bufferSprite.setPosition(windowView.getCenter());
			windowPtr->draw(bufferSprite);
			ImGui::SFML::Render(*windowPtr);
			windowPtr->display();
		}

		void setViewPosition(sf::Vector2f position)
		{
			windowView.setCenter(position);
		}

		sf::Vector2f getViewPosition()
		{
			return windowView.getCenter();
		}
	}
	namespace loop
	{
		void update(sf::Time deltaTime)
		{
			// Update ImGui
			ImGui::SFML::Update(*render::windowPtr, time::deltaTime);

			circle.setFillColor(sf::Color::Transparent);
			circle.setOutlineColor(sf::Color::White);
			circle.setOutlineThickness(4);
			circle.setOrigin(20, 20); // Move origin to center
			circle.rotate(deltaTime.asSeconds() * 10.f);
			circle.setPosition(render::pixelWidth / 2, render::pixelHeight / 2);

			render::setViewPosition(render::getViewPosition() + sf::Vector2f(deltaTime.asSeconds() * 100.f * in::getAxis(in::axis::horizontal), deltaTime.asSeconds() * -100.f * in::getAxis(in::axis::vertical)));
		}
	}

	// Pixel rendering reference -> see https://stackoverflow.com/a/50637781
	void init()
	{
		// Init debug
		dbg::init();
		in::init();
		render::init();

		while (render::windowPtr->isOpen())
		{
			sf::Event windowEvent;
			while (render::windowPtr->pollEvent(windowEvent))
			{
				ImGui::SFML::ProcessEvent(windowEvent);
				in::processEvent(windowEvent);
				switch (windowEvent.type)
				{
				case sf::Event::Closed:
					render::windowPtr->close();
					break;

				default:
					break;
				}
			}

			// reset deltaClock
			time::deltaReset();
			in::update(&(*render::windowPtr), render::scaleFactor);
			loop::update(time::deltaTime);
			render::draw();
		}

		ImGui::SFML::Shutdown();
	}
}
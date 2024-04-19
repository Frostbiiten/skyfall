// std
#include <array>
#include <functional>

// sky
#include <sky/Input.h>

// SFML
#include <SFML/System.hpp>
#include <imgui.h>

// fmt
#include <fmt/format.h>

namespace sky
{
	namespace in
	{
		// Axis Bindings
		struct axisBinding
		{
			std::string name;
			std::vector<sf::Keyboard::Key> negative;
			std::vector<sf::Keyboard::Key> positive;

			bool neg = false;
			bool pos = false;
			float value = 0;

			void setNeg(bool pressed)
			{
				if (neg != pressed)
				{
					neg = pressed;
					value = pos - neg;
				}
			}

			void setPos(bool pressed)
			{
				if (pos != pressed)
				{
					pos = pressed;
					value = pos - neg;
				}
			}

			axisBinding(std::string name, std::vector<sf::Keyboard::Key> negative, std::vector<sf::Keyboard::Key> positive) : name{ name }, negative{ negative }, positive{ positive } {  };
		};

		std::array bindings
		{
			axisBinding{"Horizontal", {sf::Keyboard::A}, {sf::Keyboard::D}},
			axisBinding{"Vertical", {sf::Keyboard::S}, {sf::Keyboard::W}},
		};

		std::array<std::vector<std::function<void(bool)>>, sf::Keyboard::KeyCount> quickBindings{};

		// Pointer
		sf::Vector2i pointerPosition;


		void init()
		{
			for (auto& binding : bindings)
			{
				for (auto key : binding.positive) quickBindings[key].push_back([&binding](bool pressed) { binding.setPos(pressed); });
				for (auto key : binding.negative) quickBindings[key].push_back([&binding](bool pressed) { binding.setNeg(pressed); });
			}
		}
		void step(sf::RenderWindow* windowPtr, int scaleFactor)
		{
			pointerPosition = sf::Mouse::getPosition(*windowPtr);
			pointerPosition.x /= scaleFactor;
			pointerPosition.y /= scaleFactor;
		}

		void imgui()
		{
			ImGui::Begin("Debug");

			if (ImGui::CollapsingHeader("Input"))
			{
				for (const auto& binding : bindings)
				{
					float dummy = binding.value;
					ImGui::LabelText(fmt::format("{}", binding.value).c_str(), binding.name.c_str());
				}

				auto pointerPos = getPointerPosition();
				ImGui::LabelText(fmt::format("({}, {})", pointerPos.x, pointerPos.y).c_str(), "Pointer Position");
			}

			ImGui::End();
		}

		void processEvent(sf::Event event)
		{
			switch (event.type)
			{

			case sf::Event::KeyPressed:
				if (event.key.code >= 0) for (auto binding : quickBindings[event.key.code]) binding(true);
				break;

			case sf::Event::KeyReleased:
				if (event.key.code >= 0) for (auto binding : quickBindings[event.key.code]) binding(false);
				break;

			default:
				break;
			}
		}

		float getAxis(int axis)
		{
			return bindings[axis].value;
		}
		sf::Vector2i getPointerPosition()
		{
			return pointerPosition;
		}
	}
}

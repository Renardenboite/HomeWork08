#pragma once
#include <SFML/Graphics.hpp>
#include <assert.h>
#include "Sprite.h"

namespace ArkanoidGame
{
	class GameObject
	{
	public:
		GameObject() = default;

		GameObject(const std::string& texturePath, const sf::Vector2f& position, float width, float height)
		{
			texture.loadFromFile(texturePath);
			InitSprite(sprite, width, height, texture);
			sprite.setPosition(position);
		}
		virtual ~GameObject() = default;

		virtual void Init() {}
		virtual void Update(float timeDelta) = 0;
		virtual void Draw(sf::RenderWindow& window) { DrawSprite(sprite, window); }
		virtual void restart() {}

		const sf::Vector2f& GetPosition() const { return sprite.getPosition(); }
		const sf::FloatRect GetRect() const { return sprite.getGlobalBounds(); }
		void SetPosition(const sf::Vector2f& pos) { sprite.setPosition(pos); }
		

	protected:
		sf::Sprite sprite;
		sf::Texture texture;
		const sf::Vector2f startPosition;
	};
}
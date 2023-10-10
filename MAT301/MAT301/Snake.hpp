#pragma once
#include <list>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


// [Snake] class used to render, move and update the snake controlled by the AI
class Snake : public sf::Drawable
{
private:
    std::list<sf::Sprite> body; // list of sprites used to represent the snake
    std::list<sf::Sprite>::iterator head;
public:
    Snake();
    ~Snake();

    sf::Vector2f GetHeadPosition();
    sf::Vector2f GetTailPosition();

    void Initialize(const sf::Texture& texture, sf::Texture headtex, sf::Texture tailtex, sf::Texture food); // initialize snake
    void Reset();
    void Move(const sf::Vector2f &direction); // move snake in given direction
    bool IsOn(const sf::Sprite &other) const; // collision checking
    bool FoodSpawnedOnBody(sf::Vector2f foodPos);
    void Grow(const sf::Vector2f &direction); // increase snake size
    bool IsSelfIntersecting() const;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    std::list<sf::Sprite>::iterator tail;
    std::list<sf::Sprite>::iterator ACTUALHEAD;

private:

    sf::Texture headTexture;
    sf::Texture tailTexture;
    sf::Texture appleTexture;
};
#include "Snake.hpp"
#include <iostream>
Snake::Snake() : body(std::list<sf::Sprite>(4))
{
    head = --body.end();
    tail = body.begin();
    ACTUALHEAD = --body.end();
     // snake:
     // 
     //    tail     head
     //     v        v 
     //    [0][1][2][3][ ]
     //     ^           ^
     //    begin()     end()
  
}

Snake::~Snake()
{
}

sf::Vector2f Snake::GetHeadPosition()
{
    return ACTUALHEAD->getPosition();
}

sf::Vector2f Snake::GetTailPosition()
{
    return tail->getPosition();
}


////////////////      INITIALIZE      ////////////////
void Snake::Initialize(const sf::Texture& initalTexture, sf::Texture headtex,  sf::Texture tailtex, sf::Texture food)
{
    float x = 64.f; // initial position
    
    headTexture = headtex;
    tailTexture = tailtex;
    appleTexture = food;
  
    for (auto &piece : body){
        piece.setTexture(initalTexture); // set the textures for the pieces of the snake
        piece.setPosition({x, 64.f});    // set the positions for the pieces
        x += 64.f;                       // increase by tilesize
    }
}


////////////////      RESET      ////////////////
void Snake::Reset()
{
    // reset the size of the snake and set it back to it's inital positon: 
    body = std::list<sf::Sprite>(4);
    head = --body.end();
    tail = body.begin();
    ACTUALHEAD = --body.end();
    float x = 64.f; // initial position
    for (auto& piece : body) {
        piece.setTexture(tailTexture);  // set the textures for the pieces of the snake
        piece.setPosition({ x, 64.f }); // set the positions for the pieces
        x += 64.f;                      // increase by tilesize
    }
}


////////////////      MOVE      ////////////////
void Snake::Move(const sf::Vector2f &direction)
{
    // movement is achieved by setting the tail of the snake to the position 
    // ahead of the front of the snake in the direction which it is moving. 
 
    tail->setPosition(head->getPosition() + direction); // set the tail of the snake to the position of the head + movement
    tail->setTexture(headTexture);
    head->setTexture(tailTexture);
    ACTUALHEAD = tail;


    head = tail; // head now becomes the tail of the snake
    ++tail;
 
    if (tail == body.end()){ // once tail reaches the end of the body it is reset back to the beginning 
        tail = body.begin();
    }
}

////////////////      IS ON      ////////////////
bool Snake::IsOn(const sf::Sprite &other) const
{ 
    // check for collision of the head with the given sprite:
    return other.getGlobalBounds().intersects(head->getGlobalBounds());
}


////////////////      FOOD SPAWNED ON BODY      ////////////////
bool Snake::FoodSpawnedOnBody(sf::Vector2f foodPos)
{

    // check if the food was spawned on the body of the snake: 
    for (auto piece = body.begin(); piece != body.end(); ++piece) {
        if (piece->getPosition() == foodPos) {
            return true;
        }
    }
    return false;
}



////////////////      GROW      ////////////////
void Snake::Grow(const sf::Vector2f &direction)
{
    
    sf::Sprite newPiece; // new snake piece sprite
    newPiece.setTexture(*(body.begin()->getTexture())); // set the piece's texture
    newPiece.setPosition(head->getPosition() + direction); // set the position of the new piece to that of the head + direction of movement
    head->setTexture(appleTexture);

    head = body.insert(++head, newPiece); // inserting new sprite at new head position
}




////////////////      IS SELF INTERSECTING      ////////////////
bool Snake::IsSelfIntersecting() const
{

    // check that any of the body pieces are intersecting with the head
    bool flag = false;

    for (auto piece = body.begin(); piece != body.end(); ++piece){

        if (head != piece){

            flag = IsOn(*piece); // check if head is on current piece 

            if (flag){
                break;
            }
        }
    }
    return flag;
}



////////////////      DRAW      ////////////////
void Snake::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (auto &piece : body){
        target.draw(piece); // render all the sprites in the body list
    }
}
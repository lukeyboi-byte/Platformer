#pragma once
#include "Editor.h"

struct Collision
{
	bool hit;
	sf::Vector2f dir;
};

class Player : public Actor
{
public:
	Player();

	int lives;
	int coins;
	bool isDead;
	//collision hitbox, check next loc for collision
	sf::FloatRect nextRect;
	//if we can move, this is the position to move to
	sf::Vector2f nextPos;
	//this is to create smooth physics
	sf::Vector2f velocity;
	//jump stuff
	bool isGrounded;
	float jumpSpeed;
	float speed;

	Collision CollisionCheck(sf::FloatRect other);

	void Refresh();
	void Respawn();
	bool isAnyKeyPressed();
	sf::Vector2f startPos;

	virtual sf::Vector2f getPosition();
	virtual void setPosition(sf::Vector2f p);
};

class Enemy : public Actor
{
public:
	Enemy();
	bool isDead;
	void Killed();


};

class GameClass
{
public:
	GameClass();
	bool Start(MainRenderWindow& mainWindow);
	void Update(MainRenderWindow& mainWindow);
	bool gameActive = false;
	static const int x = 30;
	static const int y = 20;

	Player player;
	//std::vector<Enemy> enemies;

	void SaveScore();
	void ResetLevel();
	void GameOver();

	Tile** tile = new Tile * [x];
	void LoadLevel(std::string levelName, Tile** incTile);
	//deltatime stuff
	sf::Clock clock;
	float deltaTime;
	//physics numbers
	float gravity;
	float friction;
	int coins;
};

int sign(int x);
int sign(float x);
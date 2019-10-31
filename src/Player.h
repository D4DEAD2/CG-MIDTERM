#pragma once

#include "Object.h"
#include<vector>

class Camera;

class Segment : public Object{

	Segment* next = nullptr;

public:
	Segment();
	Segment(Mesh* me, Material* ma, Hitbox* hb);
	Segment(Mesh* me, Material* ma, Hitbox* hb, glm::vec3 pos);
	~Segment();


	//Inherited?
	virtual void Update(float dt) = 0;
	virtual void Draw(Shader* shdr, std::vector<Camera*> cam);
};

class Player
{
	int lives;
	float speed;
	int size = 0;
	Segment* head;

public:

	Player(Mesh* me, Material* ma, Hitbox* hb);
	~Player();


	void Add(Mesh* me, Material* ma, Hitbox* hb);
	int Size() { return size; }
	void Move(glm::vec3 dir, float spd);
	void Draw(Shader* shdr, std::vector<Camera*> cam);


};
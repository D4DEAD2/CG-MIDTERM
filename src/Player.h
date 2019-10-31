#pragma once

#include "Object.h"
#include<vector>

class Camera;

class Segment : public Object{

	Segment* next = nullptr;

public:
	virtual void Draw(Shader* shdr, std::vector<Camera*> cam);
};

class Player
{
	Mesh* mesh;
	Material* material;
	Transform transform;
	int lives;
	float speed;

	Segment* head;

public:
	Hitbox* hitbox;

	Player();
	Player(Mesh* me, Material* ma, Hitbox* hb);
	Player(Mesh* me, Material* ma, Hitbox* hb, glm::vec3 pos);

	void Move(glm::vec3 dir, float spd);
	void Draw(Shader* shdr, std::vector<Camera*> cam);

};
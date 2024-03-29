#pragma once
#include "GLM/glm.hpp"
#include "Object.h"
#include<vector>

class Camera;
class CubeHitbox;

class Segment : public Object {

	Segment* next = nullptr;
	glm::vec3 nextMove = glm::vec3(0.0f);
	glm::vec3 curMove = glm::vec3(0.0f);

public:
	Segment();
	Segment(Mesh* me, Material* ma, Hitbox* hb);
	Segment(Mesh* me, Material* ma, Hitbox* hb, glm::vec3 pos);
	~Segment();
	
	Segment* GetNext() { return next; }
	void removeLast() { next = nullptr; }
	void SetNext(Segment* s) { next = s; }

	void SetNextMv(glm::vec3);
	void ApplyMv();

	//Inherited
	virtual void Update(float dt);
	virtual void Draw(Shader* shdr, std::vector<Camera*> cam);
	void Move(glm::vec3 dir);
	void Scale(glm::vec3 scl);
	void setRot(glm::vec3 rot);
	Transform GetTransform() { return transform; }
};

class Player
{
private:
	bool twice = false;
	int size = 0;
	Segment* head;

public:

	Player();
	Player(Mesh* me, Material* ma, Hitbox* hb);
	~Player();


	void Add(Mesh* me, Material* ma, Hitbox* hb);
	void deleteSnake();

	void setTwice(bool _two) { twice = _two; }
	bool getTwice() { return twice; }
	int Size() { return size; }
	void Move(glm::vec3 dir);
	void Scale(glm::vec3 scl);
	void Draw(Shader* shdr, std::vector<Camera*> cam);

	void setRot(glm::vec3 rot);
	Segment* getHead();
	Segment* getNext(int _size);
	void setPos(glm::vec3 _pos);

	Transform GetTransform() { return head->GetTransform(); }

};
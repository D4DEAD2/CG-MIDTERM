#include "Player.h"


Player::Player(Mesh* me, Material* ma, Hitbox* hb)
{
	size = 1;

}

Player::~Player()
{
}

Segment::Segment()
{
	mesh = nullptr;
	material = nullptr;
	hitbox = nullptr;
	transform.position = glm::vec3();
	transform.scale = glm::vec3(1.0f);
	transform.rotation = glm::vec3();
}

Segment::Segment(Mesh* me, Material* ma, Hitbox* hb)
{
	mesh = me;
	material = ma;
	hitbox = hb;
	transform.position = glm::vec3();
	transform.scale = glm::vec3(1.0f);
	transform.rotation = glm::vec3();
}

Segment::Segment(Mesh* me, Material* ma, Hitbox* hb, glm::vec3 pos)
{
	mesh = me;
	material = ma;
	hitbox = hb;
	transform.position = pos;
	transform.scale = glm::vec3(1.0f);
	transform.rotation = glm::vec3();
}

Segment::~Segment()
{
}

void Segment::Draw(Shader* shdr, std::vector<Camera*> cam)
{
	Object::Draw(shdr, cam);
	if (next != nullptr)
		next->Draw(shdr, cam);
}

void Player::Add(Mesh* me, Material* ma, Hitbox* hb)
{
	size++;
	Segment next(Mesh * me, Material * ma, Hitbox * hb);
}

void Player::Move(glm::vec3 dir, float spd)
{
	
}

void Player::Draw(Shader* shdr, std::vector<Camera*> cam)
{
	head->Draw(shdr, cam);
}

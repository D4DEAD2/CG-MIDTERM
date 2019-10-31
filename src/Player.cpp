#include "Player.h"


Player::Player()
{
	mesh = nullptr;
	material = nullptr;
	hitbox = nullptr;
	transform.position = glm::vec3();
	transform.scale = glm::vec3(1.0f);
	transform.rotation = glm::vec3();
}

Player::Player(Mesh* me, Material* ma, Hitbox* hb)
{
	mesh = me;
	material = ma;
	hitbox = hb;
	transform.position = glm::vec3();
	transform.scale = glm::vec3(1.0f);
	transform.rotation = glm::vec3();
}

Player::Player(Mesh* me, Material* ma, Hitbox* hb, glm::vec3 pos)
{
	mesh = me;
	material = ma;
	hitbox = hb;
	transform.position = pos;
	transform.scale = glm::vec3(1.0f);
	transform.rotation = glm::vec3();
}

void Segment::Draw(Shader* shdr, std::vector<Camera*> cam)
{
	Object::Draw(shdr, cam);
	if (next != nullptr)
		next->Draw(shdr, cam);
}

void Player::Draw(Shader* shdr, std::vector<Camera*> cam)
{
	head->Draw(shdr, cam);
}

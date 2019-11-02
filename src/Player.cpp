#include "Player.h"
#include"Mesh.h"
#include"Texture.h"
#include"Hitbox.h"


Player::Player()
{
	size = 0;
	lives = 3;
	head = nullptr;
}

Player::Player(Mesh* me, Material* ma, Hitbox* hb)
{
	size = 1;
	lives = 3;
	head = new Segment(me, ma, hb);
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

void Segment::Update(float dt)
{
}

void Segment::Draw(Shader* shdr, std::vector<Camera*> cam)
{
	Object::Draw(shdr, cam);
	if (next != nullptr)
		next->Draw(shdr, cam);
}

void Segment::Move(glm::vec3 dir)
{
	Object::Move(dir);
}

void Segment::Scale(glm::vec3 scl)
{
	Object::Scale(scl);
}

void Segment::SetNextMv(glm::vec3 dir)
{
	nextMove = dir;
}

void Segment::ApplyMv()
{
	Move(curMove);
	if (next != nullptr) {
		next->SetNextMv(curMove);
		next->ApplyMv();
	}
	curMove = nextMove;
}

void Player::Add(Mesh* me, Material* ma, Hitbox* hb)
{
	Segment* cur = head;
	while(cur->GetNext()!= nullptr) {
		cur = cur->GetNext();
	}

	glm::vec3 pos = cur->GetPosition();

	Segment* next = new Segment(me, ma, hb, pos);
	next->Scale({ 0.75, 0.75, 0.75 });

	cur->SetNext(next);
	size++;
}

void Player::Move(glm::vec3 dir)
{
	head->SetNextMv(dir);
	head->ApplyMv();
}

void Player::Scale(glm::vec3 scl)
{
	head->Scale(scl);
}

void Player::Draw(Shader* shdr, std::vector<Camera*> cam)
{
	if (head != nullptr) {
		head->Draw(shdr, cam);
	}
}

Segment* Player::getHead()
{
	return head;
}

void Player::setPos(glm::vec3 _pos)
{
	head->setPos(_pos);
}

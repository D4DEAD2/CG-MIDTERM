#include "Player.h"
#include"Mesh.h"
#include"Texture.h"


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
	nextMove = dir; //
}

void Segment::ApplyMv()
{
	Move(curMove); //the current direction this piece is moving
	if (next != nullptr) { //if the next block exists
		next->SetNextMv(curMove); // their previous next move is now their current move
		next->ApplyMv();  //it then applys their movement and moves to the next part of the body
	}
	curMove = nextMove; // this loop is done and then its the next one
}

void Player::Add(Mesh* me, Material* ma, Hitbox* hb)
{
	Segment* cur = head;
	while(cur->GetNext()!= nullptr) { //loop until the next body part doesnt exist
		cur = cur->GetNext();
	}

	glm::vec3 pos = cur->GetPosition(); //get the last body part's location

	Segment* next = new Segment(me, ma, hb, pos); //create new body part on that location
	next->Scale({ 0.75, 0.75, 0.75 }); //scale it to the head's size

	cur->SetNext(next); //sets this new body part to be apart of the next cycle
	size++;
}

void Player::deleteSnake()
{
	while(size > 1) { //size 1 = only head remains
		Segment* cur = head; // starts at the head
		cur->removeLast(); //sets the next value to null
		size = 1; //resets your size
	}
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

Segment* Player::getNext(int _size)
{
	Segment* cur = head;
	for (int i = 0; i < _size; i++) { //loop until the next body part doesnt exist
		cur = cur->GetNext();
	}

	return cur;
}

void Player::setPos(glm::vec3 _pos)
{
	head->setPos(_pos);
}

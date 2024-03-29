#pragma once
#include<GLM/glm.hpp>
#include<GLM/gtc/matrix_transform.hpp>
#include<GLM/gtc/quaternion.hpp>
#include<vector>

class Camera;
class Material;
class Mesh;
class Shader;
class Hitbox;

struct Transform {
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 GetWorldTransform() const {
		return
			glm::translate(glm::mat4(1.0f), position) *
			glm::mat4_cast(glm::quat(glm::radians(rotation))) *
			glm::scale(glm::mat4(1.0f), scale)
			;
	}

	glm::mat4 GetRotScale() const {
		return
			glm::mat4_cast(glm::quat(glm::radians(rotation))) *
			glm::scale(glm::mat4(1.0f), scale);
	}
};

class Object {
protected:
	Mesh* mesh;
	Material* material;
	Transform transform;
	bool isPellet = false;
	bool isSpc = false;


public:
	Hitbox* hitbox;

	Object();
	Object(Mesh* me, Material* ma, Hitbox* hb);
	Object(Mesh* me, Material* ma, Hitbox* hb, glm::vec3 pos);

	virtual void Update(float dt);
	virtual void Draw(Shader* shader, std::vector<Camera*> cam);
	void Rotate(glm::vec3 rot);
	void setRot(glm::vec3 rot);
	void Rotate(float tht, glm::vec3 dir);
	void Move(glm::vec3 dir);
	void Scale(glm::vec3 scl);
	void setPos(glm::vec3 _pos);
	void setMat(Material* ma);
	void setPellet(bool _val);		///tia
	bool getPellet();
	void setSpc(bool _val);
	bool getSpc(); //

	glm::vec3 GetPosition() { return transform.position; };
	Transform GetTransform() { return transform; }
};
#pragma once
#include "Scene.h"

class Light;
class Mesh;
class Material;
class Hitbox;

class OnePlayer: public PlayScene {
	Shader* shaderObj;
	Shader* depthShader;
	Shader* sunShader;

	Shader* DebugShader;
	Mesh* DebugQuad;

	Mesh* tail_mesh;
	Material* tail_mat;
	Hitbox* tail_hit;

	Mesh* score_mesh;
	Material* score_mat;
	Hitbox* score_hit;

	bool debug = false;
	bool f3_pressed;
	int disp_depth = 0;
	float move_count = 0.0f;
	int points = 0;

public:
	OnePlayer();

	void die();

	// Inherited via Scene
	virtual void InputHandle(GLFWwindow* window, glm::vec2 mousePos, float dt) override;
	virtual void KeyboardInput(GLFWwindow* window, glm::vec2 mousePos, int player, float dt) override;
	virtual void Update(float dt) override;
	virtual void Draw() override;
	virtual void LoadScene() override;

};

class TwoPlayer: public PlayScene {
	Shader* shaderObj;
	Shader* depthShader;

public:
	TwoPlayer();
	// Inherited via Scene
	virtual void InputHandle(GLFWwindow* window, glm::vec2 mousePos, float dt) override;
	virtual void Update(float dt) override;
	virtual void Draw() override;
	virtual void LoadScene() override;
};
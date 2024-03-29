#include "PlayScenes.h"
#include <glad/glad.h>
#include<GLFW/glfw3.h>
#include<GLM/glm.hpp>
#include <iostream>
#include <stdlib.h>

#include"Mesh.h"
#include"Shader.h"
#include"Texture.h"
#include"Camera.h"
#include"Light.h"
#include"Object.h"
#include"Mesh.h"
#include"Constants.h"
#include"Test_Primitives.h"
#include"Hitbox.h"
#include "Player.h"


OnePlayer::OnePlayer()
{
	LoadScene();
}

void OnePlayer::InputHandle(GLFWwindow* window, glm::vec2 mousePos, float dt)
{
	if (glfwJoystickPresent(GLFW_JOYSTICK_1) && glfwJoystickIsGamepad(GLFW_JOYSTICK_1)) {
		ControllerInput(GLFW_JOYSTICK_1, PLAYER_1, dt);
	}
	else {
		KeyboardInput(window, mousePos, PLAYER_1, dt);
	}

	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS && !f3_pressed) {
		if (debug) {
			if (disp_depth != lights.size() - 1)
				disp_depth++;
			else
				debug = false;
		}
		else {
			if (lights.size() > 0) {
				debug = true;
				disp_depth = 0;
			}
		}

		f3_pressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_RELEASE)
		f3_pressed = false;
}

void OnePlayer::KeyboardInput(GLFWwindow* window, glm::vec2 mousePos, int player, float dt)
{
	static glm::vec3 m = glm::vec3(0.0f, 0.0f, 0.0f);

	//directional movement bases off of which key you press and doesn't let you go directly backwards.
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && m != glm::vec3(0.0f, 0.0f, 1.0f))
		m = glm::vec3(0.0f, 0.0f, -1.0f);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && m != glm::vec3(0.0f, 0.0f, -1.0f))
		m = glm::vec3(0.0f, 0.0f, 1.0f);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && m != glm::vec3(1.0f, 0.0f, 0.0f))
		m = glm::vec3(-1.0f, 0.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && m != glm::vec3(-1.0f, 0.0f, 0.0f))
		m = glm::vec3(1.0f, 0.0f, 0.0f);
	if (m.x != 0.0f || m.y != 0.0f || m.z != 0.0f) {
		if (move_count > 0.1) { // makes snake move every 0.1 second instead of a steady flow
			play1->Move(m * PLAYER_SPEED); // moves player m being the direction they move.
			if (m.x == 1.0f) {
				play1->setRot(glm::vec3(0.0f, 180.0f, 0.0f));
			} else if (m.x == -1.0f) {
				play1->setRot(glm::vec3(0.0f, 0.0f, 0.0f));
			} else if (m.z == 1.0f) { 
				play1->setRot(glm::vec3(0.0f, 90.0f, 0.0f));
			} else if (m.z == -1.0f) {
				play1->setRot(glm::vec3(0.0f, -90.0f, 0.0f));
			}

			if (play1->getTwice()) { // if you got a special pellet after one movement cycle your next point shows up so you dont collide with it
				play1->setTwice(false);
				gainPoint();
			}

			move_count = 0;
			
		} 
		else {
			move_count += dt;
		} 

		for (int i = 2; i < play1->Size(); i++) { // Check to see if body hit detects with the head
			if (play1->getHead()->hitbox->HitDetect(play1->GetTransform(), (CubeHitbox*)(play1->getNext(i)->hitbox), play1->getNext(i)->GetTransform())) {
				die(); //if it hits the head, it dies
			}
		}

		// head teleporting
		if (play1->getHead()->GetPosition().x > 12.0f) { // if the head's x position is above 12
			play1->setPos(glm::vec3(-11.5f, play1->getHead()->GetPosition().y, play1->getHead()->GetPosition().z)); // teleport player to opposite side of map
		}
		else if (play1->getHead()->GetPosition().x < -12.0f) {
			play1->setPos(glm::vec3(11.5f, play1->getHead()->GetPosition().y, play1->getHead()->GetPosition().z));
		}
		else if (play1->getHead()->GetPosition().z > 10.0f) {
			play1->setPos(glm::vec3(play1->getHead()->GetPosition().x, play1->getHead()->GetPosition().y, -8.5f));
		}
		else if (play1->getHead()->GetPosition().z < -9.0f) {
			play1->setPos(glm::vec3(play1->getHead()->GetPosition().x, play1->getHead()->GetPosition().y, 9.5f));
		}

		//body teleporting
		for (int i = 0; i < play1->Size(); i++) {

			if (play1->getNext(i)->GetPosition().x > 12.0f) {
				play1->getNext(i)->setPos(glm::vec3(-11.5f, play1->getNext(i)->GetPosition().y, play1->getNext(i)->GetPosition().z));
			}
			else if (play1->getNext(i)->GetPosition().x < -12.0f) {
				play1->getNext(i)->setPos(glm::vec3(11.5f, play1->getNext(i)->GetPosition().y, play1->getNext(i)->GetPosition().z));
			}
			else if (play1->getNext(i)->GetPosition().z > 10.0f) {
				play1->getNext(i)->setPos(glm::vec3(play1->getNext(i)->GetPosition().x, play1->getNext(i)->GetPosition().y, -8.5f));
			}
			else if (play1->getNext(i)->GetPosition().z < -9.0f) {
				play1->getNext(i)->setPos(glm::vec3(play1->getNext(i)->GetPosition().x, play1->getNext(i)->GetPosition().y, 9.5f));
			}

		}

		for (int i = 0; i < terrain.size(); i++) { // Check to see if snake hits objects
			if (play1->getHead()->hitbox->HitDetect(play1->GetTransform(), (CubeHitbox*)(terrain[i]->hitbox), terrain[i]->GetTransform())) {
				//std::cout << ("HIT DETECTED");
				if (!terrain[i]->getPellet()) { //identifies if the object is a pellet of an object
					die(); // the object was an obstacle, snake dies
				} 
				else { 
					if (terrain[i]->getSpc()) { // if pellet is special, player gains two points instead of one.
						gainPoint(); // gains point for collecting pellet
						play1->setTwice(true); // variable allows player to get their next point when it wont collide with them
					}
					else {
						gainPoint(); // gains point for collecting pellet
					}

					//Tia // randomly places pellets, and spawns them //
					glm::vec3 n = glm::vec3(0.0f, 0.6f, 0.0f);
					float random1 = (rand() % (10 - (-10))) + (-10);
					float random2 = (rand() % (7 - (-7))) + (-7);
					n += glm::vec3(random1, 0.0f, random2); ////////////
					random1 = (rand() % (3 - 1)) + 1; // Decides if the next pellet will be special or not //

					if (random1 == 1) {

						terrain[i]->setSpc(true);
						terrain[i]->Scale(glm::vec3(0.8f));
						terrain[i]->setMat(sPellet_mat);
					}
					else {
						terrain[i]->setSpc(false);
						terrain[i]->Scale(glm::vec3(0.6f));
						terrain[i]->setMat(pellet_mat);
					} ///////////////////////////////////////////////////////////////////////////////////////

					terrain[i]->setPos(n); // sets new position of the pellet

				}
			}
		}
	}
}
void OnePlayer::gainPoint() {
	// Points increase
	Object* point = new Object(score_mesh, score_mat, score_hit);
	point->Move({ -10.0f + score.size(), 2.0f, 8.0f });
	point->Scale({ 0.5f, 0.5f, 0.5f });

	score.push_back(point);

	play1->Add(tail_mesh, tail_mat, tail_hit); // Tail grows
}

void OnePlayer::die() {
	//resets player when touching object
	play1->setPos(glm::vec3(0.0f, 0.0f, 0.0f)); // resets position
	points = score.size(); // this would go on the leaderboard if there was one.
	while (score.size() > 0) { // resets points
		score.pop_back();
	}
	play1->deleteSnake();
}

void OnePlayer::Update(float dt)
{
	
}

void OnePlayer::Draw()
{
	glCullFace(GL_FRONT);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, sun->GetFrameBuffer());
	glClear(GL_DEPTH_BUFFER_BIT);
	sun->SetupDepthShader(sunShader);
	RenderScene(sunShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, sun->GetDepthMap());

	for (int l = 0; l < lights.size(); l++) {
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, lights[l]->GetFrameBuffer());
		glClear(GL_DEPTH_BUFFER_BIT);
		lights[l]->SetupDepthShader(depthShader);
		RenderScene(depthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		glActiveTexture(GL_TEXTURE4 + l);
		glBindTexture(GL_TEXTURE_CUBE_MAP, lights[l]->GetDepthMap());
	}
	glCullFace(GL_BACK);

	sun->SetupLight(shaderObj);
	for (int c = 0; c < lights.size(); c++) {
		lights[c]->SetupLight(shaderObj, c);
	}
	shaderObj->SetI("num_lights", lights.size());

	for (int c = 0; c < Cam.size(); c++) {
		Cam[c]->SetupCam(shaderObj);

		RenderScene(shaderObj);
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void OnePlayer::LoadScene()
{
	shaderObj = new Shader("Shaders/Basic_Shader.vert", "Shaders/Basic_Shader.frag");
	depthShader = new Shader("Shaders/depth_shader.vert", "Shaders/depth_shader.frag", "Shaders/depthGeo.glsl");
	sunShader = new Shader("Shaders/sunDepth.vert", "Shaders/sunDepth.frag");

	Material* DiceTex = new Material("dice-texture.png", "d6-normal.png");
	Material* D20Tex = new Material("d20-texture.png");
	Material* snakeHead = new Material("snakeheadtexture.png");
	Material* snakeBody = new Material("snaketexture.png");
	Material* pelletTex = new Material("default-pellet.png");
	Material* sPelletTex = new Material("special-pellet.png");
	Material* pointTex = new Material("score-texture.png");
	Material* objTex = new Material("obstacletexture.png");
	Material* defaultTex = new Material("default-texture.png", "default-texture.png");

	sun = new DirectionalLight(glm::normalize(glm::vec3(0.0f, 5.0f, 0.0f)), { 1.0f, 1.0f, 1.0f }, 0.1f, 0.2f, 0.2f);
	//lights.push_back(new PointLight({ 0.5f, 30.0f, 0.5f }, { 1.0f, 0.0f, 0.0f }, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, 0.3f, 0.5f, 1.0f, 0.014f, 0.0007f));
	//lights.push_back(new PointLight({ -4.0f, 3.0f, 4.0f }, { 1.0f, 1.0f, 1.0f }, 0.1f, 0.5f, 1.0f, 0.07f, 0.017f));

	Mesh* Square = new Mesh("d6.obj");
	Mesh* d20 = new Mesh("d20.obj");

	Hitbox* basicCubeHB = new CubeHitbox(1.0f,1.0f,1.0f);
	players.push_back(new Object(Square, DiceTex, basicCubeHB));
	players[PLAYER_1]->Scale({ 0.75f,0.75f,0.75f });
	players[PLAYER_1]->Move({ 0.0f, 0.3f, 0.0f });

	tail_mesh = Square;
	tail_mat = snakeBody;
	tail_hit = basicCubeHB;

	score_mesh = Square;
	score_mat = pointTex;
	score_hit = basicCubeHB;

	pellet_mat = pelletTex;
	sPellet_mat = sPelletTex;

	play1 = new Player(Square, snakeHead, basicCubeHB);
	play1->Scale({ 0.75f,0.75f,0.75f });
	play1->Move({ 0.0f, 0.3f, 0.0f });
	//play1->Add(tail_mesh, tail_mat, tail_hit);

	//Tia //Spawning the first pellet
	Object* pellet = new Object(Square, pellet_mat, basicCubeHB);

	float random1 = (rand() % (10 - (-10))) + (-10);
	float random2 = (rand() % (7 - (-7))) + (-7);
	pellet->Move({ random1, 0.6f, random2 });
	pellet->Scale({ 0.6f, 0.6f, 0.6f }); 
	pellet->setPellet(true);

	terrain.push_back(pellet);

	Object* botWall = new Object(Square, objTex, basicCubeHB);
	botWall->Move({ 0.0f, 1.0f, -8.0f });
	botWall->Scale({ 2.0f, 2.0f, 2.0f });

	terrain.push_back(botWall);

	Object* topWall = new Object(Square, objTex, basicCubeHB);
	topWall->Move({ 0.0f, 1.0f, 8.0f });
	topWall->Scale({ 2.0f, 2.0f, 2.0f });

	terrain.push_back(topWall);

	Object* rightWall = new Object(Square, objTex, basicCubeHB);
	rightWall->Move({ 11.0f, 1.0f, 0.0f });
	rightWall->Scale({ 2.0f, 2.0f, 2.0f });

	terrain.push_back(rightWall);

	Object* leftWall = new Object(Square, objTex, basicCubeHB);
	leftWall->Move({ -11.0f, 1.0f, 0.0f });
	leftWall->Scale({ 2.0f, 2.0f, 2.0f });

	terrain.push_back(leftWall);

	Object* floor = new Object(Square, defaultTex, basicCubeHB);
	floor->Move({ 0.0f, -0.75f, 0.0f });
	floor->Scale({ 30.0f, 0.5f, 30.0f });

	terrain.push_back(floor);


	Cam = {
		new Camera(glm::vec3(0.0f, 23.0f, 2.0f), {0.0f, 0.0f, 0.0f}, glm::vec4(0,0, SCREEN_WIDTH, SCREEN_HEIGHT))
	};

	// DEBUG THINGS
	DebugShader = new Shader("Shaders/debug.vert", "Shaders/debug.frag");
	DebugQuad = new Mesh(square, 4, square_index, 6);
}







//GDW STUFF BELOW, NOT FOR SNAKE




/*******************************************************************************************
*	Two Player Scene Functions
*******************************************************************************************/

TwoPlayer::TwoPlayer()
{
	LoadScene();
}

void TwoPlayer::InputHandle(GLFWwindow* window, glm::vec2 mousePos, float dt)
{
	if (glfwJoystickPresent(GLFW_JOYSTICK_1) && glfwJoystickIsGamepad(GLFW_JOYSTICK_1)) {
		ControllerInput(GLFW_JOYSTICK_1, PLAYER_1, dt);
	}
	else {
		KeyboardInput(window, mousePos, PLAYER_2, dt);
	}

	if(glfwJoystickPresent(GLFW_JOYSTICK_2) && glfwJoystickIsGamepad(GLFW_JOYSTICK_2)) {
		ControllerInput(GLFW_JOYSTICK_2, PLAYER_2, dt);
	}
}

void TwoPlayer::Update(float dt)
{
	if (players[PLAYER_1]->hitbox->HitDetect(players[PLAYER_1]->GetTransform(), (CubeHitbox*)(players[PLAYER_2]->hitbox), players[PLAYER_2]->GetTransform())) {
		std::cout << "HOII! HIT DETECTED!" << std::endl;
	}
	
	//Square->Update(dt);
}

void TwoPlayer::Draw()
{
	for (int c = 0; c < lights.size(); c++) {
		lights[c]->SetupLight(shaderObj, c);
	}
	shaderObj->SetI("num_lights", lights.size());

	for (int c = 0; c < Cam.size(); c++) {
		Cam[c]->SetupCam(shaderObj);

		RenderScene(shaderObj);
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void TwoPlayer::LoadScene()
{
	shaderObj = new Shader("Shaders/Basic_Shader.vert", "Shaders/Basic_Shader.frag");
	depthShader = new Shader("Shaders/depth_shader.vert", "Shaders/depth_shader.frag", "Shaders/depthGeo.glsl");

	Material* DiceTex = new Material("d6-normal.png");
	Material* D20Tex = new Material("d20-texture.png");
	Material* defaultTex = new Material("default-texture.png", "default-texture.png");

	//sun = new DirectionalLight(glm::normalize(glm::vec3(1.5f, 1.0f, 0.5f)), { 1.0f, 1.0f, 1.0f }, 0.1f, 0.5f, 1.0f);
	lights.push_back(new PointLight({ 0.0f, 30.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 0.1f, 0.5f, 1.0f, 0.014f, 0.0007f));
	lights.push_back(new PointLight({ -4.0f, 1.0f, 4.0f }, { 1.0f, 1.0f, 1.0f }, 0.1f, 0.5f, 1.0f, 0.07f, 0.017f));

	Mesh* Square = new Mesh("d6.obj");
	Mesh* d20 = new Mesh("d20.obj");
	Hitbox* basicCubeHB = new CubeHitbox(1.0f, 1.0f, 1.0f);

	players.push_back(new Object(Square, DiceTex, basicCubeHB));
	players.push_back(new Object(d20, D20Tex, basicCubeHB));

	players[PLAYER_2]->Scale(glm::vec3(0.75f, 0.75f, 0.75f));

	Object* floor = new Object(Square, defaultTex, basicCubeHB);
	floor->Move({ 0.0f, -1.5f, 0.0f });
	floor->Scale({ 30.0f, 0.5f, 30.0f });

	terrain.push_back(floor);

	Cam = {
		new Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec4(0,0, SCREEN_WIDTH / 2, SCREEN_HEIGHT)), // Cam 1
		new Camera(glm::vec3(2.0f, 0.0f, -4.0f), glm::vec4(SCREEN_WIDTH / 2,0, SCREEN_WIDTH / 2, SCREEN_HEIGHT)) // Cam 2
	};
}

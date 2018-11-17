#include <SDL2/SDL.h>

#include <exception>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <math.h>
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "ShaderProgram.h"
#include "stb_image.h"
#include "Texture.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

int main(int argc, char *argv[])
{
  if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    throw std::exception();
  }

  SDL_Window *window = SDL_CreateWindow("Triangle",
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

  if (!SDL_GL_CreateContext(window))
  {
	  throw std::exception();
  }
  if (glewInit() != GLEW_OK) 
  {
	  throw std::exception();
  }
  
  VertexArray *shape = new VertexArray("../legacy/curuthers.obj");

  //Da buff stuff
  VertexBuffer *positions = new VertexBuffer();
  positions->add(glm::vec3(-0.5f, 0.5f, 0.0f));
  positions->add(glm::vec3(-0.5f, -0.5f, 0.0f));
  positions->add(glm::vec3(0.5f, -0.5f, 0.0f));

  /*VertexBuffer *colors = new VertexBuffer();
  colors->add(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  colors->add(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
  colors->add(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));*/

  VertexBuffer *texCoords = new VertexBuffer();
  texCoords->add(glm::vec2(0.5f, 0.0f));
  texCoords->add(glm::vec2(0.0f, 1.0f));
  texCoords->add(glm::vec2(1.0f, 1.0f));

  //VertexArray *shape = new VertexArray();
  shape->setBuffer("in_Position", positions);
  //shape->SetBuffer("in_Color", colors);
  shape->setBuffer("in_TexCoord", texCoords);

  Texture *texture = new Texture("image.jpg");
  ShaderProgram *shaderProgram = new ShaderProgram("../shaders/vert.txt", "../shaders/frag.txt");

  
  bool quit = false;
  // Initialising the movement bools
  bool left = false;
  bool right = false;
  bool up = false;
  bool down = false;
  // Camera variables
  glm::vec3 camPosition = glm::vec3(0.0f, 0.0f, 1.0f);
  glm::vec3 camRotation = glm::vec3(0.0f, 0.0f, 0.0f);


  float angle = 0;
  float camAngle = 0;

  while(!quit)
  {
    SDL_Event event = {0};
	
    while(SDL_PollEvent(&event))
    {
      if(event.type == SDL_QUIT)
      {
        quit = true;
      }
	  else if(event.type == SDL_KEYDOWN)
	  {
		  //Take user input for movement 
		  switch (event.key.keysym.sym)
		  {
		  case SDLK_LEFT:
			  left = true;
			  break;
		  case SDLK_RIGHT:
			  right = true;
			  break;
		  case SDLK_UP:
			  up = true;
			  break;
		  case SDLK_DOWN:
			  down = true;
			  break;
		  default:
			  break;
		  }
	  }
	  else if (event.type == SDL_KEYUP)
	  {
		  //Reset the movements on key release to stop them 
		  switch (event.key.keysym.sym)
		  {
		  case SDLK_LEFT:
			  left = false;
			  break;
		  case SDLK_RIGHT:
			  right = false;
			  break;
		  case SDLK_UP:
			  up = false;
			  break;
		  case SDLK_DOWN:
			  down = false;
			  break;
		  default:
			  break;
		  }
	  }
    }
	
	if (left == true)
	{
		camPosition.x += 1;
	}
	if (right == true)
	{
		camPosition.x -= 1;
	}
	if (up == true)
	{
		camPosition.z += 1;
	}
	if (down == true)
	{
		camPosition.z -= 1;
	}

	// Create arbitary matrix
	glm::mat4 t(1.0f);
	// Rotate it by angle (Camera's Y rotation)
	t = glm::rotate(t, glm::radians(camAngle), glm::vec3(0, 1, 0));
	// Move forward 1 unit
	t = glm::translate(t, glm::vec3(0, 0, 0));
	// Apply to an initial position
	glm::vec3 fwd = t * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	// Normalize to get the unit vector
	fwd = glm::normalize(fwd);
	// Add it to camera position
	camPosition += fwd;

	// Camera movement
    glm::mat4 model(1.0f);
	model = glm::translate(model, camPosition);
	model = glm::rotate(model, glm::radians(camAngle), glm::vec3(0, 1, 0));
	shaderProgram->setUniform("in_Model", model);
	shaderProgram->setUniform("in_View", glm::inverse(model));


	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Calling texture
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, texture->getId());


	shaderProgram->draw(shape);

	// Draw the cat
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0, -2.5f));
	model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));
	shaderProgram->setUniform("in_Model", model);
	shaderProgram->setUniform("in_Texture", texture);
	shaderProgram->draw(shape);
	//Draw the Triangle
	//InstructOpenGL to use our shader prgram and our VAO
	/*glUseProgram(programId);
	glBindVertexArray(shape->GetId());

	//Draw 3 vertices (the triangle)
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//Reset the state
	glBindVertexArray(0);
	glUseProgram(0);*/

	/*
    // Draw with perspective projection matrix (Final triangle)
	shaderProgram->setUniform("in_Texture", 1);
	shaderProgram->setUniform("in_Projection", glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f));
	
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0, -2.5f));
	model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));

	shaderProgram->setUniform("in_Model", model);
	shaderProgram->draw(shape);
	
	
	angle += 100.0f;
	*/

	//Draw with orthographic projection matrix
	/*model = glm::mat4(1.0f);

	shaderProgram->setUniform("in_Projection", glm::ortho(0.0f, (float)WINDOW_WIDTH, 0.0f, (float)WINDOW_HEIGHT, 0.0f, 1.0f));

	model = glm::translate(model, glm::vec3(100, WINDOW_HEIGHT - 100, 0));
	model = glm::scale(model, glm::vec3(100, 100, 1));

	shaderProgram->setUniform("in_Model", model);
	shaderProgram->draw(shape);*/


	SDL_GL_SwapWindow(window);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

/*
 * Copyright (c) 2016 Jonathan Glines
 * Jonathan Glines <jonathan@glines.net>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <GL/glew.h>
#include <SDL.h>
#include <cstdlib>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "../common/arcballCamera.h"
#include "../common/scene.h"
#include "scanObject.h"

using namespace mc::samples;

struct demo {
  SDL_Window *window;
  SDL_GLContext glContext;
  int window_width, window_height;
  Scene *scene;
  std::shared_ptr<ArcballCamera> camera;
  std::shared_ptr<ScanObject> scanObject;
  int res;
} demo;

void init_sdl() {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Failed to initialize SDL: %s\n",
        SDL_GetError());
    exit(EXIT_FAILURE);
  }
  demo.window_width = 640;
  demo.window_height = 480;
  demo.window = SDL_CreateWindow(
      "CT Scan",  // title
      SDL_WINDOWPOS_UNDEFINED,  // x
      SDL_WINDOWPOS_UNDEFINED,  // y
      demo.window_width, demo.window_height,  // w, h
      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE  // flags
      );
  if (demo.window == NULL) {
    fprintf(stderr, "Failed to create SDL window: %s\n",
        SDL_GetError());
    exit(EXIT_FAILURE);
  }
}

void init_gl() {
  // Create an OpenGL context for our window
  demo.glContext = SDL_GL_CreateContext(demo.window);
  if (demo.glContext == NULL) {
    fprintf(stderr, "Failed to initialize OpenGL context: %s\n",
        SDL_GetError());
    exit(EXIT_FAILURE);
  }

  // Initialize GL entry points
  GLenum error = glewInit();
  if (error != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW: %s\n",
        glewGetErrorString(error));
    exit(EXIT_FAILURE);
  }

  // Configure the GL
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClearDepth(1.0);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glDisable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glViewport(0, 0, demo.window_width, demo.window_height);
}

void main_loop() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Draw the scene
  float aspect = (float)demo.window_width / (float)demo.window_height;
  demo.scene->draw(*demo.camera, aspect);

  SDL_GL_SwapWindow(demo.window);

  // Check for SDL events (user input, etc.)
  bool gui_input = false;
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_WINDOWEVENT:
        switch (event.window.event) {
          case SDL_WINDOWEVENT_SIZE_CHANGED:
            demo.window_width = event.window.data1;
            demo.window_height = event.window.data2;
            glViewport(0, 0, demo.window_width, demo.window_height);
            break;
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
        demo.camera->mouseButtonEvent(event.button,
            demo.window_width, demo.window_height);
        break;
      case SDL_MOUSEMOTION:
        demo.camera->mouseMotionEvent(event.motion,
            demo.window_width, demo.window_height);
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
/*
          case SDLK_m:
            demo.cubeObject->setAlgorithm(MC_SIMPLE_MARCHING_CUBES);
            break;
          case SDLK_s:
            demo.cubeObject->setAlgorithm(MC_ELASTIC_SURFACE_NETS);
            break;
          case SDLK_c:
            demo.cubeObject->setAlgorithm(MC_CUBERILLE);
            break;
#define INTENSITY_DELTA 0.1f
          case SDLK_COMMA:
            demo.cubeObject->setIntensity(
                demo.cubeObject->intensity() - INTENSITY_DELTA);
            fprintf(stderr, "intensity: %g\n", demo.cubeObject->intensity());
            break;
          case SDLK_PERIOD:
            demo.cubeObject->setIntensity(
                demo.cubeObject->intensity() + INTENSITY_DELTA);
            fprintf(stderr, "intensity: %g\n", demo.cubeObject->intensity());
            break;
*/
          case SDLK_w:
            demo.scanObject->setDrawWireframe(!demo.scanObject->isDrawWireframe());
            break;
          case SDLK_n:
            demo.scanObject->setDrawNormals(!demo.scanObject->isDrawNormals());
            break;
          case SDLK_o:
            demo.scanObject->setDrawOpaque(!demo.scanObject->isDrawOpaque());
            break;
        }
        switch (event.key.keysym.scancode) {
/*
          case SDL_SCANCODE_UP:
            demo.res = min(max(demo.res * 2, 2), 64);
            demo.cubeObject->setResolution(demo.res, demo.res, demo.res);
            break;
          case SDL_SCANCODE_DOWN:
            demo.res = min(max(demo.res / 2, 2), 64);
            demo.cubeObject->setResolution(demo.res, demo.res, demo.res);
            break;
          case SDL_SCANCODE_RIGHT:
            demo.cubeObject->setCube((demo.cubeObject->cube() + 1) % 256);
            break;
          case SDL_SCANCODE_LEFT:
            demo.cubeObject->setCube((demo.cubeObject->cube() - 1) % 256);
            break;
          case SDL_SCANCODE_PAGEUP:
            for (int i = MC_CUBE_NUM_CANONICAL_ORIENTATIONS - 1; i >= 0; --i) {
              if (mcCubeCanonicalOrientations[i] < demo.cubeObject->cube()) {
                fprintf(stdout, "mcCubeCanonicalOrientations[%d]: 0x%02x\n",
                    i, mcCubeCanonicalOrientations[i]);
                demo.cubeObject->setCube(mcCubeCanonicalOrientations[i]);
                break;
              }
            }
            break;
          case SDL_SCANCODE_PAGEDOWN:
            for (int i = 0; i < MC_CUBE_NUM_CANONICAL_ORIENTATIONS; ++i) {
              if (mcCubeCanonicalOrientations[i] > demo.cubeObject->cube()) {
                fprintf(stdout, "mcCubeCanonicalOrientations[%d]: 0x%02x\n",
                    i, mcCubeCanonicalOrientations[i]);
                demo.cubeObject->setCube(mcCubeCanonicalOrientations[i]);
                break;
              }
            }
            break;
          case SDL_SCANCODE_SPACE:
            demo.cubeObject->setDrawScalarField(
                !demo.cubeObject->isDrawScalarField());
            break;
*/
          default:
            ;
        }
        break;
      case SDL_QUIT:
        exit(EXIT_SUCCESS);
    }
  }
}

int main(int argc, char **argv) {
  // Initialize the graphics
  init_sdl();
  init_gl();

  // Create the graphics scene
  demo.scene = new Scene;
  demo.camera = std::shared_ptr<ArcballCamera>(
      new ArcballCamera(
        78.0f * ((float)M_PI / 180.0f),  // fovy
        0.1f,  // near
        1000.0f,  // far
        glm::vec3(0.0f, -2.0f, 0.0f),  // position
        glm::angleAxis(
          (float)M_PI / 2.0f,
          glm::vec3(1.0f, 0.0f, 0.0f)),  // orientation
        glm::vec3(0.0f, 0.0f, 0.0f)  // followPoint
        ));
  demo.res = 128;
  demo.scanObject = std::shared_ptr<ScanObject>(
      new ScanObject("./assets/scans/cthead",
        demo.res, demo.res, demo.res,  // resolution
        MC_SIMPLE_MARCHING_CUBES,  // algorithm
        glm::vec3(0.0f, 0.0f, 0.0f),  // position
        glm::angleAxis(
          (float)M_PI,
          glm::vec3(0.0f, 1.0f, 0.0f))  // orientation
        ));
  demo.scene->addObject(demo.scanObject);

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(main_loop, 0, 1);
#else
  while (1) {
    main_loop();
    // TODO: Wait for VSync? Or should we poll input faster than that?
  }
#endif

  // TODO: Free SDL and GL resources
  delete demo.scene;

  return EXIT_SUCCESS;
}

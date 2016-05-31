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

#ifndef MC_SAMPLES_COMMON_SCENE_H_
#define MC_SAMPLES_COMMON_SCENE_H_

#include <memory>
#include <vector>

namespace mc { namespace samples {
  class Camera;
  class SceneObject;
  /**
   * This class implements a simple graphics scene.
   *
   * OpenGL is typically used under the hood, but nothing about this scene
   * class requires the use of any specific graphics library.
   */
  class Scene {
    private:
      std::vector<std::shared_ptr<SceneObject> > m_objects;

    public:
      /**
       * Constructs a graphics scene. The default scene is empty.
       */
      Scene();
      /**
       * Destroys this graphics scene. All shared pointers to scene objects
       * held by the scene are released.
       */
      ~Scene();

      /**
       * Adds the given scene object to this graphics scene.
       */
      void addObject(std::shared_ptr<SceneObject> object) {
        this->m_objects.push_back(object);
      }

      /**
       * Advances the simulation of the scene (and the simulations carried out
       * by each scene object) forward by the given time interval in seconds.
       */
      void tick(float dt);

      /**
       * Draws the scene.
       *
       * The camera parameter controls the projection of the scene.
       *
       * The alpha parameter is given to control interpolation between ticks.
       * Scene object should strive to interpolate between the values of the
       * last tick and the current tick wherever possible.
       *
       * The debug parameter is a hint given to scene objects so that they can
       * toggle the drawing of debug information.
       *
       * Since the scene class makes no provisions for supporting any
       * particular graphics API, it is the responsibility of the derived scene
       * objects to draw using the correct API.
       */
      void draw(const Camera &camera, float aspect,
          float alpha = 1.0, bool debug = false) const;
  };
} }

#endif

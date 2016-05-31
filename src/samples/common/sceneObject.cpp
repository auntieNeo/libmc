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

#include <glm/gtc/matrix_transform.hpp>

#include "transformRAII.h"

#include "sceneObject.h"

namespace mc { namespace samples {
  SceneObject::SceneObject(
      const glm::vec3 &position, const glm::quat &orientation)
    : m_position(position), m_prevPosition(position),
      m_orientation(orientation), m_prevOrientation(orientation)
  {
  }

  SceneObject::~SceneObject() {
  }
  void SceneObject::m_tick(float dt) {
    // Delegate the actual simulation to derived classes
    this->tick(dt);

    // Tick all of our children
    for (auto child : this->m_children) {
      child->m_tick(dt);
    }
  }

  void SceneObject::m_draw(Transform &modelWorld, const glm::mat4 &worldView,
      const glm::mat4 &projection, float alpha, bool debug) const
  {
    TransformRAII mw(modelWorld);
    // Translate the object into position
    mw *= glm::translate(glm::mat4(), this->position(alpha));
    // Apply the object orientation as a rotation
    mw *= glm::mat4_cast(this->orientation(alpha));

    // Delegate the actual drawing to derived classes
    this->draw(mw.peek(), worldView, projection, alpha, debug);

    // Draw our children
    for (auto child : m_children) {
      child->m_draw(mw, worldView, projection, alpha, debug);
    }
  }
} }

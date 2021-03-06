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

#include <mcxx/coloredField.h>
#include <mcxx/contour.h>

#include "../common/sceneObject.h"

namespace mc { namespace samples { namespace coloredSquares {
  class ColoredSquareObject : public SceneObject {
    private:
      class SquareColoredField : public mc::ColoredField {
        private:
          int m_square;
          float m_intensity;
        public:
          SquareColoredField(
              int square,
              float intensity = 1.0f);
          int operator()(float x, float y, float z);
      };
      typedef struct {
        float pos[3], color[3];
      } WireframeVertex;

      GLuint m_wireframeVertices, m_wireframeIndices,
             m_squareWireframeVertices, m_squareWireframeIndices;
      int m_square, m_numWireframeIndices;

      void m_initWireframe();
      void m_initSquareWireframe();

      void m_updateWireframe(const mc::Contour &contour);
      void m_update();

      void m_drawWireframe(
          const glm::mat4 &modelView,
          const glm::mat4 &projection);
      void m_drawSquareWireframe(
          const glm::mat4 &modelView,
          const glm::mat4 &projection);

    public:
      ColoredSquareObject();

      void draw(const glm::mat4 &modelWorld,
          const glm::mat4 &worldView, const glm::mat4 &projection,
          float alpha, bool debug);
  };
} } }

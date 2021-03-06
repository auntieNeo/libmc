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

#include <mcxx/isosurfaceBuilder.h>
#include <mcxx/mesh.h>
#include <mcxx/scalarField.h>
#include <mcxx/vector.h>

namespace mc {
  IsosurfaceBuilder::IsosurfaceBuilder() {
    mcIsosurfaceBuilder_init(&m_internal);
  }

  IsosurfaceBuilder::~IsosurfaceBuilder() {
    for (auto mesh : m_meshes) {
      delete mesh;
    }
    mcIsosurfaceBuilder_destroy(&m_internal);
  }

  /*
  const Mesh *IsosurfaceBuilder::buildIsosurface(
      mcScalarField sf,
      mcAlgorithmFlag algorithm,
      void *args)
  {
    const mcMesh *m = mcIsosurfaceBuilder_isosurfaceFromField(
        &m_internal, sf, algorithm);
    Mesh *mesh = new Mesh(m);
    m_meshes.push_back(mesh);
    return mesh;
  }
  */

  float IsosurfaceBuilder::m_wrapScalarField(
      float x, float y, float z, ScalarField *sf)
  {
    return (*sf)(x, y, z);
  }

  const Mesh *IsosurfaceBuilder::buildIsosurface(
      ScalarField &sf,
      mcAlgorithmFlag algorithm,
      unsigned int x_res, unsigned int y_res, unsigned int z_res,
      const Vec3 &min, const Vec3 &max)
  {
    // Pass the scalar field functor as an argument
    const mcMesh *m = mcIsosurfaceBuilder_isosurfaceFromFieldWithArgs(
        &m_internal,
        (mcScalarFieldWithArgs)IsosurfaceBuilder::m_wrapScalarField,
         &sf,
         algorithm,
         x_res, y_res, z_res,
         &min.to_mcVec3(), &max.to_mcVec3()
         );
    Mesh *mesh = new Mesh(m);
    m_meshes.push_back(mesh);
    return mesh;
  }
}

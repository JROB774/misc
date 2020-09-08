Engine.Matrix3 = class
{
  /**
   * Constructs a new object instance of the 3x3 matrix class with the values
   * passed by the caller. If no values are given an identity matrix is made.
   *
   * The values are passed in as columns left-to-right, top-to-bottom.
   *
   * @param {Number} [n00=1] - The (0,0) element of the matrix.
   * @param {Number} [n01=0] - The (0,1) element of the matrix.
   * @param {Number} [n02=0] - The (0,2) element of the matrix.
   * @param {Number} [n10=0] - The (1,0) element of the matrix.
   * @param {Number} [n11=1] - The (1,1) element of the matrix.
   * @param {Number} [n12=0] - The (1,2) element of the matrix.
   * @param {Number} [n20=0] - The (2,0) element of the matrix.
   * @param {Number} [n21=0] - The (2,1) element of the matrix.
   * @param {Number} [n22=1] - The (2,2) element of the matrix.
   */
  constructor (n00=1, n01=0, n02=0, n10=0, n11=1, n12=0, n20=0, n21=0, n22=1)
  {
    this.elements = [ n00, n10, n20, n01, n11, n21, n02, n12, n22 ]
  }

  /**
   * Assigns new values to the matrix elements.
   *
   * The values are passed in as columns left-to-right, top-to-bottom.
   *
   * @param {Number} n00 - The (0,0) element of the matrix.
   * @param {Number} n01 - The (0,1) element of the matrix.
   * @param {Number} n02 - The (0,2) element of the matrix.
   * @param {Number} n10 - The (1,0) element of the matrix.
   * @param {Number} n11 - The (1,1) element of the matrix.
   * @param {Number} n12 - The (1,2) element of the matrix.
   * @param {Number} n20 - The (2,0) element of the matrix.
   * @param {Number} n21 - The (2,1) element of the matrix.
   * @param {Number} n22 - The (2,2) element of the matrix.
   */
  assign (n00, n01, n02, n10, n11, n12, n20, n21, n22)
  {
    this.elements = [ n00, n10, n20, n01, n11, n21, n02, n12, n22 ]
    return this
  }

  /**
   * Copies the contents of the passed in matrix to the calling matrix.
   *
   * @param  {Object} matrix - The matrix to copy the contents of.
   * @return {Object} The resulting matrix.
   */
  copy (matrix)
  {
    this.elements = [ ...matrix.elements ]
    return this
  }

  /**
   * Fills the matrix's elements with the values to make an identity matrix.
   *
   * An identity matrix is a type of matrix that, when multiplied with any
   * other matrix, will give back the exact same result -- unchanged.
   *
   * @return {Object} The newly made identity matrix.
   */
  assignIdentity ()
  {
    this.elements = [ 1, 0, 0, 0, 1, 0, 0, 0, 1 ]
  }

  /**
   * Multiplies the elements of the calling matrix by the passed in one.
   *
   * This does not modify the value of the calling matrix. To change the
   * value use the Engine.Matrix3.assignMulMatrix() method.
   *
   * @param  {Object} matrix - The matrix to multiply the caller by.
   * @return {Object} The resulting matrix.
   */
  mulMatrix (matrix)
  {
    let be = matrix.elements
    let ae = this.elements

    return new Engine.Matrix3(
      ae[0] * be[0] + ae[3] * be[1] + ae[6] * be[2],
      ae[0] * be[3] + ae[3] * be[4] + ae[6] * be[5],
      ae[0] * be[6] + ae[3] * be[7] + ae[6] * be[8],

      ae[1] * be[0] + ae[4] * be[1] + ae[7] * be[2],
      ae[1] * be[3] + ae[4] * be[4] + ae[7] * be[5],
      ae[1] * be[6] + ae[4] * be[7] + ae[7] * be[8],

      ae[2] * be[0] + ae[5] * be[1] + ae[8] * be[2],
      ae[2] * be[3] + ae[5] * be[4] + ae[8] * be[5],
      ae[2] * be[6] + ae[5] * be[7] + ae[8] * be[8]
    )
  }

  /**
   * The same as Engine.Matrix3.mulMatrix(), but the result is assigned to the
   * calling matrix.
   *
   * @param  {Object} matrix - The matrix to multiply the the caller by.
   * @return {Object} The resulting matrix.
   */
  assignMultiply (matrix)
  {
    return this.copy(this.mulMatrix(matrix))
  }

  /**
   * Translates (moves) the matrix by a specified amount along the X and Y
   * directions. This operation is additive, so it takes into consideration
   * the existing translation values of the matrix when calculating.
   *
   * This does not modify the value of the calling matrix. To change the
   * value use the Engine.Matrix3.assignTranslate() method.
   *
   * @param  {Number} x - The amount to translate along the X axis.
   * @param  {Number} y - The amount to translate along the Y axis.
   *
   * @return {Object} The resulting matrix.
   */
  translate (x, y)
  {
    let result = new Engine.Matrix3().copy(this)
    let re = result.elements

    re[6] += x * re[0] + y * re[3]
    re[7] += x * re[1] + y * re[4]
    re[8] += x * re[2] + y * re[5]

    return result
  }

  /**
   * The same as Engine.Matrix3.translate(), but the result is assigned to the
   * calling matrix.
   *
   * @param  {Number} x - The amount to translate along the X axis.
   * @param  {Number} y - The amount to translate along the Y axis.
   *
   * @return {Object} The resulting matrix.
   */
  assignTranslate (x, y)
  {
    return this.copy(this.translate(x, y))
  }

  /**
   * Scales the matrix by a specified amount along the X and Y directions. This
   * operation is additive, so it takes into consideration the existing scaling
   * values of the matrix when calculating.
   *
   * This does not modify the value of the calling matrix. To change the
   * value use the Engine.Matrix3.assignScale() method.
   *
   * @param  {Number} sx - The amount to scale along the X axis.
   * @param  {Number} sy - The amount to scale along the Y axis.
   *
   * @return {Object} The resulting matrix.
   */
  scale (sx, sy)
  {
    let result = new Engine.Matrix3().copy(this)
    let re = result.elements

    re[0] *= sx
    re[1] *= sx
    re[2] *= sx

    re[3] *= sy
    re[4] *= sy
    re[5] *= sy

    return result
  }

  /**
   * The same as Engine.Matrix3.scale(), but the result is assigned to the
   * calling matrix.
   *
   * @param  {Number} sx - The amount to scale along the X axis.
   * @param  {Number} sy - The amount to scale along the Y axis.
   *
   * @return {Object} The resulting matrix.
   */
  assignScale (sx, sy)
  {
    return this.copy(this.scale(sx, sy))
  }

  /**
   * Rotates the matrix by a specified amount -- in 2D space. This operation is
   * additive, so it takes into consideration the existing rotation values of
   * the matrix when calculating.
   *
   * This does not modify the value of the calling matrix. To change the value
   * use the Engine.Matrix3.assignRotate() method.
   *
   * @param  {Number} radians - The angle, in radians, to rotate by.
   * @return {Object} The resulting matrix.
   */
  rotate (radians)
  {
    let result = new Engine.Matrix3().copy(this)
    let re = result.elements

    let c = Math.cos(radians)
    let s = Math.sin(radians)

    let n00 = re[0], n10 = re[1], n20 = re[2]
    let n01 = re[3], n11 = re[4], n21 = re[5]

    re[0] =   c * n00 + s * n01
    re[1] =   c * n10 + s * n11
    re[2] =   c * n20 + s * n21

    re[3] = - s * n00 + c * n01
    re[4] = - s * n10 + c * n11
    re[5] = - s * n20 + c * n21

    return result
  }

  /**
   * The same as Engine.Matrix3.rotate(), but the result is assigned to the
   * calling matrix.
   *
   * @param  {Number} radians - The angle, in radians, to rotate by.
   * @return {Object} The resulting matrix.
   */
  assignRotate (radians)
  {
    return this.copy(this.rotate(radians))
  }

  /**
   * Compares two matrices and determines if they are equal or not.
   *
   * @param  {Object}  matrix - The matrix to compare with.
   * @return {Boolean} True if they are equal, false is they are not.
   */
  equalTo (matrix)
  {
    var be = matrix.elements
    var ae = this.elements

    for (let i = 0; i < 9; i++) {
      if (ae[i] !== be[i]) {
        return false
      }
    }

    return true
  }

  /**
   * Converts the matrix into the Float32Array type. This is needed when
   * passing the matrix data to WebGL function calls, as WebGL needs a specific
   * element type in order to handle the floating-point data correctly.
   *
   * @return {Object} The current matrix data as a Float32Array.
   */
  toFloat32Array ()
  {
    return new Float32Array(this.elements)
  }
}

Engine.Vector2 = class
{
  /**
   * Constructs a new object instance of the 2D vector class with the values
   * passed by the caller -- or default values if nothing is provided.
   *
   * @param {Number} [x=0] - The vector's X component.
   * @param {Number} [y=0] - The vector's Y component.
   */
  constructor (x = 0, y = 0)
  {
    this.x = x, this.y = y
  }

  /**
   * Assigns a new (x,y) value to the vector.
   *
   * @param  {Number} x - The vector's X component.
   * @param  {Number} y - The vector's Y component.
   *
   * @return {Object} The resulting vector.
   */
  assign (x, y)
  {
    this.x = x, this.y = y
    return this
  }

  /**
   * Copies the contents of the passed in vector to the calling vector.
   *
   * @param  {Object} vector - The vector to copy the contents of.
   * @return {Object} The resulting vector.
   */
  copy (vector)
  {
    this.x = vector.x, this.y = vector.y
    return this
  }

  /**
   * Adds the components of the passed in vector to the calling one.
   *
   * This does not modify the value of the calling vector. To change the value
   * use the Engine.Vector2.assignAddVector() method.
   *
   * @param  {Object} vector - The vector to add the components of.
   * @return {Object} The resulting vector.
   */
  addVector (vector)
  {
    return new Engine.Vector2(this.x + vector.x, this.y + vector.y)
  }

  /**
   * Adds a scalar value to the X and Y components of the vector.
   *
   * This does not modify the value of the calling vector. To change the value
   * use the Engine.Vector2.assignAddScalar() method.
   *
   * @param  {Number} scalar - The scalar to add to the components.
   * @return {Object} The resulting vector.
   */
  addScalar (scalar)
  {
    return new Engine.Vector2(this.x + scalar, this.y + scalar)
  }

  /**
   * Subtracts the components of the passed in vector from the calling one.
   *
   * This does not modify the value of the calling vector. To change the value
   * use the Engine.Vector2.assignSubVector() method.
   *
   * @param  {Object} vector - The vector to subtract from the caller.
   * @return {Object} The resulting vector.
   */
  subVector (vector)
  {
    return new Engine.Vector2(this.x - vector.x, this.y - vector.y)
  }

  /**
   * Subtracts a scalar value from the X and Y components of the vector.
   *
   * This does not modify the value of the calling vector. To change the value
   * use the Engine.Vector2.assignSubScalar() method.
   *
   * @param  {Number} scalar - The scalar to subtract from the caller.
   * @return {Object} The resulting vector.
   */
  subScalar (scalar)
  {
    return new Engine.Vector2(this.x - scalar, this.y - scalar)
  }

  /**
   * Divides the components of the calling vector by the passed in one.
   *
   * This does not modify the value of the calling vector. To change the value
   * use the Engine.Vector2.assignDivVector() method.
   *
   * @param  {Object} vector - The vector to divide the caller by.
   * @return {Object} The resulting vector.
   */
  divVector (vector)
  {
    return new Engine.Vector2(this.x / vector.x, this.y - vector.y)
  }

  /**
   * Divides the components of the calling vector by the passed scalar.
   *
   * This does not modify the value of the calling vector. To change the value
   * use the Engine.Vector2.assignDivScalar() method.
   *
   * @param  {Number} scalar - The scalar to divide the caller by.
   * @return {Object} The resulting vector.
   */
  divScalar (scalar)
  {
    return new Engine.Vector2(this.x / scalar, this.y / scalar)
  }

  /**
   * Multiplies the components of the calling vector by the passed in one.
   *
   * This does not modify the value of the calling vector. To change the value
   * use the Engine.Vector2.assignMulVector() method.
   *
   * @param  {Object} vector - The vector to multiply the caller by.
   * @return {Object} The resulting vector.
   */
  mulVector (vector)
  {
    return new Engine.Vector2(this.x * vector.x, this.y * vector.y)
  }

  /**
   * Multiplies the components of the calling vector by the passed scalar.
   *
   * This does not modify the value of the calling vector. To change the value
   * use the Engine.Vector2.assignMulScalar() method.
   *
   * @param  {Number} scalar - The scalar to multiply the caller by.
   * @return {Object} The resulting vector.
   */
  mulScalar (scalar)
  {
    return new Engine.Vector2(this.x * scalar, this.y * scalar)
  }

  /**
   * The same as Engine.Vector2.addVector(), but the result is assigned to the
   * calling vector.
   *
   * @param  {Object} vector - The vector to add the components of.
   * @return {Object} The resulting vector.
   */
  assignAddVector (vector)
  {
    return this.copy(this.addVector(vector))
  }

  /**
   * The same as Engine.Vector2.addScalar(), but the result is assigned to the
   * calling vector.
   *
   * @param  {Number} scalar - The scalar to add to the components.
   * @return {Object} The resulting vector.
   */
  assignAddScalar (scalar)
  {
    return this.copy(this.addScalar(scalar))
  }

  /**
   * The same as Engine.Vector2.subVector(), but the result is assigned to the
   * calling vector.
   *
   * @param  {Object} vector - The vector to subtract from the caller.
   * @return {Object} The resulting vector.
   */
  assignSubScalar (vector)
  {
    return this.copy(this.subVector(vector))
  }

  /**
   * The same as Engine.Vector2.subScalar(), but the result is assigned to the
   * calling vector.
   *
   * @param  {Number} scalar - The scalar to subtract from the caller.
   * @return {Object} The resulting vector.
   */
  assignSubScalar (scalar)
  {
    return this.copy(this.subScalar(scalar))
  }

  /**
   * The same as Engine.Vector2.divVector(), but the result is assigned to the
   * calling vector.
   *
   * @param  {Object} vector - The vector to divide the caller by.
   * @return {Object} The resulting vector.
   */
  assignDivVector (vector)
  {
    return this.copy(this.divVector(vector))
  }

  /**
   * The same as Engine.Vector2.divScalar(), but the result is assigned to the
   * calling vector.
   *
   * @param  {Number} scalar - The scalar to divide the caller by.
   * @return {Object} The resulting vector.
   */
  assignDivScalar (scalar)
  {
    return this.copy(this.divScalar(scalar))
  }

  /**
   * The same as Engine.Vector2.mulVector(), but the result is assigned to the
   * calling vector.
   *
   * @param  {Object} vector - The vector to multiply the caller by.
   * @return {Object} The resulting vector.
   */
  assignMulVector (vector)
  {
    return this.copy(this.mulVector(vector))
  }

  /**
   * The same as Engine.Vector2.mulScalar(), but the result is assigned to the
   * calling vector.
   *
   * @param  {Number} scalar - The scalar to multiply the caller by.
   * @return {Object} The resulting vector.
   */
  assignMulScalar (scalar)
  {
    return this.copy(this.mulScalar(scalar))
  }

  /**
   * Normalizes the vector's components (puts them in the range -1 to 1).
   *
   * This does not modify the value of the calling vector. To change the value
   * use the Engine.Vector2.assignNormalize() method.
   *
   * @return {Object} The resulting vector.
   */
  normalize ()
  {
    return new Engine.Vector2(this.x, this.y).divScalar(this.length())
  }

  /**
   * The same as Engine.Vector2.normalize(), but the result is assigned to the
   * calling vector.
   *
   * @return {Object} The resulting vector.
   */
  assignNormalize ()
  {
    return this.copy(this.normalize())
  }

  /**
   * Rotates the direction of the vector by a specific angle. The angle should
   * be specified in radians, not degrees.
   *
   * This does not modify the value of the calling vector. To change the value
   * use the Engine.Vector2.assignRotate() method.
   *
   * @param  {Number} radians - The amount to rotate the vector by.
   * @return {Object} The resulting vector.
   */
  rotate (radians)
  {
    let c = Math.cos(radians)
    let s = Math.sin(radians)

    return new Engine.Vector2(
      this.x * c - this.y * s,
      this.x * s + this.y * c
    )
  }

  /**
   * The same as Engine.Vector2.rotate(), but the result is assigned to the
   * calling vector.
   *
   * @param  {Number} radians - The amount to rotate the vector by.
   * @return {Object} The resulting vector.
   */
  assignRotate (radians)
  {
    return this.copy(this.rotate(radians))
  }

  /**
   * Returns the absolute values of the vector's compontents.
   *
   * This does not modify the value of the calling vector. To change the value
   * use the Engine.Vector2.assignAbsolute() method.
   *
   * @return {Object} The resulting vector.
   */
  absolute ()
  {
    return new Engine.Vector2(Math.abs(this.x), Math.abs(this.y))
  }

  /**
   * The same as Engine.Vector2.absolute(), but the result is assigned to the
   * calling vector.
   *
   * @return {Object} The resulting vector.
   */
  assignAbsolute (radians)
  {
    return this.copy(this.absolute())
  }

  /**
   * Calculates the 2D vector dot product between two vectors.
   *
   * @param  {Object} vector - The other vector to use in the calculation.
   * @return {Number} The result of the dot product operation.
   */
  dot (vector)
  {
    return ((this.x * vector.x) + (this.y * vector.y))
  }

  /**
   * Calculates and returns the length of the vector's components.
   *
   * @return {Number} The length of the vector.
   */
  length ()
  {
    return (Math.sqrt((this.x * this.x) + (this.y * this.y)))
  }

  /**
   * Compares two vectors and determines if they are equal or not.
   *
   * @param  {Object}  vector - The vector to compare with.
   * @return {Boolean} True if they are equal, false is they are not.
   */
  equalTo (vector)
  {
    return ((this.x === vector.x) && (this.y === vector.y))
  }
}

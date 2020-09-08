Engine.Color = class
{
  /**
   * Constructs a new object instance of the color class with the values
   * passed by the caller -- or default values if nothing is provided.
   *
   * @param {Number} [r=1] - The color's red component.
   * @param {Number} [g=1] - The color's green component.
   * @param {Number} [b=1] - The color's blue component.
   * @param {Number} [a=1] - The color's alpha component.
   */
  constructor (r = 1, g = 1, b = 1, a = 1)
  {
    this.r = r
    this.g = g
    this.b = b
    this.a = a
  }

  /**
   * Assigns a new RGBA value to the color.
   * If the alpha is not specified it will keep its current value.
   *
   * @param  {Number}  r  - The color's red component.
   * @param  {Number}  g  - The color's green component.
   * @param  {Number}  b  - The color's blue component.
   * @param  {Number} [a] - The color's alpha component.
   *
   * @return {Object} The result of the operation.
   */
  assign (r, g, b, a)
  {
    this.r = r
    this.g = g
    this.b = b

    if (a !== undefined) {
      this.a = a
    }

    return this
  }

  /**
   * Copies the contents of the passed in color to the calling color.
   *
   * @param  {Object} color - The color to copy the contents of.
   * @return {Object} The result of the operation.
   */
  copy (color)
  {
    this.r = color.r
    this.g = color.g
    this.b = color.b
    this.a = color.a

    return this
  }

  /**
   * Compares two colors and determines if they are equal or not.
   *
   * @param  {Object}  color - The color to compare with.
   * @return {Boolean} True if equal, otherwise false.
   */
  equalTo (color)
  {
    return ((this.r === color.r) &&
            (this.g === color.g) &&
            (this.b === color.b) &&
            (this.a === color.a))
  }

  /**
   * Converts the RGBA color values into an array of the same order.
   *
   * @return {Array} An array of the RGBA components.
   */
  toArray ()
  {
    return [ this.r, this.g, this.b, this.a ]
  }
}

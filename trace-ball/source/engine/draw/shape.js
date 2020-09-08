Engine.Shape =
{
  /**
   * Draws a rectangle using the color set with Engine.Shape.setColor().
   *
   * @param {Number} x - Horizontal position of the rectangle.
   * @param {Number} y - Vertical position of the rectangle.
   * @param {Number} w - Width of the rectangle.
   * @param {Number} h - Height of the rectangle.
   */
  drawRectangle: function (x, y, w, h)
  {
    let webgl = Engine.Canvas._.webgl

    x += Engine.Canvas._.drawOffset.x
    y += Engine.Canvas._.drawOffset.y

    let vertices = new Float32Array([ x,y+h, x,y, x+w,y+h, x+w,y ])
    Engine.Shape._.drawShape(vertices, webgl.TRIANGLE_STRIP)
  },

  /**
   * Draws a triangle using the color set with Engine.Shape.setColor().
   *
   * @param {Number} x1 - Horizontal position of the first point.
   * @param {Number} y1 - Vertical position of the first point.
   * @param {Number} x2 - Horizontal position of the second point.
   * @param {Number} y2 - Vertical position of the second point.
   * @param {Number} x3 - Horizontal position of the third point.
   * @param {Number} y3 - Vertical position of the third point.
   */
  drawTriangle: function (x1,y1, x2,y2, x3,y3)
  {
    let webgl = Engine.Canvas._.webgl

    x1 += Engine.Canvas._.drawOffset.x
    y1 += Engine.Canvas._.drawOffset.y
    x2 += Engine.Canvas._.drawOffset.x
    y2 += Engine.Canvas._.drawOffset.y
    x3 += Engine.Canvas._.drawOffset.x
    y3 += Engine.Canvas._.drawOffset.y

    let vertices = new Float32Array([ x1,y1, x2,y2, x3,y3 ])
    Engine.Shape._.drawShape(vertices, webgl.TRIANGLES)
  },

  /**
   * Sets the color to draw shapes for all subsequent shape draw calls until
   * the next time that the shape draw color is set to a brand new value.
   *
   * @param {Number} r - Red component of the draw color.
   * @param {Number} g - Green component of the draw color.
   * @param {Number} b - Blue component of the draw color.
   * @param {Number} a - Alpha component of the draw color.
   */
  setColorRGBA: function (r, g, b, a)
  {
    Engine.Shape._.color.assign(r, g, b, a)
  },

  /**
   * Sets the color to draw shapes for all subsequent shape draw calls until
   * the next time that the shape draw color is set to a brand new value.
   *
   * @param {Object} color - New draw color value.
   */
  setColor: function (color)
  {
    // Make a copy to avoid passing by reference.
    Engine.Shape._.color.copy(color)
  },

  /* ------------------------------------------------------------------------ */

  _:
  {
    color: null, /** Current color used to draw any primitive shapes. */
    vbo:   null, /** Intermediate buffer for storing shape vertices.  */

    /**
     * Initializes the internals of the shape system so that it is ready for
     * use. This function is called on start-up by the engine, so it does not
     * need to be called anywhere else in the code.
     *
     * @access private
     */
    initialize: function ()
    {
      Engine.Shape._.vbo = Engine.Canvas._.webgl.createBuffer()
      Engine.Shape._.color = new Engine.Color() // Default color white.
    },

    /**
     * A generic shape drawing function that can take in an arbitrary number of
     * points and will draw the polygon/shape in the currently set draw color.
     * This function is called by the specific shape drawing functions to avoid
     * repeating the same WebGL boilerplate drawing code in multiple functions.
     *
     * The 'drawMode' value must be one of the WebGL enum values defined here:
     * <developer.mozilla.org/Web/API/WebGL_API/Constants#Rendering_primitives>
     *
     * @param {Array}  vertices - Vertex data defining the shape to draw.
     * @param {Number} drawMode - How the vertices should be interpreted.
     *
     * @access private
     */
    drawShape: function (vertices, drawMode)
    {
      let shader = Engine.Assets.untextured
      Engine.Shader.setCurrent(shader)

      let webgl = Engine.Canvas._.webgl

      webgl.bindBuffer(webgl.ARRAY_BUFFER, Engine.Shape._.vbo)
      webgl.bufferData(webgl.ARRAY_BUFFER, vertices, webgl.DYNAMIC_DRAW)

      shader.enableAttribute('in_vert_pos', 2, false, 0, 0)

      let worldSpace = Engine.Canvas._.worldSpace.toFloat32Array()
      let drawColor = Engine.Shape._.color.toArray()

      shader.setUniform('world_space_matrix', worldSpace)
      shader.setUniform('object_draw_color',  drawColor)

      // Vertices stores points and each vertex has two points so there are
      // 'half the number of points' actual vertices within the VBO to draw.
      webgl.drawArrays(drawMode, 0, vertices.length / 2)
    },
  },

  /* ------------------------------------------------------------------------ */
}

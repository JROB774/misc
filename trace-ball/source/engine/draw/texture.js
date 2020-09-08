Engine.Texture = class
{
  /**
   * Constructs a new object instance of the texture class. It is recommended
   * that engine users do not call this function directly. Instead the Asset
   * Loader system should be used to load in and create texture instances.
   *
   * For more information see file: './source/engine/core/asset-loader.js'
   *
   * @param {Object} image - A fully-loaded HTML5 image object.
   */
  constructor (image)
  {
    let webgl = Engine.Canvas._.webgl

    let texture = webgl.createTexture()
    let target  = webgl.TEXTURE_2D
    let format  = webgl.RGBA
    let type    = webgl.UNSIGNED_BYTE

    webgl.bindTexture(target, texture)
    webgl.texImage2D(target, 0, format, format, type, image)

    let wrap = webgl.CLAMP_TO_EDGE
    let min  = webgl.LINEAR_MIPMAP_LINEAR
    let max  = webgl.LINEAR

    webgl.texParameteri(target, webgl.TEXTURE_WRAP_S,     wrap)
    webgl.texParameteri(target, webgl.TEXTURE_WRAP_T,     wrap)
    webgl.texParameteri(target, webgl.TEXTURE_MIN_FILTER,  min)
    webgl.texParameteri(target, webgl.TEXTURE_MAG_FILTER,  max)

    // Mipmaps are used to generate better smoothing when scaling textures.
    webgl.generateMipmap(target)

    // VBOs are needed to allow for drawing the texture to a simplistic quad
    // mesh using WebGL. They are filled with data during a draw call as the
    // values they hold may change based on if the texture is clipped or not.

    let vertexVBO   = webgl.createBuffer()
    let texCoordVBO = webgl.createBuffer()

    this._ = {} // Private members stored in here.
    this._.texture     = texture
    this._.width       = image.width
    this._.height      = image.height
    this._.anchor      = 'center'
    this._.color       = new Engine.Color() // Default is white.
    this._.vertexVBO   = vertexVBO
    this._.texCoordVBO = texCoordVBO
    this._.image       = image
  }

  /**
   * Draws a texture to the canvas, applying the desired transformation effects
   * to the texture (scale, rotate, etc.) if they have been specified.
   *
   * @param {Number} x         - Horizontal position to draw the texture.
   * @param {Number} y         - Vertical position to draw the texture.
   * @param {Number} [sx=1]    - How much to scale the texture horizontally.
   * @param {Number} [sy=1]    - How much to scale the texture vertically.
   * @param {Number} [angle=0] - Angle to rotate by (in radians).
   */
  draw (x, y, sx = 1, sy = 1, angle = 0)
  {
    let anchor = this._getAnchorPosition(this._.width, this._.height)
    this._draw(x, y, sx, sy, anchor.x, anchor.y, angle, null, null)
  }

  /**
   * Draws a clipped portion of a texture to the canvas, applying the desired
   * transformation effects to the texture (scale, rotate, etc.) if they have
   * been specified. Note that the clip should be specified in pixel sizes.
   *
   * @param {Number} x         - Horizontal position to draw the texture.
   * @param {Number} y         - Vertical position to draw the texture.
   * @param {Object} clip      - Clipped texture bounds of the portion to draw.
   * @param {Number} [sx=1]    - How much to scale the texture horizontally.
   * @param {Number} [sy=1]    - How much to scale the texture vertically.
   * @param {Number} [angle=0] - Angle to rotate by (in radians).
   */
  drawClipped (x, y, clip, sx = 1, sy = 1, angle = 0)
  {
    // Texture coordinate positions.
    let tx1 = clip.x / this._.width
    let ty1 = clip.y / this._.height
    let tx2 = tx1 + (clip.w / this._.width)
    let ty2 = ty1 + (clip.h / this._.height)

    // Vertex coordinate sizes.
    let vw = clip.w
    let vh = clip.h

    let vertices  = new Float32Array([ 0,vh, 0,0, vw,vh, vw,0 ])
    let texCoords = new Float32Array([ tx1,ty2, tx1,ty1, tx2,ty2, tx2,ty1 ])

    let anchor = this._getAnchorPosition(clip.w, clip.h)
    this._draw(x, y, sx, sy, anchor.x, anchor.y, angle, vertices, texCoords)
  }

  /**
   * Draws a clipped portion of a texture to the canvas, applying the desired
   * transformation effects to the texture (scale, rotate, etc.) if they have
   * been specified. The portion of the texture to be drawn is determined by
   * the passed in animation, that will be used to setup the clipped section.
   *
   * @param {Number} x         - Horizontal position to draw the texture.
   * @param {Number} y         - Vertical position to draw the texture.
   * @param {Object} animation - Animation to draw the texture with.
   * @param {Number} [sx=1]    - How much to scale the texture horizontally.
   * @param {Number} [sy=1]    - How much to scale the texture vertically.
   * @param {Number} [angle=0] - Angle to rotate by (in radians).
   */
  drawAnimated (x, y, animation, sx = 1, sy = 1, angle = 0)
  {
    let frame = animation.getCurrentFrame()
    this.drawClipped(x, y, frame, sx, sy, angle)
  }

  /**
   * Sets the new anchor/draw-origin for the texture. Tbe string must be a
   * valid anchor string specifier value, one of the following values:
   *
   *    'top-left',    'top-center',    'top-right'
   * 'center-left',        'center', 'center-right'
   * 'bottom-left', 'bottom-center', 'bottom-right'
   *
   * @param {String} anchor - A valid anchor specifier string.
   */
  setAnchor (anchor)
  {
    this._.anchor = anchor
  }

  /**
   * Sets the modulation color to use when drawing. This will blend each of the
   * texture's pixel colors with the modulation color before rendering:
   *
   * OUTPUT_COLOR = SRC_RGBA * MOD_RGBA
   *
   * @param {Number} r - Modulation color's red component.
   * @param {Number} g - Modulation color's green component.
   * @param {Number} b - Modulation color's blue component.
   * @param {Number} a - Modulation color's alpha component.
   */
  setColorRGBA (r, g, b, a)
  {
    this._.color.assign(r, g, b, a)
  }

  /**
   * Sets the modulation color to use when drawing. This will blend each of the
   * texture's pixel colors with the modulation color before rendering:
   *
   * OUTPUT_COLOR = SRC_RGBA * MOD_RGBA
   *
   * @param {Number} color - Modulation color to use.
   */
  setColor (color)
  {
    this._.color = color
  }

  /**
   * Retrieves the width of the texture itself.
   *
   * @return {Number} Texture's width in pixels.
   */
  getWidth ()
  {
    return this._.width
  }

  /**
   * Retrieves the height of the texture itself.
   *
   * @return {Number} Texture's height in pixels.
   */
  getHeight ()
  {
    return this._.height
  }

  /**
   * Retrieves the type of anchor being used.
   *
   * @return {String} Texture's anchor type.
   */
  getAnchor ()
  {
    return this._.anchor
  }

  /* ------------------------------------------------------------------------ */

  /**
   * A generic draw function for the texture. All of the other draw functions
   * carry out different methods t set up the parameters that are passed to
   * this function -- which carries out the actual drawing of a WebGL texture.
   *
   * @param {Number} x         - Horizontal position to draw the texture.
   * @param {Number} y         - Vertical position to draw the texture.
   * @param {Object} animation - Animation to draw the texture with.
   * @param {Number} sx        - How much to scale the texture horizontally.
   * @param {Number} sy        - How much to scale the texture vertically.
   * @param {Number} angle     - Angle to rotate by (in radians).
   * @param {Array}  vertices  - Vertex coordinates for the texture quad.
   * @param {Array}  texCoords - Texture coordinates for the texture quad.
   *
   * @access private
   */
  _draw (x, y, sx, sy, ax, ay, angle, vertices, texCoords)
  {
    let shader = Engine.Assets.textured
    let webgl = Engine.Canvas._.webgl

    x += Engine.Canvas._.drawOffset.x
    y += Engine.Canvas._.drawOffset.y

    Engine.Shader.setCurrent(shader)

    // We use an object space matrix to perform any translation, scaling and
    // rotation to the texture before drawing it in the current world space.
    let objectSpaceMatrix = new Engine.Matrix3()
    objectSpaceMatrix.assignTranslate(   x,   y)
    objectSpaceMatrix.assignRotate   (    angle)
    objectSpaceMatrix.assignScale    (  sx,  sy)
    objectSpaceMatrix.assignTranslate( -ax, -ay) // Offsets by anchor.

    let worldSpace  = Engine.Canvas._.worldSpace.toFloat32Array()
    let objectSpace = objectSpaceMatrix.toFloat32Array()
    let modColor    = this._.color.toArray()

    var w = this._.width
    var h = this._.height

    // If no custom data is provided we set default data values.
    if (!vertices)  { vertices  = new Float32Array([ 0,h, 0,0, w,h, w,0 ]) }
    if (!texCoords) { texCoords = new Float32Array([ 0,1, 0,0, 1,1, 1,0 ]) }

    let texture     = this._.texture
    let vertexVBO   = this._.vertexVBO
    let texCoordVBO = this._.texCoordVBO

    webgl.bindBuffer(webgl.ARRAY_BUFFER, vertexVBO)
    webgl.bufferData(webgl.ARRAY_BUFFER, vertices, webgl.DYNAMIC_DRAW)
    webgl.bindBuffer(webgl.ARRAY_BUFFER, texCoordVBO)
    webgl.bufferData(webgl.ARRAY_BUFFER, texCoords, webgl.DYNAMIC_DRAW)

    webgl.bindBuffer(webgl.ARRAY_BUFFER, vertexVBO)
    shader.enableAttribute('in_vert_pos',  2, false, 2, 0)
    webgl.bindBuffer(webgl.ARRAY_BUFFER, texCoordVBO)
    shader.enableAttribute('in_tex_coord', 2, true,  2, 0)

    shader.setUniform('world_space_matrix',   worldSpace)
    shader.setUniform('object_space_matrix', objectSpace)
    shader.setUniform('texture',                       0)
    shader.setUniform('mod_color',              modColor)

    webgl.activeTexture(webgl.TEXTURE0)
    webgl.bindTexture(webgl.TEXTURE_2D, texture)

    webgl.drawArrays(webgl.TRIANGLE_STRIP, 0, 4)
  }

  /**
   * An internal texture function used to convert the anchor specifier string
   * into usable coordinates, based on the size of the texture / texture clip.
   *
   * @param {Number} w - Width of the texture or clip to draw.
   * @param {Number} h - Height of the texture or clip to draw.
   *
   * @return {Object} An engine vector containing the anchor position.
   *
   * @access private
   */
  _getAnchorPosition (w, h)
  {
    switch (this._.anchor) {
      case 'top-left':      return new Engine.Vector2(  0,   0)
      case 'top-center':    return new Engine.Vector2(w/2,   0)
      case 'top-right':     return new Engine.Vector2(  w,   0)
      case 'center-left':   return new Engine.Vector2(  0, h/2)
      case 'center':        return new Engine.Vector2(w/2, h/2)
      case 'center-right':  return new Engine.Vector2(  w, h/2)
      case 'bottom-left':   return new Engine.Vector2(  0,   h)
      case 'bottom-center': return new Engine.Vector2(w/2,   h)
      case 'bottom-right':  return new Engine.Vector2(  w,   h)

      default:
        console.error('Unknown texture anchor alignment!')
        return new Engine.Vector2()
    }
  }

  /* ------------------------------------------------------------------------ */
}

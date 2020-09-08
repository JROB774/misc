Engine.Effect =
{
  /**
   * Set a shader uniform for the currently active effect shader (a shader that
   * is applied to the entire screen to allow for post-processing effects). A
   * uniform is simply a variable in a shader whose value can be modified from
   * within the code on the CPU and sent to the GPU for the processing.
   *
   * @param {String} name  - Name of the uniform variable in the shader.
   * @param {Object} value - Value to assign to the uniform variable.
   */
  setCurrentUniform: function (name, value)
  {
    Engine.Effect._.currentEffect.setUniform(name, value)
  },

  /**
   * Sets the effect shader (a shader that is applied to the entire screen to
   * allow for post-processing) that will be applied to the frame's contents.
   * If the value passed is null then the frame's contents will be drawn to the
   * screen without any effects being applied to it.
   *
   * @param {Object} shader - Shader to set as the current effect.
   */
  setCurrent: function (shader)
  {
    Engine.Effect._.currentEffect = shader
  },

  /* ------------------------------------------------------------------------ */

  _:
  {
    targetTexture: null, /** Intermediate buffer to draw frame's content to. */
    framebuffer:   null, /** Framebuffer used to draw to the target texture. */
    currentEffect: null, /** Current effect to be used when drawing content. */

    vertexVBO:     null, /** Stores the target texture vertex data.          */
    texCoordVBO:   null, /** Stores the target texture texture coord data.   */

    /**
     * Initializes the internals of the effect system so that it is ready for
     * use. This function is called on start-up by the engine, and during a
     * canvas resize, so it does not need to be called anywhere else in code.
     *
     * @access private
     */
    initialize: function ()
    {
      let webgl = Engine.Canvas._.webgl

      // A blank texture is needed for the framebuffer to draw our content to.
      let texture = webgl.createTexture()
      let target  = webgl.TEXTURE_2D
      let format  = webgl.RGBA
      let width   = Engine.Canvas.getWidth()
      let height  = Engine.Canvas.getHeight()
      let type    = webgl.UNSIGNED_BYTE

      webgl.bindTexture(target, texture)
      webgl.texImage2D(target, 0, format, width, height, 0, format, type, null)

      let wrap = webgl.CLAMP_TO_EDGE
      let min  = webgl.NEAREST
      let max  = webgl.NEAREST

      webgl.texParameteri(target, webgl.TEXTURE_WRAP_S,     wrap)
      webgl.texParameteri(target, webgl.TEXTURE_WRAP_T,     wrap)
      webgl.texParameteri(target, webgl.TEXTURE_MIN_FILTER,  min)
      webgl.texParameteri(target, webgl.TEXTURE_MAG_FILTER,  max)

      // The framebuffer can then be created and linked with the texture.
      let framebuffer = Engine.Canvas._.webgl.createFramebuffer()
      let attach = webgl.COLOR_ATTACHMENT0

      webgl.bindFramebuffer(webgl.FRAMEBUFFER, framebuffer)
      webgl.framebufferTexture2D(webgl.FRAMEBUFFER, attach, target, texture, 0)

      // Unlike most other drawing operations within the engine the vertices
      // used by the framebuffer are in normalized coordinates and not pixels.
      // This is because the framebuffer will take up the whole screen so it
      // is easier to avoid the use of a world space matrix, and the user will
      // not be positioning the framebuffer, so there is no need to supply the
      // ability to perform pixel-perfect positioning when drawing the target.

      let vertices  = new Float32Array([ -1,1,  -1,-1,  1,1,  1,-1 ])
      let texCoords = new Float32Array([  0,1,   0, 0,  1,1,  1, 0 ])

      let vertexVBO   = webgl.createBuffer()
      let texCoordVBO = webgl.createBuffer()

      webgl.bindBuffer(webgl.ARRAY_BUFFER, vertexVBO)
      webgl.bufferData(webgl.ARRAY_BUFFER, vertices, webgl.STATIC_DRAW)
      webgl.bindBuffer(webgl.ARRAY_BUFFER, texCoordVBO)
      webgl.bufferData(webgl.ARRAY_BUFFER, texCoords, webgl.STATIC_DRAW)

      // Clean resources by deleting any potential old framebuffer content.
      // As this function can be called multiple times (on canvas resize).
      webgl.deleteTexture(Engine.Effect._.targetTexture)
      webgl.deleteFramebuffer(Engine.Effect._.framebuffer)
      webgl.deleteBuffer(Engine.Effect._.vertexVBO)
      webgl.deleteBuffer(Engine.Effect._.texCoordVBO)

      Engine.Effect._.targetTexture = texture
      Engine.Effect._.framebuffer   = framebuffer
      Engine.Effect._.vertexVBO     = vertexVBO
      Engine.Effect._.texCoordVBO   = texCoordVBO
    },

    /**
     * Sets up the framebuffer so that all draw calls made during the game's
     * draw function will be drawn to the framebuffer instead of the canvas.
     * This function is automatically called at the start of each frame by the
     * engine in its game loop function -- it shouldn't be called elsewhere.
     *
     * @access private
     */
    begin: function ()
    {
      let framebuffer = Engine.Effect._.framebuffer
      let webgl = Engine.Canvas._.webgl

      webgl.bindFramebuffer(webgl.FRAMEBUFFER, framebuffer)
    },

    /**
     * Ends the current frame draw by unsetting the framebuffer and drawing its
     * final contents to the canvas, with any shader effects that have been set.
     * This function is automatically called at the end of each frame by the
     * engine in its game loop function -- it shouldn't be called elsewhere.
     *
     * @access private
     */
    end: function ()
    {
      let webgl = Engine.Canvas._.webgl

      // We don't want to draw the framebuffer to the framebuffer!
      webgl.bindFramebuffer(webgl.FRAMEBUFFER, null)

      // Use the basic (no effects) shader if an effect isn't set.
      let currentEffect = Engine.Effect._.currentEffect
      if (currentEffect !== null) { var effect = currentEffect }
      else { var effect = Engine.Assets.basic }

      Engine.Shader.setCurrent(effect)

      let texture     = Engine.Effect._.targetTexture
      let vertexVBO   = Engine.Effect._.vertexVBO
      let texCoordVBO = Engine.Effect._.texCoordVBO

      webgl.bindBuffer(webgl.ARRAY_BUFFER, vertexVBO)
      effect.enableAttribute('in_vert_pos',  2, true, 2, 0)
      webgl.bindBuffer(webgl.ARRAY_BUFFER, texCoordVBO)
      effect.enableAttribute('in_tex_coord', 2, true, 2, 0)

      effect.setUniform('framebuffer', 0)

      webgl.activeTexture(webgl.TEXTURE0)
      webgl.bindTexture(webgl.TEXTURE_2D, texture)

      webgl.drawArrays(webgl.TRIANGLE_STRIP, 0, 4)
    },
  },

  /* ------------------------------------------------------------------------ */
}

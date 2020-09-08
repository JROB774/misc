Engine.Canvas =
{
  /**
   * Toggles the standard system cursor on and off when over the game canvas.
   *
   * @param {Boolean} show - True to show the cursor, false to hide it.
   */
  showCursor: function (show)
  {
    let state = ((show) ? 'default' : 'none')

    Engine.Canvas._.gameCanvas.style['cursor'] = state
    Engine.Canvas._.textCanvas.style['cursor'] = state
  },

  /**
   * Fills the entire canvas with the color specified in the arguments. If no
   * arguments are provided the canvas is filled with a solid black color.
   *
   * @param {Number} [r=0] - Red component of the clear color.
   * @param {Number} [g=0] - Green component of the clear color.
   * @param {Number} [b=0] - Blue component of the clear color.
   * @param {Number} [a=1] - Alpha component of the clear color.
   */
  clearRGBA: function (r = 0, g = 0, b = 0, a = 1)
  {
    let clearColor = new Engine.Color(r, g, b, a)
    Engine.Canvas.clear(clearColor)
  },

  /**
   * Fills the entire canvas with the color specified in the arguments.
   *
   * @param {Number} color - Color to clear the canvas with.
   */
  clear: function (color)
  {
    Engine.Canvas._.webgl.clearColor(color.r, color.g, color.b, color.a)
    Engine.Canvas._.webgl.clear(Engine.Canvas._.webgl.COLOR_BUFFER_BIT)

    let width = Engine.Canvas._.textCanvas.width
    let height = Engine.Canvas._.textCanvas.height

    Engine.Canvas._.text.clearRect(0, 0, width, height)
  },

  /**
   * Sets the new size of the canvas element. This update will occur in real-
   * time and will also adjust the internal WebGL viewport to match the size.
   *
   * @param {Number} width  - New width of the canvas element (pixels).
   * @param {Number} height - New height of the canvas element (pixels).
   */
  setSize: function (width, height)
  {
    // We do this part to keep the canvas centered on the website page.
    Engine.Canvas._.container.style['width']  = `${width}px`
    Engine.Canvas._.container.style['height'] = `${height}px`

    Engine.Canvas._.gameCanvas.width = width
    Engine.Canvas._.gameCanvas.height = height
    Engine.Canvas._.textCanvas.width = width
    Engine.Canvas._.textCanvas.height = height

    Engine.Canvas._.webgl.viewport(0, 0, width, height)

    // Sets up the world space matrix for top-left drawing in pixel based units
    // as this makes the drawing interface more convenient for the engine user.
    let sx = 2 / width, sy = -2 / height
    let matrix = new Engine.Matrix3().translate(-1, 1).scale(sx, sy)
    Engine.Canvas._.worldSpace = matrix

    // Resizing the text canvas seems to reset the text drawing properties so
    // we need to reassign them to their cached values after a canvas resize.
    Engine.Text._.restore()

    // We also need to resize the framebuffer target to match the new canvas.
    Engine.Effect._.initialize(width, height)
  },

  /**
   * Sets the refresh rate for the canvas to be redrawn at in frames per second
   * and also sets the rate at which the game's logic will be updated at.
   *
   * @param {Number} fps - New refresh rate to use, in frames per second.
   */
  setRefreshRate: function (fps)
  {
    Engine.Canvas._.dt = 1.0 / fps
    Engine.Canvas._.fps = fps
  },

  /**
   * Sets the horizontal and vertical offset applied to every draw call, whether
   * it's a texture, shape, etc. By default the draw offset is set to (0,0).
   *
   * @param {Number} x - Horizontal offset to apply to future draw operations.
   * @param {Number} y - Vertical offset to apply to future draw operations.
   */
  setDrawOffset: function (x, y)
  {
    Engine.Canvas._.drawOffset.assign(x, y)
  },

  /**
   * Retrieves the current width of the game canvas.
   *
   * @return {Number} Width of the canvas.
   */
  getWidth: function ()
  {
    return Engine.Canvas._.gameCanvas.width
  },

  /**
   * Retrieves the current height of the game canvas.
   *
   * @return {Number} Height of the canvas.
   */
  getHeight: function ()
  {
    return Engine.Canvas._.gameCanvas.height
  },

  /* ------------------------------------------------------------------------ */

  _:
  {
    container:  null, /** HTML div that contains the engine canvases.  */

    gameCanvas: null, /** Canvas used for drawing actual game content. */
    webgl:      null, /** A WebGL drawing context used by gameCanvas.  */

    textCanvas: null, /** Canvas used for drawing text with fonts.     */
    text:       null, /** A 2D drawing context used by textCanvas.     */

    drawOffset: null, /** Current offset to apply to draw operations.  */

    fps:           0, /** Current refresh rate, in frames per second.  */
    dt:            0, /** Current delta time used for updating logic.  */

    // The world space matrix defines the transformation applied to all objects
    // that exist within the world. For the 2D rendering of the game we want to
    // handle top-left origin rendering and work in pixel coordinates. This is
    // because it is easier for the engine user to work within that world space.
    //
    // WebGL has center origin and works in normalized coordinates, so we use
    // the world space matrix to transform our desired coords into WebGL's.

    worldSpace: null, /** Defines transformation applied to all draws. */

    /**
     * Initializes the internals of the canvas system so that it is ready for
     * use. This function is called on start-up by the engine, so it does not
     * need to be called anywhere else in the code.
     *
     * @access private
     */
    initialize: function ()
    {
      // We need to actually create the canvas elements inside the div element
      // 'game-container'. We do this so that we can position two canvases on
      // top of each other -- the WebGL game canvas itself and the text canvas.

      var container = document.getElementById('game-container')
      container.style['position'] = 'relative' // Needed for absolute position.
      container.style['margin']   = '0px auto'
      container.style['width']    = '800px'
      container.style['height']   = '600px'

      Engine.Canvas._.createCanvas(container, 'game-canvas')
      Engine.Canvas._.createCanvas(container, 'text-canvas')

      // We pass in the alpha attribute to remove ugly white edges on textures.
      var gameCanvas =  document.getElementById('game-canvas')
      var webgl = gameCanvas.getContext('webgl', { alpha: false })
      if (!webgl) {
        // On Edge and IE the normal WebGL context may not be defined.
        webgl = gameCanvas.getContext('experimental-webgl', { alpha: false })
        if (!webgl) {
          alert('WebGL is not supported in your browser!')
          return
        }
      }

      // We do this because text rendering in WebGL is extremely complicated.
      var textCanvas = document.getElementById('text-canvas')
      var text = textCanvas.getContext('2d')
      if (!text) {
        alert('Failed to create text drawing canvas!')
        return
      }

      // We need to ensure the text canvas is on top of the game canvas. So
      // that the text drawn to it is actually visible as the top-most layer.
      gameCanvas.style['z-index'] = '0'
      textCanvas.style['z-index'] = '1'

      // This is necessary for texture alpha to blend correctly in WebGL.
      webgl.blendFunc(webgl.SRC_ALPHA, webgl.ONE_MINUS_SRC_ALPHA)
      webgl.enable(webgl.BLEND)

      Engine.Canvas._.container  = container
      Engine.Canvas._.gameCanvas = gameCanvas
      Engine.Canvas._.webgl      = webgl
      Engine.Canvas._.textCanvas = textCanvas
      Engine.Canvas._.text       = text
      Engine.Canvas._.drawOffset = new Engine.Vector2()
    },

    /**
     * Code for creating a canvas and adding it to the HTML5 page. This is
     * done to avoid code repetition for making the two main canvas elements.
     *
     * @param {Object} container - Game container to add the canvas to.
     * @param {String} name      - ID/Name of the created canvas element.
     *
     * @access private
     */
    createCanvas (container, name)
    {
      let canvas = document.createElement('canvas')
      canvas.setAttribute('id', name)

      canvas.style['position']      = 'absolute'
      canvas.style['top']           = '0px'
      canvas.style['left']          = '0px'
      canvas.style['display']       = 'block'
      canvas.style['border-radius'] = '16px'

      container.appendChild(canvas)
    },
  },

  /* ------------------------------------------------------------------------ */
}

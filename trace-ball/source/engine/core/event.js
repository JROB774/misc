Engine.Event =
{
  /**
   * Adds a new handler/listener to be called when the specific JavaScript
   * event is triggered within the browser. A list of the possible events can
   * be found at: <https://developer.mozilla.org/en-US/docs/Web/Events>
   *
   * @param {String}   event   - Name of the event to listen out for.
   * @param {Function} handler - Handler to be called when the event fires.
   */
  addEventHandler: function (event, handler)
  {
    // Different browsers will only have one of these functions, so we need
    // to check which is available so that we can call that specific version.
    if (window.addEventListener) { window.addEventListener(event, handler) }
    else if (window.attachEvent) { window.attachEvent(event, handler) }
  },

  /**
   * Retrieves the current position of the cursor, relative to the canvas. If
   * the mouse has not been moved at all then the value will be set to (0,0).
   *
   * @return {Object} Cursor position stored as an Engine.Vector2.
   */
  getCursorPosition: function ()
  {
    // We make a copy to avoid the internal value being edited.
    return new Engine.Vector2().copy(Engine.Event._.cursor)
  },

  /* ------------------------------------------------------------------------ */

  _:
  {
    cursor: null, /** Stores the current relative cursor position. */

    /**
     * Initializes the internals of the event system so that it is ready for
     * use. This function is called on start-up by the engine, so it does not
     * need to be called anywhere else in the code.
     *
     * @access private
     */
    initialize: function ()
    {
      // We add an event handler so we can store the current relative cursor.
      let getCursorPosition = Engine.Event._.getCursorPosition
      Engine.Event.addEventHandler('mousemove', getCursorPosition)

      Engine.Event._.cursor = new Engine.Vector2()
    },

    /**
     * This function should not be called, as it is an event handler/callback.
     *
     * It is the event handler that is called whenever the mouse is moved. This
     * is used to store the relative cursor position for the user to then get
     * using the Engine.Event.getCursorPosition() function call.
     *
     * @param {Object} event - Information about the mouse move event.
     *
     * @access private
     */
    getCursorPosition: function (event)
    {
      // Get the canvas position so we can make the cursor relative to the
      // canvas rather than being relative to the browser window instead.
      let clientRect = Engine.Canvas._.gameCanvas.getBoundingClientRect()

      Engine.Event._.cursor.x = event.clientX - clientRect.left
      Engine.Event._.cursor.y = event.clientY - clientRect.top
    },
  },

  /* ------------------------------------------------------------------------ */
}

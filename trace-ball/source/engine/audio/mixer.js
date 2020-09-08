Engine.Mixer =
{
  /* ------------------------------------------------------------------------ */

  _:
  {
    context: null, /** Audio context manages sounds and effects. */

    /**
     * Initializes the internals of the audio mixer system so that it is ready
     * for use. This function is called on start-up by the engine, so it does
     * not need to be called anywhere else in the code.
     *
     * @access private
     */
    initialize: function ()
    {
      // If standard audio context is not available then check for webkit.
      // This is needed for the audio context to work with older browsers.
      const AudioContext = window.AudioContext || window.webkitAudioContext

      Engine.Mixer._.context = new AudioContext()
      if (Engine.Mixer._.context.state === 'suspended') {
        Engine.Mixer._.context.resume()
      }
    },
  },

  /* ------------------------------------------------------------------------ */
}

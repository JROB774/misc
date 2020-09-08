// We use strict mode to enforce stronger syntactic rules. Helping to ensure
// that code is written correctly and more secure than standard JavaScript.
//
// For info on the benefits see: <https://www.w3schools.com/js/js_strict.asp>
'use strict'

var Engine = {}

// Because all of the engine's source code files are merged into a single file
// (with this file's code being at the top) we only need to define the engine
// namespace and declare 'use strict' in this file and they will apply to all.

/** The entry-point for the engine program. */
Engine.main = function ()
{
  Engine.Canvas._.initialize()
  Engine.Effect._.initialize()
  Engine.Event ._.initialize()
  Engine.Shape ._.initialize()
  Engine.Text  ._.initialize()
  Engine.Mixer ._.initialize()

  // These are important as otherwise they would be undefined.
  Engine.Canvas.setSize(640, 480)
  Engine.Canvas.setRefreshRate(60)

  // Displays a black screen whilst the game assets are loaded.
  Engine.Canvas._.text.fillStyle = 'rgb(0, 0, 0, 1)'
  Engine.Canvas._.text.fillRect(0, 0, 640, 480)

  var engineAssetPackage = 'assets/assets.engine.json'
  var gameAssetPackage = 'assets/assets.game.json'

  Promise.all([
    Engine.AssetLoader.loadAssetPackage('engine', engineAssetPackage),
    Engine.AssetLoader.loadAssetPackage('game', gameAssetPackage)
  ])
  .then(Engine.setupGame)
}

/** The setup after loading game and engine assets. */
Engine.setupGame = function ()
{
  Engine.game = new Game()

  Engine.Assets = Engine.AssetLoader.getPackage('engine')
  Engine.game.assets = Engine.AssetLoader.getPackage('game')

  Engine.game.initialize()
  Engine.runGame()
}

/** The self-executing game loop code. */
Engine.runGame = function ()
{
  Engine.Effect._.begin()

  Engine.game.update(Engine.Canvas._.dt)
  Engine.game.draw(Engine.Canvas._.dt)

  Engine.Effect._.end()

  let delay = 1000 / Engine.Canvas._.fps
  window.setTimeout(Engine.runGame, delay)
}

// Set the entry-point function to be called once the window is fully loaded.
// This ensures that everything is properly loaded before we attempt to start
// executing the engine's code -- which needs access to certain DOM elements.
//
// We need to handle this if-check because certain browsers will only support
// one of these functions, so both cases have to be handled for compatibility.

if (window.addEventListener) { window.addEventListener('load', Engine.main) }
else if (window.attachEvent) { window.attachEvent('onload', Engine.main) }

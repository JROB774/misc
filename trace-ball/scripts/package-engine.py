
PACKAGE_DIRECTORY = 'source/engine/'
PACKAGE_NAME = 'source/engine.js'

# It is important that the files are in the correct order.
PACKAGE_FILES = [
  'engine.js',
  'utility/utility.js',
  'utility/vector2.js',
  'utility/matrix3.js',
  'core/event.js',
  'core/asset-loader.js',
  'core/collision.js',
  'draw/canvas.js',
  'draw/effect.js',
  'draw/color.js',
  'draw/shader.js',
  'draw/shape.js',
  'draw/texture.js',
  'draw/text.js',
  'draw/animation.js',
  'audio/mixer.js',
  'audio/sound.js',
]

fileContent = ''
for fileName in PACKAGE_FILES:
  with open(PACKAGE_DIRECTORY + fileName, 'r') as file:
    fileContent += file.read() + '\n'
# Remove the last redundant new line from the resulting file.
fileContent = fileContent.rsplit('\n', 1)[0]

with open(PACKAGE_NAME, 'w') as output:
  output.write(fileContent)

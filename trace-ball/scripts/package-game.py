PACKAGE_DIRECTORY = 'source/game/'
PACKAGE_NAME = 'source/game.js'

# It is important that the files are in the correct order.
PACKAGE_FILES = [
  'game.js',
  'camera.js',
  'player.js',
  'enemy.js',
]

fileContent = ''
for fileName in PACKAGE_FILES:
  with open(PACKAGE_DIRECTORY + fileName, 'r') as file:
    fileContent += file.read() + '\n'
# Remove the last redundant new line from the resulting file.
fileContent = fileContent.rsplit('\n', 1)[0]

with open(PACKAGE_NAME, 'w') as output:
  output.write(fileContent)

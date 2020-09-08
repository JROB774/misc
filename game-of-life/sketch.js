/*******************************************************************************
 * @file An implementation of Conway's Game of Life within JavaScript.
 * @license {@link https://unlicense.org/ The Unlicense (Public Domain)}
 * @author Joshua Robertson
 *
 ******************************************************************************/

const GUI_HEIGHT = 32;
const BOARD_CELL_WIDTH = 64, BOARD_CELL_HEIGHT = 64;
const CELL_SIZE = 8;
const BOARD_WIDTH = BOARD_CELL_WIDTH * CELL_SIZE, BOARD_HEIGHT = BOARD_CELL_HEIGHT * CELL_SIZE;
const UNCHANGE = -1
const DEAD = 0;
const LIVE = 1;

var canvas;
var run = false;
var posxprev = -1, posyprev = -1;
var posx = -1, posy = -1;
var tochange;
var board;

function centerCanvas ()
{
  // places the canvas at the center of the browser window
  var x = (windowWidth - width) / 2;
  var y = (windowHeight - height) / 2;

  canvas.position(x, y);
}

function setup ()
{
  // creates the canvas at the right board size in the center of the window
  canvas = createCanvas((BOARD_CELL_WIDTH * CELL_SIZE) + 1, (BOARD_CELL_HEIGHT * CELL_SIZE) + 1);
  centerCanvas();

  // zero initialises the 2d board array (filled with dead cells at the start)
  board = Array.apply(null, Array(BOARD_CELL_HEIGHT)).map(Number.prototype.valueOf, DEAD);
  for (var i = 0; i < BOARD_CELL_HEIGHT; ++i) { board[i] = Array.apply(null, Array(BOARD_CELL_WIDTH)).map(Number.prototype.valueOf, DEAD); }
  // zero initialises the 2d change array that lists what cells must change each tick
  tochange = Array.apply(null, Array(BOARD_CELL_HEIGHT)).map(Number.prototype.valueOf, UNCHANGE);
  for (var i = 0; i < BOARD_CELL_HEIGHT; ++i) { tochange[i] = Array.apply(null, Array(BOARD_CELL_WIDTH)).map(Number.prototype.valueOf, UNCHANGE); }

  // details about how stroke drawing should occur
  stroke(0x3F);
  strokeWeight(1);
}

function windowResized ()
{
  // center canvas on window size change
  centerCanvas();
}


function mousePressed ()
{
  // stops inability to alter a cell's state on a new click
  posprevx = -1, posprevy = -1;
  posx = -1, posy = -1;
}

function keyPressed ()
{
  // updates the board by one tick
  if (key === ' ') { updateBoard(); }
  // generates a completely random board
  else if (key === 'r' || key === 'R') {
    for (var iy = 0; iy < BOARD_CELL_HEIGHT; ++iy) {
      for (var ix = 0; ix < BOARD_CELL_WIDTH; ++ix) {
        board[iy][ix] = floor(random(LIVE+1));
        tochange[iy][ix] = UNCHANGE;
      }
    }
  }

  // toggles auto-update mode
  if (keyCode == RETURN) { run = !run; }
  // clears the board with dead cells
  else if (keyCode == BACKSPACE) {
    for (var iy = 0; iy < BOARD_CELL_HEIGHT; ++iy) {
      for (var ix = 0; ix < BOARD_CELL_WIDTH; ++ix) {
        board[iy][ix] = DEAD;
        tochange[iy][ix] = UNCHANGE;
      }
    }
  }
}

function updateBoard ()
{
  // checks which cells need to die this turn based on GOL rules
  for (var iy = 0; iy < BOARD_CELL_HEIGHT; ++iy) {
    for (var ix = 0; ix < BOARD_CELL_WIDTH; ++ix) {
      // check for the number of surrounding neighbours with bounds check
      var neighbours = 0;
      for (var iyy = max(0, iy-1); iyy <= min(iy+1, BOARD_CELL_HEIGHT-1); ++iyy) {
        for (var ixx = max(0, ix-1); ixx <= min(ix+1, BOARD_CELL_WIDTH-1); ++ixx) {
          if (iyy != iy || ixx != ix) {
            if (board[iyy][ixx] == LIVE) { ++neighbours; }
          }
        }
      }
      // handle the current cell based on number of neighbours
      if (board[iy][ix] == LIVE && (neighbours < 2 || neighbours > 3)) { tochange[iy][ix] = DEAD; }
      else if (board[iy][ix] == DEAD && neighbours == 3) { tochange[iy][ix] = LIVE; }
      else { tochange[iy][ix] = UNCHANGE; }
    }
  }
  // kill/birth all cells simultaneously to prevent calculation issues in previous step
  for (var iy = 0; iy < BOARD_CELL_HEIGHT; ++iy) {
    for (var ix = 0; ix < BOARD_CELL_WIDTH; ++ix) {
      if (tochange[iy][ix] != UNCHANGE) { board[iy][ix] = tochange[iy][ix]; }
    }
  }
}

function draw ()
{
  // clears the canvas with a background color
  background(0x3F);

  // alters the color cells are drawn in based on if its in auto-update
  if (run) { fill(0xFF, 0xFF, 0x40); }
  else { fill(0xFF); }

  // allows for click-drag drawing (much more smooth/intuitive)
  if (mouseIsPressed) {
    // pevents out of bounds error on the board array
    if (mouseX >= 0 && mouseY >= 0 && mouseX < BOARD_WIDTH && mouseY < BOARD_HEIGHT) {
      // prevents cell flickering when drawing on board
      prevposx = posx, prevposy = posy;
      posx = floor(mouseX / CELL_SIZE), posy = floor(mouseY / CELL_SIZE);
      if (posx != prevposx || posy != prevposy) {
        // changes the state of the cell based on the button pressed
        if (mouseButton == LEFT) { board[posy][posx] = LIVE; }
        else if (mouseButton == RIGHT) { board[posy][posx] = DEAD; }
      }
    }
  }

  // if run is set then the board will update automatically
  if (run && ((frameCount % 10) == 0)) { updateBoard(); }

  // iterates through the entire board array and draws all live cells
  for (var iy = 0; iy < BOARD_CELL_HEIGHT; ++iy) {
    for (var ix = 0; ix < BOARD_CELL_WIDTH; ++ix) {
      if (board[iy][ix] == LIVE) {
        rect(ix * CELL_SIZE, iy * CELL_SIZE, CELL_SIZE, CELL_SIZE);
      }
    }
  }
}

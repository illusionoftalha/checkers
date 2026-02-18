# Checkers
# Checkers Turn Based Game - Function Documentation

## Data Structures

### `Piece` Structure

```c
typedef struct {
    int color; // 0 none, 1 red, 2 white
    int king;  // 0 not king, 1 king
} Piece;
```

Represents a checkers piece on the board.

* **color**: Integer representing piece color (0=empty, 1=red, 2=white)
* **king**: Integer flag indicating if piece is promoted to king (0=regular, 1=king)

### `GameState` Enumeration

```c
typedef enum { HOME, PLAYING, PAUSED } GameState;
```

Defines the current state of the game application.

## Global Variables

* `Piece board[BOARD_SIZE][BOARD_SIZE]`: 8x8 game board array
* `int selX, selY`: Currently selected piece coordinates (-1 if none selected)
* `int turn`: Current player turn (1=red, 2=white)
* `GameState state`: Current game state
* `SDL_Rect playButton, restartButton`: UI button rectangles

## Core Game Functions

### `void initBoard()`

Initializes the checkers board to starting position.

* Places white pieces on rows 0-2 on dark squares
* Places red pieces on rows 5-7 on dark squares
* Resets selection and sets turn to red (1)
* All pieces start as non-kings

### `int legalMove(int sx, int sy, int dx, int dy)`

Validates if a move from source to destination is legal.

**Parameters:**

* `sx, sy`: Source coordinates (x, y)
* `dx, dy`: Destination coordinates (x, y)

**Returns:**

* `1` if move is legal, `0` if illegal

**Move Rules Checked:**

* Piece belongs to current player
* Destination is empty
* Regular pieces move diagonally forward one square
* Kings move diagonally in any direction one square
* Jump moves (2 squares) over opponent pieces

### `void doMove(int sx, int sy, int dx, int dy)`

Executes a validated move on the board.

**Parameters:**

* `sx, sy`: Source coordinates
* `dx, dy`: Destination coordinates

**Actions Performed:**

* Removes jumped opponent pieces
* Moves piece to destination
* Promotes pieces to kings when reaching opposite end
* Switches turn to other player

## Utility Functions

### `int insideRect(int x, int y, SDL_Rect r)`

Checks if a point is inside a rectangle.

**Parameters:**

* `x, y`: Point coordinates
* `r`: Rectangle to test against

**Returns:**

* `1` if point is inside rectangle, `0` otherwise

## Rendering Functions

### `void drawHome(SDL_Renderer *ren, int winW, int winH)`

Renders the home screen with play button.

**Parameters:**

* `ren`: SDL renderer
* `winW, winH`: Window dimensions

**Functionality:**

* Draws dark gray background
* Creates and renders green play button (200x100 pixels)
* Centers button in window

### `void drawBoard(SDL_Renderer *ren, int winW, int winH)`

Renders the game board and pieces.

**Parameters:**

* `ren`: SDL renderer
* `winW, winH`: Window dimensions

**Functionality:**

* Calculates board size based on window dimensions
* Draws alternating colored squares (light/dark brown)
* Highlights selected piece in green
* Renders pieces as colored rectangles (red/white)
* Draws blue squares on king pieces
* Renders restart button in top-right corner

### `void drawPaused(SDL_Renderer *ren, int winW, int winH)`

Renders pause overlay screen.

**Parameters:**

* `ren`: SDL renderer
* `winW, winH`: Window dimensions

**Functionality:**

* Draws semi-transparent black overlay covering entire window

## Main Function

### `int main(int argc, char **argv)`

Entry point for the checkers game application.

**Parameters:**

* `argc`: Argument count
* `argv`: Argument values

**Returns:**

* `0` on successful execution, `1` on SDL initialization error

**Functionality:**

* Initializes SDL video subsystem
* Creates resizable, maximized window (640x640 base size)
* Creates hardware-accelerated renderer
* Main game loop handling:
    * Event processing (mouse clicks, key presses, window close)
    * State-based input handling (HOME, PLAYING, PAUSED)
    * Board interaction (piece selection and movement)
    * Pause/unpause with 'P' key
    * State-appropriate rendering
* Cleanup of SDL resources

**Event Handling:**

* **Mouse clicks**: Button interaction, piece selection/movement
* **Keyboard**: Pause toggle with 'P' key
* **Window events**: Quit handling

**Game Flow:**

1. Start in HOME state
2. Click play button to initialize board and enter PLAYING state
3. Click pieces to select, click destination to move
4. Press 'P' to pause/unpause
5. Click restart button to reset game

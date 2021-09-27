#include <iostream>
#include "sdlandnet.hpp"

// CONSTANTS
//{
// The title for the program's window.
constexpr const char* TITLE = "Dominion";

// The size of the program's window.
constexpr int SIZE = 400;

// The number of cells per row and column.
constexpr int CELLS = 10;

// The width of a cell's border.
constexpr int LINE_WIDTH = 1;

// The size of a cell.
constexpr int CELL_SIZE = SIZE / CELLS;

// The size of a cell's interior.
constexpr int HOLE_SIZE = CELL_SIZE - 2 * LINE_WIDTH;

// The colour of the grid background.
constexpr Sprite::Colour BACKGROUND_COLOUR = Sprite::BLACK;

// The colour of the grid lines.
constexpr Sprite::Colour LINE_COLOUR = Sprite::WHITE;

// The key used to quit the game.
constexpr int QUIT_KEY = Events::ESCAPE;

// The key used to reset the game.
constexpr int RESET_KEY = Events::LETTERS['r' - 'a'];

// The key used to display the game info/
constexpr int INFO_KEY = Events::ENTER;

// The number of players.
constexpr int PLAYERS = 2;

// The colours for the players.
constexpr Sprite::Colour PLAYER_COLOURS[PLAYERS] = {
    Sprite::RED,
    Sprite::BLUE
};

// The number to represent an empty grid cell.
constexpr int EMPTY = -1;
//}

/* A board game by Chigozie Agomo.

   The aim of the game is to completely fill the grid's cells with your colour.
   One can do so by deploying troops into a cell, expanding one's forces in a cell
   to neighbouring cells, or uniting one's troops in a cell to other cells.
   
   Deployment can only be performed in empty cells.
   Expansion cannot be performed for two turns in a row.
   Unison can only occur in a straight line that is free from any opposition.
   Unison is attempted in all 8 directions simulataneously and terminates.
    at the closest friendly cell.
 */
int main(int argc, char** argv) {
    // The game name and version of sdlandnet are displayed.
    std::cout << '\n' << "Dominion by Chigozie Agomo." << "\n\n" << System::info() << '\n';
    
    // The required sub-systems are initialised.
    System::initialise();
    
    // Scope used to destroy objects before system shutdown.
    {
        // A square display is created with a defined title and size.
        Display display(TITLE, SIZE, SIZE);
        
        // The grid line colour fills the display.
        display.fill(LINE_COLOUR);
        
        // The rectangle used to draw the grid's background.
        Rectangle hole(0, 0, HOLE_SIZE, HOLE_SIZE);
        
        // The grid background is drawn.
        for (int i = 0; i < CELLS; ++i) {
            // The x coordinate of the rectangle is set.
            hole.set_x(i * CELL_SIZE + LINE_WIDTH);
            
            for (int j = 0; j < CELLS; ++j) {
                // The y cvoordinate of the rectangle is set.
                hole.set_y(j * CELL_SIZE + LINE_WIDTH);
                
                // The cell's interior is drawn.
                display.fill(hole, BACKGROUND_COLOUR);
            }
        }
        
        // Once complete, the grid is displayed.
        display.update();
        
        // The grid's array representation.
        std::array<std::array<int, CELLS>, CELLS> grid;
        
        // The grid is emptied.
        for (int i = 0; i < CELLS; ++i) {
            for (int j = 0; j < CELLS; ++ j) {
                grid[i][j] = EMPTY;
            }
        }
        
        // An uninitialised event is created for event handling.
        Event event;
        
        // The current player's turn.
        int turn = 0;
        
        // True if the corresponding player expanded last turn.
        std::array<bool, PLAYERS> expanded;
        
        // Loop to handle user input.
        while (true) {
            // An event is waited for.
            event.wait();
            
            // The player chose to end the program (by clicking the x or pressing escape).
            if (
                event.type() == Event::TERMINATE
                || event.type() == Event::KEY_PRESS && event.key() == QUIT_KEY
            ) {
                break;
            }
            
            // The player chose to restart the game.
            else if (event.type() == Event::KEY_PRESS && event.key() == RESET_KEY) {
                // The grid background is drawn.
                for (int i = 0; i < CELLS; ++i) {
                    // The x coordinate of the rectangle is set.
                    hole.set_x(i * CELL_SIZE + LINE_WIDTH);
                    
                    for (int j = 0; j < CELLS; ++j) {
                        // The y cvoordinate of the rectangle is set.
                        hole.set_y(j * CELL_SIZE + LINE_WIDTH);
                        
                        // The cell's interior is drawn.
                        display.fill(hole, BACKGROUND_COLOUR);
                    }
                }
                
                // The cleared board is displayed.
                display.update();
                
                // The first player takes their turn.
                turn = 0;
        
                // The grid is emptied.
                for (int i = 0; i < CELLS; ++i) {
                    for (int j = 0; j < CELLS; ++ j) {
                        grid[i][j] = EMPTY;
                    }
                }
            }
            
            // The player chose to view the game details.
            else if (event.type() == Event::KEY_PRESS && event.key() == INFO_KEY) {
                // New line for clarity.
                std::cout << '\n';
                
                // An array to store the players' scores.
                std::array<int, PLAYERS> scores = {};
                
                // The grid is sent to standard output and the scores are calculated.
                for (int i = 0; i < CELLS; ++i) {
                    for (int j = 0; j < CELLS; ++j) {
                        if (grid[j][i] == EMPTY) {
                            std::cout << "_ ";
                        }
                        
                        else {
                            std::cout << grid[j][i] + 1 << ' ';
                            ++scores[grid[j][i]];
                        }
                    }
                    
                    std::cout << '\n';
                }
                
                // New line for clarity.
                std::cout << '\n';
                
                // The scores for each player are displayed.
                for (int i = 0; i < PLAYERS; ++i) {
                    std::cout << "Player " << i + 1 << ": " << scores[i] << '\n';
                }
                
                // The current player's turn is displayed.
                std::cout << "\nIt is player " << turn + 1 << "'s turn.\n\n";
            }
            
            // The player chose to deploy troops.
            else if (event.type() == Event::LEFT_UNCLICK) {
                // The position of the click is resolved.
                Point position = event.click_position();
                
                // The cell chosen is calculated.
                int x = position.get_x() * CELLS / SIZE;
                int y = position.get_y() * CELLS / SIZE;
                
                // Troops can only be deployed in an empty cell.
                if (grid[x][y] == EMPTY) {
                    // The grid's status is updated.
                    grid[x][y] = turn;
                    
                    // The hole's position is updated.
                    hole.set_x(x * CELL_SIZE + LINE_WIDTH);
                    hole.set_y(y * CELL_SIZE + LINE_WIDTH);
                    
                    // The hole is filled with the player's colour.
                    display.fill(hole, PLAYER_COLOURS[turn]);
                    
                    // The display is updated.
                    display.update();
                    
                    // The player can expand next turn.
                    expanded[turn] = false;
                    
                    // The next player takes their turn.
                    turn = (turn + 1) % PLAYERS;
                }
            }
            
            // The player chose to expand their forces.
            else if (event.type() == Event::RIGHT_UNCLICK && !expanded[turn]) {
                // The position of the click is resolved.
                Point position = event.click_position();
                
                // The cell chosen is calculated.
                int x = position.get_x() * CELLS / SIZE;
                int y = position.get_y() * CELLS / SIZE;
                
                // Troops can only expand if they have already been stationed.
                if (grid[x][y] == turn) {
                    // Expansion to the left is not possible on the left edge of the grid.
                    if (x > 0) {
                        // The cell is expanded into.
                        grid[x - 1][y] = turn;
                    
                        // The hole's position is updated.
                        hole.set_x((x - 1) * CELL_SIZE + LINE_WIDTH);
                        hole.set_y(y * CELL_SIZE + LINE_WIDTH);
                        
                        // The hole is filled with the player's colour.
                        display.fill(hole, PLAYER_COLOURS[turn]);
                    }
                    
                    // Expansion to the right is not possible on the right edge of the grid.
                    if (x < CELLS - 1) {
                        // The cell is expanded into.
                        grid[x + 1][y] = turn;
                    
                        // The hole's position is updated.
                        hole.set_x((x + 1) * CELL_SIZE + LINE_WIDTH);
                        hole.set_y(y * CELL_SIZE + LINE_WIDTH);
                        
                        // The hole is filled with the player's colour.
                        display.fill(hole, PLAYER_COLOURS[turn]);
                    }
                    
                    // Expansion upwards is not possible on the top edge of the grid.
                    if (y > 0) {
                        // The cell is expanded into.
                        grid[x][y - 1] = turn;
                    
                        // The hole's position is updated.
                        hole.set_x(x * CELL_SIZE + LINE_WIDTH);
                        hole.set_y((y - 1) * CELL_SIZE + LINE_WIDTH);
                        
                        // The hole is filled with the player's colour.
                        display.fill(hole, PLAYER_COLOURS[turn]);
                    }
                    
                    // Expansion downwards is not possible on the bottom edge of the grid.
                    if (y < CELLS - 1) {
                        // The cell is expanded into.
                        grid[x][y + 1] = turn;
                    
                        // The hole's position is updated.
                        hole.set_x(x * CELL_SIZE + LINE_WIDTH);
                        hole.set_y((y + 1) * CELL_SIZE + LINE_WIDTH);
                        
                        // The hole is filled with the player's colour.
                        display.fill(hole, PLAYER_COLOURS[turn]);
                    }
                    
                    // The display is updated.
                    display.update();
                    
                    // The player can't expand next turn.
                    expanded[turn] = true;
                    
                    // The next player takes their turn.
                    turn = (turn + 1) % PLAYERS;
                }
            }
            
            // The player chose to unite their forces.
            else if (event.type() == Event::MIDDLE_UNCLICK) {
                // The position of the click is resolved.
                Point position = event.click_position();
                
                // The cell chosen is calculated.
                int x = position.get_x() * CELLS / SIZE;
                int y = position.get_y() * CELLS / SIZE;
                
                // Troops can only unite if they have already been stationed.
                if (grid[x][y] == turn) {
                    // True if a friendly troop was found.
                    bool friendly;
                    
                    // The distance from the clicked troop.
                    int i;
                    
                    // Friendly troops to the left are searched for.
                    friendly = false;
                    
                    for (i = 1; x - i >= 0; ++i) {
                        // A friendly cell was found.
                        if (grid[x - i][y] == turn) {
                            friendly = true;
                            break;
                        }
                        
                        // An unfriendly cell was found.
                        else if (grid[x - i][y] != EMPTY) {
                            break;
                        }
                    }
                    
                    // A friendly troop was found.
                    if (friendly) {
                        // The cells between the two cells are taken.
                        for (--i; i > 0; --i) {
                            // The empty cell is filled.
                            grid[x - i][y] = turn;
                            
                            // The hole's position is updated.
                            hole.set_x((x - i) * CELL_SIZE + LINE_WIDTH);
                            hole.set_y(y * CELL_SIZE + LINE_WIDTH);
                            
                            // The hole is filled with the player's colour.
                            display.fill(hole, PLAYER_COLOURS[turn]);
                        }
                    }
                    
                    // Friendly troops to the right are searched for.
                    friendly = false;
                    
                    for (i = 1; x + i < CELLS; ++i) {
                        // A friendly cell was found.
                        if (grid[x + i][y] == turn) {
                            friendly = true;
                            break;
                        }
                        
                        // An unfriendly cell was found.
                        else if (grid[x + i][y] != EMPTY) {
                            break;
                        }
                    }
                    
                    // A friendly troop was found.
                    if (friendly) {
                        // The cells between the two cells are taken.
                        for (--i; i > 0; --i) {
                            // The empty cell is filled.
                            grid[x + i][y] = turn;
                            
                            // The hole's position is updated.
                            hole.set_x((x + i) * CELL_SIZE + LINE_WIDTH);
                            hole.set_y(y * CELL_SIZE + LINE_WIDTH);
                            
                            // The hole is filled with the player's colour.
                            display.fill(hole, PLAYER_COLOURS[turn]);
                        }
                    }
                    
                    // Friendly troops above are searched for.
                    friendly = false;
                    
                    for (i = 1; y - i >= 0; ++i) {
                        // A friendly cell was found.
                        if (grid[x][y - i] == turn) {
                            friendly = true;
                            break;
                        }
                        
                        // An unfriendly cell was found.
                        else if (grid[x][y - i] != EMPTY) {
                            break;
                        }
                    }
                    
                    // A friendly troop was found.
                    if (friendly) {
                        // The cells between the two cells are taken.
                        for (--i; i > 0; --i) {
                            // The empty cell is filled.
                            grid[x][y - i] = turn;
                            
                            // The hole's position is updated.
                            hole.set_x(x * CELL_SIZE + LINE_WIDTH);
                            hole.set_y((y - i) * CELL_SIZE + LINE_WIDTH);
                            
                            // The hole is filled with the player's colour.
                            display.fill(hole, PLAYER_COLOURS[turn]);
                        }
                    }
                    
                    // Friendly troops below are searched for.
                    friendly = false;
                    
                    for (i = 1; y + i < CELLS; ++i) {
                        // A friendly cell was found.
                        if (grid[x][y + i] == turn) {
                            friendly = true;
                            break;
                        }
                        
                        // An unfriendly cell was found.
                        else if (grid[x][y + i] != EMPTY) {
                            break;
                        }
                    }
                    
                    // A friendly troop was found.
                    if (friendly) {
                        // The cells between the two cells are taken.
                        for (--i; i > 0; --i) {
                            // The empty cell is filled.
                            grid[x][y + i] = turn;
                            
                            // The hole's position is updated.
                            hole.set_x(x * CELL_SIZE + LINE_WIDTH);
                            hole.set_y((y + i) * CELL_SIZE + LINE_WIDTH);
                            
                            // The hole is filled with the player's colour.
                            display.fill(hole, PLAYER_COLOURS[turn]);
                        }
                    }
                    
                    // Friendly troops up-left are searched for.
                    friendly = false;
                    
                    for (i = 1; x - i >= 0 && y - i >= 0; ++i) {
                        // A friendly cell was found.
                        if (grid[x - i][y - i] == turn) {
                            friendly = true;
                            break;
                        }
                        
                        // An unfriendly cell was found.
                        else if (grid[x - i][y - i] != EMPTY) {
                            break;
                        }
                    }
                    
                    // A friendly troop was found.
                    if (friendly) {
                        // The cells between the two cells are taken.
                        for (--i; i > 0; --i) {
                            // The empty cell is filled.
                            grid[x - i][y - i] = turn;
                            
                            // The hole's position is updated.
                            hole.set_x((x - i) * CELL_SIZE + LINE_WIDTH);
                            hole.set_y((y - i) * CELL_SIZE + LINE_WIDTH);
                            
                            // The hole is filled with the player's colour.
                            display.fill(hole, PLAYER_COLOURS[turn]);
                        }
                    }
                    
                    // Friendly troops down-right are searched for.
                    friendly = false;
                    
                    for (i = 1; x + i < CELLS && y + i < CELLS; ++i) {
                        // A friendly cell was found.
                        if (grid[x + i][y + i] == turn) {
                            friendly = true;
                            break;
                        }
                        
                        // An unfriendly cell was found.
                        else if (grid[x + i][y + i] != EMPTY) {
                            break;
                        }
                    }
                    
                    // A friendly troop was found.
                    if (friendly) {
                        // The cells between the two cells are taken.
                        for (--i; i > 0; --i) {
                            // The empty cell is filled.
                            grid[x + i][y + i] = turn;
                            
                            // The hole's position is updated.
                            hole.set_x((x + i) * CELL_SIZE + LINE_WIDTH);
                            hole.set_y((y + i) * CELL_SIZE + LINE_WIDTH);
                            
                            // The hole is filled with the player's colour.
                            display.fill(hole, PLAYER_COLOURS[turn]);
                        }
                    }
                    
                    // Friendly troops up-right are searched for.
                    friendly = false;
                    
                    for (i = 1; x + i < CELLS && y - i >= 0; ++i) {
                        // A friendly cell was found.
                        if (grid[x + i][y - i] == turn) {
                            friendly = true;
                            break;
                        }
                        
                        // An unfriendly cell was found.
                        else if (grid[x + i][y - i] != EMPTY) {
                            break;
                        }
                    }
                    
                    // A friendly troop was found.
                    if (friendly) {
                        // The cells between the two cells are taken.
                        for (--i; i > 0; --i) {
                            // The empty cell is filled.
                            grid[x + i][y - i] = turn;
                            
                            // The hole's position is updated.
                            hole.set_x((x + i) * CELL_SIZE + LINE_WIDTH);
                            hole.set_y((y - i) * CELL_SIZE + LINE_WIDTH);
                            
                            // The hole is filled with the player's colour.
                            display.fill(hole, PLAYER_COLOURS[turn]);
                        }
                    }
                    
                    // Friendly troops down-left are searched for.
                    friendly = false;
                    
                    for (i = 1; x - i >= 0 && y - i < CELLS; ++i) {
                        // A friendly cell was found.
                        if (grid[x - i][y + i] == turn) {
                            friendly = true;
                            break;
                        }
                        
                        // An unfriendly cell was found.
                        else if (grid[x - i][y + i] != EMPTY) {
                            break;
                        }
                    }
                    
                    // A friendly troop was found.
                    if (friendly) {
                        // The cells between the two cells are taken.
                        for (--i; i > 0; --i) {
                            // The empty cell is filled.
                            grid[x - i][y + i] = turn;
                            
                            // The hole's position is updated.
                            hole.set_x((x - i) * CELL_SIZE + LINE_WIDTH);
                            hole.set_y((y + i) * CELL_SIZE + LINE_WIDTH);
                            
                            // The hole is filled with the player's colour.
                            display.fill(hole, PLAYER_COLOURS[turn]);
                        }
                    }
                    
                    // The display is updated.
                    display.update();
                    
                    // The player can expand next turn.
                    expanded[turn] = false;
                    
                    // The next player takes their turn.
                    turn = (turn + 1) % PLAYERS;
                }
            }
        }
    }
    
    // The sub-systems used are terminated before the program terminates.
    System::terminate();
    
    // Program end.
    return 0;
}
//SnakeGame
//Jesus Jimenez
//06/9/2021
//CS 1337.0U2
//Purpose: Improving code (readability, extensibility, maintainability, testability, debugging, modularity, valuable commentary, self-documentation)
//Changelog: Changed the name of the variables to a more meaningful name, Checked for bugs and fixed the, used modularity, added comments
//truly randomized the fruit location in the game, added game speed functions, added a function to play again after a game ends,
//added functions to let the user know the reason why the game ended, added a pause function, added a new game mode, made the routine case-insensitive,
//Modified the loop that writes the width edge of the game's board to correct an error.
//Notes: Get rid of global variables.
//The snake eats it self when changing movement to the opposite direction when the tail length is greater than 1 but not when it is 1.
//Comments: Add the capability of having from 2 to 10 fruits

//Edited: 6/9/2021

// SnakeGame - Orig from the Web
// Original code by N. Vitanovic
// see his YouTube video here: https://bit.ly/29WZ5Ml

//This program recreates the Snake Game

//Program headers
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
using namespace std;

//Declaration of global variables
bool g_game_over;
char g_play_again_answer;
const int g_kGameBoardWidth = 20, g_kGameBoardHeight = 20, g_kMaxFruits = 10, g_kMinimumFruits = 2;
int g_head_position_x_axis, g_head_position_y_axis, g_fruit_position_x_axis, g_fruit_position_y_axis, g_score, g_tail_length, g_number_of_fruits, g_deadly_wall_on_off, g_deadly_wall_option = 2, g_game_speed = 100;
int g_tail_position_x_axis[400], g_tail_position_y_axis[400];
enum kChangeSnakeDirecton {kSNAKE_STOP = 0, kSNAKE_LEFT, kSNAKE_RIGHT, kSNAKE_UP, kSNAKE_DOWN};
kChangeSnakeDirecton ksnake_direction;

//This function displays the game instructions to the user
void GameInstructions()
{
    cout << "SNAKE GAME" << endl << endl;
    cout << "Instructions:" << endl << endl;
    cout << "In this game you will be controlling a snake head." << endl;
    cout << "Press 'w' to go up, 's' to go down, 'a' to go left, and 'd' to go right." << endl;
    cout << "Press 'x' to exit the game." << endl;
    cout << "Press 'p' to pause the game." << endl;
    cout << "press 'g' at any time during the game if you want to play the Deadly Wall game mode" << endl << endl;
    cout << "Your objective is to eat the fruits that will appear in a random location" << endl;
    cout << "across the game board by passing the snake's head trough the fruit." << endl;
    cout << "Each time you eat a fruit, your tail length will increase, the game speed will increase," << endl;
    cout << "and your score will increase by 10." << endl;
    cout << "You can also control the game speed at any time by hitting the '+' and '-' keys." << endl << endl;
    cout << "Good Luck!" << endl << endl;
}

//This function asks the user about how many fruits he/she wants to appear on the game
void FruitNumber()
{
    cout << "How many fruits do you want to appear on the game? Pick a number between 2 and 10" << endl;
    //Get the number of fruits from the user
    cin >> g_number_of_fruits;
    //Validate the data is correct. We want a number between 2 and 10 inclusive. Keep asking the user for new data until the value is valid
    while (g_number_of_fruits < g_kMinimumFruits || g_number_of_fruits > g_kMaxFruits)
    {
        cout << "The number of fruits is out of range. Pick a number between 2 and 10: " << endl;
        cin >> g_number_of_fruits;
    }
}

//Function used to initialize the starting position of the snake, the fruit, and to declare the starting score and if the game is over or not
void GameSetup()
{
    //Create a seed for a random number so it is truly random each time the program is opened
    unsigned random_number_seed = time(0);
    srand(random_number_seed);
    //The game is not over
    g_game_over = false;
    //Set the snake with no movement at the beginning of the program
    ksnake_direction = kSNAKE_STOP;
    //Position the snake at the middle of the game board
    g_head_position_x_axis = g_kGameBoardWidth / 2;
    g_head_position_y_axis = g_kGameBoardHeight / 2;
    //Position the fruit at a random location in the board
    g_fruit_position_x_axis = (rand() % g_kGameBoardWidth) + 1;
    g_fruit_position_y_axis = rand() % g_kGameBoardHeight;
    //Set the starting score to 0
    g_score = 0;
}

//This function clears the system so we are able to display new data on the screen
void ClearSystem()
{
    system("cls");
}

//Function used to draw the game board, the fruit, the snakes's head, and the snake's tail
void DrawGameBoard()
{
    //Draw the top edge of the game board
    for (int top_width_edge_counter = 0; top_width_edge_counter < g_kGameBoardWidth + 2; top_width_edge_counter++)
        cout << "#";
    cout << endl;
    //Code dedicated to the body of the game board between the top and bottom edges
    for (int writer_position_y_axis = 0; writer_position_y_axis < g_kGameBoardHeight; writer_position_y_axis++)
    {
        for (int writer_position_x_axis = 0; writer_position_x_axis < g_kGameBoardWidth + 2; writer_position_x_axis++)
        {
            //Write "#" when the writer position on the x axis is 0 to draw the left edge of the game board
            if (writer_position_x_axis == 0)
                cout << "#";
            //If the writer position on the x and y axis are the same as the snake's head position on the x and y axis, draw the snake's head
            else if (writer_position_y_axis == g_head_position_y_axis && writer_position_x_axis == g_head_position_x_axis)
                cout << "S";
            //If the writer position on the x and y axis are the same as the fruit position on the x and y axis, draw the fruit
            else if (writer_position_y_axis == g_fruit_position_y_axis && writer_position_x_axis == g_fruit_position_x_axis)
               cout << "F";
            //Write "#" if the writer position is the same as the game board width to draw the right edge of the game board
            else if (writer_position_x_axis == g_kGameBoardWidth + 1)
                cout << "#";
            //The following code is used to write the tail of the snake
            else
            {
                bool print_new_tail = false;
                //Draw as many pieces of tail as the total length of the tail
                for (int tail_counter = 0; tail_counter < g_tail_length; tail_counter++)
                {
                    //If the writer position is located were a piece of tail should be located, write "o"
                    if (g_tail_position_x_axis[tail_counter] == writer_position_x_axis && g_tail_position_y_axis[tail_counter] == writer_position_y_axis)
                    {
                        cout << "o";
                        print_new_tail = true;
                    }
                }
                //If a piece of tail is not needed, write a blank space
                if (!print_new_tail)
                {
                    cout << " ";
                }
            }
        }
        //Move the writer to the next y axis position using endl
        cout << endl;
    }
    //Draw the bottom edge of the game board
    for (int bottom_width_edge_counter = 0; bottom_width_edge_counter < g_kGameBoardWidth + 2; bottom_width_edge_counter++)
        cout << "#";
    cout << endl;
}

//This function is used to display the score
void DisplayScore()
{
    //Write the new score
    cout << "Score: " << g_score << endl;
    //If the user decides to play the Deadly Wall mode, display a message stating that the mode is on
    if (g_deadly_wall_on_off == 1)
        cout << "Deadly Wall game mode is on" << endl;
}

//Function used to control the snake's movements and quit the game if a specific key is pressed
void GameControlsInput()
{
    //the following code changes the direction of the snake if the user presses the following keys, case insensitive:
    //'a': Change the direction to the left
    //'d': Change the direction to the right
    //'w': Change the direction up
    //'s': Change the direction down
    //If 'p' is pressed, the program pauses
    //If '+' is pressed, the game speed increases
    //If '-' is pressed, the game speed decreases
    //If 'g' is pressed, the Deadly Wall mode is enabled
    //If 'x' is pressed, the program ends
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'a':
            ksnake_direction = kSNAKE_LEFT;
            break;
        case 'A':
            ksnake_direction = kSNAKE_LEFT;
            break;
        case 'd':
            ksnake_direction = kSNAKE_RIGHT;
            break;
        case 'D':
            ksnake_direction = kSNAKE_RIGHT;
            break;
        case 'w':
            ksnake_direction = kSNAKE_UP;
            break;
        case 'W':
            ksnake_direction = kSNAKE_UP;
            break;
        case 's':
            ksnake_direction = kSNAKE_DOWN;
            break;
        case 'S':
            ksnake_direction = kSNAKE_DOWN;
            break;
        case 'p':
            system("pause");
            break;
        case 'P':
            system("pause");
            break;
        case '-':
            if (g_game_speed < 100 && g_game_speed >= 0)
                g_game_speed = g_game_speed + 2;
            break;
        case '+':
            if (g_game_speed <= 100 && g_game_speed > 0)
                g_game_speed = g_game_speed - 2;
            break;
        case 'g':
            g_deadly_wall_option++;
            g_deadly_wall_on_off = g_deadly_wall_option % 2;
            break;
        case 'G':
            g_deadly_wall_option++;
            g_deadly_wall_on_off = g_deadly_wall_option % 2;
            break;
        case 'x':
            g_game_over = true;
            break;
        case 'X':
            g_game_over = true;
            break;
        }
    }
}

//This function moves the tail
void SnakeTailMovement()
{
    int previous_tail_position_x = g_tail_position_x_axis[0];
    int previous_tail_position_y = g_tail_position_y_axis[0];
    int previous_position_2_x, previous_position_2_y;
    g_tail_position_x_axis[0] = g_head_position_x_axis;
    g_tail_position_y_axis[0] = g_head_position_y_axis;
    for (int counter = 1; counter < g_tail_length; counter++)
    {
        previous_position_2_x = g_tail_position_x_axis[counter];
        previous_position_2_y = g_tail_position_y_axis[counter];
        g_tail_position_x_axis[counter] = previous_tail_position_x;
        g_tail_position_y_axis[counter] = previous_tail_position_y;
        previous_tail_position_x = previous_position_2_x;
        previous_tail_position_y = previous_position_2_y;
    }
}

//This function maintains a constant movement of the snake
void SnakeDirection()
{
    //If the snakes direction changes to the left, right, up, or down, the snake's head keeps going until the direction is changed again
    switch (ksnake_direction)
    {
    case kSNAKE_LEFT:
        g_head_position_x_axis--;
        break;
    case kSNAKE_RIGHT:
        g_head_position_x_axis++;
        break;
    case kSNAKE_UP:
        g_head_position_y_axis--;
        break;
    case kSNAKE_DOWN:
        g_head_position_y_axis++;
        break;
    default:
        break;
    }
}

//This function moves the snake from one side of the board to the other
void SnakeTeleportation()
{
    //If the snake's head is located where the right wall should be, move the snake's head to the opposite side of the game board
    if (g_head_position_x_axis >= g_kGameBoardWidth + 1)
        g_head_position_x_axis = 1;
    //If the snake's head is located where the left wall should be, move the snake's head to the opposite side of the game board
    else if (g_head_position_x_axis <= 0)
        g_head_position_x_axis = g_kGameBoardWidth;
    //If the snake's head is located where the bottom wall should be, move the snake's head to the opposite side of the game board
    if (g_head_position_y_axis >= g_kGameBoardHeight)
        g_head_position_y_axis = 0;
    //If the snake's head is located where the top wall should be, move the snake's head to the opposite side of the game board
    else if (g_head_position_y_axis < 0)
        g_head_position_y_axis = g_kGameBoardHeight - 1;
}

//This function ends the game if the snake eats itself
void SnakeEatsItself()
{
    for (int tail_address_counter = 0; tail_address_counter < g_tail_length; tail_address_counter++)
    {
        //If the snake's head is located where a piece of tail should be, display a message and set the game over variable to true
        if (g_tail_position_x_axis[tail_address_counter] == g_head_position_x_axis && g_tail_position_y_axis[tail_address_counter] == g_head_position_y_axis)
        {
            cout << endl << "FATALITY" << endl;
            cout << "You ate your own tail" << endl;
            g_game_over = true;
        }
    }
}

//This function set up the Deadly Wall mode
void DeadlyWallGameMode()
{
    //If the snake's head is located where a wall should be, display a message and set the game over variable to true
    if (g_head_position_x_axis >= g_kGameBoardWidth + 1 || g_head_position_x_axis <= 0 || g_head_position_y_axis >= g_kGameBoardHeight || g_head_position_y_axis < 0)
    {
        cout << endl << "FATALITY" << endl;
        cout << "You hit a wall" << endl;
        g_game_over = true;
    }
}

//This function makes the snake eat fruit
void SnakeEatsFruit()
{
    //If the snake's head moves over a fruit, add 10 points to the score, move the fruit to a new random location, and add a new piece of tail
    if (g_head_position_x_axis == g_fruit_position_x_axis && g_head_position_y_axis == g_fruit_position_y_axis)
    {
        g_score += 10;
        g_fruit_position_x_axis = (rand() % g_kGameBoardWidth) + 1;
        g_fruit_position_y_axis = rand() % g_kGameBoardHeight;
        g_tail_length++;
        //Also when the snake eats a fruit, increase the game speed but do not get out of the range of 100-0 (this value is passed to the sleep function)
        if (g_game_speed <= 100 && g_game_speed > 0)
            g_game_speed = g_game_speed - 4;
    }
}

//This function resets the program to the original values
void ProgramReset()
{
    //Assign the original values to the variables, just as they were at the beginning of the program
    g_head_position_x_axis = 0;
    g_head_position_y_axis = 0;
    g_fruit_position_x_axis = 0;
    g_fruit_position_y_axis = 0;
    g_tail_length = 0;
    g_deadly_wall_on_off = 0;
    g_deadly_wall_option = 2;
    g_game_speed = 100;
}

//This function asks the user if he or she wishes to play again, and get the data
void PlayAgainYesNo()
{
    cout << endl << "Would you like to play again? press 'y' for yes, 'n' for no" << endl;
    cin >> g_play_again_answer;
}

//Main function that calls all the previous functions
int main()
{
    do
    {
        //Clear the system
        ClearSystem();
        //Reset the program
        ProgramReset();
        //Display game instructions
        GameInstructions();
        //Asks the user how many fruits they wish to appear on the game
        FruitNumber();
        //Set up the game calling the setup function
        GameSetup();
        //While the game is not over, call the rest of the functions
        while (!g_game_over)
        {
            ClearSystem();
            DrawGameBoard();
            DisplayScore();
            GameControlsInput();
            SnakeTailMovement();
            SnakeDirection();
            //By default, the Deadly Wall mode is deactivated and the snake teleportation is activated
            if (g_deadly_wall_on_off == 0)
                SnakeTeleportation();
            //If the user wishes to play the Deadly wall mode, call its correspondent function instead of the teleportation function
            else if (g_deadly_wall_on_off == 1)
                DeadlyWallGameMode();
            SnakeEatsItself();;
            SnakeEatsFruit();
            //Make the game slower by calling the sleep function
            Sleep(g_game_speed);
        }
        //If the game over variable is set to true, call the the play again function
        PlayAgainYesNo();
    //Every time the user wants to play again, repeat the do loop
    } while (g_play_again_answer == 'y' || g_play_again_answer == 'Y');
    //If the user do not wish to play again, end the program
    if (g_play_again_answer == 'n' || g_play_again_answer == 'N')
        return 0;
}

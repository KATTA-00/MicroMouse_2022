// File:          cpp.cpp
// Date:
// Description:
// Author:
// Modifications:

// You may need to add webots include files such as
// <webots/DistanceSensor.hpp>, <webots/Motor.hpp>, etc.
// and/or to add some other includes
#include <webots/Robot.hpp>

// All the webots classes are defined in the "webots" namespace
using namespace webots;
template <byte ROWS, byte COLUMNS>

class NanoMouseMaze
{
  private:


  public:
    // value array
    byte values[ROWS][COLUMNS];

    /*byte mouseRow;
      byte mouseColumn;
      byte mouseHeading;*/

    byte targetRow = 2;
    byte targetColumn = 3;

    // vertical walls array
    boolean verticalWalls[ROWS][COLUMNS + 1];

    // horizontal walls array
    boolean horizontalWalls[ROWS + 1][COLUMNS];

    // vertical mind walls array
    boolean verticalMindWalls[ROWS][COLUMNS + 1];

    // horizontal mind walls array
    boolean horizontalMindWalls[ROWS + 1][COLUMNS];

    //Constructor method (called when the maze is created)
    NanoMouseMaze()
    {
      //initialize verticalWalls (add exterior walls)
      for (byte i = 0; i < ROWS; i++)
      {
        for (byte j = 0; j < COLUMNS + 1; j++)
        {
          if (j == 0 || j == COLUMNS)
          {
            verticalWalls[i][j] = true;
          }
        }
      }

      //initialize horizontalWalls (add exterior walls)
      for (byte i = 0; i < ROWS + 1; i++)
      {
        for (byte j = 0; j < COLUMNS; j++)
        {
          if (i == 0 || i == ROWS)
          {
            horizontalWalls[i][j] = true;
          }
        }
      }
    }
    ////////////////////////////////////////////
    void addtestwalls() {
      horizontalWalls[1][1] = true;
      horizontalWalls[1][2] = true;
      horizontalWalls[3][2] = true;

      verticalWalls[3][1] = true;
      verticalWalls[2][1] = true;
      verticalWalls[2][2] = true;
      verticalWalls[1][2] = true;
      verticalWalls[2][3] = true;
      //verticalWalls[0][3] = true;
    }
    ////////////////////////////////////////////
    /*void addnorthwall() {
      horizontalWalls[mouseRow][mouseColumn] = true;
      }
      void addsouthwall() {
      horizontalWalls[mouseRow + 1][mouseColumn] = true;
      }
      void addwestwall() {
      verticalWalls[mouseRow][mouseColumn] = true;
      }
      void addeastwall() {
      verticalWalls[mouseRow][mouseColumn + 1] = true;
      }*/
    void addwalls(int dir) {
      if (dir == 0 || dir == 4) {
        horizontalWalls[mouseRow][mouseColumn] = true;
      }
      else if (dir == 1) {
        verticalWalls[mouseRow][mouseColumn + 1] = true;
      }
      else if (dir == 2) {
        horizontalWalls[mouseRow + 1][mouseColumn] = true;
      }
      else if (dir == 3 || dir == -1) {
        verticalWalls[mouseRow][mouseColumn] = true;
      }

    }

    void addMindwalls(int dir) {
      if (dir == 0) {
        horizontalMindWalls[mouseRow][mouseColumn] = true;
      }
      else if (dir == 1) {
        verticalMindWalls[mouseRow][mouseColumn + 1] = true;
      }
      else if (dir == -2) {
        horizontalMindWalls[mouseRow + 1][mouseColumn] = true;
      }
      else if (dir == -1) {
        verticalMindWalls[mouseRow][mouseColumn] = true;
      }

    }

    void solve() {
      for (byte i = 0; i < ROWS; i++) {
        for (byte j = 0; j < COLUMNS; j++) {
          if (i == targetRow && j == targetColumn) {
            values[i][j] = 0;
          }
          else {
            values[i][j] = 255;
          }
        }
      }
      int count = 1;
      byte foold_val = 0;
      while (count != 0) {
        count = 0;
        for (byte i = 0; i < ROWS; i++) {
          for (byte j = 0; j < COLUMNS; j++) {
            if (values[i][j] == foold_val) {
              if (horizontalWalls[i][j] == false && values[i - 1][j] > foold_val && (i - 1) >= 0) {
                values[i - 1][j] = foold_val + 1;
              }
              if (horizontalWalls[i + 1][j] == false && values[i + 1][j] > foold_val && (i + 1) < ROWS) {
                values[i + 1][j] = foold_val + 1;
              }

              if (verticalWalls[i][j] == false && values[i][j - 1] > foold_val && (j - 1) >= 0) {
                values[i][j - 1] = foold_val + 1;
              }
              if (verticalWalls[i][j + 1] == false && values[i][j + 1] > foold_val && (j + 1) < ROWS) {
                values[i][j + 1] = foold_val + 1;
              }
            }
            else if (values[i][j] > foold_val) {
              count++;
            }
          }
        }
        foold_val++;
      }
    }


    //Add your code here


    /*Do not change or add code below this line

      NanoMouseMaze Print Function Version 2
      This version of the print function has been modified to print
      any size maze (the previous version could not print large
      mazes) and to work with the btMonitor Android App I wrote,
      which is available through my free online course at
      http://www.akrobotnerd.com
    */

    void print()
    {
      for (byte i = 0; i < 2 * ROWS + 1; i++)
      {
        for (byte j = 0; j < 2 * COLUMNS + 1; j++)
        {
          //Add Horizontal Walls
          if (i % 2 == 0 && j % 2 == 1)
          {
            if (horizontalWalls[i / 2][j / 2] == true)
            {
              Serial.print(" ---");
            }
            else
            {
              Serial.print("    ");
            }
          }

          //Add Vertical Walls
          if (i % 2 == 1 && j % 2 == 0)
          {
            if (verticalWalls[i / 2][j / 2] == true)
            {
              Serial.print("|");
            }
            else
            {
              Serial.print(" ");
            }
          }

          //Add Flood Fill Values
          if (i % 2 == 1 && j % 2 == 1)
          {
            if ((i - 1) / 2 == mouseRow && (j - 1) / 2 == mouseColumn)
            {
              if (mouseHeading == NORTH)
              {
                Serial.print(" ^ ");
              }
              else if (mouseHeading == EAST)
              {
                Serial.print(" > ");
              }
              else if (mouseHeading == SOUTH)
              {
                Serial.print(" v ");
              }
              else if (mouseHeading == WEST)
              {
                Serial.print(" < ");
              }
            }
            else
            {
              byte value = values[(i - 1) / 2][(j - 1) / 2];
              if (value >= 100)
              {
                Serial.print(value);
              }
              else
              {
                Serial.print(" ");
                Serial.print(value);
              }
              if (value < 10)
              {
                Serial.print(" ");
              }
            }
          }
        }
        Serial.print("\n");
      }
      Serial.print("\n");
    }
};

NanoMouseMaze<6, 6> maze;

// This is the main program of your controller.
// It creates an instance of your Robot instance, launches its
// function(s) and destroys it at the end of the execution.
// Note that only one instance of Robot should be created in
// a controller program.
// The arguments of the main function can be specified by the
// "controllerArgs" field of the Robot node
int main(int argc, char **argv) {
  // create the Robot instance.
  Robot *robot = new Robot();

  // get the time step of the current world.
  int timeStep = (int)robot->getBasicTimeStep();

  // You should insert a getDevice-like function in order to get the
  // instance of a device of the robot. Something like:
  //  Motor *motor = robot->getMotor("motorname");
  //  DistanceSensor *ds = robot->getDistanceSensor("dsname");
  //  ds->enable(timeStep);

  // Main loop:
  // - perform simulation steps until Webots is stopping the controller
  while (robot->step(timeStep) != -1) {
    // Read the sensors:
    // Enter here functions to read sensor data, like:
    //  double val = ds->getValue();

    // Process sensor data here.

    // Enter here functions to send actuator commands, like:
    //  motor->setPosition(10.0);
  };

  // Enter here exit cleanup code.

  delete robot;
  return 0;
}

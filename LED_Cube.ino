#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>

// Global Variables
int column[4][4];          // 16 colums LED cube
int volatile cube[4][4][4];         // 4x4x4 LED cube
int layer[4];      
const int SPEED = 100;    // length of delays (makes program run faster/slower)
const int SIZE = 4;        // Size of Cube

// Time_overflow = 1/16MHz * 2^n * divisor = T each interrupt is called
#define divisor 0x04  // 1, 8, 32, 64, 128, 256, 1024

  // T = 1/16Mhz * 2^8*divisor = .000016*divisor

void configure_Interrupt();
void configure_SerialCommunication();
void configure_Pins();

void test_each_LED();  // Test each LED 

int randomNumber(int MIN, int MAX);   // Generates random number between MIN and MAX
void Rain(int iterations); // LEDs Raining down

int wallCollision(int x, int y, int z, int dx, int dy, int dz);
void WallBall(int _speed, int iterations);

// Draw Shapes ***************************************************************************************
    void drawLine(int z1, int y1, int x1, int z2, int y2, int x2); // Connects two points with a line
    void drawCube(int z1, int y1, int x1,  int z2, int y2, int x2); // Draws cube given 2 corner points
// Light Up Functions ********************************************************************************
                      void All_LEDs_OFF();   // Turn all LEDs OFF
                      void All_LEDs_ON();    // Turn all LEDs ON
                      void Layer_ON(int L);  // Turn all LEDs on Level L on
                      
                      void XY_Plane_ON(int Z);// Turn on XY Plane on Z coordinate
                      void XZ_Plane_ON(int Y); // Turn on XZ Plane on at Y coordinate
                      void YZ_Plane_ON(int X); // Turn on YZ Plane on at X coordinate 
                      
                      void XY_Plane_OFF(int Z);// Turn off XY Plane on Z coordinate
                      void XZ_Plane_OFF(int Y); // Turn off XZ Plane on at Y coordinate
                      void YZ_Plane_OFF(int X); // Turn on XZ Plane on at X coordinate

void XY_Plane_Oscillate(int _speed);  // XY plane movement back and forth
void XZ_Plane_Oscillate(int _speed);  // XZ plane movement back and forth
void YZ_Plane_Oscillate(int _speed);  // YZ plane movement back and forth

// Shift Functions  **********************************************************************************************
                    void Shift_All(int z, int y, int x);  // Shift all LEDs in specified z, y, x direction
                    void Shift_All_Right();   // Shift all LEDs Right by 1
                    void Shift_All_Left();    // Shift all LEDs left by 1
                    void Shift_All_Forward(); // Shift all LEDs(+y) by 1s
                    void Shift_All_Backward();// Shift all LEDs(-y) by 1
                    void Shift_All_Up();      // Shift all LEDs up (+z) by 1
                    void Shift_All_Down();    // Shift all LEDs down (-z) by 1

                    void Shift_All_X_Reflect();  // Shift all LEDs back and forth in X directing  
                    void Shift_All_Y_Reflect();  // Shift all LEDs back and forth in Y directing  
                    void Shift_All_Z_Reflect();  // Shift all LEDs back and forth in X directing  

                    void Shift_Row_Right(int z, int y);   // Shift entire row right at height z, row y
                    void Shift_Row_Left(int z, int y);   // Shift entire row left  at height z, row y
                    void Shift_Col_Forward(int z, int x);// Shift entire column forward (+y) at height z, column 
                    void Shift_Col_Backward(int z, int x);// Shift entire column backward (-y) at height z, column x
                    void Shift_Vert_Col_Up(int y, int x); // Shift Vertical column up (+z) at point (y, x)
                    void Shift_Vert_Col_Down(int y, int x); // Shift Vertical column down (-z) at point (y, x)

                    void Shift_Array_Right(int _array[], int _size);   // Shift entire row of array right at height z
                    void Shift_Array_Left(int _array[], int _size);    // Shift entire row of array left at height z
                    

// Rotate Functions ************************************************************************************************
                    void Rotate_XY_Rectangular(int Z, int _direction); // Rotate All on XY plane at height z and direction (1 = clockwise, -1 = counterclockwise)
                    void Rotate_YZ_Rectangular(int X, int _direction); // Rotate All on XY plane at height x and direction (1 = clockwise, -1 = counterclockwise)
                    void Rotate_XZ_Rectangular(int Y, int _direction); // Rotate All on XZ plane at height y and direction (1 = clockwise, -1 = counterclockwise)

                    void Rotate_XY_Rectangular(int _direction); // Rotate All on XY plane at rotation and direction (1 = clockwise, -1 = counterclockwise)
                    void Rotate_YZ_Rectangular(int _direction); // Rotate All on YZ plane at rotation and direction (1 = clockwise, -1 = counterclockwise)
                    void Rotate_XZ_Rectangular(int _direction); // Rotate All on XZ plane at rotation and direction (1 = clockwise, -1 = counterclockwise)
                    void printArray(int myArray[]);

// Patterns **********************************************************************************************************
     void PATTERN_Rotating_Cube_1();     // Cube with rectangular rotations in different planes
     void PATTERN_Rotating_Line_1();     // Line with rectangular rotations in different planes
     void PATTERN_Plane_Oscillate();    // movement in all planes
     void PATTERN_Rotating_Plane_1();   // Rotates each plane in its own plane
     void PATTERN_Rotating_Plane_2();   // Rotates A Single plane in all directions  
     void PATTERN_Rotating_Plane_3();   // Rotates A Single plane in all directions 
     void PATTERN_Cube_Expansion();   // Cube expanding and contracting in corners of box

// Light Shows **********************************************************************************************************
     void LIGHTSHOW_1();

void setup() {
  // put your setup code here, to run once:
   
  configure_Pins();
  //configure_SerialCommunication();
  configure_Interrupt();
  
}
void loop()
{
  LIGHTSHOW_1();
}

void PATTERN_Cube_Expansion()   // Cube expanding and contracting in corners of box
{
  for (int i = 0; i < SIZE; i++)
  {
    drawCube(0,0,0, i,i,i);
    delay(SPEED);
    All_LEDs_OFF();
  }
  for (int i = 0; i < SIZE; i++)
  {
    drawCube(SIZE-1,SIZE-1,SIZE-1, i,i,i);
    delay(SPEED);
    All_LEDs_OFF();
  }
  for (int i = SIZE-1; i >= 0; i--)
  {
    drawCube(SIZE-1,SIZE-1,SIZE-1, i,i,i);
    delay(SPEED);
    All_LEDs_OFF();
  }
   
  for (int i = 0; i < SIZE; i++)
  {
    drawCube(0,0,SIZE-1, SIZE-1-i, SIZE-1-i, i);
    delay(SPEED);
    All_LEDs_OFF();
  }
  for (int i = SIZE-1; i >= 0; i--)
  {
    drawCube(0,0,SIZE-1, SIZE-1-i, SIZE-1-i, i);
    delay(SPEED);
    All_LEDs_OFF();
  }
  
  for (int i = 0; i < SIZE; i++)
  {
    drawCube(0,SIZE-1,0, SIZE-1-i, i,  SIZE-1-i);
    delay(SPEED);
    All_LEDs_OFF();
  }
  for (int i = 0; i < SIZE; i++)
  {
    drawCube(0,SIZE-1,0, i, SIZE-1-i, i);
    delay(SPEED);
    All_LEDs_OFF();
  }

  for (int i = SIZE-1; i >= 0; i--)
  {
    drawCube(SIZE-1,0,0,  SIZE-1-i, i, i);
    delay(SPEED);
    All_LEDs_OFF();
  }

   for (int i = 0; i < SIZE; i++)
  {
    drawCube(SIZE-1,0,0,  SIZE-1-i, i, i);
    delay(SPEED);
    All_LEDs_OFF();
  }
  
  for (int i = 0; i < SIZE; i++)
  {
    drawCube(0,0,SIZE-1,  SIZE-1-i, SIZE-1-i, i);
    delay(SPEED);
    All_LEDs_OFF();
  }
  for (int i = 0; i < SIZE; i++)
  {
    drawCube(0,0,SIZE-1,  i, i, SIZE-1-i);
    delay(SPEED);
    All_LEDs_OFF();
  }

   for (int i = 0; i < SIZE; i++)
  {
    drawCube(SIZE-1,SIZE-1,0,  i, i, SIZE-1-i);
    delay(SPEED);
    All_LEDs_OFF();
  }

   for (int i = 0; i < SIZE; i++)
  {
    drawCube(SIZE-1,SIZE-1,0,  SIZE-1-i, SIZE-1-i, i);
    delay(SPEED);
    All_LEDs_OFF();
  }

   for (int i = 0; i < SIZE; i++)
  {
    drawCube(0,0,0,  SIZE-1-i, SIZE-1-i, SIZE-1-i);
    delay(SPEED);
    All_LEDs_OFF();
  } 
}

void drawCube(int z1, int y1, int x1,  int z2, int y2, int x2)
{
  int temp_x, temp_y, temp_z;
  if (z1 > z2)
  {
    temp_z = z1;
    z1 = z2;
    z2 = temp_z;
  }
   if (y1 > y2)
  {
    temp_y = y1;
    y1 = y2;
    y2 = temp_y;
  }
   if (x1 > x2)
  {
    temp_x = x1;
    x1 = x2;
    x2 = temp_x;
  }
  for (int y = y1; y <= y2; y++)
  {
    
    cube[z1][y][x1] = 1;
    cube[z1][y][x2] = 1;
    cube[z2][y][x1] = 1;
    cube[z2][y][x2] = 1;
  }
   for (int x = x1; x <= x2; x++)
  {
    cube[z1][y1][x] = 1;
    cube[z1][y2][x] = 1;
    cube[z2][y1][x] = 1;
    cube[z2][y2][x] = 1;
  }
    for (int z = z1; z <= z2; z++)
  {
    cube[z][y1][x1] = 1;
    cube[z][y1][x2] = 1;
    cube[z][y2][x1] = 1;
    cube[z][y2][x2] = 1;
  }
  
}
void LIGHTSHOW_1()
{
   PATTERN_Rotating_Line_1();     // Line with rectangular rotations in different planes
   PATTERN_Rotating_Cube_1();     // Cube with rectangular rotations in different planes
   PATTERN_Plane_Oscillate();    // movement in all planes
   PATTERN_Rotating_Plane_1();   // Rotates each plane in its own plane
   PATTERN_Rotating_Plane_2();   // Rotates A Single plane in all directions  
   PATTERN_Rotating_Plane_3();   // Rotates A Single plane in all directions 
   PATTERN_Cube_Expansion();
   Rain(50);
   All_LEDs_OFF();
}

void Shift_All_X_Reflect()  // Shift all LEDs back and forth in X directing  
{
  static int _direction[SIZE][SIZE];  // Directing of each row (1 or -1)
  static int counter = 0;
  int x;
  // reset direction
    // Check if any LEDs at boundaries
    for (int z = 0; z<SIZE; z++)
      for (int y = 0; y < SIZE; y++)
      { 
          x = 0;
          if (cube[z][y][x] == 1)
            _direction[z][y] = 1;   // Direction = +x
  
          x = SIZE-1;
          if (cube[z][y][x] == 1)
            _direction[z][y] = 0;  // Direction = -x
  
          if (_direction[z][y] == 1)
            Shift_Row_Right(z, y);
          else
            Shift_Row_Left(z, y);
      }

    counter++;
}
void Shift_All_Y_Reflect()   // Shift all LEDs back and forth in Y directing
{
    static int _direction[SIZE][SIZE];  // Directing of each row (1 or -1)

  // Check if any LEDs at boundaries
  int y;
  for (int z = 0; z < SIZE; z++)
    for (int x = 0; x < SIZE; x++)
    { 
        y = 0;
        if (cube[z][y][x] == 1)
          _direction[z][x] = 1;   // Direction = +x

        y = SIZE-1;
        if (cube[z][y][x] == 1)
          _direction[z][x] = 0;  // Direction = -x

        if (_direction[z][x] == 1)
          Shift_Col_Forward(z, x);
        else
          Shift_Col_Backward(z, x);
    }
}
void Shift_All_Z_Reflect()  // Shift all LEDs back and forth in Z directing  
{
  static int _direction[SIZE][SIZE];  // Directing of each row (1 or -1)

  // Check if any LEDs at boundaries
  int z;
  for (int y = 0; y < SIZE; y++)
    for (int x = 0; x < SIZE; x++)
    { 
        z = 0;
        if (cube[z][y][x] == 1)
          _direction[y][x] = 1;   // Direction = +z

        z = SIZE-1;
        if (cube[z][y][x] == 1)
          _direction[y][x] = 0;  // Direction = -z

        if (_direction[y][x] == 1)
          Shift_Vert_Col_Up(y,x);
        else
          Shift_Vert_Col_Down(y,x);
    }
}

void printArray(int myArray[], int len)
{
  for (int i = 0; i < len; i++)
  {
    Serial.print("[");    Serial.print(i);    Serial.print("] = ");   Serial.print(myArray[i]);Serial.print("\t");
  }
  Serial.print("\n");
}

// XY: x y    z
// XZ: x z    y
void ShiftPlane()
{
  XY_Plane_ON(0);

  
  All_LEDs_OFF();
}

void PATTERN_Rotating_Cube_1()
{
  cube[0][0][0] = 1;
  cube[0][1][0] = 1;
  cube[0][0][1] = 1;
  cube[0][1][1] = 1;

  cube[1][0][0] = 1;
  cube[1][1][0] = 1;
  cube[1][0][1] = 1;
  cube[1][1][1] = 1;

  for (int i = 0; i < 12; i++)
  {
    delay(SPEED);
    Rotate_XY_Rectangular(1);
  }
   for (int i = 0; i < 12; i++)
  {
    delay(SPEED);
    Rotate_XZ_Rectangular(1);
  }
  
   for (int i = 0; i < 12; i++)
  {
    delay(SPEED);
    Rotate_YZ_Rectangular(1);
  }

  cube[0][0][0] = 0;
  cube[0][1][0] = 0;
  cube[0][0][1] = 0;
  cube[0][1][1] = 0;

  cube[1][0][0] = 0;
  cube[1][1][0] = 0;
  cube[1][0][1] = 0;
  cube[1][1][1] = 0;
}

void PATTERN_Rotating_Line_1()
{
  cube[0][0][0] = 1;
  cube[0][0][1] = 1;
  cube[0][0][2] = 1;
  cube[0][0][3] = 1;

  for (int i = 0; i < 12; i++)
  {
    delay(SPEED);
    Rotate_XY_Rectangular(1);
  }
   for (int i = 0; i < 12; i++)
  {
    delay(SPEED);
    Rotate_XZ_Rectangular(1);
  }
  
   for (int i = 0; i < 12; i++)
  {
    delay(SPEED);
    Rotate_YZ_Rectangular(1);
  }

  cube[0][0][0] = 0;
  cube[0][0][1] = 0;
  cube[0][0][2] = 0;
  cube[0][0][3] = 0;
}

void PATTERN_Rotating_Plane_1()   // Rotates each plane in its own plane
{
   drawLine(0,0,0, 0,3,3); // Connects two points with a line
   drawLine(1,0,0, 1,3,3); // Connects two points with a line
   drawLine(2,0,0, 2,3,3); // Connects two points with a line
   drawLine(3,0,0, 3,3,3); // Connects two points with a line

   for (int i = 0; i < 24; i++)
   {
     delay(SPEED);
     Rotate_XY_Rectangular(1);
   }
     All_LEDs_OFF();
     
   drawLine(0,0,0, 3,3,0); // Connects two points with a line
   drawLine(0,0,1, 3,3,1); // Connects two points with a line
   drawLine(0,0,2, 3,3,2); // Connects two points with a line
   drawLine(0,0,3, 3,3,3); // Connects two points with a line
   
   for (int i = 0; i < 24; i++)
   {
     delay(SPEED);
     Rotate_YZ_Rectangular(1);
   }
   All_LEDs_OFF();
   drawLine(0,0,0, 3,0,3); // Connects two points with a line
   drawLine(0,1,0, 3,1,3); // Connects two points with a line
   drawLine(0,2,0, 3,2,3); // Connects two points with a line
   drawLine(0,3,0, 3,3,3); // Connects two points with a line
   
    for (int i = 0; i < 24; i++)
   {
     delay(SPEED);
     Rotate_XZ_Rectangular(1);
   }

  All_LEDs_OFF();
}

void PATTERN_Rotating_Plane_2()   // Rotates A Single plane in all directions
{
   drawLine(0,0,0, 0,3,3); // Connects two points with a line
   drawLine(1,0,0, 1,3,3); // Connects two points with a line
   drawLine(2,0,0, 2,3,3); // Connects two points with a line
   drawLine(3,0,0, 3,3,3); // Connects two points with a line


   for (int i = 0; i < 12; i++)
   {
     delay(SPEED);
     Rotate_XY_Rectangular(1);
   }
   for (int i = 0; i < 24; i++)
   {
     delay(SPEED);
     Rotate_YZ_Rectangular(1);
   }
    for (int i = 0; i < 12; i++)
   {
     delay(SPEED);
     Rotate_XY_Rectangular(1);
   }
    for (int i = 0; i < 24; i++)
   {
     delay(SPEED);
     Rotate_XZ_Rectangular(1);
   }

  All_LEDs_OFF();
}

void PATTERN_Rotating_Plane_3()   // Rotates A Single plane in all directions
{
  drawLine(0,0,0,  3,0,3);
  drawLine(0,1,0,  3,1,3);
  drawLine(0,2,0,  3,2,3);
  drawLine(0,3,0,  3,3,3);
  int num = 3;

  for (int i = 0; i < 12*num; i++)
  {
    delay(SPEED);
    Rotate_XY_Rectangular(1);
  }
   for (int i = 0; i < 12*num; i++)
  {
    delay(SPEED);
    Rotate_XZ_Rectangular(1);
  }
  
   for (int i = 0; i < 12*num; i++)
  {
    delay(SPEED);
    Rotate_YZ_Rectangular(1);
  }

  All_LEDs_OFF();
}

void Rotate_XZ_Rectangular(int _direction) // Rotate All on XY plane and direction (1 = clockwise, -1 = counterclockwise)
{
   Rotate_XZ_Rectangular(0, _direction);
   Rotate_XZ_Rectangular(1, _direction);
   Rotate_XZ_Rectangular(2, _direction);
   Rotate_XZ_Rectangular(3, _direction);
}

void Rotate_XY_Rectangular(int _direction) // Rotate All on XY plane at rotation and direction (1 = clockwise, -1 = counterclockwise)
{
   Rotate_XY_Rectangular(0, _direction);
   Rotate_XY_Rectangular(1, _direction);
   Rotate_XY_Rectangular(2, _direction);
   Rotate_XY_Rectangular(3, _direction);
}
void Rotate_YZ_Rectangular(int _direction) // Rotate All on YZ plane at rotation and direction (1 = clockwise, -1 = counterclockwise)
{
   Rotate_YZ_Rectangular(0, _direction);
   Rotate_YZ_Rectangular(1, _direction);
   Rotate_YZ_Rectangular(2, _direction);
   Rotate_YZ_Rectangular(3, _direction);
}

void Rotate_XZ_Rectangular(int Y, int _direction) // Rotate All on XY plane at height z and direction (1 = clockwise, -1 = counterclockwise)
{ 
  int offset;
  static int side_counter[SIZE] = {1,1,1,1};   // Initial value is -1, but resets back to 0 when = 2
  int side1[SIZE], side2[SIZE], side3[SIZE], side4[SIZE];  // Temp variables for storing sides of rotation
  int inner_side1[SIZE-2], inner_side2[SIZE-2], inner_side3[SIZE-2], inner_side4[SIZE-2]; 
  //        side1
  //side4           side2
  //        side3
  int y = Y;
  int z, x;

  // Copy Sides 1 & 3
  for (int x = 0; x < SIZE; x++)    
  {
      z = 0;
      side1[x] = cube[z][y][x];
        //Serial.print("side1["); Serial.print(y); Serial.print("]["); Serial.print(x); Serial.print("] = "); Serial.print(side1[x]); Serial.print("\t");  
      z = SIZE-1;
      side3[x] = cube[z][y][x]; 
        //Serial.print("side3["); Serial.print(y); Serial.print("]["); Serial.print(x); Serial.print("] = "); Serial.print(side3[x]); Serial.print("\t");
        //Serial.print("\n");  
  }

  // Copy Sides 2 & 4
  for (int z = 0; z < SIZE; z++)    
  {
      x = SIZE-1;
      side2[z] = cube[z][y][x];
        //Serial.print("side2["); Serial.print(y); Serial.print("]["); Serial.print(x); Serial.print("] = "); Serial.print(side2[y]); Serial.print("\t");  
      x = 0;
      side4[z] = cube[z][y][x];
        //Serial.print("side4["); Serial.print(y); Serial.print("]["); Serial.print(x); Serial.print("] = "); Serial.print(side4[y]); Serial.print("\t");  
        //Serial.print("\n"); 
  }

  // Copy Inner Sides 1 & 3
  offset = 1;
  for (int x = 1; x < SIZE-offset; x++)    
  {
      z = 0+offset;
      inner_side1[x-offset] = cube[z][y][x];
      z = SIZE-1 - offset;
      inner_side3[x-offset] = cube[z][y][x]; 
  }

  // Copy Inner Sides 2 & 4
  for (int z = 0+offset; z < SIZE-offset; z++)    
  {
      x = SIZE-1 - offset;
      inner_side2[z-offset] = cube[z][y][x];
      x = 0 + offset;
      inner_side4[z-offset] = cube[z][y][x];
  }
    // Erase previous XY layer z of cube to add in new layer
    XZ_Plane_OFF(y);
  if (_direction == 1)
  {
    // Rotate Positive
    
      Shift_Array_Right(side1, SIZE);
      Shift_Array_Right(side2, SIZE);
      Shift_Array_Left(side3, SIZE);     
      Shift_Array_Left(side4, SIZE);

      if (side_counter[y] == 2)
      {
          Shift_Array_Right(inner_side1, 2);
          Shift_Array_Right(inner_side2, 2);
          Shift_Array_Left(inner_side3, 2);    
          Shift_Array_Left(inner_side4, 2);
          side_counter[y] = 0;
      }
       else
        side_counter[y]++;  
  }
   else if (_direction == -1)
   {
      // Rotate Negative
      Shift_Array_Left(side1, SIZE);
      Shift_Array_Left(side2, SIZE);
      Shift_Array_Right(side3, SIZE);     
      Shift_Array_Right(side4, SIZE);

      if (side_counter[y] == 2)
      {
          Shift_Array_Left(inner_side1, 2);
          Shift_Array_Left(inner_side2, 2);
          Shift_Array_Right(inner_side3, 2);    
          Shift_Array_Right(inner_side4, 2);
          side_counter[y] = 0;
      }
       else
        side_counter[y]++;  
   }   

  // Copy temp arrays back into main 3d array

  // Copy Sides 1 & 3
  for (int x = 0; x < SIZE; x++)    
  {
      z = 0;
      cube[z][y][x] |= side1[x];
      z = SIZE-1;
      cube[z][y][x] |= side3[x];
  }
  
  // Copy Sides 2 & 4
  for (int z = 0; z < SIZE; z++)    
  {
      x = SIZE-1;
      cube[z][y][x] |= side2[z];
      x = 0;
      cube[z][y][x] |= side4[z];
  }
  
  // Copy Inner Sides 1 & 3
  offset = 1;
  for (int x = 0+offset; x < SIZE-offset; x++)    
  {
      z = 0+offset;
      cube[z][y][x] |= inner_side1[x-offset];
      z = SIZE-1 - offset;
      cube[z][y][x] |= inner_side3[x-offset]; 
  }

  // Copy Inner Sides 2 & 4
  for (int z = 0+offset; z < SIZE-offset; z++)    
  {
      x = SIZE-1 - offset;
      cube[z][y][x] |= inner_side2[z-offset];
      x = 0 + offset;
      cube[z][y][x] |= inner_side4[z-offset];
  }
  //Serial.print("Rotate\n"); 
}


void Rotate_YZ_Rectangular(int X, int _direction) // Rotate All on XY plane at height z and rotation amount i (left or right)
{ 
  int offset;
  static int side_counter[SIZE] = {1,1,1,1};   // Initial value is -1, but resets back to 0 when = 2
  int side1[SIZE], side2[SIZE], side3[SIZE], side4[SIZE];  // Temp variables for storing sides of rotation
  int inner_side1[SIZE-2], inner_side2[SIZE-2], inner_side3[SIZE-2], inner_side4[SIZE-2]; 
  //        side1
  //side4           side2
  //        side3
  int x = X;
  int z, y;

  // Copy Sides 1 & 3
  for (int y = 0; y < SIZE; y++)    
  {
      z = 0;
      side1[y] = cube[z][y][x];
        //Serial.print("side1["); Serial.print(y); Serial.print("]["); Serial.print(x); Serial.print("] = "); Serial.print(side1[x]); Serial.print("\t");  
      z = SIZE-1;
      side3[y] = cube[z][y][x]; 
        //Serial.print("side3["); Serial.print(y); Serial.print("]["); Serial.print(x); Serial.print("] = "); Serial.print(side3[x]); Serial.print("\t");
        //Serial.print("\n");  
  }

  // Copy Sides 2 & 4
  for (int z = 0; z < SIZE; z++)    
  {
      y = SIZE-1;
      side2[z] = cube[z][y][x];
        //Serial.print("side2["); Serial.print(y); Serial.print("]["); Serial.print(x); Serial.print("] = "); Serial.print(side2[y]); Serial.print("\t");  
      y = 0;
      side4[z] = cube[z][y][x];
        //Serial.print("side4["); Serial.print(y); Serial.print("]["); Serial.print(x); Serial.print("] = "); Serial.print(side4[y]); Serial.print("\t");  
        //Serial.print("\n"); 
  }

  // Copy Inner Sides 1 & 3
  offset = 1;
  for (int y = 1; y < SIZE-offset; y++)    
  {
      z = 0+offset;
      inner_side1[y-offset] = cube[z][y][x];
      z = SIZE-1 - offset;
      inner_side3[y-offset] = cube[z][y][x]; 
  }

  // Copy Inner Sides 2 & 4
  for (int z = 0+offset; z < SIZE-offset; z++)    
  {
      y = SIZE-1 - offset;
      inner_side2[z-offset] = cube[z][y][x];
      y = 0 + offset;
      inner_side4[z-offset] = cube[z][y][x];
  }
    // Erase previous XY layer z of cube to add in new layer
    YZ_Plane_OFF(x);
  if (_direction == 1)
  {
    // Rotate Positive
    
      Shift_Array_Right(side1, SIZE);
      Shift_Array_Right(side2, SIZE);
      Shift_Array_Left(side3, SIZE);     
      Shift_Array_Left(side4, SIZE);

      if (side_counter[x] == 2)
      {
          Shift_Array_Right(inner_side1, 2);
          Shift_Array_Right(inner_side2, 2);
          Shift_Array_Left(inner_side3, 2);    
          Shift_Array_Left(inner_side4, 2);
          side_counter[x] = 0;
      }
       else
        side_counter[x]++;  
  }
   else if (_direction == -1)
   {
      // Rotate Negative
      Shift_Array_Left(side1, SIZE);
      Shift_Array_Left(side2, SIZE);
      Shift_Array_Right(side3, SIZE);     
      Shift_Array_Right(side4, SIZE);

      if (side_counter[x] == 2)
      {
          Shift_Array_Left(inner_side1, 2);
          Shift_Array_Left(inner_side2, 2);
          Shift_Array_Right(inner_side3, 2);    
          Shift_Array_Right(inner_side4, 2);
          side_counter[x] = 0;
      }
       else
        side_counter[x]++;  
   }   

  // Copy temp arrays back into main 3d array

  // Copy Sides 1 & 3
  for (int y = 0; y < SIZE; y++)    
  {
      z = 0;
      cube[z][y][x] |= side1[y];
      z = SIZE-1;
      cube[z][y][x] |= side3[y];
  }
  
  // Copy Sides 2 & 4
  for (int z = 0; z < SIZE; z++)    
  {
      y = SIZE-1;
      cube[z][y][x] |= side2[z];
      y = 0;
      cube[z][y][x] |= side4[z];
  }
  
  // Copy Inner Sides 1 & 3
  offset = 1;
  for (int y = 0+offset; y < SIZE-offset; y++)    
  {
      z = 0+offset;
      cube[z][y][x] |= inner_side1[y-offset];
      z = SIZE-1 - offset;
      cube[z][y][x] |= inner_side3[y-offset]; 
  }

  // Copy Inner Sides 2 & 4
  for (int z = 0+offset; z < SIZE-offset; z++)    
  {
      y = SIZE-1 - offset;
      cube[z][y][x] |= inner_side2[z-offset];
      y = 0 + offset;
      cube[z][y][x] |= inner_side4[z-offset];
  }
  //Serial.print("Rotate\n"); 
}


void Rotate_XY_Rectangular(int Z, int _direction) // Rotate All on XY plane at height z and rotation amount i (left or right)
{ 
  int offset;
  static int side_counter[SIZE] = {1,1,1,1};   // Initial value is -1, but resets back to 0 when = 2
  int side1[SIZE], side2[SIZE], side3[SIZE], side4[SIZE];  // Temp variables for storing sides of rotation
  int inner_side1[SIZE-2], inner_side2[SIZE-2], inner_side3[SIZE-2], inner_side4[SIZE-2]; 
  //        side1
  //side4           side2
  //        side3
  int z = Z;
  int y, x;

  // Copy Sides 1 & 3
  for (int x = 0; x < SIZE; x++)    
  {
      y = 0;
      side1[x] = cube[z][y][x];
        //Serial.print("side1["); Serial.print(y); Serial.print("]["); Serial.print(x); Serial.print("] = "); Serial.print(side1[x]); Serial.print("\t");  
      y = SIZE-1;
      side3[x] = cube[z][y][x]; 
        //Serial.print("side3["); Serial.print(y); Serial.print("]["); Serial.print(x); Serial.print("] = "); Serial.print(side3[x]); Serial.print("\t");
        //Serial.print("\n");  
  }

  // Copy Sides 2 & 4
  for (int y = 0; y < SIZE; y++)    
  {
      x = SIZE-1;
      side2[y] = cube[z][y][x];
        //Serial.print("side2["); Serial.print(y); Serial.print("]["); Serial.print(x); Serial.print("] = "); Serial.print(side2[y]); Serial.print("\t");  
      x = 0;
      side4[y] = cube[z][y][x];
        //Serial.print("side4["); Serial.print(y); Serial.print("]["); Serial.print(x); Serial.print("] = "); Serial.print(side4[y]); Serial.print("\t");  
        //Serial.print("\n"); 
  }

  // Copy Inner Sides 1 & 3
  offset = 1;
  for (int x = 1; x < SIZE-offset; x++)    
  {
      y = 0+offset;
      inner_side1[x-offset] = cube[z][y][x];
      y = SIZE-1 - offset;
      inner_side3[x-offset] = cube[z][y][x]; 
  }

  // Copy Inner Sides 2 & 4
  for (int y = 0+offset; y < SIZE-offset; y++)    
  {
    
      x = SIZE-1 - offset;
      inner_side2[y-offset] = cube[z][y][x];
      x = 0 + offset;
      inner_side4[y-offset] = cube[z][y][x];
  }
    // Erase previous XY layer z of cube to add in new layer
    XY_Plane_OFF(z);
  if (_direction == 1)
  {
    // Rotate Positive
    
      Shift_Array_Right(side1, SIZE);
      Shift_Array_Right(side2, SIZE);
      Shift_Array_Left(side3, SIZE);     
      Shift_Array_Left(side4, SIZE);

      if (side_counter[z] == 2)
      {
          Shift_Array_Right(inner_side1, 2);
          Shift_Array_Right(inner_side2, 2);
          Shift_Array_Left(inner_side3, 2);    
          Shift_Array_Left(inner_side4, 2);
          side_counter[z] = 0;
      }
       else
        side_counter[z]++;  
  }
   else if (_direction == -1)
   {
      // Rotate Negative
      Shift_Array_Left(side1, SIZE);
      Shift_Array_Left(side2, SIZE);
      Shift_Array_Right(side3, SIZE);     
      Shift_Array_Right(side4, SIZE);

      if (side_counter[z] == 2)
      {
          Shift_Array_Left(inner_side1, 2);
          Shift_Array_Left(inner_side2, 2);
          Shift_Array_Right(inner_side3, 2);    
          Shift_Array_Right(inner_side4, 2);
          side_counter[z] = 0;
      }
       else
        side_counter[z]++;  
   }   

  // Copy temp arrays back into main 3d array

  // Copy Sides 1 & 3
  for (int x = 0; x < SIZE; x++)    
  {
      y = 0;
      cube[z][y][x] |= side1[x];
      y = SIZE-1;
      cube[z][y][x] |= side3[x];
  }
  
  // Copy Sides 2 & 4
  for (int y = 0; y < SIZE; y++)    
  {
      x = SIZE-1;
      cube[z][y][x] |= side2[y];
      x = 0;
      cube[z][y][x] |= side4[y];
  }
  
  // Copy Inner Sides 1 & 3
  offset = 1;
  for (int x = 0+offset; x < SIZE-offset; x++)    
  {
      y = 0+offset;
      cube[z][y][x] |= inner_side1[x-offset];
      y = SIZE-1 - offset;
      cube[z][y][x] |= inner_side3[x-offset]; 
  }

  // Copy Inner Sides 2 & 4
  for (int y = 0+offset; y < SIZE-offset; y++)    
  {
      x = SIZE-1 - offset;
      cube[z][y][x] |= inner_side2[y-offset];
      x = 0 + offset;
      cube[z][y][x] |= inner_side4[y-offset];
  }
  //Serial.print("Rotate\n"); 
}


void Shift_Row_Right(int z, int y)   // Shift entire row right at height z, row y
{
   for (int x = SIZE-1; x > 0; x--)
   { 
      cube[z][y][x] = cube[z][y][x-1];
      cube[z][y][x-1] = 0;
   }
}

void Shift_Array_Right(int _array[], int _size)   // Shift entire row of array right at height z
{
   for (int x = _size-1; x > 0; x--)
   { 
      _array[x] = _array[x-1];
      _array[x-1] = 0;
   }
}

void Shift_Row_Left(int z, int y)   // Shift entire row left at height z, row y
{
    for (int x = 0; x < SIZE-1; x++)
      { 
          cube[z][y][x] = cube[z][y][x+1];
          cube[z][y][x+1] = 0;
      }
}

void Shift_Array_Left(int _array[], int _size)   // Shift entire row of array left at height z
{
   for (int x = 0; x < _size-1; x++)
   { 
      _array[x] = _array[x+1];
      _array[x+1] = 0;
   }
}

void Shift_Col_Forward(int z, int x)  // Shift entire column forward (+y) at height z, column x
{
   for (int y = SIZE-1; y > 0; y--)
   { 
      cube[z][y][x] = cube[z][y-1][x];
      cube[z][y-1][x] = 0;
   }
}

void Shift_Col_Backward(int z, int x)  // Shift entire column backward (-y) at height z, column x
{
    for (int y = 0; y < SIZE-1; y++)
      { 
          cube[z][y][x] = cube[z][y+1][x];
          cube[z][y+1][x] = 0;
      }
}

void Shift_Vert_Col_Up(int y, int x)  // Shift Vertical column up (+z) at point (y, x)
{
    for (int z = SIZE-1; z > 0; z--)
    { 
       cube[z][y][x] = cube[z-1][y][x];
       cube[z-1][y][x] = 0;
    }
}

void Shift_Vert_Col_Down(int y, int x) // Shift Vertical column down (-z) at point (y, x)
{
   // z = layer    y = row     x = column
   for (int z = 0; z < SIZE-1; z++)
   { 
      cube[z][y][x] = cube[z+1][y][x];
      cube[z+1][y][x] = 0;
   }
}


void WallBall(int _speed, int iterations)
{
  int X = 0;
  int Y = 0;
  int Z = 0;
  
  int dx = 1;
  int dy = 1;
  int dz = 1;
  int x = X;
  int y = Y;
  int z = Z;
  cube[z][y][x] = 1;

  
  for (int i = 0; i < iterations; i++)
  {
    delay(_speed);
      if (x + dx < 0 || x + dx >= SIZE)
          dx *= -1;
      if (y + dy < 0 || y + dy >= SIZE)
          dy *= -1;
      if (z + dz < 0 || z + dz >= SIZE)
          dz *= -1;
    
    // Advance
    cube[z][y][x] = 0;  // Turn off previous frame
    if (i%8 == 0)
    {
       X++;
       if (X%SIZE == 0)
       {
          X = 0;
          Y++;
       }
       if (Y%SIZE == 0)
       {
          Y = 0;
          Z++;
       }  
       if (Z%SIZE == 0)
       {
        Z = 0;
       }
       x = X; y = Y; z = Z;
    }
    x += dx;   y += dy;   z += dz;
    
    cube[z][y][x] = 1;
  }
}

int wallCollision(int x, int y, int z, int dx, int dy, int dz)
// 0 - No collisions
// 1 - X Collision
// 2 - Y Collision
// 3 - Z Collision
{
  bool x_collision = 0;
  bool y_collision = 0;
  bool z_collision = 0;
  
  if (x + dx < -1 || x + dx > SIZE)
      x_collision = 1;
  else if (y + dy < -1 || y + dy > SIZE)
      y_collision = 1;
  else if (z + dz < -1 || z + dz > SIZE)
      z_collision = 1;

  if (x_collision && y_collision && z_collision)
      return 3;
  if (x_collision && y_collision)
      return 2;
  
   return 0;
}

void drawLine(int z1, int y1, int x1, int z2, int y2, int x2) // Connects two points with a line
{
  // slope: m  = (y2 - y1)/(x2 - x1)

  // Flip if necessary

  int x_line[SIZE];   // Temp array to hold x coord line data
  int y_line[SIZE];   // Temp array to hold y coord line data
  int z_line[SIZE];   // Temp array to hold z coord line data
  int pointCounter = 0;         // Counts number of plotted points in cube
  int temp_z, temp_y, temp_x;
  bool flip_y = false;
  bool flip_x = false;
  if (y1 > y2)
  {
    //Serial.print("Flip y\n");
    flip_y = true;
  }
  
  if (x1 > x2)
  {
    //Serial.print("Flip x\n");
    flip_x = true;
  }
  float m;      // xy slope
  float z_slope;

  z_slope = z2-z1;    
  //if (x2-x1 == 0)
    //  m = 0;
  //else
      m = (y2-y1)/float(x2-x1);

  if (flip_x && flip_y)
  {
    //Serial.print("Coordinate Flip\n");
    temp_y = y1;    temp_x = x1;
    y1 = y2;        x1 = x2;
    y2 = temp_y;    x2 = temp_x;
    flip_x = false;   flip_y = false;
  }
  
  Serial.print("(");
  Serial.print(y1);
  Serial.print(", ");
  Serial.print(x1);
  Serial.print(")->");
   Serial.print("(");
  Serial.print(y2);
  Serial.print(", ");
  Serial.print(x2);
  Serial.print(")");
   Serial.print("\n");
  
  //Serial.print("m = ");
  //Serial.print(m);
  //Serial.print("\n");
    // y = mx
  if (abs(x2-x1) >= abs(y2-y1))
  {
    Serial.print("y = mx\n");
 
   //for (int x = x1; x <= x2; x++)
   int x = x1;  
   while ((!flip_x && x <= x2) || (flip_x && x > x2))
   {
    static float _y;
    int y;
      if (x == x1)  // Plot first point manually
      {
          _y = y1;
          //Serial.print("y = ");
          //Serial.print(_y);
          //Serial.print("\n");
      } 
      else
      {
          _y += m;  // increment y by slope
          //Serial.print("y = ");
          //Serial.print(_y);
          //Serial.print(" + ");
          //Serial.print(m);
          //Serial.print("\n");
      }
      y = _y + .5;  // convert to integer (round up)
       //Serial.print("if ");
         // Serial.print(flip_y);
          //Serial.print(" && ");
          //Serial.print(y);
          //Serial.print(" > ");
          //Serial.print(y2);
          //Serial.print("\n");
      if ((!flip_y && y <= y2) || flip_y && y >= y2)
      {
          //Serial.print("plotted\n");
          //Serial.print(y);
          //Serial.print(", ");
          //Serial.print(x);
          //Serial.print(")\n");

          x_line[pointCounter] = x;
          y_line[pointCounter] = y;
    //digitalWrite(column[y][x], HIGH);

          pointCounter++;
          
          //cube[0][y][x] = 1;   // Light up y = m(1), m(2), m(3), and m(4)
      }
      else
      {
         //Serial.print(y);
          //Serial.print(", ");
          //Serial.print(x);
          //Serial.print(")\n");
      }

      if (flip_x)
          x--;
      else
          x++;
          
   }
   
  }
  
  else
  {
    Serial.print("x = y/m\n");
   int y = y1;  
   m = abs(1/m);
   /*
   if (flip_x && x1 > x2)
        m *= -1;
        */
   while ((!flip_y && y <= y2) || (flip_y && y >= y2))
   {
    static float _x;
    int x;
      if (y == y1)  // Plot first point manually
      {
          _x = x1;
          //Serial.print("x = ");
          //Serial.print(_x);
          //Serial.print("\n");
      } 
      else
      {
          _x += m;  // increment x by slope
          //Serial.print("x = ");
          //Serial.print(_x);
          //Serial.print(" + ");
          //Serial.print(m);
          //Serial.print("\n");
      }
      x = _x + .5;  // convert to integer (round up)
       /*Serial.print("if ");
        Serial.print(flip_x);
          Serial.print(" && ");
          Serial.print(x);
          Serial.print(" <= ");
          Serial.print(x2);
          Serial.print("\n");
          */
      if ((!flip_x && x <= x2) || flip_x && x >= x2)
      {
          //Serial.print("plotted: (");
          //Serial.print(y);
          //Serial.print(", ");
          //Serial.print(x);
          //Serial.print(")\n");

          x_line[pointCounter] = x;
          y_line[pointCounter] = y;
          pointCounter++;
    //digitalWrite(column[y][x], HIGH);
          //cube[0][y][x] = 1;   // Light up y = m(1), m(2), m(3), and m(4)
      }
      else
      {
          //Serial.print(y);
          //Serial.print(", ");
          //Serial.print(x);
          //Serial.print(")\n");
      }

      if (flip_y)
          y--;
      else
          y++;      
   }
  }

  // Get data for z coordinates
  static float z_float = 0;
  static int z;
  
  for (int i = 0; i < pointCounter; i++)
  {
     if (i == 0)     // First point
        z_float = z1;
     else
        z_float += z_slope/(pointCounter-1);

    z = z_float + .5;   // Round to int

    z_line[i] = z;      // Store z cord data
  }

  // Plot line
  for (int i = 0; i < pointCounter; i++)
  {
    int x, y, z;
    x = x_line[i];
    y = y_line[i];
    z = z_line[i];
    //digitalWrite(layer[z], HIGH);
    //digitalWrite(column[y][x], HIGH);
    cube[z][y][x] = 1;
    Serial.print("(");
    Serial.print(z);
    Serial.print(",");
    Serial.print(y);
    Serial.print(",");
    Serial.print(x);
    Serial.print(")\n");
  }
}
int randomNumber(int MIN, int MAX)
{
  // rand()  : Generate random number between 0 and RAND_MAX
   int n;
   n = rand()%(MAX+1) + MIN;
   //Serial.print(n);
   //Serial.print(" ");
   return n;
}

void Rain(int iterations)   // Recommended speed: 100
{
  static int count = 0;
  int _speed = 100;
  int z = SIZE-1;
  for (int i = 0; i < iterations; i++)
  {
    int d = randomNumber(0, 1);
    
    for (int k = 0; k < d; k++)
    {
       int x = randomNumber(0, 3);
       int y = randomNumber(0, 3);
       cube[z][y][x] = 1;
    }
    for (int j = 0; j < 1; j++) // Raindrop spacing (z)
    {
        delay(_speed);
        Shift_All(-1, 0, 0);
    }
  }    
}

void XY_Plane_ON(int Z) // Turn on XY Plane on Z coordinate
{
  int z = Z;
  for (int y = 0; y < SIZE; y++)
  {
    for (int x = 0; x < SIZE; x++)
    {
      cube[z][y][x] = 1; 
    }
  }
}

void XY_Plane_OFF(int Z) // Turn on XY Plane on Z coordinate
{
  int z = Z;
  for (int y = 0; y < SIZE; y++)
  {
    for (int x = 0; x < SIZE; x++)
    {
      cube[z][y][x] = 0; 
    }
  }
}

void XZ_Plane_ON(int Y) // Turn on XZ Plane on at Y coordinate
{
  int y = Y;
  for (int z = 0; z < SIZE; z++)
  {
    for (int x = 0; x < SIZE; x++)
    {
      cube[z][y][x] = 1; 
    }
  }
}

void XZ_Plane_OFF(int Y) // Turn on XZ Plane on at X coordinate
{
  int y = Y;
  for (int z = 0; z < SIZE; z++)
  {
    for (int x = 0; x < SIZE; x++)
    {
      cube[z][y][x] = 0; 
    }
  }
}

void YZ_Plane_ON(int X) // Turn on YZ Plane on at X coordinate 
{
  int x = X;
  for (int z = 0; z < SIZE; z++)
  {
    for (int y = 0; y < SIZE; y++)
    {
      cube[z][y][x] = 1; 
    }
  }
}

void YZ_Plane_OFF(int X) // Turn on XZ Plane on at X coordinate
{
  int x = X;
  for (int z = 0; z < SIZE; z++)
  {
    for (int y = 0; y < SIZE; y++)
    {
      cube[z][y][x] = 0; 
    }
  }
}

void PATTERN_Plane_Oscillate()    // movement in all planes
{
  int count = 1;
  int _speed = SPEED;
  for(int i = 0; i < count; i++)
     XY_Plane_Oscillate(_speed);
     
  for(int i = 0; i < count; i++)
     YZ_Plane_Oscillate(_speed);
     
  for(int i = 0; i < count; i++)
     XZ_Plane_Oscillate(_speed);
}

void XY_Plane_Oscillate(int _speed)  // XY plane movement back and forth
{
  XY_Plane_ON(0);
  int i = 0;
  delay(_speed);
  while (i < SIZE-1)  // 0, 1, 2, 
  {
    Shift_All_Up();
    delay(_speed);
    i++;
  }
  
  while (i >= 1)
  {
    Shift_All_Down();
    i--;
    //if (i != 0)
       delay(_speed);
  }
  XY_Plane_OFF(0);
}

void YZ_Plane_Oscillate(int _speed)  // XZ plane movement back and forth
{
  YZ_Plane_ON(0);
  int i = 0;
  delay(_speed);
  while (i < SIZE-1)
  {
    Shift_All_Right();
    delay(_speed);
    i++;
  }
  
  while (i >= 1)
  {
    Shift_All_Left();
    i--;
    //if (i != 0)
       delay(_speed);
  }

   YZ_Plane_OFF(0);
}

void XZ_Plane_Oscillate(int _speed)  // YZ plane movement back and forth
{
  XZ_Plane_ON(0);
  int i = 0;
  delay(_speed);
  while (i < SIZE-1)
  {
    Shift_All_Forward();
    delay(_speed);
    i++;
  }
  
  while (i >= 1)
  {
    Shift_All_Backward();
    i--;
    //if (i != 0)
       delay(_speed);
  }

  XZ_Plane_OFF(0);
}


void Shift_All(int z, int y, int x)     // Shift all LEDs in specified z, y, x direction
{
  if (z != 0)
  {
    // Shift z axis, z number of times
     if (z > 0)   
     {
        while (z != 0)
        {
          Shift_All_Up();
          z--;
        }
     }
     else
     {
         while (z != 0)
        {
          Shift_All_Down();
          z++;
        }
     }
  }

  
 if (y != 0)
  {
    // Shift y axis, y number of times
     if (y > 0) 
     {
        while (y != 0)
        {
          Shift_All_Forward();
          y--;
        }
     }
     else
     {
         while (y != 0)
        {
          Shift_All_Backward();
          y++;
        }
     }
  }
  
  if (x != 0)
  {
    // Shift x axis, x number of times
     if (x > 0) 
     {
        while (x != 0)
        {
          Shift_All_Right();
          x--;
        }
     }
     else
     {
         while (x != 0)
        {
          Shift_All_Left();
          x++;
        }
     }
  }
}

void Shift_All_Right()    // Shift all LEDs right by 1
{
    // z = layer    x = row     y = column
    for (int z = 0; z < SIZE; z++)
    { 
        for (int y = 0; y < SIZE; y++)
        {
            for (int x = SIZE-1; x > 0; x--)
            { 
                cube[z][y][x] = cube[z][y][x-1];
                cube[z][y][x-1] = 0;
            }
        }
    }
}

void Shift_All_Left()    // Shift all LEDs left by 1
{
    // z = layer    y = row     x = column
    for (int z = 0; z < SIZE; z++)
    { 
        for (int y = 0; y < SIZE; y++)
        {
            for (int x = 0; x < SIZE-1; x++)
            { 
                cube[z][y][x] = cube[z][y][x+1];
                cube[z][y][x+1] = 0;
            }
        }
    }
}

void Shift_All_Forward() // Shift all LEDs(+y) by 1
{
    // z = layer    y = row     x = column
     for (int z = 0; z < SIZE; z++)
    { 
        for (int x = 0; x < SIZE; x++)
        {
            for (int y = SIZE-1; y > 0; y--)
            { 
                cube[z][y][x] = cube[z][y-1][x];
                cube[z][y-1][x] = 0;
            }
        }
    }  
}

void Shift_All_Backward()// Shift all LEDs(-y) by 1
{
    // z = layer    y = row     x = column
    for (int z = 0; z < SIZE; z++)
    { 
        for (int x = 0; x < SIZE; x++)
        {
            for (int y = 0; y < SIZE-1; y++)
            { 
                cube[z][y][x] = cube[z][y+1][x];
                cube[z][y+1][x] = 0;
            }
        }
    }
}

void Shift_All_Up()// Shift all LEDs up (+z) by 1
{
    // z = layer    y = row     x = column
    
    for (int y = 0; y < SIZE; y++)
    { 
        for (int x = 0; x < SIZE; x++)
        {
             for (int z = SIZE-1; z > 0; z--)
            { 
                cube[z][y][x] = cube[z-1][y][x];
                cube[z-1][y][x] = 0;
            }
        }
    }
}

void Shift_All_Down()// Shift all LEDs down (-z) by 1
{
    // z = layer    y = row     x = column
    for (int y = 0; y < SIZE; y++)
    { 
        for (int x = 0; x < SIZE; x++)
        {
             for (int z = 0; z < SIZE-1; z++)
            { 
                cube[z][y][x] = cube[z+1][y][x];
                cube[z+1][y][x] = 0;
            }
        }
    }
}
void All_LEDs_OFF()    // Turn all LEDs OFF
{
    // z = layer    y = row     x = column
    for (int z = 0; z < SIZE; z++)
    { 
      //digitalWrite(layer[z], LOW);
        for (int y = 0; y < SIZE; y++)
        {
            for (int x = 0; x < SIZE; x++)
            {
                cube[z][y][x] = 0;
                //digitalWrite(column[y][x], LOW);
                
            }
        }
    }
}
void All_LEDs_ON()    // Turn all LEDs ON
{
    // z = layer    y = row     x = column
    for (int z = 0; z < SIZE; z++)
    { 
        for (int y = 0; y < SIZE; y++)
        {
            for (int x = 0; x < SIZE; x++)
            {
                cube[z][y][x] = 1;
            }
        }
    }
}

void Layer_ON(int L)    // Turn all LEDs on Level L on
{
    // z = layer    y = row     x = column
        for (int y = 0; y < SIZE; y++)
        {
            for (int x = 0; x < SIZE; x++)
            {
                cube[L][y][x] = 1;
            }
        }
}

void configure_Pins()
{
    // Configure output pins for 16 column
    pinMode(0, OUTPUT);
    pinMode(1, OUTPUT);
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);
    pinMode(14, OUTPUT);  // A0
    pinMode(15, OUTPUT);  // A1
  
    // Configure output pins for 4 layers
    pinMode(16, OUTPUT);  // A2
    pinMode(17, OUTPUT);  // A3
    pinMode(18, OUTPUT);  // A4
    pinMode(19, OUTPUT);  // A5
  
    // Assign pin numbers to column array
    column[0][0] = 4;    column[0][1] = 8;    column[0][2] = 17;    column[0][3] = 18;
    column[1][0] = 5;    column[1][1] = 9;    column[1][2] = 16;    column[1][3] = 19;
    column[2][0] = 6;    column[2][1] = 10;    column[2][2] = 15;    column[2][3] = 13;
    column[3][0] = 7;   column[3][1] = 11;   column[3][2] = 14;   column[3][3] = 12;
  
    // Assign pin numbers to layer array
    layer[0] = 0;  layer[1] = 1;  layer[2] = 3;  layer[3] = 2;
}

void configure_SerialCommunication()
{
  // Setup Serial communication to send messages to the console
    Serial.begin(9600);
    Serial.print("Hello World. This is the LED Cube!\n");  
}

void configure_Interrupt()
{
  // Setup Timer Interrupt
    TCCR2B = (TCCR2B & B11111000) | divisor; // Use mask so that only the least significant bits are affected
    
    // Enable timer overflow, must set the TOIE bit on TIMSK2 register
    TIMSK2 = (TIMSK2 & B11111110) | 0x01; // Use mask so that only the least significant bit is affected   
 
}

void test_each_LED()  // Test each LED
{
    // z = layer    y = row     x = column

    for (int z = 0; z < SIZE; z++)
    {
        digitalWrite(layer[z], HIGH);
        for (int y = 0; y < SIZE; y++)
            for (int x = 0; x < SIZE; x++)
            {
                digitalWrite(column[y][x], HIGH);
                delay(SPEED);
                digitalWrite(column[y][x], LOW);
            }
        digitalWrite(layer[z], LOW);
    }    
}

ISR(TIMER2_OVF_vect)
{
  // Cube refresh
   // z = layer    y = row     x = column
  static int currentLayer = 0;
  static int currentRow = 0;
  static int tempCounter = 0;   


  if (tempCounter == 0)    // 61 counts = interrupt every 1 Second
  {
        /*Serial.print("Refresh\n");
        Turn Previous Half Level Off
        Serial.print("Layer: ");
        Serial.print(currentLayer);
        Serial.print("\tRow: ");
        Serial.print(currentRow);
        Serial.print("\n");
        */
        digitalWrite(layer[currentLayer], LOW);
        for (int y = 0; y < SIZE/2; y++)  // 2 iterations (half a layer)
        {
          for (int x = 0; x < SIZE; x++)
          {          
              digitalWrite(column[currentRow+y][x], LOW);
          }
        }
      
        currentRow += 2;       // Increment row by 2, light up other half of layer
        if (currentRow >= SIZE) // if row = 4, reset back to 0 and increment layer
        {
           currentRow = 0;
           digitalWrite(layer[currentLayer], LOW);
           currentLayer++;
        }
        currentLayer = currentLayer%SIZE;   // if layer = 4, reset back to 0
      
         // Turn Next Half Level ON
         digitalWrite(layer[currentLayer], HIGH);
        for (int y = 0; y < SIZE/2; y++)  // 2 iterations (half a layer)
        {
          for (int x = 0; x < SIZE; x++)
          {
            if (cube[currentLayer][currentRow+y][x] == 1)     // If cube matrix is enabled for voxel, then turn this LED on
                 digitalWrite(column[currentRow+y][x], HIGH);
          }
        }

     tempCounter = 0;
  }
  else
    tempCounter++;
}

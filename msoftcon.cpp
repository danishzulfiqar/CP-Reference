//msoftcon.cpp
//provides routines to access Windows console functions

//compiler needs to be able to find this file
//in MCV++, /Tools/Options/Directories/Include/type path name

#include "msoftcon.h"
#include <conio.h>
HANDLE hConsole;         //console handle
char fill_char;          //character used for fill
//--------------------------------------------------------------
void init_graphics()
   {
   COORD console_size = {80, 25};
   //open i/o channel to console screen
   hConsole = CreateFile("CONOUT$", GENERIC_WRITE | GENERIC_READ,
                   FILE_SHARE_READ | FILE_SHARE_WRITE,
                   0L, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0L);
   //set to 80x25 screen size
   SetConsoleScreenBufferSize(hConsole, console_size);
   //set text to white on black
   SetConsoleTextAttribute( hConsole, (WORD)((0 << 4) | 15) );

   fill_char = '\xDB';  //default fill is solid block
   clear_screen();
   }
//--------------------------------------------------------------
void set_color(color foreground, color background)
   {
   SetConsoleTextAttribute( hConsole,
                        (WORD)((background << 4) | foreground) );
   }  //end setcolor()

/* 0  Black          8  Dark gray
   1  Dark blue      9  Blue
   2  Dark green     10 Green
   3  Dark cyan      11 Cyan
   4  Dark red       12 Red
   5  Dark magenta   13 Magenta
   6  Brown          14 Yellow
   7  Light gray     15 White
*/
//--------------------------------------------------------------
void set_cursor_pos(int x, int y)
   {
   COORD cursor_pos;              //origin in upper left corner
   cursor_pos.X = x - 1;          //Windows starts at (0, 0)
   cursor_pos.Y = y - 1;          //we start at (1, 1)
   SetConsoleCursorPosition(hConsole, cursor_pos);
   }
//--------------------------------------------------------------
void clear_screen()
   {
   set_cursor_pos(1, 25);
   for(int j=0; j<25; j++)
      putch('\n');
   set_cursor_pos(1, 1);
   }
//--------------------------------------------------------------
void wait(int milliseconds)
   {
   Sleep(milliseconds);
   }
//--------------------------------------------------------------
void clear_line()                    //clear to end of line
   {                                 //80 spaces
   //.....1234567890123456789012345678901234567890
   //.....0........1.........2.........3.........4
   _cputs("                                        ");
   _cputs("                                        ");
   }
//--------------------------------------------------------------
void draw_rectangle(int left, int top, int right, int bottom)
   {
   char temp[80];
   int width = right - left + 1;
	int j;
   for(j=0; j<width; j++)      //string of squares
      temp[j] = fill_char;
   temp[j] = 0;                    //null

   for(int y=top; y<=bottom; y++)  //stack of strings
      {
      set_cursor_pos(left, y);
      _cputs(temp);
      }
   }
//--------------------------------------------------------------
void draw_circle(int xC, int yC, int radius)
   {
   double theta, increment, xF, pi=3.14159;
   int x, xN, yN;

   increment = 0.8 / static_cast<double>(radius);
   for(theta=0; theta<=pi/2; theta+=increment)  //quarter circle
      {
      xF = radius * cos(theta);
      xN = static_cast<int>(xF * 2 / 1); //pixels not square
      yN = static_cast<int>(radius * sin(theta) + 0.5);
      x = xC-xN;
      while(x <= xC+xN)          //fill two horizontal lines
         {                       //one for each half circle
         set_cursor_pos(x,   yC-yN); putch(fill_char);  //top
         set_cursor_pos(x++, yC+yN); putch(fill_char);  //bottom
         }
      }  //end for
   }
//--------------------------------------------------------------
void draw_line(int x1, int y1, int x2, int y2)
   {

   int w, z, t, w1, w2, z1, z2;
   double xDelta=x1-x2, yDelta=y1-y2, slope;
   bool isMoreHoriz;

   if( fabs(xDelta) > fabs(yDelta) ) //more horizontal
      {
      isMoreHoriz = true;
      slope = yDelta / xDelta;
      w1=x1; z1=y1; w2=x2, z2=y2;    //w=x, z=y
      }
   else                              //more vertical
      {
      isMoreHoriz = false;
      slope = xDelta / yDelta;
      w1=y1; z1=x1; w2=y2, z2=x2;    //w=y, z=x
      }

   if(w1 > w2)                       //if backwards w
      {
      t=w1; w1=w2; w2=t;             //   swap (w1,z1)
      t=z1; z1=z2; z2=t;             //   with (w2,z2)
      }
   for(w=w1; w<=w2; w++)
      {
      z = static_cast<int>(z1 + slope * (w-w1));
      if( !(w==80 && z==25) )        //avoid scroll at 80,25
         {
         if(isMoreHoriz)
            set_cursor_pos(w, z);
         else
            set_cursor_pos(z, w);
         putch(fill_char);
         }
      }
   }
//--------------------------------------------------------------
void draw_pyramid(int x1, int y1, int height)
   {
   int x, y;
   for(y=y1; y<y1+height; y++)
      {
      int incr = y - y1;
      for(x=x1-incr; x<=x1+incr; x++)
         {
         set_cursor_pos(x, y);
         putch(fill_char);
         }
      }
   }
//--------------------------------------------------------------
void set_fill_style(fstyle fs)
   {
   switch(fs)
      {
      case SOLID_FILL:  fill_char = '\xDB'; break;
      case DARK_FILL:   fill_char = '\xB0'; break;
      case MEDIUM_FILL: fill_char = '\xB1'; break;
      case LIGHT_FILL:  fill_char = '\xB2'; break;
      case X_FILL:      fill_char = 'X';    break;
      case O_FILL:      fill_char = 'O';    break;
      }
   }
//--------------------------------------------------------------

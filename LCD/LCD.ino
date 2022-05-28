#include <SPI.h>
#include "Ucglib.h"
#include <math.h>
#define PI 3.14159265
// use st7735 chip: gnd ,vcc, scl,sda,res(oled 復位),dc, blk(背光,可不插)
Ucglib_ST7735_18x128x160_SWSPI ucg(/*sclk=*/ 22, /*data=*/ 21, /*cd=*/ 14, /*cs=*/ 15, /*reset=*/ 25);
static const unsigned char u8d_tab[3]  = { 100, 10, 1 } ;

void draw_text(void) {
  ucg.setFont(ucg_font_ncenR14_tr); 

  //X AXIS
  ucg.setRotate90();
  ucg.setPrintPos(100,120);
  ucg.print("t(s)");

  // Y AXIS
//  ucg.setRotate90();
  ucg.setPrintPos(10,20);
  ucg.print("Hz");
  ucg.undoRotate();
}


// returns FPS*10
uint16_t measure(void (*draw_fn)(void)) {
  uint16_t FPS10 = 0;
  uint32_t time;
  //10* 1000* 0.0001(s)=10(s)
  time = millis() + 5*1000;
  do {
    draw_fn();
    FPS10++;
  } while( millis() < time );


  return FPS10;  
}


//static const unsigned char u8d_tab[3]  = { 100, 10, 1 } ;
const char *u8dp(char * dest, uint8_t v)
{
  uint8_t pos;
  uint8_t d;
  uint8_t c;
  for( pos = 0; pos < 3; pos++ )
  {
      d = '0';
      c = *(u8d_tab+pos);
      while( v >= c )
      {
      v -= c;
      d++;
      }
      dest[pos] = d;
  }  
  dest[3] = '\0';
  return dest;
}

/* v = value, d = number of digits */
const char *u8d(uint8_t v, uint8_t d)
{
  static char buf[8];
  d = 3-d;
  return u8dp(buf, v) + d;
}

const char *convert_FPS(uint16_t fps) {
  static char buf[6];
  strcpy(buf, u8d( (uint8_t)(fps/10), 3));
  buf[3] =  '.';
  buf[4] = (fps % 10) + '0';
  buf[5] = '\0';
  return buf;
}

uint16_t w = 0;
uint16_t v = 0;

uint16_t x = 10;
uint16_t y =60;

void draw_line(void){
  ucg_int_t x0 ;
  ucg_int_t y0;
  // init box
  x0 = x;
  int T = 5;
  int gain = 30;
  y0 = y+gain*sin(T*x*PI/180);
  ucg.setRotate90();
  ucg.setColor(0,0,255);
  ucg.drawPixel(x0,y0);
  ucg.drawPixel(x0,y0);
  Serial.println((uint16_t)x0,(uint16_t)y0);
  x++; 

  //0.0001(s)
   delay(10);  

   if (x >=120){
      x = 10;
      y =30;
//      ucg.clearScreen();

    }
  ucg.undoRotate();
 
}

//void show_result(const char *s, uint16_t fps)  {
////  ucg.setFont(ucg_font_helvR18_tr);
////  ucg.setColor(255, 255, 255);
////  ucg.setPrintPos(0,25);
//}


void setup(void)
{
  Serial.begin(115200);
  ucg.begin(UCG_FONT_MODE_TRANSPARENT);
  ucg.clearScreen(); 


  // Frame Box: x,y,128,169
  // (10,20)
  // -----------
  // 1         1
  // 1         1         
  // ----------- (10+dx,20+dy)=(10+110,20+80)
  ucg.drawFrame(20,10,80,110);

  // Print Grid 
  //(10,40)
  // ...............
  //(10,60) .
  // ...............
  //(10,80) .    
  // ...............
  
  char *grid,*grid2;
  grid = "...............";
  grid2= "               ";
  
  ucg.setFont(ucg_font_ncenB18_tf);
  ucg.setColor(255, 255, 255);
  ucg.setRotate90();
  
  ucg.setPrintPos(10,43);
  ucg.print(grid2);
  
  ucg.setPrintPos(10,67);
  ucg.print(grid);
  
  ucg.setPrintPos(10,100);
  ucg.print(grid2);
  ucg.undoRotate();

}



void loop(void)
{  
  measure(draw_line);
//  show_result("",measure(draw_line));
}

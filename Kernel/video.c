#include "video.h"
#include "bitMap.h"


/* VBEModeInfoBlock = 0X5C00 ---> /Bootloader/Pure64/src/sysvar.asm */

static VESAInfoBlock * VBEModeInfo = (VESAInfoBlock *)0x5C00;

color_t WHITE = {.red = 0xFF, .green = 0xFF, .blue = 0xFF};
color_t RED = {.red = 0xFF, .green = 0, .blue = 0};
color_t GREEN = {.red = 0, .green = 0xFF, .blue = 0};
color_t BLUE = {.red = 0, .green = 0, .blue = 0xFF};
color_t BLACK = {.red = 0, .green = 0, .blue = 0};


position_t current_position = {0, 0};

void drawAPixelWithColour(int x, int y, color_t color) {
    char * video = (char *) ((uint64_t)(VBEModeInfo->PhysBasePtr + VBEModeInfo->pitch *y + x* (int)(VBEModeInfo->BitsPerPixel/8)));
    video[0] = color.blue;
    video[1] = color.green;
    video[2] = color.red;
}

void drawConsoleCharacter(uint8_t character){
  drawACharacter(character,WHITE,&current_position);
}

void copyPixel(int x_source, int y_source, int x_dest, int y_dest){
  char* videoSource = (char *) ((uint64_t)(VBEModeInfo->PhysBasePtr + VBEModeInfo->pitch *y_source + x_source* (int)(VBEModeInfo->BitsPerPixel/8)));
	char* videoDest = (char *) ((uint64_t)(VBEModeInfo->PhysBasePtr + VBEModeInfo->pitch *y_dest + x_dest* (int)(VBEModeInfo->BitsPerPixel/8)));
	videoDest[0] = videoSource[0];
	videoDest[1] = videoSource[1];
	videoDest[2] = videoSource[2];
}
void copyCharacter(int x_source, int y_source, int x_dest, int y_dest){
  for(int i=0;i<CHAR_HEIGHT;i++){
    for(int j=0;j<CHAR_WIDTH;j++){
      copyPixel(x_source+j,y_source+i,x_dest+j,y_dest+i);
    }
  }
}
void copyLine(int source, int dest){
  for(int i=0;i<(VBEModeInfo->XResolution)/(CHAR_WIDTH);i++){
    copyCharacter(i*CHAR_WIDTH,source,i*CHAR_WIDTH,dest);
  }
}

void clearLine(int line){
  position_t position={0,line};
  for (int j = 0; j < (VBEModeInfo->XResolution)/(VBEModeInfo->XcharSize); j++) {
    drawACharacter(' ',BLACK,&position);
  }
}

void dragScreenUp(){
  int last_line=VBEModeInfo->YResolution-CHAR_HEIGHT;
  int i=0;
  for( ;i<last_line;i+=CHAR_HEIGHT){
    copyLine(i+CHAR_HEIGHT,i);
  }
  clearLine(i);
}

void drawACharacter(uint8_t character, color_t color, position_t * position) {
	if(position->x >= VBEModeInfo->XResolution){
		position->x = 0;
		if(position->y >= VBEModeInfo->YResolution - CHAR_HEIGHT){
			dragScreenUp();
		}else
			position->y += CHAR_HEIGHT;
	}
    uint8_t * charMap = pixelMap(character);
	for(int i = 0; i < CHAR_HEIGHT; i++) {
      for(int j = 0; j < CHAR_WIDTH; j++) {

         if(1<<j & charMap[i])
              drawAPixelWithColour(CHAR_WIDTH - 1 - j + position->x, i + position->y, color);
         else
            drawAPixelWithColour(CHAR_WIDTH - 1 - j + position->x, i + position->y, BLACK);

      }
    }
    position->x += CHAR_WIDTH;
}

void drawNewLine(){
  if(current_position.y==VBEModeInfo->YResolution){     // I'm at the last line
    dragScreenUp();
    current_position.y=VBEModeInfo->YResolution-CHAR_HEIGHT;
  }
  else
    current_position.y+=CHAR_HEIGHT;

  current_position.x=0;
}

void backwards(){
  if(current_position.y != 0){

  if(current_position.x == 0){
    current_position.x = VBEModeInfo->XResolution - CHAR_WIDTH;
    current_position.y-=CHAR_HEIGHT;
  }
  else
    current_position.x -= CHAR_WIDTH;
  }
  else if(current_position.x != 0)
    current_position.x -= CHAR_WIDTH;
}

void drawTab(){
  drawConsoleCharacter(' ');
  drawConsoleCharacter(' ');
  drawConsoleCharacter(' ');
  return;
}

void clearScreen(){
  for(int i=0;i<=current_position.y;i++){
    clearLine(i);
  }
  current_position.x=0;
  current_position.y=0;
}


void drawAFigure(uint64_t x, uint64_t y){
  x=x*CHAR_WIDTH;
  y=y*CHAR_HEIGHT;
  uint8_t * charMap = square();
	for(int i = 0; i < CHAR_HEIGHT; i++) {
      for(int j = 0; j < CHAR_WIDTH; j++) {

         if(1<<j & charMap[i])
              drawAPixelWithColour(CHAR_WIDTH - 1 - j + x, i + y, RED);
         else
              drawAPixelWithColour(CHAR_WIDTH - 1 - j + x, i + y, BLACK);
      }
    }
}
void deleteAt(uint64_t x, uint64_t y){
  x=x*CHAR_WIDTH;
  y=y*CHAR_HEIGHT;
  uint8_t * charMap = square();
	for(int i = 0; i < CHAR_HEIGHT; i++) {
      for(int j = 0; j < CHAR_WIDTH; j++) {
              drawAPixelWithColour(CHAR_WIDTH - 1 - j + x, i + y, BLACK);
      }
    }
}

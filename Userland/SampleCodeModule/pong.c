#include "include/commands.h"
#include "include/pong.h"
#include "include/stdlib.h"

static const char * intromessage= "Bienvenido a pong!\n\nINSTRUCCIONES:\n\nW: mueve hacia arriba jugador izquierdo\nZ: mueve hacia abajo jugador izquierdo\nI: mueve hacia arriba jugador derecho\nM: mueve hacia abajo jugador derecho\nF: finaliza el juego\n\nPRESIONE ENTER PARA CONTINUAR";



//Game
int ticks_start;
int ticks_end;
static int running=1;
static int direction=RIGHT;
static int ballX=WIDTH/2;
static int ballY=HEIGHT/2;

//Player 1
static int leftPlayerX=0;
static int leftPlayerY=HEIGHT/2;
static int leftScore=0;

//Player 2
static int rightPlayerX=127;
static int rightPlayerY=HEIGHT/2;
static int rightScore=0;

void hold(int ticks){
  int80(7,(uint64_t)&ticks_start,0,0,0,0);
  ticks_end=ticks_start;
  while(ticks_end<ticks_start+ticks){
    int80(7,(uint64_t)&ticks_end,0,0,0,0);
  }
}

void modifyBall(int x, int y, int action){
   int80(6,x,y,action,0,0);
}
void insertBall(int x, int y){
    modifyBall(x,y,1);
}
void removeBallFrom(int x, int y){
    modifyBall(x,y,0);
}
void modifyPlayer(int x,int y, int action){
    int80(6,x,y,action,0,0);
    for(int i=0;i<(RACKET_SIZE)/2;i++){
      int80(6,x,y+i,action,0,0);
      int80(6,x,y-i,action,0,0);
  }
}
void insertPlayer(int x, int y){
    modifyPlayer(x,y,1);
}
void removePlayerFrom(int x, int y){
    modifyPlayer(x,y,0);
}
void moveBall(int direction){
  removeBallFrom(ballX,ballY);
  if(direction==RIGHT)
      insertBall(++ballX,ballY);
  else if(direction==LEFT)
      insertBall(--ballX,ballY);
  else if(direction==UPRIGHT)
      insertBall(++ballX,--ballY);
  else if(direction==DOWNRIGHT)
      insertBall(++ballX,++ballY);
  else if(direction==UPLEFT)
      insertBall(--ballX,--ballY);
  else if(direction==DOWNLEFT)
      insertBall(--ballX,++ballY);
  else if(direction==UP)
        insertBall(ballX,--ballY);
  else if(direction==DOWN)
      insertBall(ballX,++ballY);
  return;
}
void moveRightPlayer(int direction){
  removePlayerFrom(rightPlayerX,rightPlayerY);
  if(direction==RUP)
    insertPlayer(rightPlayerX,--rightPlayerY);
  else if(direction==RDOWN)
    insertPlayer(rightPlayerX, ++rightPlayerY);
}
void moveLeftPlayer(int direction){
  removePlayerFrom(leftPlayerX,leftPlayerY);
  if(direction==LUP)
    insertPlayer(leftPlayerX,--leftPlayerY);
  else if(direction==LDOWN)
    insertPlayer(leftPlayerX, ++leftPlayerY);
}


void startBall(){
    insertBall(WIDTH/2,HEIGHT/2); // Paints ball at the center
}
void startPlayers(){
    insertPlayer(0,HEIGHT/2);
    insertPlayer(127,HEIGHT/2);
}

void checkEndGame(){
  if(leftScore==3 || rightScore==3)
    running=0;
}


/* El tablero se divide en 8 secciones y dependiendo de donde choco, es como se modifica la direction
*/
void checkDirection(){
  if(ballX==0){
    if(ballY>=0 || ballY<=15)
      direction=DOWNRIGHT;
    if(ballY<=31)
      direction=RIGHT;
    if(ballY<=47)
      direction=UPRIGHT;
  }
  if(ballX==126){
    if(ballY>=0 || ballY<=15)
      direction=DOWNLEFT;
    if(ballY<=31)
      direction=LEFT;
    if(ballY<=47)
      direction=UPLEFT;
  }
  if(ballY==0){
    if(ballX>=0 || ballX<=42)
      direction=DOWNRIGHT;
    if(ballX<=84)
      direction=DOWN;
    if(ballX<=127)
      direction=DOWNLEFT;
  }
  if(ballY==46){
    if(ballX>=0 && ballX<=42)
      direction=UPRIGHT;
    if(ballX<=84)
      direction=UP;
    if(ballX<=127)
      direction=UPLEFT;
  }
}

void moveRacket(){
  char c=getChar();
  if(c==ENDGAME){
    running=0;
    return;
  }
  if(c==LUP || c==LDOWN){
    moveLeftPlayer(c);
  }
  else if(c==RUP || c==RDOWN){
    moveRightPlayer(c);
  }
}

void checkBounce(){
  if(ballX==1){
    if(ballY>=leftPlayerY-3 && ballY<=leftPlayerY-2)
      direction=UPRIGHT;
    if(ballY>=leftPlayerX-1 && ballY<=leftPlayerY+1)
      direction=RIGHT;
    if(ballY<=leftPlayerY+3)
      direction=DOWNRIGHT;
  }
  if(ballX==126){
    if(ballY>=rightPlayerY-3 && ballY<=rightPlayerY-2)
      direction=UPLEFT;
    if(ballY>=rightPlayerX-1 && ballY<=rightPlayerY+1)
      direction=LEFT;
    if(ballY<=rightPlayerY+3)
      direction=DOWNLEFT;
  }
}

void beep(){
//  int80(8,0,0,0,0,0);
//  hold(4);
//  int80(9,0,0,0,0,0);
}

void checkGol(){
  if(ballX==1 && (ballY<leftPlayerY-3 || ballY>leftPlayerY+3)){
    leftScore+=1;
    beep();
  }
  else if(ballX==46 && (ballY<rightPlayerY-3 || ballY>rightPlayerY+3)){
    rightScore+=1;
    beep();
  }
}

void clearPong(){
  for(int i=0;i<WIDTH;i++){ //x
    for(int j=0;j<HEIGHT;j++) //y
      int80(6,i,j,0,0,0);
  }
}

void restartPong(){
  running=1;
  direction=RIGHT;
  ballX=WIDTH/2;
  ballY=HEIGHT/2;

  //Player 1
  leftPlayerX=0;
  leftPlayerY=HEIGHT/2;
  leftScore=0;

  //Player 2
  rightPlayerX=127;
  rightPlayerY=HEIGHT/2;
  rightScore=0;

}

void pong(){
  clear();
  printf(intromessage);
  char ans=0;
  while((ans=getChar())!=ENTER)
    ;
  clear();
  startBall();
  startPlayers();
  while(running){
    moveRacket();
    checkBounce();
    checkDirection();
    checkGol();
    hold(1);
    moveBall(direction);
    checkEndGame();
  }
  if(leftScore==3)
  printf("\n\n\t\tLEFT PLAYER WON!!!\n");
  else if(rightScore==3)
  printf("\n\n\t\tRIGHT PLAYER WON!!!\n");
  hold(24);
  restartPong();
  clearPong();
}

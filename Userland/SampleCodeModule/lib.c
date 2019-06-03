#include <stdarg.h>
#include <stdint.h>

#define TRUE 1
#define FALSE 0

//Constantes para printf
#define SPECIAL 1
#define NORMAL 0

extern uint64_t int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

void read(int filed, char * buffer, int size, uint8_t callingPID){
  int80(3,filed,(uint64_t)buffer,(uint64_t)size,(uint64_t)callingPID,0);
}

void write(int filed, char * buffer, int size, uint8_t otherPID){
  int80(4,filed,(uint64_t)buffer,(uint64_t)size,(uint64_t)otherPID,0);
}

void strncpy(char * destination, const char * source, int length){
	int i=0;
	for(; i<=length && source[i]!=0 ;i++){
		destination[i]=source[i];
	}
	destination[i]='\0';
}

int strcmp(const char * str1, const char * str2){
	int i=0;
	for(;str1[i]!='\0' && str2[i]!='\0';i++){
		if(str1[i]!=str2[i])
			return FALSE;
	}
	if(str2[i]=='\0' && str1[i]=='\0')
		return TRUE;
	else
		return FALSE;
}

char getChar(){
  char buf[1];
  read(0,buf,1,0);
  return (char)*buf;
}

void putChar(char c){
  write(1,&c,1,0);
  return;
}

int strlen(const char* str){
	int i=0;
	for (; str[i]!='\0'; i++)
		;
	return i;
}

void swap(char * str, int index1, int index2){ //swapea dos valores en un string
  char aux = str[index1];
  str[index1] = str[index2];
  str[index2] = aux;
}

char * intToString(int num, char * str){
  int i =0, j;
  if (num != 0){
    if (num< 0){
      str[i++] = '-'; //negativo
    }
    for(;num >0; i++){
        str[i]=num%10 + '0'; //caracter correspondiente al numero
        num = num/10;
    }
    for (j =0; j< i/2; j++){ //No es necesario recorrerlo completo
      swap(str, j, i-j-1); //lo doy vuelta porque lo va agregando al reves
    }
    str[i] = 0; //caracter de finalizacion
  }else{
    str[0] = '0';
		str[1] = '\0'; //caracter de finalizacion
  }
  return str;
}
int upcmp(const char * str){
  if(strlen(str)<4)
    return FALSE;
  if(str[0] == 'u' && str[1] == 'p' && str[2] == ' ')
    return TRUE;
  return FALSE;
}
int downcmp(const char * str){
  if(strlen(str)<6)
    return FALSE;
  if(str[0] == 'd' && str[1] == 'o' && str[2] == 'w'&& str[3] == 'n'&& str[4] == ' ')
    return TRUE;
  return FALSE;
}
int numUp(const char * str){
  int num = str[3] - '0';
  if(str[4]==0)
    return num;
  num*=10;
  num+=str[4] - '0';
  return num;
}
int numDown(const char * str){
  int num = str[5] - '0';
  if(str[6]==0)
    return num;
  num*=10;
  num+=str[6] - '0';
  return num;
}
void printf(const char * str, ...){
  char num[12];
  va_list argsList;
  va_start(argsList, str);
  int length = strlen(str);
  // State = 0 -> Caracter normal
  // state = 1 -> caracter con %
  int state = NORMAL; //Caracteres normales son aquellos distintos de %
  for (int i = 0; i< length; i++){
    if (state == NORMAL){
      if (str[i] == '%'){
        state = SPECIAL;
      }else{
          putChar(str[i]);
      }
    }else{
      if (str[i] == 'd'){
        intToString(va_arg(argsList, int), num); //paso a string el parametro correspondiente a ese enter
        printf(num); //llamada a printf desde printf pero en este caso no se entran a los caracteres especiales
      }else if(str[i] == 's'){
        printf(va_arg(argsList, char*)); //imprime string
      }else if(str[i] == 'c'){
        putChar((char)va_arg(argsList, int)); //putchar
      }else{
        putChar('%'); //no correspondia a un argumento
        putChar(str[i]);
      }
			state=NORMAL;
    }
  }
  va_end(argsList); //finalizacion de la funcion
}

void * malloc(uint64_t bytes){
  void * address;
  int80(10,bytes,(uint64_t)&address,0,0,0);
  return address;
}

void free(void * pointer){
  int80(11,(uint64_t)pointer,0,0,0,0);
}

void printOccupiedMemory(){
  int80(34,0,0,0,0,0);
}

void exec(char * description, int priority, uint64_t functionPointer, uint8_t * pid, uint8_t redirects){
  int80(12,(uint64_t)description,(uint64_t)priority,functionPointer,(uint64_t)pid,(uint64_t)redirects);
}

uint64_t shmCreate(uint8_t id){
  uint64_t address=0;
  int80(15,(uint64_t)id, (uint64_t)(&address), 0, 0, 0);
  return address;
}

uint64_t shmOpen(uint8_t id){
  uint64_t address=0;
  int80(16,(uint64_t)id, (uint64_t)(&address), 0, 0, 0);
  return address;
}

void shmClose(uint8_t id){
  int80(17,(uint64_t)id, 0, 0, 0, 0);
}

void block(uint8_t pid){
  int80(18,(uint64_t)pid, 0, 0, 0, 0);
}

void unblock(uint8_t pid){
  int80(19,(uint64_t)pid, 0, 0, 0, 0);
}

void initMutex(uint8_t * mutexID, uint8_t pid){
  int80(20,(uint64_t)(&mutexID), (uint64_t)pid, 0, 0, 0);
}

void destroyMutex(uint8_t mutexID){
  int80(21,(uint64_t)mutexID, 0, 0, 0, 0);
}

void mutexLock(uint8_t mutexID, uint8_t callingPID){
  int80(22,(uint64_t)mutexID, (uint64_t)callingPID, 0, 0, 0);
}

void mutexUnlock(uint8_t mutexID){
  int80(23,(uint64_t)mutexID, 0, 0, 0, 0);
}

// Stores in filed the file descriptor I need
void createPipe(uint8_t id, uint8_t * filed){
  int80(24, (uint64_t)id, (uint64_t)filed, 0, 0, 0);
}

// Stores in filed the file descriptor I need
void openPipe(uint8_t id, uint8_t * filed){
  int80(24, (uint64_t)id, (uint64_t)filed, 0, 0, 0);
}

void closePipe(uint8_t id){
  int80(25, (uint64_t)id, 0, 0, 0, 0);
}

// Stores in pid what I need
void getPID(char * description, uint8_t * pid){
  int80(26, (uint64_t)description, (uint64_t)pid, 0, 0, 0);
}

void connectMutex(uint8_t mutexID,uint8_t pid){
  int80(27,(uint64_t)mutexID,(uint64_t)pid,0,0,0);
}

void checkSides(uint64_t mutexID, uint8_t callingPID){
  int80(28, (uint64_t) mutexID, (uint64_t)callingPID, 0,0,0);
}

void changeState(uint64_t mutexID, uint8_t callingPID, uint8_t state){
  int80(29, (uint64_t) mutexID, (uint64_t)callingPID, (uint64_t)state,0,0);
}

void mutexRemove(uint64_t mutexID, uint8_t callingPID){
  int80(30, (uint64_t) mutexID, (uint64_t)callingPID, 0,0,0);
}

void circleTesting(uint64_t mutexID){
  int80(31, (uint64_t) mutexID, 0, 0,0,0);
}
void up(uint8_t pid){
  int80(32,(uint64_t)pid,0,0,0,0);
}
void down(uint8_t pid){
  int80(33,(uint64_t)pid,0,0,0,0);
}

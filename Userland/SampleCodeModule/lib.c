#include <stdarg.h>
#include <stdint.h>

#define TRUE 1
#define FALSE 0

//Constantes para printf
#define SPECIAL 1
#define NORMAL 0

extern uint64_t int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

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
  int80(3,1,(uint64_t)buf,1,0,0);
  return (char)*buf;
}

void putChar(char c){
  int80(4,1,(uint64_t)&c,1,0,0);
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

void exec(char * description, int priority, uint64_t functionPointer, uint8_t * pid){
  int80(12,(uint64_t)description,(uint64_t)priority,functionPointer,0,0);
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

uint8_t initMutex(){
  uint8_t mutexID=0;
  int80(20,(uint64_t)(&mutexID), 0, 0, 0, 0);
  return mutexID;
}

void destroyMutex(uint8_t mutexID){
  int80(21,(uint64_t)mutexID, 0, 0, 0, 0);
}

void mutexLock(uint8_t mutexID, uint8_t callingPID){
  int80(22,(uint64_t)mutexID, (uint64_t)callingPID, 0, 0, 0);
}

void mutexUnlock(uint8_t mutexID, uint8_t otherPID){
  int80(23,(uint64_t)mutexID, (uint64_t)otherPID, 0, 0, 0);
}
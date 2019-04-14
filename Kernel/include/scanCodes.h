/* Scancodes para el driver de teclado.
Código tomado de un scancode realizado por un alumno de otro año.
*/

#ifndef SCANCODES_H_
#define SCANCODES_H_


const char ASCII_VALUE[]=
        //    0  1  2   3   4   5   6   7   8   9   A   B   C   D   E   F
        {     0,27 ,'1','2','3','4','5','6','7','8','9','0','\'','¿','\b',
           '\t','q','w','e','r','t','y','u','i','o','p',96,'+','\n',/*CTRL*/0,'a','s',
           'd','f','g','h','j','k','l'        ,'ñ','{'      ,'|',/*LSHIFT*/0,'}','z' ,'x'      ,'c' ,'v',
           'b','n','m',',','.','-',/*RSHIFT*/0,'*',/*LALT*/0,' '};

const char SHIFT_ASCII_VALUE[]=
        //    0  1  2   3   4   5   6   7   8   9   A   B   C   D   E   F
        {    0,27,'!','\"','#','$','%','&','/','(',')','=','?','¡','\b',
            '\t','Q','W','E','R','T','Y','U','I','O','P','¨','*','\n',
            /*CTRL*/0,'A','S','D','F','G','H','J','K','L','Ñ','[','°',/*LSHIFT*/0,']','Z','X','C','V',
             'B','N','M',';',':','_',/*RSHIFT*/0,'*',/*LALT*/0,' '};


#endif

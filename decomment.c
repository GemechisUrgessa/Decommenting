/**
 * @filename: decomment.c
 *
 * @Description: program that will implement a subset of
 * the De‑commenting job of the C preprocessor.
 * specifically given a C source code the decomment program performs
 * the following:
 * replace each single-line comment with a space,
 * define "comment" as in the C90 standard,
 * allow a comment to span multiple lines,
 * not recognize nested comments,
 * handle C string literals and character literals,
 * handle newline characters in C string literals
 * without generating errors or warnings,
 * handle unterminated string and character
 * literals without generating errors or warnings,
 * detect an unterminated comment.
 *
 * @Author: Gemechis Urgessa
 */
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include <unistd.h>
#include <stdlib.h>

/* we use enum STATE to define our list of states and intialize it to state_start. enum Valid_state to list accepting and rejecting states and intialize it to accepting.we also use Line_count to count number of new lines till comment starts. */
enum STATE {state_start, state_comnt_start , state_in_comnt, state_comnt_end ,state_single_quote, state_double_quote,state_escape};
enum VALID_SATE {accepting, rejecting};

enum VALID_SATE valid_state = accepting;
enum STATE state = state_start;
int line_count = 1;
/* here we goes our function declaretions. */
enum STATE State_start(int c);
enum STATE State_comnt_start(int c);
enum STATE State_in_comnt(int c);
enum STATE State_comnt_end(int c);
enum STATE State_single_quote(int c);
enum STATE State_double_quote(int c);
enum STATE State_escape(int c);

/* our main function reads character and using the states mnaages wether t print the character or decomment it. */
int main(void){
  int c;
  while ( (c =getchar()) != EOF ){
    switch(state) {

    case(state_start):
      state = State_start(c);
      break;
    case(state_comnt_start):
      state = State_comnt_start(c);
      break;
    case(state_in_comnt):
      state = State_in_comnt(c);
      break;
    case(state_comnt_end):
      state = State_comnt_end(c);
      break;
    case(state_single_quote):
      state = State_single_quote(c);
      break;
    case(state_double_quote):
      state = State_double_quote(c);
      break;
    case(state_escape):
      state = State_escape(c);
      break;
      
    }
    
  }
  if (state == state_in_comnt){
    putchar('/');
  }
  if (valid_state == rejecting){
    fprintf(stderr,"Error: line %d: unterminated comment\n",line_count);
    exit(EXIT_FAILURE);
  }
  else {
    exit(EXIT_SUCCESS);
  }
  
  return 0;
  
}

/* our inital state which prints charcters other than '/', "'" and '"' 
and calls state_comnt_start, state_single_quote,state_double_quote based on the states.*/

enum STATE State_start(int c){
  if ((char)c == '\n'){
    line_count+=1;
  }
  
  if ((char)c == '/'){
    state = state_comnt_start;
  }
  
  else if( (char)c == '\'') {
    putchar(c);
    state = state_single_quote;
  }
  else if ((char)c == '"'){
    putchar(c);
    state = state_double_quote;
  }
     
  else{
     putchar(c);
     state = state_start;
   } 
  
  return state;
  
}

/* our state_comnt_start which prints charcters '/' if the next character is different from '*'
and calls state_in_comnt if it gets '*'.*/
enum STATE State_comnt_start(int c){

  if ((char)c == '\n'){
    line_count+=1;
  }
  
  if ((char)c == '/' ) {
    putchar(c);

    state = state_comnt_start;
  }
  
  else if((char)c == '*'){
    valid_state = rejecting;
    state = state_in_comnt;
  }
  
  else {
    putchar('/');
    putchar(c);
    state = state_start;
  }
  
  return state;
  
}

/* our state_in_comnt which loops till it gets '*' 
and calls state_comnt_end when it gets '*'.*/

enum STATE State_in_comnt(int c){


  if((char)c == '*') {
  
    state = state_comnt_end;
  }
  else if ((char)c == '\n' ) {
    putchar('\n');
  }

  else {
    state = state_in_comnt;
  }

  return state;
}
enum STATE State_comnt_end(int c){

  if ((char)c == '/') {
    valid_state = accepting;
    putchar(' ');
    state = state_start;
  }

  else {
    state = state_in_comnt;
  }

  return state;
}

/* our state_single_quote which loops till it gets '"' or "'" 
and calls state_double_quote,state_start or it selfe when it gets one of them.*/
enum STATE State_single_quote(int c) {

  if ((char)c == '\n'){
    line_count+=1;
  }

  if ((char)c == '\'') {
    putchar(c);
    state = state_start;
  }

  else if ((char)c == '\\'){
    putchar('\\');
    state = state_escape;
  }
  else{
    putchar(c);
    state = state_single_quote;
  }
  return state;
  
}

/* our state_single_quote which loops till it gets '"' or "'" 
and calls state_double_quote ,it selfe or state_start when it gets one of them.*/

enum STATE State_double_quote(int c) {

  if ((char)c == '\n'){
    line_count+=1;
  }
  if ((char)c == '"') {
    putchar(c);
    state = state_start;
  }

  else{
    putchar(c);
    state = state_double_quote;
  }
  return state;
  
}

enum STATE State_escape(int c) {


    putchar(c);
    state = state_single_quote;
 

  
  return state;
  
}



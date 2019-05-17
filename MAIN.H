// ======================================================================
// File: user/typing-practice/main.h
// ======================================================================

#ifndef _MAIN_H_
#define _MAIN_H_


#define NO_OF_THREAD 5 
#define THREAD_STACK_SIZE 256 

#define CORRECT 0
#define FAIL 1 
#define stage_number 3
struct voca_info{
  int x_start;
  int y_start;
  int voca_number;
  int count;
};

void dic_master(void);
void clear(int x,int y);
#endif

// ======================================================================

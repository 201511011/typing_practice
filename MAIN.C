// ======================================================================
// File: user/user-template/main.c
// ======================================================================

#include "stdio.h"
#include "main.h"

// ======================================================================
// Main thread.

unsigned int dic_master_thread_stack[5][THREAD_STACK_SIZE];
unsigned int input_handler_thread_stack [THREAD_STACK_SIZE];
int level,dic_num,score,fail;
int x=23;
int i=0,k=0,count;
struct voca_info voca[5];
char input[15]="\0";
char output[15]="\0";
char *dic[5][7]={
{"apple","banana","cherry","orange","mango","plums","lemon"},
{"actor","artist","chef","nurse","doctor","police","carpenter"},
{"carrot","onion","radish","celery","pea","lettuce","zucchini"},
{"gray","black","red","emerald","blue","fuchsia","beige"},
{"vote","press","law","rib","state","society","castle"}
};
// ======================================================================
int main(void)
{
  int mutex_print=mutex_create();
  int c;
  char cc;
  SYS_STAT stat;
  score=0;
  dic_num=0;
  fail=0;
  int i=0;
  for(i=0;i<5;i++){
    voca[i].x_start=(i)*10;
    voca[i].y_start=4;
    voca[i].voca_number=i;
    voca[i].count=0;
  }
   
//Clear screen
  mutex_wait(mutex_print,0);
  level=1;
//clear:
  putclear();

  putgoto(0,0);  
  printf("CM-Kernel:Typing Practice ");
  mutex_release(mutex_print);
  putgoto(0,3);
  printf("==================================================");
  putgoto(0,15);
  printf("==================================================");
  putgoto(16,16);
  printf("Input :");
//Thread create 
  for(i=0;i<3;i++){
    thread_create(dic_master,&dic_master_thread_stack[i][THREAD_STACK_SIZE],7);	
  }
//Display stat information 
 x=23;
 k=0;

putgoto(23,16);
for(;;){
  if(x==23){
    k=0;
  }
  putgoto(x,16);
  c=getc_nb();
  mutex_wait(mutex_print,0);
  if(c!=EOF){
     cc=(char)c;
     printf("%c",cc);
//input over 
    if(cc=='\r'||cc=='\n'){
      x=23;
      for(i=0;i<k;i++) {
	output[i]=input[i];
      }
      output[k]=cc;
      count=k;
      input[k]=cc; 
      putgoto(23,16);
      printf("                     ");	
      putgoto(23,16);
    }
//still input
    else {
    	input[k]=cc;
	k++;
	x++;
    }
  }
  mutex_release(mutex_print);  
  putgoto(0,2);
  get_stat(&stat);
  printf("Ticks=%d Level=%d Score=%2d Fail=%2d",stat.os_ticks,level,score,fail);
  if(fail>10){
    putgoto(40,2);
    putcolor(ANSI_COLOR_RED);	
    printf("--> Game over       ");
    putcolor(ANSI_COLOR_BLACK);	
    thread_exit();
  }
  if(score>=10&&level==1){
    thread_create(dic_master,&dic_master_thread_stack[3][THREAD_STACK_SIZE],7);	
    level=2;
    score=0;
    fail=0;
    putgoto(40,2);
    putcolor(ANSI_COLOR_GREEN);	
    printf("--> Level Up 2");
    putcolor(ANSI_COLOR_BLACK);	
  }
  if(score>=20&&level==2){
    thread_create(dic_master,&dic_master_thread_stack[4][THREAD_STACK_SIZE],7);	
    level=3;
    score=0;
    fail=0;
    putgoto(40,2);
    putcolor(ANSI_COLOR_GREEN);	
    printf("--> Level Up 3");
    putcolor(ANSI_COLOR_BLACK);
   }
   mutex_release(mutex_print);
  thread_delay(100);
  }  
  
  return 0;
}

// ======================================================================
void dic_master(void){
int q=0;
int num=dic_num;
int fcount=0;
int i=voca[num].count;
dic_num++;       
int x=voca[num].x_start;
int y=voca[num].y_start;
  for(;;){
//Correct 

    for(q=0;*(dic[num][i]+q)!='\0';q++){
      if(output[q]!=*(dic[num][i]+q)){
	    goto aaa;
          }				
     }
	  clear(x,y);
  	  i=(i+1)%7;
	  y=4;
	  score++;
          goto bbb;
	
//Wrong     
aaa:
        clear(x,y);
	putgoto(x,y);
        puts(dic[num][i]);
	//Down
	if(y<14){
	  y++;
	  if(fcount==1&&y==5){ 
	    fcount=0;
	    fail++;
	    
	      if(fail>=10)	thread_exit(); 	
	  }
	}
	//Over 
        else {
 	  y=4;
	  i=(i+1)%7;
	  fcount=1;
          if(fail>=10)	thread_exit();   		  	
	}
bbb:
	thread_delay(1000);
  }
 
}
// ====================================================================
void clear(int x, int y){
int fx=x;
int fy=y;
  if(y==4) fy=15;
  putgoto(fx,fy-1);
  printf("         ");  

}

#include "mapImp.h"

void move(){

    CELL nextCell;
    int moves[4]={0,0,0,0}; //hard coded for fleet number
    XY a0[4];
    //temporary hard coded solution for a fleet of 4 aedv's
    //Entire path finding algorithm requires reworking
    while(STOP != 2){
        a0[0].x = fleet[0].x;
        a0[0].y = fleet[0].y;
        a0[1].x = fleet[1].x;
        a0[1].y = fleet[1].y;
        a0[2].x = fleet[2].x;
        a0[2].y = fleet[2].y;
        a0[3].x = fleet[3].x;
        a0[3].y = fleet[3].y;
        
        for(int i = 0; i < 4; i++){
            if(fleet[i].x != fleet[i].destx){
                if(roads[fleet[i].x][fleet[i].y].next1){
                    nextCell = (*roads[fleet[i].x][fleet[i].y].next1);
                    if(abs(fleet[i].destx - nextCell.x) < abs(fleet[i].destx - fleet[i].x)){
                        fleet[i].x = nextCell.x;
                        moves[i] = 1;
                    }  
                }
                if(roads[fleet[i].x][fleet[i].y].next2){
                    nextCell = (*roads[fleet[i].x][fleet[i].y].next2);
                    if(abs(fleet[i].destx - nextCell.x) < abs(fleet[i].destx - fleet[i].x)){
                        fleet[i].x = nextCell.x;
                        moves[i] = 2;
                    }
                }
            }
            else if(fleet[i].y != fleet[i].desty){
                if(roads[fleet[i].x][fleet[i].y].next3){
                    nextCell = (*roads[fleet[i].x][fleet[i].y].next3);
                    if(abs(fleet[i].desty - nextCell.y) < abs(fleet[i].desty - fleet[i].y)){
                        fleet[i].y = nextCell.y;
                        moves[i] = 3;
                    }
                }
                if(roads[fleet[i].x][fleet[i].y].next4){
                    nextCell = (*roads[fleet[i].x][fleet[i].y].next4);
                    if(abs(fleet[i].desty - nextCell.y) < abs(fleet[i].desty - fleet[i].y)){
                        fleet[i].y = nextCell.y;
                        moves[i] = 4;
                    }  
                }
            }
            if (_kbhit())
			    STOP = check_kb();
        }
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                if(moves[j] == 1){
                    CUP(a0[j].x*4+1-i,a0[j].y*4+1)
                    putchar('@');
                    Sleep(50);
                    CUP(a0[j].x*4+1-i,a0[j].y*4+1)
                    putchar(' ');
                }
                else if(moves[j] == 2){
                    CUP(a0[j].x*4+1+i,a0[j].y*4+1)
                    putchar('@');
                    Sleep(25);
                    CUP(a0[j].x*4+1+i,a0[j].y*4+1)
                    putchar(' ');
                }
                else if(moves[j] == 3){
                    CUP(a0[j].x*4+1,a0[j].y*4+1-i)
                    putchar('@');
                    Sleep(25);
                    CUP(a0[j].x*4+1,a0[j].y*4+1-i)
                    putchar(' ');
                }
                else if(moves[j] == 4){
                    CUP(a0[j].x*4+1,a0[j].y*4+1+i)
                    putchar('@');
                    Sleep(25);
                    CUP(a0[j].x*4+1,a0[j].y*4+1+i)
                    putchar(' ');
                }
                else{
                    CUP(a0[j].x*4+1,a0[j].y*4+1)
                    putchar('@');
                    Sleep(25);
                }
            
            } 
        }
        moves[0] = 0;
        moves[1] = 0;
        moves[2] = 0;
        moves[3] = 0;
    }
    
}
#include "mapImp.h"

void move(){

    CELL nextCell;
    int moves[fleetSize]; 
    XY a0[fleetSize];

    while(STOP != 2){
        for(int i = 0; i < fleetSize; i++){
            a0[i].x = fleet[i].x;
            a0[i].y = fleet[i].y;
        }
        
        for(int i = 0; i < fleetSize; i++){
            fleet[i].state = 0;

            if(fleet[i].x != fleet[i].destx){
                fleet[i].state = 1;
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
                fleet[i].state = 1;
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
        }
        if (_kbhit())
			    check_kb();

        for(int i = 0; i < fleetSize; i++){
            if(fleet[i].state == 1)
                fleet[i].battery--; //decrement battery level if AEDV has moved

            for(int j = 0; j < fleetSize; j++){
                if(moves[j] == 1){
                    CUP(a0[j].x*4+1-i,a0[j].y*4+1)
                    putchar('@');
                    Sleep(25);
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
        CUP(4, scr_size.Y-2)
        printf("Current time: %.2f", TIME);
        TIME++;
        for(int i = 0; i < fleetSize; i++){
            moves[i] = 0;
        }

    }
    
}
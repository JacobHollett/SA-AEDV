#include "mapImp.h"

void move(){

    CELL nextCell, nextNextCell; //placeholder next cell to compare coordinates against
    int moves[fleetSize]; //records direction of movement
    XY a0[fleetSize]; //saved previous positions for displaying movement
    

    while(STOP != 2){

        for(int i = 0; i < fleetSize; i++){
            a0[i].x = fleet[i].x;
            a0[i].y = fleet[i].y;
        }

        for(int i = 0; i < fleetSize; i++){
            if(fleet[i].x != fleet[i].destx || fleet[i].y != fleet[i].desty){
                fleet[i].state = 1;
                moves[i] = fleet[i].path[fleet[i].pathStep];
                switch(moves[i]){
                    case 1:
                        fleet[i].x--;
                        break;
                    case 2:
                        fleet[i].x++;
                        break;
                    case 3:
                        fleet[i].y--;
                        break;
                    case 4:
                        fleet[i].y++;
                        break;
                }
                fleet[i].pathStep++;
            }
            else{
                fleet[i].state = 0;    //if we are at our destination reset our path inex and go inactive
                fleet[i].pathStep = 0;
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


void find_path(int k){  //simple pathfinding algorithm

    int parent = 0; //keeping track of parent nodes so that we don't return to them
    int i = 0; //index of move in path
    int j = 0; //direction of chosen move (1,2,3,4)
    int z = BIGNUM; //lowest f value for comparison
    int g; //distance between cell and starting cell
    int h; //square of distance between cell and destination cell
    int f[4]; //list of possible
    CELL currentCell = roads[fleet[k].x][fleet[k].y];
    CELL nextCell;
    while(currentCell.x != fleet[k].destx || currentCell.y != fleet[k].desty){
        if(currentCell.next1 && parent != 2){
            nextCell = *currentCell.next1;
            g = abs(nextCell.x-fleet[k].x)+abs(nextCell.y-fleet[k].y);
            h = abs(fleet[k].destx - nextCell.x)*abs(fleet[k].destx - nextCell.x) + 
                abs(fleet[k].desty - nextCell.y)*abs(fleet[k].desty - nextCell.y);
            f[0] = g+h;
        }else{f[0] = BIGNUM;} //Arbitrarily high number so as not to choose this direction

        if(currentCell.next2 && parent != 1){
            nextCell = *currentCell.next2;
            g = abs(nextCell.x-fleet[k].x)+abs(nextCell.y-fleet[k].y);
            h = abs(fleet[k].destx - nextCell.x)*abs(fleet[k].destx - nextCell.x) + 
                abs(fleet[k].desty - nextCell.y)*abs(fleet[k].desty - nextCell.y);
            f[1] = g+h;
        }else{f[1] = BIGNUM;}

        if(currentCell.next3 && parent != 4){
            nextCell = *currentCell.next3;
            g = abs(nextCell.x-fleet[k].x)+abs(nextCell.y-fleet[k].y);
            h = abs(fleet[k].destx - nextCell.x)*abs(fleet[k].destx - nextCell.x) + 
                abs(fleet[k].desty - nextCell.y)*abs(fleet[k].desty - nextCell.y);
            f[2] = g+h;
        }else{f[2] = BIGNUM;}

        if(currentCell.next4 && parent != 3){
            nextCell = *currentCell.next4;
            g = abs(nextCell.x-fleet[k].x)+abs(nextCell.y-fleet[k].y);
            h = abs(fleet[k].destx - nextCell.x)*abs(fleet[k].destx - nextCell.x) + 
                abs(fleet[k].desty - nextCell.y)*abs(fleet[k].desty - nextCell.y);
            f[3] = g+h;
        }else{f[3] = BIGNUM;}

        for(int l = 0; l < 4; l++){
            
            if(f[l] <= z){
                z = f[l];
                j = l+1;
            }
            f[l] = BIGNUM; //reset each f after it has been compared
        }

        switch(j){ //setting current cell to the appropiate next cell
            case 1:
                currentCell = *currentCell.next1;
                parent = 1;
                break;
            case 2:
                currentCell = *currentCell.next2;
                parent = 2;
                break;
            case 3:
                currentCell = *currentCell.next3;
                parent = 3;
                break;
            case 4:
                currentCell = *currentCell.next4;
                parent = 4;
                break;
        }

        fleet[k].path[i] = j;
        i++;
        z = BIGNUM; //reset z
    }
}
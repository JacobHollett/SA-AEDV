#include "mapImp.h"
/*Collection of functions related to AEDV movement,
both drawn and calculated. Jacob Hollett & Paul Sujith OCT 31 2023*/
void move(){

    CELL nextCell;        //placeholder next cell to check for occupancy
    int moves[fleetSize]; //records direction of movement
    XY a0[fleetSize];     //saved previous positions for displaying movement
    

    while(STOP != 2){

        for(int i = 0; i < fleetSize; i++){
            a0[i].x = fleet[i].x;
            a0[i].y = fleet[i].y;
        }

        for(int i = 0; i < fleetSize; i++){
            roads[fleet[i].x][fleet[i].y].occupied = 0; //stopped AEDV's are considered off the road and passable
            if(fleet[i].battery < 20){
                check_for_charger(i);
                fleet[i].state = 2;
            }
            if(fleet[i].x != fleet[i].destx || fleet[i].y != fleet[i].desty){
                fleet[i].state = 1;
                moves[i] = fleet[i].path[fleet[i].pathStep];
                if(moves[i] == 1 && roads[fleet[i].x-1][fleet[i].y].occupied == 0){
                    fleet[i].x--;
                    fleet[i].pathStep++;
                    roads[fleet[i].x][fleet[i].y].occupied = 1;
                    }
                else if(moves[i] == 2 && roads[fleet[i].x+1][fleet[i].y].occupied == 0){
                    fleet[i].x++;
                    fleet[i].pathStep++;
                    roads[fleet[i].x][fleet[i].y].occupied = 1;
                }
                else if(moves[i] == 3 && roads[fleet[i].x][fleet[i].y-1].occupied == 0){
                    fleet[i].y--;
                    fleet[i].pathStep++;
                    roads[fleet[i].x][fleet[i].y].occupied = 1;
                }
                else if(moves[i] == 4 && roads[fleet[i].x][fleet[i].y+1].occupied == 0){
                    fleet[i].y++;
                    fleet[i].pathStep++;
                    roads[fleet[i].x][fleet[i].y].occupied = 1;
                }
                else{
                    moves[i] = 0; //if the cell an AEDV is trying to move to is occupied, we avoid the collision
                }                 //and simply do not move for that time
                
            }
            else{   //if we are at our destination reset our path index and go inactive, start charging or deliver as appropiate
                if(fleet[i].state == 2 || (fleet[i].state == 3 && fleet[i].battery < 60)){
                    fleet[i].prevx = fleet[i].destx;
                    fleet[i].prevy = fleet[i].desty;
                    fleet[i].state = 3;
                }
                else if(fleet[i].state == 3 && fleet[i].battery == 60){
                    fleet[i].state = 0;
                    fleet[i].destx = fleet[i].prevx;
                    fleet[i].desty = fleet[i].prevy;
                    find_path(i);
                }
                else if(fleet[i].state == 1 && fleet[i].nextx != 0 && fleet[i].nexty != 0){
                    fleet[i].climbTime++;
                    if(fleet[i].climbTime%4 == 0){
                        fleet[i].load = fleet[i].potLoad;
                        fleet[i].potLoad = 0;
                        fleet[i].destx = fleet[i].nextx;
                        fleet[i].desty = fleet[i].nexty;
                        fleet[i].nextx = 0;
                        fleet[i].nexty = 0;
                        find_path(i);
                    }

                }
                else{
                    fleet[i].state = 0;
                    fleet[i].load = 0;
                    }
                fleet[i].pathStep = 0;
            }
        }

        if (_kbhit())
			check_kb();

        for(int i = 0; i < fleetSize; i++){
            if(fleet[i].state == 1 || fleet[i].state == 2)
                fleet[i].battery--; //decrement battery level if AEDV has moved
            if(fleet[i].state == 3)
                fleet[i].battery++;

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

        //check next event to see if it needs to be handled, and give event to the first idle AEDV
        check_events();
    }
}


void find_path(int k){  //simple pathfinding algorithm

    int parent = 0; //keeping track of parent nodes so that we don't return to them
    int i = 0; //index of move in path
    int j = 0; //direction of chosen move (1,2,3,4)
    int z = BIGNUM; //lowest f value for comparison
    int h = 0; //square of distance between cell and destination cell
    int f[4]; //list of scores of possible moves
    CELL currentCell = roads[fleet[k].x][fleet[k].y];
    CELL nextCell;
    while(currentCell.x != fleet[k].destx || currentCell.y != fleet[k].desty){
        if(currentCell.next1 && parent != 2){
            nextCell = *currentCell.next1;
            h = abs(fleet[k].destx - nextCell.x)*abs(fleet[k].destx - nextCell.x) + 
                abs(fleet[k].desty - nextCell.y)*abs(fleet[k].desty - nextCell.y);
            f[0] = h;
        }else{f[0] = BIGNUM;} //Arbitrarily high number so as not to choose this direction

        if(currentCell.next2 && parent != 1){
            nextCell = *currentCell.next2;
            h = abs(fleet[k].destx - nextCell.x)*abs(fleet[k].destx - nextCell.x) + 
                abs(fleet[k].desty - nextCell.y)*abs(fleet[k].desty - nextCell.y);
            f[1] = h;
        }else{f[1] = BIGNUM;}

        if(currentCell.next3 && parent != 4){
            nextCell = *currentCell.next3;
            h = abs(fleet[k].destx - nextCell.x)*abs(fleet[k].destx - nextCell.x) + 
                abs(fleet[k].desty - nextCell.y)*abs(fleet[k].desty - nextCell.y);
            f[2] = h;
        }else{f[2] = BIGNUM;}

        if(currentCell.next4 && parent != 3){
            nextCell = *currentCell.next4;
            h = abs(fleet[k].destx - nextCell.x)*abs(fleet[k].destx - nextCell.x) + 
                abs(fleet[k].desty - nextCell.y)*abs(fleet[k].desty - nextCell.y);
            f[3] = h;
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

void check_for_charger(int k){ //finds nearest charging station, sets nearby street as new destination and
                               //calls find_path to set a path to it.
    int f = BIGNUM;
    int tempf;
    XY chgLoc;

    //looking for closest charging station
    for(int i = 0; i < bounds.x; i++){
        for(int j = 0; j < bounds.y; j++){
            if(block[i][j].bt == 0 || block[i][j].bt == 2){
                tempf = abs(fleet[k].x-2*i+1)+abs(fleet[k].y-2*j+1); //accounts for the fact building[0][0] is cell[1][1] etc.
                if(tempf < f){
                    f = tempf;
                    switch(block[i][j].qd){
                        case 0:
                        case 1:
                        case 2:
                            chgLoc.x = 2*i + 1;
                            chgLoc.y = 2*j + 1 - 1;
                            break;
                        case 3:
                        case 4:
                            chgLoc.x = 2*i + 1 + 1;
                            chgLoc.y = 2*j + 1;
                            break;
                        case 5:
                            chgLoc.x = 2*i + 1 - 1;
                            chgLoc.y = 2*j + 1;
                            break;
                        case 6:
                        case 7:
                        case 8:
                            chgLoc.x = 2*i + 1;
                            chgLoc.y = 2*j + 1 + 1;
                            break;
                    }
                }
            }
        }
    }
    fleet[k].prevx = fleet[k].destx;
    fleet[k].prevy = fleet[k].prevy;
    fleet[k].destx = chgLoc.x;
    fleet[k].desty = chgLoc.y;
    fleet[k].pathStep = 0; //reset path index to reroute
    find_path(k);
}

void check_events(){

    CUSTOMER customer1; //src customer
    CUSTOMER customer2; //destination customer
    if(currentEvent.time <= TIME && currentEvent.time != -1){
            for(int i = 0; i < fleetSize; i++){
                if(fleet[i].state == 0){
                    fseek(cdf, currentEvent.srcID%1000*sizeof(CUSTOMER),SEEK_SET);
                    fread(&customer1, sizeof(CUSTOMER), 1, cdf);
                    fseek(cdf, currentEvent.destID%1000*sizeof(CUSTOMER),SEEK_SET);
                    fread(&customer2, sizeof(CUSTOMER), 1, cdf);
                    //should make this switch it's own function, used frequently
                    switch(customer1.quadrant){
                        case 0:
                        case 1:
                        case 2:
                            fleet[i].destx = 2*customer1.bld.x+1;
                            fleet[i].desty = 2*customer1.bld.y+1-1;
                            break;
                        case 3:
                        case 4:
                            fleet[i].destx = 2*customer1.bld.x+1+1;
                            fleet[i].desty = 2*customer1.bld.y+1;
                            break;
                        case 5:
                            fleet[i].destx = 2*customer1.bld.x+1-1;
                            fleet[i].desty = 2*customer1.bld.y+1;
                            break;
                        case 6:
                        case 7:
                        case 8:
                            fleet[i].destx = 2*customer1.bld.x+1;
                            fleet[i].desty = 2*customer1.bld.y+1+1;
                            break;
                    }
                    switch(customer2.quadrant){
                        case 0:
                        case 1:
                        case 2:
                            fleet[i].nextx = 2*customer2.bld.x+1;
                            fleet[i].nexty = 2*customer2.bld.y+1-1;
                            break;
                        case 3:
                        case 4:
                            fleet[i].nextx = 2*customer2.bld.x+1+1;
                            fleet[i].nexty = 2*customer2.bld.y+1;
                            break;
                        case 5:
                            fleet[i].nextx = 2*customer2.bld.x+1-1;
                            fleet[i].nexty = 2*customer2.bld.y+1;
                            break;
                        case 6:
                        case 7:
                        case 8:
                            fleet[i].nextx = 2*customer2.bld.x+1;
                            fleet[i].nexty = 2*customer2.bld.y+1+1;
                            break;
                    }
                    fleet[i].potLoad = currentEvent.weight;
                    find_path(i); //find new path to destination
                    get_next_event(); //and bump forward the current event
                    break;
                }
            }
        }
}
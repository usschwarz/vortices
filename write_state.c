#include "header.h"
#include "Sporo_agent_header.h"

/*
 writes on the message board the state of a Sporo agent
 the state of a Sporo agent is given by the coordinates of its nodes
 the routine takes into account the periodic boundary conditions
*/

/* for the periodic boundary */
float f_remainder_W(float,int);

int write_state()
{
    ITER++; /* keep track of the iterations performed so far */

    int my_id;
    float *my_coordx;
    float *my_coordy;
    
    my_id = get_id();
    my_coordx = get_coordx();
    my_coordy = get_coordy();

    //-------------------------
    /* translate in case the coordinates fall outside the simulation box */
    float max_x = my_coordx[0];
    float min_x = my_coordx[0];
    for (int ii=1; ii<NNODES; ii++)
    {
        if (my_coordx[ii] > max_x) {max_x = my_coordx[ii];} /* find max(my_coord(x))*/
        if (my_coordx[ii] < min_x) {min_x = my_coordx[ii];} /* find min(my_coord(x))*/
    }
        
    if (max_x < 0 || min_x > SIDEX) { /* if --all-- outside the bounding box */
        for (int ii=0; ii<NNODES; ii++) { /* and prevent negative coordinates */
            my_coordx[ii] = f_remainder_W(my_coordx[ii],SIDEX);
        }
    }
    //-------------------------in Y
    float max_y = my_coordy[0];
    float min_y = my_coordy[0];
    for (int ii=1; ii<NNODES; ii++)
    {
        if (my_coordy[ii] > max_y) {max_y = my_coordy[ii];} /* find max(my_coord(y))*/
        if (my_coordy[ii] < min_y) {min_y = my_coordy[ii];} /* find min(my_coord(y))*/
    }
    
    if (max_y < 0 || min_y > SIDEY) { /* if --all-- outside the bounding box */
        for (int ii=0; ii<NNODES; ii++) { /* and prevent negative coordinates */
            my_coordy[ii] = f_remainder_W(my_coordy[ii],SIDEY);
        }
    }    
    //-------------------------
    
    /* posts the status of the agent on the message board 
       the variables defining the status are user-defined in the 'messages' section in generate_more_sporos_xml.py */
    add_state_message(my_id,my_coordx,my_coordy);
    
    return 0;
}

float f_remainder_W(float x,int side)
{
    float y = remainderf(x,(float)side);
    if (y < 0) { y = y + (float)side; }
    return y;
}

#include "header.h"
#include "Sporo_agent_header.h"

/*
 read the state of a Sporo agent from the message board
 trivial in the case of just one Sporo, not necessary to use pointers because coord are not modified by the function
 */

void f_reset_RS();
void f_add_RS(float x[],float y[]);

/* for the periodic boundary */
float f_remainder_RS(float,int);

int read_states()
{    
    int my_id;
    float *my_coordx;
    float *my_coordy;

    my_id = get_id();
    my_coordx = get_coordx();
    my_coordy = get_coordy();
    
    /* to reduce computational costs, each agent reads the status of the neighbours that fall in its neighbour box (see fig 3.5 in the phd thesis) 
       because of the periodic boundaries, the neighbour box, also called collision zone, can be divided 
       the following code takes proper care of such a case */

    /* non-split collision zone -> the neighbour box is not influenced by the periodic boundary */
    /* define collision zone: max(x)+delta min(x)-delta max(y)+delta min(y)-delta */
    float delta = 2 * R0;
    float max_x = my_coordx[0];
    float min_x = my_coordx[0];
    float max_y = my_coordy[0];
    float min_y = my_coordy[0];
    for (int ii=1; ii<NNODES; ii++)
    {
        if (my_coordx[ii] > max_x) {max_x = my_coordx[ii];} /* find max(my_coord(x))*/
        if (my_coordx[ii] < min_x) {min_x = my_coordx[ii];} /* find min(my_coord(x))*/
        if (my_coordy[ii] > max_y) {max_y = my_coordy[ii];} /* find max(my_coord(y))*/
        if (my_coordy[ii] < min_y) {min_y = my_coordy[ii];} /* find min(my_coord(y))*/
    }

    //-------------------------
    /* split the collision zone based on the bounding box - X coordinate */
    max_x = f_remainder_RS(max_x+delta,SIDEX);
    min_x = f_remainder_RS(min_x-delta,SIDEX);
    /* NB if bbox is split then max_x < min_x */
 
    /* split the collision zone based on the bounding box - Y coordinate */
    max_y = f_remainder_RS(max_y+delta,SIDEY);
    min_y = f_remainder_RS(min_y-delta,SIDEY);
    /* NB if bbox is split then max_y < min_y */
    
    /* these arrays are needed to manage the coordinates of the neighbours */
    float mes_coordx[NNODES];
    float mes_coordy[NNODES];
    float hmes_coordx[NNODES];
    float hmes_coordy[NNODES];

    f_reset_RS();
    int index_nbr = 0;
    
    /* here the agent starts reading the information concerning the agents
       first it determines whether an agent is in the neighbour box of the agent of interest
       if it is, i.e. in>0, then the coordinates of the neighbour are stored in dynamic arrays */
    START_STATE_MESSAGE_LOOP;
    
    if (state_message -> id != my_id) /* for all agents, excluding the agent that is due to perform a step */
    {
        int in = 0;

        for (int ii=0; ii<NNODES; ii++)  /* check if mes_coord is in the bounding box */
        {
            //-----------------
            /* bring the coordinate within the bounding box - x and y */
            mes_coordx[ii] = state_message -> coordx[ii];
            mes_coordy[ii] = state_message -> coordy[ii];
            hmes_coordx[ii] = f_remainder_RS(state_message -> coordx[ii],SIDEX);
            hmes_coordy[ii] = f_remainder_RS(state_message -> coordy[ii],SIDEY);

            if (min_x < max_x && min_y < max_y) { /* regular x and y */
                if ((hmes_coordx[ii]>min_x) && (hmes_coordx[ii]<max_x) && (hmes_coordy[ii]>min_y) && (hmes_coordy[ii]<max_y)) { in++; }
            } else if (max_x < min_x && min_y < max_y) { /* overlapping x, regular y */
                if ((hmes_coordy[ii]>min_y) && (hmes_coordy[ii]<max_y)) {
                    if ((hmes_coordx[ii]>0 && hmes_coordx[ii]<max_x) || (hmes_coordx[ii]>min_x && hmes_coordx[ii]<SIDEX)) { in++; }
                }
            } else if (min_x < max_x && max_y < min_y) { /* regular x, overlapping y */
                if ((hmes_coordx[ii]>min_x) && (hmes_coordx[ii]<max_x)) {
                    if ((hmes_coordy[ii]>0 && hmes_coordy[ii]<max_y) || (hmes_coordy[ii]>min_y && hmes_coordy[ii]<SIDEY)) { in++; }
                }
            }
            else { /* overlapping x and y */
                if ((hmes_coordx[ii]>0 && hmes_coordx[ii]<max_x) || (hmes_coordx[ii]>min_x && hmes_coordx[ii]<SIDEX)) {
                    if ((hmes_coordy[ii]>0 && hmes_coordy[ii]<max_y) || (hmes_coordy[ii]>min_y && hmes_coordy[ii]<SIDEY)) { in++; }
                }
            }
        }
        
	/* if the agent labelled by id is inside the neighbour box */
        if (in > 0) /* ADD DATA TO THE DYNAMIC ARRAYS */
        {
            /* for each neighbour - SHIFT in X */
            float dist_plus = pow(mes_coordx[0] + SIDEX - my_coordx[0],2);
            float dist_minus = pow(mes_coordx[0] - SIDEX - my_coordx[0],2);
            float dist_0 = pow(mes_coordx[0] - my_coordx[0],2);
            if (dist_plus < dist_minus && dist_plus < dist_0) {
                for (int ii=0; ii<NNODES; ii++) { mes_coordx[ii] = mes_coordx[ii] + SIDEX; }
            }
            else if (dist_minus < dist_plus && dist_minus < dist_0) {
                for (int ii=0; ii<NNODES; ii++) { mes_coordx[ii] = mes_coordx[ii] - SIDEX; }
            }

            /* for each neighbour - SHIFT in Y */
            dist_plus = pow(mes_coordy[0] + SIDEY - my_coordy[0],2);
            dist_minus = pow(mes_coordy[0] - SIDEY - my_coordy[0],2);
            dist_0 = pow(mes_coordy[0] - my_coordy[0],2);
            if (dist_plus < dist_minus && dist_plus < dist_0) {
                for (int ii=0; ii<NNODES; ii++) { mes_coordy[ii] = mes_coordy[ii] + SIDEY; }
            }
            else if (dist_minus < dist_plus && dist_minus < dist_0) {
                for (int ii=0; ii<NNODES; ii++) { mes_coordy[ii] = mes_coordy[ii] - SIDEY; }
            }
                    
            f_add_RS(mes_coordx,mes_coordy);    /* adds the NON-split coordinates */
            index_nbr++;           /* non split but with an overlap with the bbox */
        }
    }
    
    FINISH_STATE_MESSAGE_LOOP;
    
    return 0;
}

/* this function resets the list of the neighbours 
resetting happens before scanning the agents to determine if they qualify as neighbours or not */
void f_reset_RS()
{
    reset_float_array(&XNBR_NODE0);
    reset_float_array(&XNBR_NODE1);
    reset_float_array(&XNBR_NODE2);
    reset_float_array(&XNBR_NODE3);
    reset_float_array(&XNBR_NODE4);
    reset_float_array(&XNBR_NODE5);
    reset_float_array(&XNBR_NODE6);
    reset_float_array(&XNBR_NODE7);
    reset_float_array(&XNBR_NODE8);
    reset_float_array(&XNBR_NODE9);
    reset_float_array(&XNBR_NODE10);
    //reset_float_array(&XNBR_NODE11);
    //reset_float_array(&XNBR_NODE12);
    //reset_float_array(&XNBR_NODE13);
    //reset_float_array(&XNBR_NODE14); 

    reset_float_array(&YNBR_NODE0);
    reset_float_array(&YNBR_NODE1);
    reset_float_array(&YNBR_NODE2);
    reset_float_array(&YNBR_NODE3);
    reset_float_array(&YNBR_NODE4);
    reset_float_array(&YNBR_NODE5);
    reset_float_array(&YNBR_NODE6);
    reset_float_array(&YNBR_NODE7);
    reset_float_array(&YNBR_NODE8);
    reset_float_array(&YNBR_NODE9);
    reset_float_array(&YNBR_NODE10);
    //reset_float_array(&YNBR_NODE11);
    //reset_float_array(&YNBR_NODE12);
    //reset_float_array(&YNBR_NODE13);
    //reset_float_array(&YNBR_NODE14);

}

/* this function adds a neighbour to the dynamic array 
note that the number of nodes is "static", this is not very elegant but I could not find a way to make it work in a loop */
void f_add_RS(float x[],float y[])
{
    add_float(&XNBR_NODE0,x[0]);
    add_float(&XNBR_NODE1,x[1]);
    add_float(&XNBR_NODE2,x[2]);
    add_float(&XNBR_NODE3,x[3]);
    add_float(&XNBR_NODE4,x[4]);
    add_float(&XNBR_NODE5,x[5]);
    add_float(&XNBR_NODE6,x[6]);
    add_float(&XNBR_NODE7,x[7]);
    add_float(&XNBR_NODE8,x[8]);
    add_float(&XNBR_NODE9,x[9]);
    add_float(&XNBR_NODE10,x[10]);
    //add_float(&XNBR_NODE11,x[11]);
    //add_float(&XNBR_NODE12,x[12]);
    //add_float(&XNBR_NODE13,x[13]);
    //add_float(&XNBR_NODE14,x[14]);
    
    add_float(&YNBR_NODE0,y[0]);
    add_float(&YNBR_NODE1,y[1]);
    add_float(&YNBR_NODE2,y[2]);
    add_float(&YNBR_NODE3,y[3]);
    add_float(&YNBR_NODE4,y[4]);
    add_float(&YNBR_NODE5,y[5]);
    add_float(&YNBR_NODE6,y[6]);
    add_float(&YNBR_NODE7,y[7]);
    add_float(&YNBR_NODE8,y[8]);
    add_float(&YNBR_NODE9,y[9]);
    add_float(&YNBR_NODE10,y[10]);
    //add_float(&YNBR_NODE11,y[11]);
    //add_float(&YNBR_NODE12,y[12]);
    //add_float(&YNBR_NODE13,y[13]);
    //add_float(&YNBR_NODE14,y[14]);
}

float f_remainder_RS(float x, int side)
{
    float y = remainderf(x,(float)side);
    if (y < 0) { y = y + (float)side; }
    return y;
}

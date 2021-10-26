#include "header.h"
#include "Sporo_agent_header.h"
# include <time.h>

/*
 this function integrates the equations of motion of the nodes
 integration is performed using a 4th order Runge-Kutta method
 the equation of motion is described in section 3.3.2 of the thesis
 */

float f_remainder_F(float,int);
float f_rotmatrix_F(float,float,float,int);
float f_norm_F(float,float,float,float);
void f_forces_stretch_F(float*,float*,float cx[],float cy[],int,int);
void f_forces_bend_F(float*,float*,float*,float*,float cx[],float cy[],int,int);
void f_forces_prop_F(float*,float*,float cx[],float cy[],int,int);
void f_testnbr_F(float*,float*,int);
void f_forces_inter_F(float*,float*,float cx[],float cy[],int,int, int cross[]);
double Normal(double mu, double std);
/* the following two functions are needed to handle the intersections between the agents */
/* to limit computational cost the assumption is that intersecting agents are invisible to each other, and therefore their migration is not affected by the intersection */
float f_testint_F(float (*hsl)[6],int,int,float,int);
int f_sweepline_F(float spo_x[],float spo_y[],int dim,float nbr_x[],float nbr_y[],int dim_nbr);


/* depending on the system, it may not be necessary to define pi */
const float M_PI = 3.14159265359;

int forces()
{
   

    int my_id;
    float *my_coordx;
    float *my_coordy;
     
    my_id = get_id();
    my_coordx = get_coordx();
    my_coordy = get_coordy();
        
      
    /* determine runge-kutta coefficients, for each node */
    int nf = 4;
    float k1[2*NNODES], k2[2*NNODES], k3[2*NNODES], k4[2*NNODES], hfx[nf], hfy[nf];
    /* auxiliary positions and velocities */
    float xx[NNODES], yy[NNODES];
    for (int ii=0; ii<NNODES; ii++) {
        xx[ii] = my_coordx[ii];
        yy[ii] = my_coordy[ii];       
    }
    /* reset coefficients */
    for (int ii=0; ii<2*NNODES; ii++) {
        k1[ii] = 0;
        k2[ii] = 0;
        k3[ii] = 0;
        k4[ii] = 0;
    }
    
    int cross[XNBR_NODE0.size];
    /* this array says whether the agent we are considering intersects with its neighbours */
    
    /* COEFFICIENT k1 */

    /* intersections */
    /* this loop determines with which of its neighbours an agent is intersecting */
    for (int ii=0; ii<XNBR_NODE0.size; ii++) {
        float nbrx[NNODES], nbry[NNODES];
        f_testnbr_F(nbrx,nbry,ii);
        cross[ii] = f_sweepline_F(xx,yy,NNODES,nbrx,nbry,NNODES);
 	
    }

    
    /* runge-kutta */
    float m[3], ang[3];
    for (int ii=0; ii<3; ii++) { m[ii] = 0; ang[ii] = 0; }
    for (int node = 0; node<NNODES; node++) {
        /*Evaluate all the forces*/
        f_forces_stretch_F(hfx,hfy,xx,yy,node,0);
        f_forces_bend_F(hfx,hfy,ang,m,xx,yy,node,1);
        f_forces_prop_F(hfx,hfy,xx,yy,node,2);
	f_forces_inter_F(hfx,hfy,xx,yy,node,3,cross);
        
        for (int ii=0; ii<nf; ii++) {
            k1[2*node] = k1[2*node] + hfx[ii];
            k1[2*node+1] = k1[2*node+1] + hfy[ii];
        }
    }
    
    
    /* COEFFICIENT k2 */

    float hx[NNODES], hy[NNODES];
    for (int node=0; node<NNODES; node++) {
        hx[node] = xx[node] + 0.5 * DT * k1[2*node];
        hy[node] = yy[node] + 0.5 * DT * k1[2*node+1];       
    }

    /* intersections */
    for (int ii=0; ii<XNBR_NODE0.size; ii++) {
        float nbrx[NNODES], nbry[NNODES];
        f_testnbr_F(nbrx,nbry,ii);
        cross[ii] = f_sweepline_F(hx,hy,NNODES,nbrx,nbry,NNODES);
    }

    
    /* runge-kutta */
    for (int ii=0; ii<3; ii++) { m[ii] = 0; ang[ii] = 0; }
    for (int node = 0; node<NNODES; node++) {
        f_forces_stretch_F(hfx,hfy,hx,hy,node,0);
        f_forces_bend_F(hfx,hfy,ang,m,hx,hy,node,1);
        f_forces_prop_F(hfx,hfy,hx,hy,node,2);
        f_forces_inter_F(hfx,hfy,hx,hy,node,3,cross);
        
        
        for (int ii=0; ii<nf; ii++) {
            k2[2*node] = k2[2*node] + hfx[ii];
            k2[2*node+1] = k2[2*node+1] + hfy[ii];
        }
    }

    /* COEFFICIENT k3 */

    for (int node=0; node<NNODES; node++) {
        hx[node] = xx[node] + 0.5 * DT * k2[2*node];
        hy[node] = yy[node] + 0.5 * DT * k2[2*node+1];        
    }

    /* intersections */
    for (int ii=0; ii<XNBR_NODE0.size; ii++) {
        float nbrx[NNODES], nbry[NNODES];
        f_testnbr_F(nbrx,nbry,ii);
        cross[ii] = f_sweepline_F(hx,hy,NNODES,nbrx,nbry,NNODES);
    }    

    /* runge-kutta */
    for (int ii=0; ii<3; ii++) { m[ii] = 0; ang[ii] = 0; }
    for (int node = 0; node<NNODES; node++) {
        f_forces_stretch_F(hfx,hfy,hx,hy,node,0);
        f_forces_bend_F(hfx,hfy,ang,m,hx,hy,node,1);
        f_forces_prop_F(hfx,hfy,hx,hy,node,2);
        f_forces_inter_F(hfx,hfy,hx,hy,node,3,cross);
        
        
        for (int ii=0; ii<nf; ii++) {
            k3[2*node] = k3[2*node] + hfx[ii];
            k3[2*node+1] = k3[2*node+1] + hfy[ii];
        }
    }

    /* COEFFICIENT k4 */

    for (int node=0; node<NNODES; node++) {
        hx[node] = xx[node] + DT * k3[2*node];
        hy[node] = yy[node] + DT * k3[2*node+1];        
    }

    /* intersections */
    for (int ii=0; ii<XNBR_NODE0.size; ii++) {
        float nbrx[NNODES], nbry[NNODES];
        f_testnbr_F(nbrx,nbry,ii);
        cross[ii] = f_sweepline_F(hx,hy,NNODES,nbrx,nbry,NNODES);
    }    
    
    /* runge-kutta */
    for (int ii=0; ii<3; ii++) { m[ii] = 0; ang[ii] = 0; }
    for (int node = 0; node<NNODES; node++) {
        f_forces_stretch_F(hfx,hfy,hx,hy,node,0);
        f_forces_bend_F(hfx,hfy,ang,m,hx,hy,node,1);
        f_forces_prop_F(hfx,hfy,hx,hy,node,2);
        f_forces_inter_F(hfx,hfy,hx,hy,node,3,cross);
        
        
        for (int ii=0; ii<nf; ii++) {
            k4[2*node] = k4[2*node] + hfx[ii];
            k4[2*node+1] = k4[2*node+1] + hfy[ii];
        }
    }
    
    float step, hstep;  
    double Ns,Ndx,Ndy;
    step = 0;
    hstep = 0;
    //my_TAU[0] = my_TAU[0]+DT;//Update time clock by time-step   
    
    /* update positions and velocities */
    for (int node = 0; node<NNODES; node++) {

	Ns=sqrt(4*NOISE*DT)*Normal(0,1);
  	Ndx=Normal(0,1);
   	Ndy=Normal(0,1);  	

        my_coordx[node] = xx[node] + Ns*Ndx/sqrt(Ndx*Ndx+Ndy*Ndy) + (1.0/6.0)*DT*(k1[2*node]+2*k2[2*node]+2*k3[2*node]+k4[2*node]);
        my_coordy[node] = yy[node] + Ns*Ndy/sqrt(Ndx*Ndx+Ndy*Ndy) + (1.0/6.0)*DT*(k1[2*node+1]+2*k2[2*node+1]+2*k3[2*node+1]+k4[2*node+1]);
                
        hstep = sqrt(pow(my_coordx[node]-xx[node],2)+pow(my_coordy[node]-yy[node],2));
        if (hstep>step) {step = hstep;}
    }

    // ----------------------------
    /* IMPORTANT: the integration can diverge because the integration step is too large and the forces involved too strong
	it is possible to encounter such situation especially at high agent densities 
	for this reason, IF the step resulting from the above integration exceeds 0.8*R0, the integration is repeated with an appropriate number of finer steps */
    if (step > 0.8 * R0) {
	/* that's just a message telling that there's a finer integration ongoing and which agent is involved 
	   non terribly useful, but still */        
        //printf("IN %d\t\n",ID);
        
        float tau = 0.1;
        int ndt = 10;
        
        /* to refine the integration, first we go back one step, that is the one too large */
        for (int ii=0; ii<NNODES; ii++) {
            my_coordx[ii] = xx[ii];
            my_coordy[ii] = yy[ii];
            
        }
        /* reset coefficients */
        for (int ii=0; ii<2*NNODES; ii++) {
            k1[ii] = 0;
            k2[ii] = 0;
            k3[ii] = 0;
            k4[ii] = 0;
        }
        
        /* FINER INTEGRATION */
        for (int tt=1; tt<=ndt; tt++) {
        
	    /* COEFFICIENT k1 */

	    /* intersections */
	    for (int ii=0; ii<XNBR_NODE0.size; ii++) {
		float nbrx[NNODES], nbry[NNODES];
		f_testnbr_F(nbrx,nbry,ii);
		cross[ii] = f_sweepline_F(hx,hy,NNODES,nbrx,nbry,NNODES);
	    }   
                        
            /* runge-kutta */
            for (int ii=0; ii<3; ii++) { m[ii] = 0; ang[ii] = 0; }
            for (int node = 0; node<NNODES; node++) {
                
                f_forces_stretch_F(hfx,hfy,xx,yy,node,0);
                f_forces_bend_F(hfx,hfy,ang,m,xx,yy,node,1);
                f_forces_prop_F(hfx,hfy,xx,yy,node,2);
                f_forces_inter_F(hfx,hfy,xx,yy,node,3,cross);
                
                for (int ii=0; ii<nf; ii++) {
                    k1[2*node] = k1[2*node] + hfx[ii];
                    k1[2*node+1] = k1[2*node+1] + hfy[ii];
                }
            }
            
            /* COEFFICIENT k2 */

            for (int node=0; node<NNODES; node++) {
                hx[node] = xx[node] + 0.5 * DT * k1[2*node] * tau;
                hy[node] = yy[node] + 0.5 * DT * k1[2*node+1] * tau;                
            }
            
 	    /* intersections */
            for (int ii=0; ii<XNBR_NODE0.size; ii++) {
                float nbrx[NNODES], nbry[NNODES];
                f_testnbr_F(nbrx,nbry,ii);
                cross[ii] = f_sweepline_F(hx,hy,NNODES,nbrx,nbry,NNODES);
            }
           
	    /* runge-kutta */            
            for (int ii=0; ii<3; ii++) { m[ii] = 0; ang[ii] = 0; }
            for (int node = 0; node<NNODES; node++) {
                f_forces_stretch_F(hfx,hfy,hx,hy,node,0);
                f_forces_bend_F(hfx,hfy,ang,m,hx,hy,node,1);
                f_forces_prop_F(hfx,hfy,hx,hy,node,2);
                f_forces_inter_F(hfx,hfy,hx,hy,node,3,cross);
                
               
                for (int ii=0; ii<nf; ii++) {
                    k2[2*node] = k2[2*node] + hfx[ii];
                    k2[2*node+1] = k2[2*node+1] + hfy[ii];
                }
            }

            /* COEFFICIENT k3 */

            for (int node=0; node<NNODES; node++) {
                hx[node] = xx[node] + 0.5 * DT * k2[2*node] * tau;
                hy[node] = yy[node] + 0.5 * DT * k2[2*node+1] * tau;
                
            }
            
	    /* intersections */
            for (int ii=0; ii<XNBR_NODE0.size; ii++) {
                float nbrx[NNODES], nbry[NNODES];
                f_testnbr_F(nbrx,nbry,ii);
                cross[ii] = f_sweepline_F(hx,hy,NNODES,nbrx,nbry,NNODES);
            }
            
	    /* runge-kutta */            
            for (int ii=0; ii<3; ii++) { m[ii] = 0; ang[ii] = 0; }
            for (int node = 0; node<NNODES; node++) {
                f_forces_stretch_F(hfx,hfy,hx,hy,node,0);
                f_forces_bend_F(hfx,hfy,ang,m,hx,hy,node,1);
                f_forces_prop_F(hfx,hfy,hx,hy,node,2);
                f_forces_inter_F(hfx,hfy,hx,hy,node,3,cross);
                
               
                for (int ii=0; ii<nf; ii++) {
                    k3[2*node] = k3[2*node] + hfx[ii];
                    k3[2*node+1] = k3[2*node+1] + hfy[ii];
                }
            }

            /* COEFFICIENT k4 */

            for (int node=0; node<NNODES; node++) {
                hx[node] = xx[node] + DT * k3[2*node] * tau;
                hy[node] = yy[node] + DT * k3[2*node+1] * tau;
                
            }
            
		 /* intersections */
            for (int ii=0; ii<XNBR_NODE0.size; ii++) {
                float nbrx[NNODES], nbry[NNODES];
                f_testnbr_F(nbrx,nbry,ii);
                cross[ii] = f_sweepline_F(hx,hy,NNODES,nbrx,nbry,NNODES);
            }
           
	    /* runge-kutta */            
            for (int ii=0; ii<3; ii++) { m[ii] = 0; ang[ii] = 0; }
            for (int node = 0; node<NNODES; node++) {
                f_forces_stretch_F(hfx,hfy,hx,hy,node,0);
                f_forces_bend_F(hfx,hfy,ang,m,hx,hy,node,1);
                f_forces_prop_F(hfx,hfy,hx,hy,node,2);
                f_forces_inter_F(hfx,hfy,hx,hy,node,3,cross);
                
                
                for (int ii=0; ii<nf; ii++) {
                    k4[2*node] = k4[2*node] + hfx[ii];
                    k4[2*node+1] = k4[2*node+1] + hfy[ii];
                }
            }
            
            /* update positions and velocities */
            for (int node = 0; node<NNODES; node++) {

		Ns=sqrt(4*NOISE*DT*tau)*Normal(0,1);
  		Ndx=Normal(0,1);
   		Ndy=Normal(0,1);  	

                my_coordx[node] = xx[node] + Ns*Ndx/sqrt(Ndx*Ndx+Ndy*Ndy) + (1.0/6.0)*DT*(k1[2*node]+2*k2[2*node]+2*k3[2*node]+k4[2*node])*tau;
                my_coordy[node] = yy[node] + Ns*Ndy/sqrt(Ndx*Ndx+Ndy*Ndy) + (1.0/6.0)*DT*(k1[2*node+1]+2*k2[2*node+1]+2*k3[2*node+1]+k4[2*node+1])*tau;   
         
            }
            
        }
	/* END OF THE FINER INTEGRATION */
                
        for (int ii=0; ii<NNODES; ii++) {
            xx[ii] = my_coordx[ii];
            yy[ii] = my_coordy[ii];
           
        }
        /* reset coefficients */
        for (int ii=0; ii<2*NNODES; ii++) {
            k1[ii] = 0;
            k2[ii] = 0;
            k3[ii] = 0;
            k4[ii] = 0;
        }
        
    }
    
    return 0;
}

/* ---------- FUNCTIONS ---------- */

/* f_rotmatrix_F rotates a two dimensional vector by an angle ang
   depending on the value of h, it returns either the x or the y component of the rotated vector */
float f_rotmatrix_F(float cx, float cy, float ang, int h)
{
    if (h == 0) { /* return x component */
        return cx*cos(ang) - cy*sin(ang);
    } else if (h == 1) { /* return y component */
        return cx*sin(ang) + cy*cos(ang);
    } else { /* error message */
        printf("Error, specify output coordinate!");
        exit(1);
    }
}

/* f_norm_F returns the euclidean distance between two points in the plane*/
float f_norm_F(float x1, float y1, float x2, float y2)
{
    return pow(pow(x2-x1,2)+pow(y2-y1,2),0.5);
}

/* f_forces_stretch_F computes the contribution to the equation of motion coming from the constraint on the distance between two consecutive nodes 
   two consecutive nodes are connected by a harmonic spring of elastic constant KEL and rest length RO */
void f_forces_stretch_F(float* fx, float* fy, float cx[], float cy[], int node, int ii)
{
    fx[ii] = 0;
    fy[ii] = 0;
    float norm = 0;
    
    if (node > 0) { /* elastic force front */
        norm = f_norm_F(cx[node-1],cy[node-1],cx[node],cy[node]);
        fx[ii] = fx[ii] + KEL * (norm - R0) * (cx[node-1]-cx[node]) / norm;
        fy[ii] = fy[ii] + KEL * (norm - R0) * (cy[node-1]-cy[node]) / norm;
    }
    if (node < NNODES-1) { /* elastic force rear */
        norm = f_norm_F(cx[node+1],cy[node+1],cx[node],cy[node]);
        fx[ii] = fx[ii] - KEL * (norm - R0) * (cx[node]-cx[node+1]) / norm;
        fy[ii] = fy[ii] - KEL * (norm - R0) * (cy[node]-cy[node+1]) / norm;
    }
}

/* f_forces_prop_F computes the contribution to the equation of motion coming from the propulsion forces of each node
   commented parts labelled by [SENS] denote code that can be used for implementing short-range mutual attraction between the agents 
   the mutual attraction is shortly described in section 3.7.1 of the thesis */
void f_forces_prop_F(float* fx, float* fy, float cx[], float cy[], int node, int ii)
{
    float norm = 0;
    fx[ii] = 0;
    fy[ii] = 0;
    
       /* // [SENS] 
    float dir[2] = {0,0};*/
   

    if (node > 0 && node < NNODES-1) {
        norm = f_norm_F(cx[node+1],cy[node+1],cx[node-1],cy[node-1]);
        fx[ii] = FP * (cx[node-1]-cx[node+1]) / norm;
        fy[ii] = FP * (cy[node-1]-cy[node+1]) / norm;
        /* // [SENS] 
        dir[0] = f_rotmatrix_F(cx[node-1]-cx[node+1],cy[node-1]-cy[node+1],M_PI*0.5,0);
        dir[1] = f_rotmatrix_F(cx[node-1]-cx[node+1],cy[node-1]-cy[node+1],M_PI*0.5,1);
        */
    } else if (node == 0) {
        norm = f_norm_F(cx[node],cy[node],cx[node+1],cy[node+1]);
        fx[ii] = FP * (cx[node]-cx[node+1]) / norm;
        fy[ii] = FP * (cy[node]-cy[node+1]) / norm;
        /* // [SENS] 
        dir[0] = f_rotmatrix_F(cx[node]-cx[node+1],cy[node]-cy[node+1],M_PI*0.5,0);
        dir[1] = f_rotmatrix_F(cx[node]-cx[node+1],cy[node]-cy[node+1],M_PI*0.5,1);
        */
    } else if (node == NNODES-1) {
        norm = f_norm_F(cx[node-1],cy[node-1],cx[node],cy[node]);
        fx[ii] = FP * (cx[node-1]-cx[node]) / norm;
        fy[ii] = FP * (cy[node-1]-cy[node]) / norm;
        
    }
}

/* f_remainder_F is needed for managing the periodic boundary conditions  */
float f_remainder_F(float x,int side)
{
    float y = remainderf(x,(float)side);
    if (y < 0) { y = y + (float)side; }
    return y;
}

/* f_ang_F is used in f_forces_bend_F (see below) 
   the context is the computation of the force on a node caused by the bending stiffness */
float f_ang_F(float x0, float x1, float x2, float y0, float y1, float y2, float n1, float n2)
{
    float arg_acos = ((x0 - x1)*(x1 - x2) + (y0 - y1)*(y1 - y2)) / (n1*n2);
    if (arg_acos > 1) {
        arg_acos = 1;
    } else if (arg_acos < -1) {
        arg_acos = -1;
    }
    return acos(arg_acos);
}

/* f_test_ang_F is used in f_forces_bend_F (see below) 
   the context is the computation of the force on a node caused by the bending stiffness */
float f_test_ang_F(float x0, float x1, float x2, float y0, float y1, float y2)
{
    return (x1 - x2)*(y0 - y1) - (x0 - x1)*(y1 - y2);
}

/* f_forces_bend_F computes the contribution to the equation of motion coming from the bending stiffness 
   starting from the bending energy at each node computes the corresponding torque applied to the neighbouring nodes
   the torques are converted into forces by considering the distance between two nodes */
void f_forces_bend_F(float* fx, float* fy, float* ang, float* m, float cx[], float cy[], int node, int ii)
{
    /* variables defined in the main body of the program */
    /* float m[3] = {0,0,0}; 0 node-1, 1 node, 2 node+1 -- absolute values */
    /* float ang[3] = {0,0,0}; ang in node-1, node e node+1 */
    /* m[0] torque from node-1 on the left rod (abs) */
    /* m[1] torque from node (abs) */
    /* m[2] torque from node+1 on the right rod (abs) */

    fx[ii] = 0;
    fy[ii] = 0;
    int s0 = 1;
    int s1 = 1;
    int s2 = 1;
    
    /* left rod - torque given by node-1 and node */
    /* torques known from the previous step, perform sum with signs */
    /* the sum gives ml, momentum on the left rod */
    /* if ang[1]>TH0 sgn di m[1] = +1, if ang[1]<=TH0 sgn di m[1] = -1 */
    /* if ang[0]>TH0 sgn di m[0] = -1, if ang[0]<=TH0 sgn di m[0] = +1 */
    if ( ang[1] <= TH0 ) { s1 = -1; }
    if ( ang[0] > TH0 ) { s0 = -1; }
    float ml = s0 * m[0] + s1 * m[1];
    
    /* right rod - torque given by node and node+1 */
    /* compute angle in node+1 and store in ang[2] */
    /* compute torque from node+1 and store in m[2] */
    if (node < NNODES-2) {
        float nr = f_norm_F(cx[node],cy[node],cx[node+1],cy[node+1]);
        float nrr = f_norm_F(cx[node+1],cy[node+1],cx[node+2],cy[node+2]);
        ang[2] = f_ang_F(cx[node],cx[node+1],cx[node+2],cy[node],cy[node+1],cy[node+2],nr,nrr);
        float test = f_test_ang_F(cx[node],cx[node+1],cx[node+2],cy[node],cy[node+1],cy[node+2]);
        if (test < 0) { ang[2] = M_PI + ang[2]; }
        else { ang[2] = M_PI - ang[2]; }
        m[2] = KTH * sqrt(pow(ang[2]-TH0,2));
    }
    /* if ang[1]>TH0 -> sgn di m[1] = -1, if ang[1]<=TH0 -> sgn di m[1]=+1 */
    /* if ang[2]>TH0 -> sgn di m[2] = +1, if ang[2]<=TH0 -> sgn di m[2]=-1 */
    /* the sum gives mr, momentum on the right rod */
    s1 = 1;
    if ( ang[1] > TH0 ) { s1 = -1; }
    if ( ang[2] <= TH0 ) { s2 = -1; }
    float mr = s1 * m[1] + s2 * m[2];

    /* torques to forces - left and right rods */
    /* if ml>0 Rot(leftrod,-0.5*M_PI), if ml<=0 Rot(leftrod,0.5*M_PI) */
    /* magnitude of the force ml/(0.5*norm(leftrod)) */
    float fl = 0;
    float dir_l[2] = {0,0};
    float nl = 1;
    int sgn_l = 0;
    if (node > 0) {
        sgn_l = 1;
        if (ml>0) { sgn_l = -1; }
        dir_l[0] = f_rotmatrix_F(cx[node-1]-cx[node],cy[node-1]-cy[node],sgn_l*0.5*M_PI,0);
        dir_l[1] = f_rotmatrix_F(cx[node-1]-cx[node],cy[node-1]-cy[node],sgn_l*0.5*M_PI,1);
        nl = f_norm_F(cx[node-1],cy[node-1],cx[node],cy[node]);
        fl = 2.0 * sqrt(pow(ml,2)) / nl;
    }
    /* if mr>0 Rot(rightrod,0.5*M_PI), if mr<=0 Rot(rightrod,-0.5*M_PI) */
    /* magnitude of the force mr/(0.5*norm(rightrod)) */
    float fr = 0;
    float dir_r[2] = {0,0};
    float nr = 1;
    int sgn_r = 0;
    if (node < NNODES-1) {
        sgn_r = 1;
        if (mr<=0) { sgn_r = -1; }
        dir_r[0] = f_rotmatrix_F(cx[node]-cx[node+1],cy[node]-cy[node+1],sgn_r*0.5*M_PI,0);
        dir_r[1] = f_rotmatrix_F(cx[node]-cx[node+1],cy[node]-cy[node+1],sgn_r*0.5*M_PI,1);
        nr = f_norm_F(cx[node],cy[node],cx[node+1],cy[node+1]);
        fr = 2.0 * sqrt(pow(mr,2)) / nr;
    }
    
    fx[ii] = fl * dir_l[0] / nl + fr * dir_r[0] / nr;
    fy[ii] = fl * dir_l[1] / nl + fr * dir_r[1] / nr;
    
    /* shift torques and angles positions for the next node (abs) */
    for (int ii=1; ii<3; ii++) {
        m[ii-1] = m[ii];
        ang[ii-1] = ang[ii];
    }
    m[2] = 0;
    ang[2] = 0;
    /* ok boundary treatment */
}

/* f_testnbr_F builds two temporary arrays storing the x andd y coordinates of the neighbour agent of interest */
void f_testnbr_F(float* x,float* y,int ii)
{
    x[0] = XNBR_NODE0.array[ii];   y[0] = YNBR_NODE0.array[ii];
    x[1] = XNBR_NODE1.array[ii];   y[1] = YNBR_NODE1.array[ii];
    x[2] = XNBR_NODE2.array[ii];   y[2] = YNBR_NODE2.array[ii];
    x[3] = XNBR_NODE3.array[ii];   y[3] = YNBR_NODE3.array[ii];
    x[4] = XNBR_NODE4.array[ii];   y[4] = YNBR_NODE4.array[ii];
    x[5] = XNBR_NODE5.array[ii];   y[5] = YNBR_NODE5.array[ii];
    x[6] = XNBR_NODE6.array[ii];   y[6] = YNBR_NODE6.array[ii];
    x[7] = XNBR_NODE7.array[ii];   y[7] = YNBR_NODE7.array[ii];
    x[8] = XNBR_NODE8.array[ii];   y[8] = YNBR_NODE8.array[ii];
    x[9] = XNBR_NODE9.array[ii];   y[9] = YNBR_NODE9.array[ii];
    x[10] = XNBR_NODE10.array[ii];   y[10] = YNBR_NODE10.array[ii];
    //x[11] = XNBR_NODE11.array[ii];   y[11] = YNBR_NODE11.array[ii];
    //x[12] = XNBR_NODE12.array[ii];   y[12] = YNBR_NODE12.array[ii];
    //x[13] = XNBR_NODE13.array[ii];   y[13] = YNBR_NODE13.array[ii];
    //x[14] = XNBR_NODE14.array[ii];   y[14] = YNBR_NODE14.array[ii];
}

/* f_forces_inter_F computes the contribution to the equation of motion coming from the interaction with the neighbours */
void f_forces_inter_F(float* fx,float* fy,float cx[],float cy[],int node,int ll,int cross[])//,float* ww) // weight per OP
{
    fx[ll] = 0;
    fy[ll] = 0;
    float dist = 0;
    float fi = 0;
    
    /* standard deviation of the Gaussian associated to each node */
    float sig2=pow(2,1/3)-(2*WIDTH)*(2*WIDTH);//= 0.5*WIDTH;//WIDTH=0.3
    
    for (int ii=0; ii<XNBR_NODE0.size; ii++) {
        /* build test neighbour */
        float nbrx[NNODES], nbry[NNODES];
        f_testnbr_F(nbrx,nbry,ii);

	if (cross[ii] == 0) { /* if the two agents do not intersect */
           for (int jj=0; jj<NNODES; jj++) 	   
	   { dist = f_norm_F(nbrx[jj],nbry[jj],cx[node],cy[node]);
             if (dist <= 2*WIDTH)
		fi = KI*dist*(2/pow(pow(dist,2)+sig2,7)-1/pow(pow(dist,2)+sig2,4));//LJ
	     else
		fi = 0;	                

	     fx[ll] = fx[ll] + fi * (cx[node]-nbrx[jj])/dist;
             fy[ll] = fy[ll] + fi * (cy[node]-nbry[jj])/dist;
           }
	 }
       }
}


//-------- SWEEPLINE FUNCTION ---------//

/* f_sweepline_F is the function responsible for determining whether two agents intersect each other or not 
   for that, an agent is considered as a series of NNODES-1 connected segments, and the connection points coincide with the positions of the nodes 
   for info on the algorithm, see the entry "sweep line algorithm" on wikipedia
   pseudocode can be read at http://geomalgorithms.com/a09-_intersect-3.html 
   the function below returns 0 if two agents do not intersect, 1 if they do
   the function does not count how many intersections two agents have */

int f_sweepline_F(float spo_x[], float spo_y[], int dim, float nbr_x[], float nbr_y[], int dim_nbr)
{
    /* loop indeces */
    int ii=0;
    int jj=0;
    int hh=0;
    
    /* tmp vars */
    int n_seg = (dim_nbr-1 + dim-1); /* takes into account the auxiliary segments */
    
    //--------------------- EVENT QUEUE
    
    /* event queue eq */
    int l_eq = 2*n_seg;
    int c_eq = 4; // x, y, (0 for left point, 1 for right point), segment id 

    float in_eq[l_eq][c_eq];
    float h_eq[c_eq];
    
    for (ii=0; ii<l_eq; ii++) {
        for (jj=0; jj<c_eq; jj++) { in_eq[ii][jj]=0; }
    }
    for (ii=0; ii<c_eq; ii++) { h_eq[ii]=0; }
    
    //---------------------
    /* fill in_eq from RAW data */
    for (ii=0; ii<dim-1; ii++) {
        in_eq[2*ii][3] = ii;
        in_eq[2*ii+1][3] = ii;
        for (jj=0; jj<2; jj++) {
            in_eq[2*ii+jj][0] = spo_x[ii+jj];
            in_eq[2*ii+jj][1] = spo_y[ii+jj];
        }
        if (in_eq[2*ii][0] < in_eq[2*ii+1][0]) {
            in_eq[2*ii][2] = 0;
            in_eq[2*ii+1][2] = 1;
        }  else {
            in_eq[2*ii][2] = 1; //Exchange order if x is not increasing from Left to Right 
            in_eq[2*ii+1][2] = 0;
            for (jj=0; jj<c_eq; jj++) {
                h_eq[jj] = in_eq[2*ii][jj];
                in_eq[2*ii][jj] = in_eq[2*ii+1][jj];
                in_eq[2*ii+1][jj] = h_eq[jj];
            }
        }
    }
    
    int ind_start = 2*(dim-1);
    for (ii=0; ii<dim_nbr-1; ii++) {
        in_eq[ind_start+2*ii][3] = ind_start/2+ii;
        in_eq[ind_start+2*ii+1][3] = ind_start/2+ii;
        for (jj=0; jj<2; jj++) {
            in_eq[ind_start+2*ii+jj][0] = nbr_x[ii+jj];
            in_eq[ind_start+2*ii+jj][1] = nbr_y[ii+jj];
        }
        if (in_eq[ind_start+2*ii][0] < in_eq[ind_start+2*ii+1][0]) {
            in_eq[ind_start+2*ii][2] = 0;
            in_eq[ind_start+2*ii+1][2] = 1;
        }  else {
            in_eq[ind_start+2*ii][2] = 1;  //Exchange order if x is not increasing from Left to Right 
            in_eq[ind_start+2*ii+1][2] = 0;
            for (jj=0; jj<c_eq; jj++) {
                h_eq[jj] = in_eq[ind_start+2*ii][jj];
                in_eq[ind_start+2*ii][jj] = in_eq[ind_start+2*ii+1][jj];
                in_eq[ind_start+2*ii+1][jj] = h_eq[jj];
            }
        }
    }
    
    /* declare and fill eq */
    float eq[l_eq][c_eq];
    for ( ii=0; ii<l_eq; ii++) {
        for ( jj=0; jj<c_eq; jj++) { eq[ii][jj] = in_eq[ii][jj]; }
    }
    
    /* order rows based on x, then based on y */
    for ( ii=0; ii<l_eq; ii++) {
        for ( jj=0; jj<l_eq-ii-1; jj++) {
            if (eq[jj][1] > eq[jj+1][1]) {
                for ( hh = 0; hh<c_eq; hh++) {
                    float tmp = 0;
                    tmp = eq[jj][hh];
                    eq[jj][hh] = eq[jj+1][hh];
                    eq[jj+1][hh] = tmp;
                }
            }
        }
    }
    for ( ii=0; ii<l_eq; ii++) {
        for ( jj=0; jj<l_eq-ii-1; jj++) {
            if (eq[jj][0] > eq[jj+1][0]) {
                for ( hh = 0; hh<c_eq; hh++) {
                    float tmp = eq[jj][hh];
                    eq[jj][hh] = eq[jj+1][hh];
                    eq[jj+1][hh] = tmp;
                }
            }
        }
    }
    /* end order eq by rows */
    
    /* swap identical rows so that 1 is before zero */
    for ( ii=0; ii<l_eq-1; ii++) {
        float dx = sqrt(pow(eq[ii][0]-eq[ii+1][0],2));
        float dy = sqrt(pow(eq[ii][1]-eq[ii+1][1],2));
        if (dx < pow(10,-6) && dy < pow(10,-6)) {
            if (eq[ii][2] < eq[ii+1][2]) { /* if the new segm starts before the other ends */
                for ( jj = 0; jj<c_eq; jj++) { /* swap the lines */
                    float tmp = eq[ii][jj];
                    eq[ii][jj] = eq[ii+1][jj];
                    eq[ii+1][jj] = tmp;
                }
            }
        }
    }
    /* end final swap */
    //------------------- EVENT QUEUE ORDERED CORRECTLY
    
    /* define sweep line sl */
    int c_sl = 6; /* Y on the sl, coord x0,y0 and x1,y1 of the segment, ID */
    float sl[l_eq][c_sl];  /* current y, x, y and ---id--- of the segm. */
    for ( ii=0; ii<l_eq; ii++) {
        for ( jj=0; jj<c_sl; jj++) { sl[ii][jj] = 0; }
    }
    /* end definition of the sweep line */
    
    int numel_eq = l_eq;
    int i_eq = 0; /* index of the element in eq */
    int i_sl = 0; /* (initial) number of points on sl */
    
    while (numel_eq>0) {
        
        float ev[c_eq];
        for ( ii=0; ii<c_eq; ii++) { ev[ii] = eq[i_eq][ii]; } /* loads next event of eq */ //i_eq increase at the end of the loop
        
        /* load seg_ev */
        float seg_ev[2][c_eq];
        int ind = 2 * (int)ev[3]; /* convert to int the index of ev */
        for ( ii=0; ii<2; ii++) { /* load segment from in_eq */
            for ( jj=0; jj<c_eq; jj++) { seg_ev[ii][jj] = in_eq[ind + ii][jj]; }
        }
        if (seg_ev[0][2] > seg_ev[1][2]) { /* swap the lines so that 0 is in line 0 */
            for ( ii=0; ii<c_eq; ii++) {
                h_eq[ii] = seg_ev[0][ii];
                seg_ev[0][ii] = seg_ev[1][ii];
                seg_ev[1][ii] = h_eq[ii];
            }
        }
        
        if (ev[2]==0 || seg_ev[0][0]==seg_ev[1][0]) { /* if left point or vertical segment */
            
            //-------------------------------
            /* add seg_ev to sl */
            for ( ii=0; ii<i_sl; ii++) { /* update to the actual Y value */
                float ratio = sqrt(pow(seg_ev[0][0]-sl[ii][1],2)) / sqrt(pow((sl[ii][3]-sl[ii][1]),2));
                sl[ii][0] = sl[ii][2] + (sl[ii][4]-sl[ii][2]) * ratio;
            }
            //-------------------------------
            
            if (seg_ev[0][0] == seg_ev[1][0]) { /* if vertical line */
                if (ev[1] < (seg_ev[1][1]+seg_ev[0][1])*0.5) {
                    for ( ii=0; ii<i_sl; ii++) {
                        if ((sl[ii][0] < seg_ev[0][1] && sl[ii][0] > seg_ev[1][1]) ||
                            (sl[ii][0] > seg_ev[0][1] && sl[ii][0] < seg_ev[1][1])) {
                            return 1;
                        }
                    }
                }
            }
            else { /* general case, non vertical lines */
                int kk = 0;
                for ( ii=0; ii<i_sl; ii++) { /* scrolls the Ys on sl */
                    if (sl[ii][0] < ev[1]) { kk++; } /* index of seg_ev */
                    /* cfr con nbr down - test starting point */
                }
                
                float dx = sqrt(pow(seg_ev[0][0]-sl[kk][1],2));
                float dy = sqrt(pow(seg_ev[0][1]-sl[kk][2],2));
                if (dx < pow(10,-6) && dy < pow(10,-6)) { /* test end points */
                    if (sl[kk][4] < seg_ev[1][1]) { kk++; }
                }
                
                for ( ii=i_sl-1; ii>=kk; ii--) { /* shifts to make space for seg_ev*/
                    for ( jj=0; jj<c_sl; jj++) { sl[ii+1][jj] = sl[ii][jj]; }
                }
                /* adds the point to the sweep line */
                sl[kk][0] = ev[1]; /* Y on the line */
                for ( ii=0; ii<2; ii++) { sl[kk][ii+1] = seg_ev[0][ii]; }
                for ( ii=0; ii<2; ii++) { sl[kk][ii+3] = seg_ev[1][ii]; }
                sl[kk][5] = ev[3]; /* id */
                
                /* what if identical starting point */
                
                i_sl++; /* updates the nr of points on the sweep line */
                
                /* check for intersections of seg_ev with segA and seg_ev with segB*/
                /* look at the y coordinates */
                if (i_sl > 1) {
                    float test;
                    if (kk > 0 && kk < i_sl-1) { /* segA and seg_ev, segB and seg_ev */
                        /* compute the positions on the right sweep line */
                        test = f_testint_F(sl,kk,-1,sl[kk][1],1);
                        if (test < 0) { return 1; }
                        test = f_testint_F(sl,kk,+1,sl[kk][1],1);
                        if (test < 0) { return 1; }
                    } else if (kk == 0) { /* seg_ev and segB */
                        test = f_testint_F(sl,kk,+1,sl[kk][1],1);
                        if (test < 0) { return 1; }
                    } else /* if kk == i_sl-1 */ { /* segA and seg_ev */
                        test = f_testint_F(sl,kk,-1,sl[kk][1],1
                                            );
                        if (test < 0) { return 1; }
                    }
                }
            }
        } else /* if ev is a right endpoint */ {
            
            //-------------------------------
            for ( ii=0; ii<i_sl; ii++) { /* update to the actual Y value */
                float ratio = sqrt(pow(seg_ev[1][0]-sl[ii][1],2)) / sqrt(pow((sl[ii][3]-sl[ii][1]),2));
                sl[ii][0] = sl[ii][2] + (sl[ii][4]-sl[ii][2]) * ratio;
            }
            //-------------------------------
            
            /* find segment ev in sl, id is ev[3] */
            int i_ev = 0;
            for ( ii=0; ii<i_sl; ii++) {
                if ((int)sl[ii][5] == (int)ev[3]) { i_ev = ii; }
            }
            
            for ( ii=i_ev; ii<i_sl-1; ii++) { /* delete seg_ev from sl - shift up */
                for ( jj=0; jj<c_sl; jj++) { sl[ii][jj] = sl[ii+1][jj]; }
            }
            i_sl--;
            
            /* intersection of segments above and below seg_ev in sl */
            /* now i_ev is the index of the lower neighbour */
            float test;
            //if (i_ev > 0 && i_ev < i_sl-1) {
            if (i_ev > 0) {
                // test intersection to the right
                test = f_testint_F(sl,i_ev,-1,ev[0],1);
                if (test < 0) { return 1; } /* possibly not necessary */
                //test intersection to the left
                //test = f_testint(sl,i_ev,-1,ev[0],-1);
                //if (test < 0) { return 1; }
            }
        }
        /* increase index and decrease the number of points in eq */
        i_eq++;
        numel_eq--;
    }
    
    return 0;
    
}

float f_testint_F(float (*hsl)[6], int k, int nb, float x_sl, int sg)
{
    //which of the two segments terminates first?
    int ii, jj;
    if (sg*hsl[k][3] < sg*hsl[k+nb][3]) { /* seg_ev terminates first */
        ii = k+nb;
        jj = k;
    } else {
        ii = k;
        jj = k+nb;
    }
    float rt = (hsl[jj][3]-sg*x_sl)/(hsl[ii][3]-sg*x_sl);
    float hy = hsl[ii][0] + rt*(hsl[ii][4]-hsl[ii][0]);
    //compute test
    return (hsl[ii][0]-hsl[jj][0]) * (hy-hsl[jj][4]);
}


double Normal(double mu, double std)
{ 
   double U1, U2, W, mult;
   static double A1, A2;
   static int call = 0;
   if (call == 1)
    { call = !call;
      return (A2);
     }
   else
    {
      U1 = ((double) rand () / RAND_MAX) ;
      U2 = ((double) rand () / RAND_MAX) ;
      
    mult = sqrt(-2*log(U1));
    A1 = mult*cos(2.0*M_PI*U2);
    A2 = mult*sin(2.0*M_PI*U2);
    call = !call; 
    return (A1);
    }
   
}



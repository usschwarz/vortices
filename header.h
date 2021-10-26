/**
 * \file  header.h
 * \brief Header for xmachine data structures and transition functions.
 */
#ifndef _HEADER_H
#define _HEADER_H

#ifdef _DEBUG_MODE
    #define ERRCHECK
#else
    #define NDEBUG
#endif

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include "mboard.h"

#define FLAME_TEST_PRINT_START_AND_END_OF_MODEL_FUNCTIONS 0
#define FLAME_TEST_PRINT_START_AND_END_OF_LIBMBOARD_CALLS 0
#define FLAME_USE_FILTERS_IN_SYNC 1

/*#define PAUSE printf("PAUSE - type anything to continue");i=getc(stdin);*/
#define PAUSE

#ifdef START_END
int start_simulation(void);
int end_simulation(void);
int start_iteration(void);
int end_iteration(void);
#endif


/* Checking macros */
#ifdef CHECK_MEMORY
#define CHECK_POINTER(PT) if(PT==NULL){printf("**** ERROR in Memory check 1\n");exit(1);}
#else
#define CHECK_POINTER(PT)
#endif










#define START_LOOP_OVER_Sporo_AGENTS  \
    current_xmachine_Sporo_holder = Sporo_start_state->agents;\
    while (current_xmachine_Sporo_holder) {\
	temp_xmachine_Sporo_holder = current_xmachine_Sporo_holder->next;\
	current_xmachine_Sporo = current_xmachine_Sporo_holder->agent;\
	current_xmachine->xmachine_Sporo = current_xmachine_Sporo;
#define END_LOOP_OVER_Sporo_AGENTS \
	current_xmachine_Sporo = NULL;\
	current_xmachine->xmachine_Sporo = NULL;\
	current_xmachine_Sporo_holder = temp_xmachine_Sporo_holder;\
    }





/** \def NNODES
 * \brief Provide access to environment variables in uppercase. */
#define NNODES (FLAME_get_environment_variable_NNODES())
/** \def WIDTH
 * \brief Provide access to environment variables in uppercase. */
#define WIDTH (FLAME_get_environment_variable_WIDTH())
/** \def DT
 * \brief Provide access to environment variables in uppercase. */
#define DT (FLAME_get_environment_variable_DT())
/** \def NOISE
 * \brief Provide access to environment variables in uppercase. */
#define NOISE (FLAME_get_environment_variable_NOISE())
/** \def DELTA
 * \brief Provide access to environment variables in uppercase. */
#define DELTA (FLAME_get_environment_variable_DELTA())
/** \def KEL
 * \brief Provide access to environment variables in uppercase. */
#define KEL (FLAME_get_environment_variable_KEL())
/** \def KI
 * \brief Provide access to environment variables in uppercase. */
#define KI (FLAME_get_environment_variable_KI())
/** \def KTH
 * \brief Provide access to environment variables in uppercase. */
#define KTH (FLAME_get_environment_variable_KTH())
/** \def SIDEBOX
 * \brief Provide access to environment variables in uppercase. */
#define SIDEBOX (FLAME_get_environment_variable_SIDEBOX())
/** \def SIDEX
 * \brief Provide access to environment variables in uppercase. */
#define SIDEX (FLAME_get_environment_variable_SIDEX())
/** \def SIDEY
 * \brief Provide access to environment variables in uppercase. */
#define SIDEY (FLAME_get_environment_variable_SIDEY())
/** \def ARRAY_BLOCK_SIZE
 * \brief The block size to allocate to dynamic arrays. */
#define ARRAY_BLOCK_SIZE 10
/** \def ARRAY_GROWTH_RATE
 * \brief Growth ratio to scale size of dynamic arrays. 
 * Use golden ratio. This makes for a Fibonacci sequence, so the next allocation size 
 * is the sum of the current size and the previous size thus improving the chance of 
 * memory block reuse. It also allows for moderate rate of growth.
 */
#define ARRAY_GROWTH_RATE 1.618034
/** \def START_STATE_MESSAGE_LOOP
 * \brief Start of loop to process state messages. */
#define START_STATE_MESSAGE_LOOP  for(state_message = get_first_state_message(); state_message != NULL; state_message = get_next_state_message(state_message)) {
/** \def FINISH_STATE_MESSAGE_LOOP
 * \brief Finish of loop to process state messages. */
#define FINISH_STATE_MESSAGE_LOOP }


struct FLAME_output
{
	int type; /* 0=snapshot 1=Sporo */
	int format; /* 0=XML */
	char * location;
	int period;
	int phase;
	int flag; /* Used when outputting to show it has been used */

	struct FLAME_output * next;
};
typedef struct FLAME_output FLAME_output;

/** \struct int_array
 * \brief Dynamic array to hold integers.
 *
 * Holds a pointer to an integer array and values for size and memory size.
 */
struct int_array
{
	int size;
	int total_size;

	int * array;
};

/** \struct float_array
 * \brief Dynamic array to hold floats.
 *
 * Holds a pointer to an float array and values for size and memory size.
 */
struct float_array
{
	int size;
	int total_size;

	float * array;
};

/** \struct double_array
 * \brief Dynamic array to hold doubles.
 *
 * Holds a pointer to a double array and values for size and memory size.
 */
struct double_array
{
	int size;
	int total_size;

	double * array;
};

/** \struct char_array
 * \brief Dynamic array to hold chars.
 *
 * Holds a pointer to a char array and values for size and memory size.
 */
struct char_array
{
	int size;
	int total_size;

	char * array;
};

/** \typedef struct int_array int_array
 * \brief Typedef for int_array struct.
 */
typedef struct int_array int_array;
/** \typedef struct float_array float_array
 * \brief Typedef for float_array struct.
 */
typedef struct float_array float_array;
/** \typedef struct double_array double_array
 * \brief Typedef for double_array struct.
 */
typedef struct double_array double_array;
/** \typedef struct char_array char_array
 * \brief Typedef for char_array struct.
 */
typedef struct char_array char_array;


/** \struct xmachine_memory_Sporo
 * \brief Holds memory of xmachine Sporo.
 */
struct xmachine_memory_Sporo
{
	int id;	/**< X-machine memory variable id of type int. */
	int iter;	/**< X-machine memory variable iter of type int. */
	float FP;	/**< X-machine memory variable FP of type float. */
	float K0;	/**< X-machine memory variable K0 of type float. */
	float TH0;	/**< X-machine memory variable TH0 of type float. */
	float R0;	/**< X-machine memory variable R0 of type float. */
	float coordx[11];	/**< X-machine memory variable coordx of type float. */
	float coordy[11];	/**< X-machine memory variable coordy of type float. */
	float_array xnbr_node0;	/**< X-machine memory variable xnbr_node0 of type float_array. */
	float_array xnbr_node1;	/**< X-machine memory variable xnbr_node1 of type float_array. */
	float_array xnbr_node2;	/**< X-machine memory variable xnbr_node2 of type float_array. */
	float_array xnbr_node3;	/**< X-machine memory variable xnbr_node3 of type float_array. */
	float_array xnbr_node4;	/**< X-machine memory variable xnbr_node4 of type float_array. */
	float_array xnbr_node5;	/**< X-machine memory variable xnbr_node5 of type float_array. */
	float_array xnbr_node6;	/**< X-machine memory variable xnbr_node6 of type float_array. */
	float_array xnbr_node7;	/**< X-machine memory variable xnbr_node7 of type float_array. */
	float_array xnbr_node8;	/**< X-machine memory variable xnbr_node8 of type float_array. */
	float_array xnbr_node9;	/**< X-machine memory variable xnbr_node9 of type float_array. */
	float_array xnbr_node10;	/**< X-machine memory variable xnbr_node10 of type float_array. */
	float_array ynbr_node0;	/**< X-machine memory variable ynbr_node0 of type float_array. */
	float_array ynbr_node1;	/**< X-machine memory variable ynbr_node1 of type float_array. */
	float_array ynbr_node2;	/**< X-machine memory variable ynbr_node2 of type float_array. */
	float_array ynbr_node3;	/**< X-machine memory variable ynbr_node3 of type float_array. */
	float_array ynbr_node4;	/**< X-machine memory variable ynbr_node4 of type float_array. */
	float_array ynbr_node5;	/**< X-machine memory variable ynbr_node5 of type float_array. */
	float_array ynbr_node6;	/**< X-machine memory variable ynbr_node6 of type float_array. */
	float_array ynbr_node7;	/**< X-machine memory variable ynbr_node7 of type float_array. */
	float_array ynbr_node8;	/**< X-machine memory variable ynbr_node8 of type float_array. */
	float_array ynbr_node9;	/**< X-machine memory variable ynbr_node9 of type float_array. */
	float_array ynbr_node10;	/**< X-machine memory variable ynbr_node10 of type float_array. */
};

/** \struct xmachine_memory_Sporo_holder
 * \brief Holds struct of memory of xmachine Sporo.
 */
struct xmachine_memory_Sporo_holder
{
	/*@dependent@*/ struct xmachine_memory_Sporo * agent;	/**< Pointer to X-machine memory Sporo. */

	/*@dependent@*/ struct xmachine_memory_Sporo_holder * prev;	/**< Pointer to previous Sporo agent in the list.  */
	/*@dependent@*/ struct xmachine_memory_Sporo_holder * next;	/**< Pointer to next Sporo agent in the list.  */
};

/** \struct xmachine_memory_Sporo_holder
 * \brief Holds struct of memory of xmachine Sporo.
 */
struct xmachine_memory_Sporo_state
{
	/*@dependent@*/ struct xmachine_memory_Sporo_holder * agents;	/**< Pointer to X-machine memory Sporo. */

	int count;	/**< Number of agents that were in this state.  */
};

/** \struct xmachine
 * \brief Holds xmachines.
 */
struct xmachine
{
	/*@dependent@*/ /*@null@*/ /*@out@*/ struct xmachine_memory_Sporo * xmachine_Sporo;	/**< Pointer to X-machine memory of type Sporo.  */
};

/** \var void* FLAME_m_state_composite_params\n
 * \brief Pointer to message sync agent composite params */
void* FLAME_m_state_composite_params;

/** \struct m_state
 * \brief Holds message of type state_message.
 */
struct m_state
{
	int id;	/**< Message memory variable id of type int. */
	float coordx[11];	/**< Message memory variable coordx of type float. */
	float coordy[11];	/**< Message memory variable coordy of type float. */
};

/** \typedef struct xmachine xmachine
 * \brief Typedef for xmachine struct.
 */
typedef struct xmachine xmachine;
/** \var typedef xmachine_memory_Sporo xmachine_memory_Sporo
 * \brief Typedef for xmachine_memory_Sporo struct.
 */
typedef struct xmachine_memory_Sporo xmachine_memory_Sporo;
/** \var typedef xmachine_memory_Sporo xmachine_memory_Sporo
 * \brief Typedef for xmachine_memory_Sporo struct.
 */
typedef struct xmachine_memory_Sporo_holder xmachine_memory_Sporo_holder;
/** \var typedef xmachine_memory_Sporo xmachine_memory_Sporo
 * \brief Typedef for xmachine_memory_Sporo struct.
 */
typedef struct xmachine_memory_Sporo_state xmachine_memory_Sporo_state;
/** \typedef m_state m_state
 * \brief Typedef for m_state struct.
 */
typedef struct m_state m_state;


/** \struct location
 * \brief Holds location for calculating space partitioning .
 */
struct location
{
	double point;		/**< Point on an axis. */

	struct location * next;	/**< Pointer to next location on the list. */
};

/** \struct node_information
 * \brief Holds node information .
 */
struct node_information
{
	int node_id;	/**< Node ID. */
	double partition_data[6];	/**< Defines bounding box. */
	int neighbours[10]; /**< Defines the neighbours to this node - -99 if no neighbour */
	int agents_in_halo;	/**< Number of agents in the halo region. */
	int agent_total;	/**< Total number of agents on the node. */
	struct xmachine * agents;	/**< Pointer to list of X-machines. */
	struct m_state * state_messages;	/**< Pointer to state message list. */

	struct node_information * next;	/**< Pointer to next node on the list. */
};


/** \typedef struct location location
 * \brief Typedef for location struct.
 */
typedef struct location location;
/** \typedef struct node_information node_information
 * \brief Typedef for node_information struct.
 */
typedef struct node_information node_information;

/** \var int NNODES
* \brief A constant variable from the environment. */
int FLAME_environment_variable_NNODES;
/** \var float WIDTH
* \brief A constant variable from the environment. */
float FLAME_environment_variable_WIDTH;
/** \var float DT
* \brief A constant variable from the environment. */
float FLAME_environment_variable_DT;
/** \var float NOISE
* \brief A constant variable from the environment. */
float FLAME_environment_variable_NOISE;
/** \var float DELTA
* \brief A constant variable from the environment. */
float FLAME_environment_variable_DELTA;
/** \var float KEL
* \brief A constant variable from the environment. */
float FLAME_environment_variable_KEL;
/** \var float KI
* \brief A constant variable from the environment. */
float FLAME_environment_variable_KI;
/** \var float KTH
* \brief A constant variable from the environment. */
float FLAME_environment_variable_KTH;
/** \var float SIDEBOX
* \brief A constant variable from the environment. */
float FLAME_environment_variable_SIDEBOX;
/** \var int SIDEX
* \brief A constant variable from the environment. */
int FLAME_environment_variable_SIDEX;
/** \var int SIDEY
* \brief A constant variable from the environment. */
int FLAME_environment_variable_SIDEY;
/** \var xmachine * temp_xmachine
* \brief Pointer to xmachine to initialise linked list. */
xmachine * temp_xmachine;

/** \var m_state * temp_state_message
* \brief Pointer to m_state to initialise linked list. */
m_state * temp_state_message;
/** \var node_information * temp_node_info
* \brief Pointer to node_information to initialise linked list. */
node_information * temp_node_info;
/** \var char outputpath[1000]
* \brief Output path for files. */
char outputpath[1000];
/** \var long total_time
* \brief Total time for the simulation run (in seconds) */
double total_time;
/** \var int total_messages
* \brief Total messages sent between nodes for the simulation run */
int total_messages;
/** \var int totalnodes
* \brief Number of nodes */
int totalnodes;
/** \var xmachine ** p_xmachine
* \brief Pointer to first pointer of x-machine memory list */
//xmachine ** p_xmachine;
/** \var xmachine * current_xmachine
* \brief Pointer to current x-machine memory that is being processed */
xmachine * current_xmachine;
/* Pointer to current $agent_name agent */
/*@dependent@*/ /*@null@*/ /*@out@*/ xmachine_memory_Sporo * current_xmachine_Sporo;
/*@dependent@*/ xmachine_memory_Sporo_holder * temp_xmachine_Sporo_holder;
/*@dependent@*/ /*@null@*/ /*@out@*/ xmachine_memory_Sporo_holder * current_xmachine_Sporo_holder;
xmachine_memory_Sporo_state * current_xmachine_Sporo_next_state; /* New agents added to this state */
/* Pointer to list of Sporo agents in state end state */
//xmachine_memory_Sporo * temp_xmachine_Sporo_end;
xmachine_memory_Sporo_state * Sporo_end_state;
/* Pointer to list of Sporo agents in state forces state */
//xmachine_memory_Sporo * temp_xmachine_Sporo_forces;
xmachine_memory_Sporo_state * Sporo_forces_state;
/* Pointer to list of Sporo agents in state read_states state */
//xmachine_memory_Sporo * temp_xmachine_Sporo_read_states;
xmachine_memory_Sporo_state * Sporo_read_states_state;
/* Pointer to list of Sporo agents in state start state */
//xmachine_memory_Sporo * temp_xmachine_Sporo_start;
xmachine_memory_Sporo_state * Sporo_start_state;



MBt_Board b_state;
MBt_Iterator i_state;


/** \var m_state * state_message
* \brief Pointer to message struct for looping through state message list */
m_state * state_message;
/** \var FLAME_output ** FLAME_outputs
* \brief Pointer to list of outputs */
FLAME_output * FLAME_outputs;
/** \var node_information * p_node_info
* \brief Pointer to first pointer of node list */
node_information ** p_node_info;
/** \var node_information * current_node
* \brief Pointer to current node */
node_information * current_node;


/** \var int node_number\n
 *  \brief Node number (not needed for serial codes) */
int node_number;



/** \var int iteration_loop
* \brief The current iteration number */
int iteration_loop;
/** \var int output_frequency
* \brief Frequency to output results */
int output_frequency;
/** \var int output_offset
* \brief Offset to output results */
int output_offset;

/** \def SPINF
* \brief Dummy inf value for space partition data. */
#define SPINF 999999.123456
/** \def RELEASE
* \brief Used to kill an agent via 'return RELEASE;'. */
#define RELEASE 1
/** \def kill_me_now
* \brief Used to kill an agent via 'kill_me_now'. */
#define kill_me_now return 1

void initialise_pointers(void);
void initialise_unit_testing(void);
FLAME_output * add_FLAME_output(FLAME_output ** outputs);
void free_FLAME_outputs(FLAME_output ** outputs);
void add_location(double point, location ** p_location);
void freelocations(location ** p_location);
void add_node(int node_id, double minx, double maxx, double miny, double maxy, double minz, double maxz);
void clean_up(int code);
void propagate_agents(void);
void propagate_messages_init(void);
void propagate_messages_complete(void);
void create_partitions(char * filename, int * itno);
void free_node_info(void);
void free_agent(void);
void freexmachines(void);
/* model datatypes */

void init_int_static_array(/*@out@*/ int * array, int size);
void init_int_array(int_array * array);
void reset_int_array(int_array * array);
void free_int_array(int_array * array);
void copy_int_array(int_array * from, int_array * to);
//void sort_int_array(int_array array);
//int quicksort_int(int array, int elements);
void add_int(int_array * array, int new_int);
void remove_int(int_array * array, int index);
void print_int_array(int_array * array);
void init_float_static_array(float * array, int size);
void init_float_array(float_array * array);
void reset_float_array(float_array * array);
void free_float_array(float_array * array);
void copy_float_array(float_array * from, float_array * to);
//void sort_float_array(float_array array);
//int quicksort_float(float array, int elements);
void add_float(float_array * array, float new_float);
void remove_float(float_array * array, int index);
void print_float_array(float_array * array);
void init_double_static_array(/*@out@*/ double* array, int size);
void init_double_array(double_array * array);
void reset_double_array(double_array * array);
void free_double_array(double_array * array);
void copy_double_array(double_array * from, double_array * to);
//void sort_double_array(double_array array);
//int quicksort_double(double array, int elements);
void add_double(double_array * array, double new_double);
void remove_double(double_array * array, int index);
void print_double_array(double_array * array);
void init_char_static_array(/*@out@*/ char * array, int size);
void init_char_array(char_array * array);
void reset_char_array(char_array * array);
void free_char_array(char_array * array);
void copy_char_array(char_array * from, char_array * to);
void add_char(char_array * array, char new_char);
void remove_char(char_array * array, int index);
char * copy_array_to_str(char_array * array);
void print_char_array(char_array * array);
int idle(void);
/* xml.c */
int read_int_static_array(char * buffer, int buffer_size, int * j, int * int_static_array, int size);
int read_float_static_array(char * buffer, int buffer_size, int * j, float * float_static_array, int size);
int read_double_static_array(char * buffer, int buffer_size, int * j, double * double_static_array, int size);
int read_char_static_array(char * buffer, int buffer_size, int * j, char * char_static_array, int size);
int read_int_dynamic_array(char * buffer, int buffer_size, int * j, int_array * int_dynamic_array);
int read_float_dynamic_array(char * buffer, int buffer_size, int * j, float_array * float_dynamic_array);
int read_double_dynamic_array(char * buffer, int buffer_size, int * j, double_array * double_dynamic_array);
int read_char_dynamic_array(char * buffer, int buffer_size, int * j, char_array * char_dynamic_array);


void readinitialstates(char * filename, char * filelocation, int * itno, double cloud_data[],
					   int partition_method, int flag);
void saveiterationdata(int iteration_number);


xmachine_memory_Sporo_state * init_Sporo_state();
xmachine_memory_Sporo * init_Sporo_agent();
void free_Sporo_agent(xmachine_memory_Sporo_holder * tmp, xmachine_memory_Sporo_state * state);
void transition_Sporo_agent(xmachine_memory_Sporo_holder * tmp, xmachine_memory_Sporo_state * from_state, xmachine_memory_Sporo_state * to_state);
void add_Sporo_agent_internal(xmachine_memory_Sporo * agent, xmachine_memory_Sporo_state * state);
void add_Sporo_agent(int id, int iter, float FP, float K0, float TH0, float R0, float coordx[], float coordy[], float_array * xnbr_node0, float_array * xnbr_node1, float_array * xnbr_node2, float_array * xnbr_node3, float_array * xnbr_node4, float_array * xnbr_node5, float_array * xnbr_node6, float_array * xnbr_node7, float_array * xnbr_node8, float_array * xnbr_node9, float_array * xnbr_node10, float_array * ynbr_node0, float_array * ynbr_node1, float_array * ynbr_node2, float_array * ynbr_node3, float_array * ynbr_node4, float_array * ynbr_node5, float_array * ynbr_node6, float_array * ynbr_node7, float_array * ynbr_node8, float_array * ynbr_node9, float_array * ynbr_node10);
void unittest_init_Sporo_agent();
void unittest_free_Sporo_agent();

void add_state_message(int id, float coordx[], float coordy[]);
m_state * add_state_message_internal(void);
m_state * get_first_state_message(void);
m_state * get_next_state_message(m_state * current);
void freestatemessages(void);


void set_id(int id);
int get_id();
void set_iter(int iter);
int get_iter();
void set_FP(float FP);
float get_FP();
void set_K0(float K0);
float get_K0();
void set_TH0(float TH0);
float get_TH0();
void set_R0(float R0);
float get_R0();
float * get_coordx();
float * get_coordy();
float_array * get_xnbr_node0();
float_array * get_xnbr_node1();
float_array * get_xnbr_node2();
float_array * get_xnbr_node3();
float_array * get_xnbr_node4();
float_array * get_xnbr_node5();
float_array * get_xnbr_node6();
float_array * get_xnbr_node7();
float_array * get_xnbr_node8();
float_array * get_xnbr_node9();
float_array * get_xnbr_node10();
float_array * get_ynbr_node0();
float_array * get_ynbr_node1();
float_array * get_ynbr_node2();
float_array * get_ynbr_node3();
float_array * get_ynbr_node4();
float_array * get_ynbr_node5();
float_array * get_ynbr_node6();
float_array * get_ynbr_node7();
float_array * get_ynbr_node8();
float_array * get_ynbr_node9();
float_array * get_ynbr_node10();
int agent_get_id(void);
double agent_get_x(void);
double agent_get_y(void);
double agent_get_z(void);
/* partitioning.c */
void partition_data(int totalnodes, xmachine ** agent_list, double cloud_data[], int partition_method);


void save_partition_data(void);
void generate_partitions(double cloud_data[], int partitions, int partition_method);

/* messageboard.c */
m_state * get_next_message_state_in_range(m_state * current);




/* memory.c */
xmachine * add_xmachine(void);
int FLAME_get_environment_variable_NNODES();
float FLAME_get_environment_variable_WIDTH();
float FLAME_get_environment_variable_DT();
float FLAME_get_environment_variable_NOISE();
float FLAME_get_environment_variable_DELTA();
float FLAME_get_environment_variable_KEL();
float FLAME_get_environment_variable_KI();
float FLAME_get_environment_variable_KTH();
float FLAME_get_environment_variable_SIDEBOX();
int FLAME_get_environment_variable_SIDEX();
int FLAME_get_environment_variable_SIDEY();


/* rules.c */
int FLAME_integer_in_array(int a, int * b, int size);

/* timing.c */
double get_time(void);

int write_state(void);
int read_states(void);
int forces(void);
#endif

/**
 * \file  main.c
 * \brief Holds main function of the program.
 */
#include "header.h"
#include <time.h>



#define COMPACT_PRINTOUT_P_THRESHOLD 8

/** \fn int main(int argc, char * argv[])
 * \brief Main program loop.
 * \param argc Argument count.
 * \param argv Pointer Pointer to Argument vector.
 */
int main(int argc, char * argv[])
{
	/* Timing variables */
	double start, stop, interval;
	FILE *file;
	char data[100];
	char logfilepath[1000];
	char inputpath[1000];
	char * c;
	int lastd = 0;
	int i;
	int rc;
	int iteration_number = 0;
	int iteration_total;
	int * p_iteration_number = &iteration_number;
	//xmachine_memory_Sporo * temp_xmachine_Sporo;

	int FLAME_state_message_board_write;
	int FLAME_state_message_board_read;

	/* Particle cloud data */
	double cloud_data[6] = {0.0,0.0,0.0,0.0,0.0,0.0};


/* For partition method. Makes geometric (-g flag) the default but may be overridden with -r for round-robin */
	int partition_method=1;



	/* Output frequency is 1 as default */
	output_frequency = 1;

	/* Set random seed */
	srand((unsigned int) time(NULL));


	

	rc = MB_Env_Init();
	#ifdef ERRCHECK
	if (rc != MB_SUCCESS)
	{
	   fprintf(stderr, "ERROR: Failed to initialise Message Board environment\n");
	   switch(rc) {
	       case MB_ERR_MPI:
	           fprintf(stderr, "\t reason: MPI library not initialised\n");
	           break;
	       case MB_ERR_MEMALLOC:
	           fprintf(stderr, "\t reason: out of memory\n");
	           break;
           default:
               fprintf(stderr, "\t MB_Env_Init returned error code: %d (see libmboard docs for details)\n", rc);
               break;
	   }

	   
       exit(rc);

	}
	#endif
    
	/* For backwards compatabilty allocate current_xmachine */
	current_xmachine = (xmachine *)malloc(sizeof(xmachine));
	if(current_xmachine == NULL) {printf("**** ERROR in Memory check current_xmachine\n");exit(EXIT_FAILURE);}
	/*CHECK_POINTER(current_xmachine);*/

	/* Initialise pointers */
	initialise_pointers();


	printf("FLAME Application: More Spos \n");






if(argc < 2)
	{

		printf("Usage: %s <number of iterations> [<states_directory>]/<init_state> <partitions> [-f # | -f #+#]\n",argv[0]);
		printf("\t-f\tOutput frequency, 1st # is frequency, 2nd # is the offset if required\n");



		exit(0);
	}
	iteration_total = atoi(argv[1]);
printf("Iterations: %i\n", iteration_total);

	/* Read initial states of x-machines */
	if(argc < 3)
	{
		printf("Need two parameters\n");
		exit(0);
	}
	strcpy(inputpath, argv[2]);
/*printf("Initial states: %s\n", inputpath);*/

	i = 0;
	lastd = -1;
	while(inputpath[i] != '\0')
	{
		/* For windows directories */
		if(inputpath[i] == '\\') lastd=i;
		/* For unix directories */
		if(inputpath[i] == '/') lastd=i;
		i++;
	}
	strcpy(outputpath, inputpath);
	outputpath[lastd+1] = '\0';

/*printf("Ouput dir: %s\n", outputpath);*/


	/* Read number of space partitions (1 by default) */
	totalnodes = 1;
	if(argc > 3)
	{
		totalnodes = atoi(argv[3]);
	}


	i = 3;
	while(argc > i)
	{
		if(strcmp(argv[i],"-f") == 0)
		{
			if(argc > (i+1))
			{
				output_offset = 0;
				/* Find offset, separated by the char '+' */
				c = strchr(argv[(i+1)], '+');
				if(c == NULL)
				{
					output_frequency = atoi(argv[(i+1)]);
					printf("Using output frequency of: %d\n", output_frequency);
				}
				else
				{
					output_offset = atoi(argv[(i+1)]+(c-argv[(i+1)])+1);
					argv[(i+1)][c-argv[(i+1)]] = '\0';
					output_frequency = atoi(argv[(i+1)]);
					printf("Using output frequency of: %d with offset %d\n", output_frequency, output_offset);
				}

				if(output_frequency == 0)
				{
					printf("Output frequency cannot be zero\n");
					exit(0);
				}

				i++;
			}
			else
			{
				printf("Output frequency number not defined\n");
				exit(0);
			}
		}
		/* Partitioning method: -g = geometric, -r = round-robin */
		if(strcmp(argv[i],"-g") == 0) partition_method = 1;
		if(strcmp(argv[i],"-r") == 0) partition_method = 2;
		i++;
	}


	    /* Read initial data into p_xmachine  */

       //agent_list = p_xmachine;
       readinitialstates(inputpath, outputpath, p_iteration_number, cloud_data, partition_method, 0);
       /* Generate partitions */
       generate_partitions(cloud_data,totalnodes,partition_method);
       save_partition_data();





    /* Partition data */
    /* stc: no partitions in serial */
	//partition_data(totalnodes, agent_list, cloud_data, partition_method);





/* Use MB_IndexMap routines from libmboard v0.2 */
/* For each agent constant (that is used in a filter(?))
 * declare an index map handle */



		/*i = 0;
		current_node = *p_node_info;
		while(current_node)
		{
			printf("No of agents on partition %d: %d\n", current_node->node_id, current_node->agent_total);
			i += current_node->agent_total;
			current_node = current_node->next;
		}
		printf("Agent total check: %d\n", i);*/

        /* restore current_node pointer */
		//current_node = *p_node_info;



	/* Start log file (now so that xvisualiser can read straight away) */

	/* Write log file */
	sprintf(logfilepath, "%slog.xml", outputpath);
	if((file = fopen(logfilepath, "w"))==NULL)
	{
		printf("Error: cannot open file '%s' for writing\n", logfilepath);
		exit(0);
	}
	(void)fputs("<model_run>\n", file);
	(void)fputs("<codetype>", file);
	(void)fputs("serial", file);

	(void)fputs("</codetype>\n", file);
	(void)fputs("<nodes>", file);
	sprintf(data, "%i", totalnodes);
	(void)fputs(data, file);
	(void)fputs("</nodes>\n", file);

	/* print timer into */
	(void)fputs("<!-- <time> unit: milliseconds -->\n", file);

	sprintf(data, "unspecified");
	
	(void)fputs("<!-- <time> timer resolution: ", file);
	(void)fputs(data, file);
	(void)fputs(")-->\n", file);

	start = get_time();
	stop = get_time();
	sprintf(data, "%.2e ms", (stop - start) * 1000.0);
	(void)fputs("<!-- <time> timer overhead: ~", file);
	(void)fputs(data, file);
	(void)fputs(")-->\n", file);


	(void)fclose(file);


	/* For each message check if their exists agents that input/output the message */
	FLAME_state_message_board_write = 0;
	FLAME_state_message_board_read = 0;
	/* Sending agents */
	if(Sporo_start_state->agents != NULL) FLAME_state_message_board_write = 1;
	
	/* Reading agents */
	if(Sporo_start_state->agents != NULL) FLAME_state_message_board_read = 1;
	
	/* Call message board library with details */
	if(FLAME_state_message_board_write == 0 &&
		FLAME_state_message_board_read == 0)
			rc = MB_SetAccessMode(b_state, MB_MODE_IDLE);
	if(FLAME_state_message_board_write == 1 &&
		FLAME_state_message_board_read == 0)
			rc = MB_SetAccessMode(b_state, MB_MODE_WRITEONLY);
	if(FLAME_state_message_board_write == 0 &&
		FLAME_state_message_board_read == 1)
			rc = MB_SetAccessMode(b_state, MB_MODE_READONLY);
	if(FLAME_state_message_board_write == 1 &&
		FLAME_state_message_board_read == 1)
			rc = MB_SetAccessMode(b_state, MB_MODE_READWRITE);
	#ifdef ERRCHECK
	if (rc != MB_SUCCESS)
	{
	   fprintf(stderr, "ERROR: Could not set access mode of 'state' board\n");
	   switch(rc) {
		   case MB_ERR_INVALID:
			   fprintf(stderr, "\t reason: 'state' board is invalid\n");
			   break;
		   case MB_ERR_LOCKED:
			   fprintf(stderr, "\t reason: 'state' board is locked\n");
			   break;
		   case MB_ERR_MEMALLOC:
			   fprintf(stderr, "\t reason: out of memory\n");
			   break;
		   case MB_ERR_INTERNAL:
			   fprintf(stderr, "\t reason: internal error. Recompile libmoard in debug mode for more info \n");
			   break;
		   default:
			   fprintf(stderr, "\t MB_SyncStart returned error code: %d (see libmboard docs for details)\n", rc);
			   break;
	   }
		   
		   exit(rc);
	}
	#endif
	

#ifdef START_END	
/* Call initialisation function */
	start_simulation();
#endif

/* Start timing */
	start = get_time();

	for(iteration_loop=iteration_number+1; iteration_loop < iteration_number+iteration_total+1; iteration_loop++)
	{
#ifdef START_END
/* Start iteration function */
	start_iteration();
#endif
		interval = get_time();

        /* Call message board library with details */
        if(FLAME_state_message_board_write == 0 &&
            FLAME_state_message_board_read == 0)
                rc = MB_SetAccessMode(b_state, MB_MODE_IDLE);
        if(FLAME_state_message_board_write == 1 &&
            FLAME_state_message_board_read == 0)
                rc = MB_SetAccessMode(b_state, MB_MODE_WRITEONLY);
        if(FLAME_state_message_board_write == 0 &&
            FLAME_state_message_board_read == 1)
                rc = MB_SetAccessMode(b_state, MB_MODE_READONLY);
        if(FLAME_state_message_board_write == 1 &&
            FLAME_state_message_board_read == 1)
                rc = MB_SetAccessMode(b_state, MB_MODE_READWRITE);
        #ifdef ERRCHECK
        if (rc != MB_SUCCESS)
        {
           fprintf(stderr, "ERROR: Could not set access mode of 'state' board\n");
           switch(rc) {
               case MB_ERR_INVALID:
                   fprintf(stderr, "\t reason: 'state' board is invalid\n");
                   break;
               case MB_ERR_LOCKED:
                   fprintf(stderr, "\t reason: 'state' board is locked\n");
                   break;
               case MB_ERR_MEMALLOC:
                   fprintf(stderr, "\t reason: out of memory\n");
                   break;
               case MB_ERR_INTERNAL:
                   fprintf(stderr, "\t reason: internal error. Recompile libmoard in debug mode for more info \n");
                   break;
               default:
                   fprintf(stderr, "\t MB_SyncStart returned error code: %d (see libmboard docs for details)\n", rc);
                   break;
           }
               
               exit(rc);
        }
        #endif
        
        
		
		/* Start sync message boards that don't write */
		if(FLAME_state_message_board_write == 0)
		{
			/*printf("%d> state message board sync start early as no agents sending any messages of this type\n", node_number);*/
			
			/* ********** sync message board here **********  */
			if(FLAME_TEST_PRINT_START_AND_END_OF_LIBMBOARD_CALLS) printf("start MB_SyncStart(b_state)\n");
			rc = MB_SyncStart(b_state);
			if(FLAME_TEST_PRINT_START_AND_END_OF_LIBMBOARD_CALLS) printf("finish MB_SyncStart(b_state)\n");
			#ifdef ERRCHECK
			if (rc != MB_SUCCESS)
			{
			   fprintf(stderr, "ERROR: Could not start sync of 'state' board\n");
			   switch(rc) {
				   case MB_ERR_INVALID:
					   fprintf(stderr, "\t reason: 'state' board is invalid\n");
					   break;
				   case MB_ERR_LOCKED:
					   fprintf(stderr, "\t reason: 'state' board is locked\n");
					   break;
				   case MB_ERR_MEMALLOC:
					   fprintf(stderr, "\t reason: out of memory\n");
					   break;
				   case MB_ERR_INTERNAL:
					   fprintf(stderr, "\t reason: internal error. Recompile libmoard in debug mode for more info \n");
					   break;
				   default:
					   fprintf(stderr, "\t MB_SyncStart returned error code: %d (see libmboard docs for details)\n", rc);
					   break;
			   }
			   
					   
					   exit(rc);
			   }
			   #endif
		}
		
		

	if(FLAME_TEST_PRINT_START_AND_END_OF_MODEL_FUNCTIONS) printf("start write_state\n");
	current_xmachine_Sporo_holder = Sporo_start_state->agents;
	while(current_xmachine_Sporo_holder)
	{
		temp_xmachine_Sporo_holder = current_xmachine_Sporo_holder->next;
		current_xmachine_Sporo = current_xmachine_Sporo_holder->agent;
		current_xmachine_Sporo_next_state = Sporo_read_states_state;
		/* For backwards compatibility set current_xmachine */
		current_xmachine->xmachine_Sporo = NULL;
		current_xmachine->xmachine_Sporo = current_xmachine_Sporo;

		

		

			i = write_state();

		

			if(i == 1)
			{
				free_Sporo_agent(current_xmachine_Sporo_holder, Sporo_start_state);
			}
			else
			{
				transition_Sporo_agent(current_xmachine_Sporo_holder, Sporo_start_state, Sporo_read_states_state);
			}
		

		current_xmachine_Sporo = NULL;

		current_xmachine_Sporo_holder = temp_xmachine_Sporo_holder;
	}
	if(FLAME_TEST_PRINT_START_AND_END_OF_MODEL_FUNCTIONS) printf("finish write_state\n");

	if(FLAME_state_message_board_write == 1)
	{

		if(FLAME_TEST_PRINT_START_AND_END_OF_LIBMBOARD_CALLS) printf("start MB_SyncStart(b_state)\n");
		rc = MB_SyncStart(b_state);
		if(FLAME_TEST_PRINT_START_AND_END_OF_LIBMBOARD_CALLS) printf("finish MB_SyncStart(b_state)\n");
		#ifdef ERRCHECK
		if (rc != MB_SUCCESS)
		{
		   fprintf(stderr, "ERROR: Could not start sync of 'state' board\n");
		   switch(rc) {
			   case MB_ERR_INVALID:
				   fprintf(stderr, "\t reason: 'state' board is invalid\n");
				   break;
			   case MB_ERR_LOCKED:
				   fprintf(stderr, "\t reason: 'state' board is locked\n");
				   break;
			   case MB_ERR_MEMALLOC:
				   fprintf(stderr, "\t reason: out of memory\n");
				   break;
			   case MB_ERR_INTERNAL:
				   fprintf(stderr, "\t reason: internal error. Recompile libmoard in debug mode for more info \n");
				   break;
			   default:
				   fprintf(stderr, "\t MB_SyncStart returned error code: %d (see libmboard docs for details)\n", rc);
				   break;
		   }

			
			exit(rc);
		}
		#endif
    }
    


/* End of layer number 0 */

/* Clear message boards that have finished being used
 * and sync complete if doing late sync complete */


	/* If mb is not read then leave sync complete until last possible moment */
	if(FLAME_state_message_board_read == 1)
	{
		if(FLAME_TEST_PRINT_START_AND_END_OF_LIBMBOARD_CALLS) printf("start MB_SyncComplete(b_state)\n");
		rc = MB_SyncComplete(b_state);
		if(FLAME_TEST_PRINT_START_AND_END_OF_LIBMBOARD_CALLS) printf("finsh MB_SyncComplete(b_state)\n");
		#ifdef ERRCHECK
		if (rc != MB_SUCCESS)
		{
		   fprintf(stderr, "ERROR: Could not complete sync of 'state' board\n");
		   switch(rc) {
				case MB_ERR_INVALID:
				   fprintf(stderr, "\t reason: 'state' board is invalid\n");
				   break;
			   case MB_ERR_MEMALLOC:
				   fprintf(stderr, "\t reason: out of memory\n");
				   break;
			   case MB_ERR_INTERNAL:
				   fprintf(stderr, "\t reason: internal error. Recompile libmoard in debug mode for more info \n");
				   break;
			   default:
				   fprintf(stderr, "\t MB_SyncComplete returned error code: %d (see libmboard docs for details)\n", rc);
				   break;
		   }
	
		   
		   exit(rc);
		}
		#endif
    
    
    
	}
	
	if(FLAME_TEST_PRINT_START_AND_END_OF_MODEL_FUNCTIONS) printf("start read_states\n");
	current_xmachine_Sporo_holder = Sporo_read_states_state->agents;
	while(current_xmachine_Sporo_holder)
	{
		temp_xmachine_Sporo_holder = current_xmachine_Sporo_holder->next;
		current_xmachine_Sporo = current_xmachine_Sporo_holder->agent;
		current_xmachine_Sporo_next_state = Sporo_forces_state;
		/* For backwards compatibility set current_xmachine */
		current_xmachine->xmachine_Sporo = NULL;
		current_xmachine->xmachine_Sporo = current_xmachine_Sporo;

		

		
		
		
		  rc = MB_Iterator_Create(b_state, &i_state);
		  
		
		#ifdef ERRCHECK
		if (rc != MB_SUCCESS)
		{
		   fprintf(stderr, "ERROR: Could not create Iterator for 'state'\n");
		   switch(rc) {
		       case MB_ERR_INVALID:
		           fprintf(stderr, "\t reason: 'state' board is invalid\n");
		           break;
		       case MB_ERR_LOCKED:
	               fprintf(stderr, "\t reason: 'state' board is locked\n");
	               break;
	           case MB_ERR_MEMALLOC:
	               fprintf(stderr, "\t reason: out of memory\n");
	               break;
	           case MB_ERR_INTERNAL:
	               fprintf(stderr, "\t reason: internal error. Recompile libmoard in debug mode for more info \n");
	               break;
	           default:
	           
               
                   fprintf(stderr, "\t MB_Iterator_Create returned error code: %d (see libmboard docs for details)\n", rc);
               
                   break;
		   }

		   
           exit(rc);
		}
		#endif
		
		

			i = read_states();

		
		    rc = MB_Iterator_Delete(&i_state);
		    #ifdef ERRCHECK
		    if (rc != MB_SUCCESS)
		    {
		       fprintf(stderr, "ERROR: Could not delete 'state' iterator\n");
		       switch(rc) {
		           case MB_ERR_INVALID:
		               fprintf(stderr, "\t reason: 'state' iterator is invalid\n");
		               break;
		           case MB_ERR_INTERNAL:
		               fprintf(stderr, "\t reason: internal error. Recompile libmoard in debug mode for more info \n");
		               break;
		           default:
                       fprintf(stderr, "\t MB_Iterator_Delete returned error code: %d (see libmboard docs for details)\n", rc);
                       break;
		       }

		       
               exit(rc);
		    }
		    #endif
		

			if(i == 1)
			{
				free_Sporo_agent(current_xmachine_Sporo_holder, Sporo_read_states_state);
			}
			else
			{
				transition_Sporo_agent(current_xmachine_Sporo_holder, Sporo_read_states_state, Sporo_forces_state);
			}
		

		current_xmachine_Sporo = NULL;

		current_xmachine_Sporo_holder = temp_xmachine_Sporo_holder;
	}
	if(FLAME_TEST_PRINT_START_AND_END_OF_MODEL_FUNCTIONS) printf("finish read_states\n");


/* End of layer number 1 */

/* Clear message boards that have finished being used
 * and sync complete if doing late sync complete */

if(FLAME_state_message_board_read == 0)
{
	/*printf("%d> state message board sync complete late as no agents reading any messages of this type\n", node_number);*/
	
	if(FLAME_TEST_PRINT_START_AND_END_OF_LIBMBOARD_CALLS) printf("start MB_SyncComplete(b_state)\n");
	rc = MB_SyncComplete(b_state);
	if(FLAME_TEST_PRINT_START_AND_END_OF_LIBMBOARD_CALLS) printf("finsh MB_SyncComplete(b_state)\n");
	#ifdef ERRCHECK
	if (rc != MB_SUCCESS)
	{
	   fprintf(stderr, "ERROR: Could not complete sync of 'state' board\n");
	   switch(rc) {
			case MB_ERR_INVALID:
			   fprintf(stderr, "\t reason: 'state' board is invalid\n");
			   break;
		   case MB_ERR_MEMALLOC:
			   fprintf(stderr, "\t reason: out of memory\n");
			   break;
		   case MB_ERR_INTERNAL:
			   fprintf(stderr, "\t reason: internal error. Recompile libmoard in debug mode for more info \n");
			   break;
		   default:
			   fprintf(stderr, "\t MB_SyncComplete returned error code: %d (see libmboard docs for details)\n", rc);
			   break;
	   }

	   
	   exit(rc);
	}
	#endif
}

    /* Delete any search trees */

    rc = MB_Clear(b_state);
    #ifdef ERRCHECK
    if (rc != MB_SUCCESS)
    {
       fprintf(stderr, "ERROR: Could not clear 'state' board\n");
       switch(rc) {
           case MB_ERR_INVALID:
               fprintf(stderr, "\t reason: 'state' board is invalid\n");
               break;
           case MB_ERR_LOCKED:
               fprintf(stderr, "\t reason: 'state' board is locked\n");
               break;
           case MB_ERR_INTERNAL:
               fprintf(stderr, "\t reason: internal error. Recompile libmoard in debug mode for more info \n");
               break;
           default:
               fprintf(stderr, "\t MB_Clear returned error code: %d (see libmboard docs for details)\n", rc);
               break;

       }

       
       exit(rc);
    }
    #endif


	if(FLAME_TEST_PRINT_START_AND_END_OF_MODEL_FUNCTIONS) printf("start forces\n");
	current_xmachine_Sporo_holder = Sporo_forces_state->agents;
	while(current_xmachine_Sporo_holder)
	{
		temp_xmachine_Sporo_holder = current_xmachine_Sporo_holder->next;
		current_xmachine_Sporo = current_xmachine_Sporo_holder->agent;
		current_xmachine_Sporo_next_state = Sporo_end_state;
		/* For backwards compatibility set current_xmachine */
		current_xmachine->xmachine_Sporo = NULL;
		current_xmachine->xmachine_Sporo = current_xmachine_Sporo;

		

		

			i = forces();

		

			if(i == 1)
			{
				free_Sporo_agent(current_xmachine_Sporo_holder, Sporo_forces_state);
			}
			else
			{
				transition_Sporo_agent(current_xmachine_Sporo_holder, Sporo_forces_state, Sporo_end_state);
			}
		

		current_xmachine_Sporo = NULL;

		current_xmachine_Sporo_holder = temp_xmachine_Sporo_holder;
	}
	if(FLAME_TEST_PRINT_START_AND_END_OF_MODEL_FUNCTIONS) printf("finish forces\n");


/* End of layer number 2 */

/* Clear message boards that have finished being used
 * and sync complete if doing late sync complete */


	if(iteration_loop%output_frequency == output_offset)
	{
		saveiterationdata(iteration_loop);
	}

	/*printf("Sporo_end_state->count = %d\n", Sporo_end_state->count);*/
	Sporo_end_state->count = 0;

	/*printf("Sporo_forces_state->count = %d\n", Sporo_forces_state->count);*/
	Sporo_forces_state->count = 0;

	/*printf("Sporo_read_states_state->count = %d\n", Sporo_read_states_state->count);*/
	Sporo_read_states_state->count = 0;

	/*printf("Sporo_start_state->count = %d\n", Sporo_start_state->count);*/
	Sporo_start_state->count = 0;

	/* Move agents to their start states */

	current_xmachine_Sporo_holder = Sporo_end_state->agents;
	while(current_xmachine_Sporo_holder)
	{
		temp_xmachine_Sporo_holder = current_xmachine_Sporo_holder->next;
		transition_Sporo_agent(current_xmachine_Sporo_holder, Sporo_end_state, Sporo_start_state);

		current_xmachine_Sporo_holder = temp_xmachine_Sporo_holder;
	}

    if(iteration_loop%output_frequency == output_offset)
    {
        saveiterationdata(iteration_loop);
    }

		/* Calculate if any agents need to jump S.P. */
		/* propagate_agents(); */
	/* Save iteration time to log file */

		if((file = fopen(logfilepath, "a"))==NULL)
		{
			printf("Error: cannot open file '%s' for writing\n", logfilepath);
			exit(0);
		}
		(void)fputs("<iteration><no>", file);
		sprintf(data, "%i", iteration_loop);
		(void)fputs(data, file);
		(void)fputs("</no><time>", file);
		sprintf(data, "%d", (int)((get_time() - interval) * 1000) );
		(void)fputs(data, file);
		(void)fputs("</time></iteration>\n", file);
		(void)fclose(file);


#ifdef START_END
/* End iteration function */
	end_iteration();
#endif
	}

#ifdef START_END
/* End simualtion function */
	end_simulation();
#endif

    



	/* Stop timing and print total time */
	stop = get_time();
	total_time = stop - start;
	printf("Execution time - %d:%02d:%03d [mins:secs:msecs]\n",
       (int)(total_time/60), ((int)total_time)%60, (((int)(total_time * 1000.0)) % 1000));

	clean_up(0);

	/* Exit successfully by returning zero to Operating System */
	return 0;
}

/**
 * \file  xml.c
 * \brief Holds xml reading and writing functions.
 */

#include "header.h"

/** \fn void read_int_static_array(char * buffer, int * j, int ** int_static_array)
 * \brief Reads integer static array.
 */
int read_int_static_array(char * buffer, int /*@unused@*/ buffer_size, int * j, int * int_static_array, int /*@unused@*/ size)
{
	int arraycount = 0;
	int array_k;
	char arraydata[100000];
	
	while(buffer[(*j)] != '{')
	{
		if(buffer[(*j)] != ' ') return -1;
		else if(buffer[(*j)] == '\0') return -1;
		else (*j)++;
	}
	(*j)++;

	while(buffer[(*j)] != '}')
	{
		array_k = 0;
		while(buffer[(*j)] != ',' && buffer[(*j)] != '}')
		{
			if(buffer[(*j)] == '\0') return -1;
			arraydata[array_k] = buffer[(*j)];
			array_k++;
			(*j)++;
		}
		arraydata[array_k] = '\0';
		if (arraycount < size) int_static_array[arraycount] = atoi(arraydata);
		arraycount++;
		if(buffer[(*j)] != '}') (*j)++;
	}

	if (arraycount > size)
	{
				fprintf(stderr, "WARNING: %d 'int' values provided for static array of size %d. Surplus values ignored.\n",
				arraycount, size);
	}
	
	(*j)++;
	return 0;
}

/** \fn void read_float_static_array(char * buffer, int * (*j), int ** float_static_array)
 * \brief Reads float static array.
 */
int read_float_static_array(char * buffer, int /*@unused@*/ buffer_size, int * j, float * float_static_array, int /*@unused@*/ size)
{
	int arraycount = 0;
	int array_k;
	char arraydata[100000];

	while(buffer[(*j)] != '{')
	{
		if(buffer[(*j)] != ' ') return -1;
		else if(buffer[(*j)] == '\0') return -1;
		else (*j)++;
	}
	(*j)++;

	while(buffer[(*j)] != '}')
	{
		array_k = 0;
		while(buffer[(*j)] != ',' && buffer[(*j)] != '}')
		{
			if(buffer[(*j)] == '\0') return -1;
			arraydata[array_k] = buffer[(*j)];
			array_k++;
			(*j)++;
		}
		arraydata[array_k] = '\0';
		if (arraycount < size) float_static_array[arraycount] = (float)atof(arraydata);
		arraycount++;
		if(buffer[(*j)] != '}') (*j)++;
	}
	
	if (arraycount > size)
	{
				fprintf(stderr, "WARNING: %d 'float' values provided for static array of size %d. Surplus values ignored.\n",
				arraycount, size);
	}
	
	(*j)++;
	return 0;
}

/** \fn void read_double_static_array(char * buffer, int * (*j), int ** double_static_array)
 * \brief Reads double static array.
 */
int read_double_static_array(char * buffer, int /*@unused@*/ buffer_size, int * j, double * double_static_array, int /*@unused@*/ size)
{
	int arraycount = 0;
	int array_k;
	char arraydata[100000];

	while(buffer[(*j)] != '{')
	{
		if(buffer[(*j)] != ' ') return -1;
		else if(buffer[(*j)] == '\0') return -1;
		else (*j)++;
	}
	(*j)++;

	while(buffer[(*j)] != '}')
	{
		array_k = 0;
		while(buffer[(*j)] != ',' && buffer[(*j)] != '}')
		{
			if(buffer[(*j)] == '\0') return -1;
			arraydata[array_k] = buffer[(*j)];
			array_k++;
			(*j)++;
		}
		arraydata[array_k] = '\0';
		if (arraycount < size) double_static_array[arraycount] = atof(arraydata);		
		arraycount++;
		if(buffer[(*j)] != '}') (*j)++;
	}

	if (arraycount > size)
	{
		fprintf(stderr, "WARNING: %d 'double' values provided for static array of size %d. Surplus values ignored.\n",
				arraycount, size);
	}
	
	(*j)++;
	return 0;
}

/** \fn void read_char_static_array(char * buffer, int * (*j), int ** char_static_array)
 * \brief Reads char static array.
 */
int read_char_static_array(char * buffer, int /*@unused@*/ buffer_size, int * j, char * char_static_array, int size)
{
	int arraycount;
	
	while(buffer[(*j)] == ' ')
	{
		(*j)++;
	}
	
	for(arraycount = 0; arraycount < size; arraycount++)
	{
		char_static_array[arraycount] = buffer[(*j)];
		(*j)++;
	}

	//(*j)++;
	return 0;
}

/** \fn void read_int_dynamic_array(char * buffer, int * (*j), int ** int_dynamic_array)
 * \brief Reads integer dynamic array.
 */
int read_int_dynamic_array(char * buffer, int /*@unused@*/ buffer_size, int * j, int_array * int_dynamic_array)
{
	int arraycount = 0;
	int array_k;
	char arraydata[100000];

	while(buffer[(*j)] != '{')
	{
		if(buffer[(*j)] != ' ') return -1;
		else if(buffer[(*j)] == '\0') return -1;
		else (*j)++;
	}
	(*j)++;

	while(buffer[(*j)] != '}')
	{
		array_k = 0;
		while(buffer[(*j)] != ',' && buffer[(*j)] != '}')
		{
			if(buffer[(*j)] == '\0') return -1;
			arraydata[array_k] = buffer[(*j)];
			array_k++;
			(*j)++;
		}
		arraydata[array_k] = '\0';
		add_int(int_dynamic_array, atoi(arraydata));
		arraycount++;
		if(buffer[(*j)] != '}') (*j)++;
	}

	(*j)++;
	return 0;
}

/** \fn void read_float_dynamic_array(char * buffer, int * (*j), int ** float_dynamic_array)
 * \brief Reads float dynamic array.
 */
int read_float_dynamic_array(char * buffer, int /*@unused@*/ buffer_size, int * j, float_array * float_dynamic_array)
{
	int arraycount = 0;
	int array_k;
	char arraydata[100000];

	while(buffer[(*j)] != '{')
	{
		if(buffer[(*j)] != ' ') return -1;
		else if(buffer[(*j)] == '\0') return -1;
		else (*j)++;
	}
	(*j)++;

	while(buffer[(*j)] != '}')
	{
		array_k = 0;
		while(buffer[(*j)] != ',' && buffer[(*j)] != '}')
		{
			if(buffer[(*j)] == '\0') return -1;
			arraydata[array_k] = buffer[(*j)];
			array_k++;
			(*j)++;
		}
		arraydata[array_k] = '\0';
		add_float(float_dynamic_array, (float)atof(arraydata));
		arraycount++;
		if(buffer[(*j)] != '}') (*j)++;
	}

	(*j)++;
	return 0;
}

/** \fn void read_double_dynamic_array(char * buffer, int * (*j), int ** double_dynamic_array)
 * \brief Reads double dynamic array.
 */
int read_double_dynamic_array(char * buffer, int /*@unused@*/ buffer_size, int * j, double_array * double_dynamic_array)
{
	int arraycount = 0;
	int array_k;
	char arraydata[100000];

	while(buffer[(*j)] != '{')
	{
		if(buffer[(*j)] != ' ') return -1;
		else if(buffer[(*j)] == '\0') return -1;
		else (*j)++;
	}
	(*j)++;

	while(buffer[(*j)] != '}')
	{
		array_k = 0;
		while(buffer[(*j)] != ',' && buffer[(*j)] != '}')
		{
			if(buffer[(*j)] == '\0') return -1;
			arraydata[array_k] = buffer[(*j)];
			array_k++;
			(*j)++;
		}
		arraydata[array_k] = '\0';
		add_double(double_dynamic_array, atof(arraydata));
		arraycount++;
		if(buffer[(*j)] != '}') (*j)++;
	}

	(*j)++;
	return 0;
}

/** \fn void read_char_dynamic_array(char * buffer, int * (*j), int ** char_dynamic_array)
 * \brief Reads char dynamic array.
 */
int read_char_dynamic_array(char * buffer, int /*@unused@*/ buffer_size, int * j, char_array * char_dynamic_array)
{
	if(*j > buffer_size) return -1;
	
	while(buffer[(*j)] == ' ')
	{
		(*j)++;
	}
	
	while(buffer[(*j)] != '\0' && buffer[(*j)] != ',' && buffer[(*j)] != '}')
	{
		add_char(char_dynamic_array, buffer[(*j)]);
		(*j)++;
	}
	
	return 0;
}




int readEnvironmentXML(char * location)
{
	FILE * file;
	char c = '\0';
	char buffer[100000];
	int index = 0;
	int in_environment = 0;
	int in_NNODES = 0;
	int in_WIDTH = 0;
	int in_DT = 0;
	int in_NOISE = 0;
	int in_DELTA = 0;
	int in_KEL = 0;
	int in_KI = 0;
	int in_KTH = 0;
	int in_SIDEBOX = 0;
	int in_SIDEX = 0;
	int in_SIDEY = 0;
	

	buffer[0] = '\0';

	/* Open file */
	if((file = fopen(location, "r"))==NULL)
	{
		printf("Error: cannot open import xml file '%s'\n", location);
		exit(0);
	}
	printf("Reading environment data from: %s\n", location);
	/* Keep reading file until EOF */
	while(c != (char)EOF)
	{
		/* Get the next char from the file */
		c = (char)fgetc(file);
		if(c == '>')
		{
			buffer[index] = '\0';
			if(strcmp(buffer, "environment") == 0) in_environment = 1;
			if(strcmp(buffer, "/environment") == 0) in_environment = 0;
			if(strcmp(buffer, "NNODES") == 0) in_NNODES = 1;
			if(strcmp(buffer, "/NNODES") == 0) in_NNODES = 0;
			if(strcmp(buffer, "WIDTH") == 0) in_WIDTH = 1;
			if(strcmp(buffer, "/WIDTH") == 0) in_WIDTH = 0;
			if(strcmp(buffer, "DT") == 0) in_DT = 1;
			if(strcmp(buffer, "/DT") == 0) in_DT = 0;
			if(strcmp(buffer, "NOISE") == 0) in_NOISE = 1;
			if(strcmp(buffer, "/NOISE") == 0) in_NOISE = 0;
			if(strcmp(buffer, "DELTA") == 0) in_DELTA = 1;
			if(strcmp(buffer, "/DELTA") == 0) in_DELTA = 0;
			if(strcmp(buffer, "KEL") == 0) in_KEL = 1;
			if(strcmp(buffer, "/KEL") == 0) in_KEL = 0;
			if(strcmp(buffer, "KI") == 0) in_KI = 1;
			if(strcmp(buffer, "/KI") == 0) in_KI = 0;
			if(strcmp(buffer, "KTH") == 0) in_KTH = 1;
			if(strcmp(buffer, "/KTH") == 0) in_KTH = 0;
			if(strcmp(buffer, "SIDEBOX") == 0) in_SIDEBOX = 1;
			if(strcmp(buffer, "/SIDEBOX") == 0) in_SIDEBOX = 0;
			if(strcmp(buffer, "SIDEX") == 0) in_SIDEX = 1;
			if(strcmp(buffer, "/SIDEX") == 0) in_SIDEX = 0;
			if(strcmp(buffer, "SIDEY") == 0) in_SIDEY = 1;
			if(strcmp(buffer, "/SIDEY") == 0) in_SIDEY = 0;
			
			index = 0;
			buffer[index] = '\0';
		}
		else if(c == '<')
		{
			buffer[index] = '\0';
			if(in_environment == 1)
			{
				if(in_NNODES == 1) { FLAME_environment_variable_NNODES = atoi(buffer); }
				if(in_WIDTH == 1) { FLAME_environment_variable_WIDTH = (float)atof(buffer); }
				if(in_DT == 1) { FLAME_environment_variable_DT = (float)atof(buffer); }
				if(in_NOISE == 1) { FLAME_environment_variable_NOISE = (float)atof(buffer); }
				if(in_DELTA == 1) { FLAME_environment_variable_DELTA = (float)atof(buffer); }
				if(in_KEL == 1) { FLAME_environment_variable_KEL = (float)atof(buffer); }
				if(in_KI == 1) { FLAME_environment_variable_KI = (float)atof(buffer); }
				if(in_KTH == 1) { FLAME_environment_variable_KTH = (float)atof(buffer); }
				if(in_SIDEBOX == 1) { FLAME_environment_variable_SIDEBOX = (float)atof(buffer); }
				if(in_SIDEX == 1) { FLAME_environment_variable_SIDEX = atoi(buffer); }
				if(in_SIDEY == 1) { FLAME_environment_variable_SIDEY = atoi(buffer); }
				
			}
			index = 0;
			buffer[index] = '\0';
		}
		else
		{
			buffer[index] = c;
			if(index < 999) index++;
		}
	}
	/* Close file */
	(void)fclose(file);

	return 0;
}

int readAgentXML(char * location,
				double cloud_data[],
				int partition_method,
				int flag,
				int number_partitions,
				int agent_count,
				int * itno)
{
	FILE * file;
	char c = '\0';
	char buffer[100000];
	char agentname[10000];
	int index = 0;
	int j; /* Index for reading arrays */
	int rc;
	int in_itno = 0;
	int FLAME_in_xagent = 0;
	int FLAME_in_name = 0;
	int in_Sporo_agent = 0;
	
	int in_id = 0;
	int in_iter = 0;
	int in_FP = 0;
	int in_K0 = 0;
	int in_TH0 = 0;
	int in_R0 = 0;
	int in_coordx = 0;
	int in_coordy = 0;
	int in_xnbr_node0 = 0;
	int in_xnbr_node1 = 0;
	int in_xnbr_node2 = 0;
	int in_xnbr_node3 = 0;
	int in_xnbr_node4 = 0;
	int in_xnbr_node5 = 0;
	int in_xnbr_node6 = 0;
	int in_xnbr_node7 = 0;
	int in_xnbr_node8 = 0;
	int in_xnbr_node9 = 0;
	int in_xnbr_node10 = 0;
	int in_ynbr_node0 = 0;
	int in_ynbr_node1 = 0;
	int in_ynbr_node2 = 0;
	int in_ynbr_node3 = 0;
	int in_ynbr_node4 = 0;
	int in_ynbr_node5 = 0;
	int in_ynbr_node6 = 0;
	int in_ynbr_node7 = 0;
	int in_ynbr_node8 = 0;
	int in_ynbr_node9 = 0;
	int in_ynbr_node10 = 0;
	
	xmachine_memory_Sporo * current_Sporo_agent = NULL;
	
	/* Things for round-robin partitioning */
	int geometric = 1;
	int other = 2;
	double posx=0.0, posy=0.0, posz=0.0;

	j = 0;
	rc = 0;
	
	/* Use j and rc to stop compiler warnings if not used */
	if(j == 0 && rc == 0) {}
	
	agentname[0] = '\0';
	buffer[0] = '\0';

	/* Open file */
	if((file = fopen(location, "r"))==NULL)
	{
		printf("Error: cannot open import xml file '%s'\n", location);
		exit(0);
	}
	printf("Reading agent data from: %s\n", location);
	/* Keep reading file until EOF */
	while(c != (char)EOF)
	{
		/* Get the next char from the file */
		c = (char)fgetc(file);
		if(c == '>')
		{
			buffer[index] = '\0';
			if(strcmp(buffer, "itno") == 0) in_itno = 1;
			if(strcmp(buffer, "/itno") == 0) in_itno = 0;
			if(strcmp(buffer, "xagent") == 0)
			{
				FLAME_in_xagent = 1;
				agentname[0] = '\0';
			}
			if(strcmp(buffer, "/xagent") == 0)
			{	
				if(strcmp(agentname, "Sporo") == 0)
				{
					if(current_Sporo_agent == NULL) { printf("Memory error reading Sporo agent\n"); exit(0); }
					
					posx = (double)0.0;
					posy = (double)0.0;
					posz = (double)0.0;
					
					/* If flag is zero just read the data. We'll partition later.
					 * If flag is not zero we aleady have partition data so can read and distribute to the current node.*/
					if( flag == 0 )
					{
						/* Next line should be commented out? */
						add_Sporo_agent_internal(current_Sporo_agent, Sporo_start_state);

						/* Update the cloud data */
						if ( posx < cloud_data[0] ) cloud_data[0] = posx;
						if ( posx > cloud_data[1] ) cloud_data[1] = posx;
						if ( posy < cloud_data[2] ) cloud_data[2] = posy;
						if ( posy > cloud_data[3] ) cloud_data[3] = posy;
						if ( posz < cloud_data[2] ) cloud_data[4] = posz;
						if ( posz > cloud_data[3] ) cloud_data[5] = posz;
					}
					else
					{
						if(partition_method == geometric)
						{
							if (
								((current_node->partition_data[0] == SPINF) || (current_node->partition_data[0] != SPINF && posx >= current_node->partition_data[0])) &&
								((current_node->partition_data[1] == SPINF) || (current_node->partition_data[1] != SPINF && posx < current_node->partition_data[1])) &&
								((current_node->partition_data[2] == SPINF) || (current_node->partition_data[2] != SPINF && posy >= current_node->partition_data[2])) &&
								((current_node->partition_data[3] == SPINF) || (current_node->partition_data[3] != SPINF && posy < current_node->partition_data[3])) &&
								((current_node->partition_data[4] == SPINF) || (current_node->partition_data[4] != SPINF && posz >= current_node->partition_data[4])) &&
								((current_node->partition_data[5] == SPINF) || (current_node->partition_data[5] != SPINF && posz < current_node->partition_data[5]))
							)
							{
								add_Sporo_agent_internal(current_Sporo_agent, Sporo_start_state);
							}
						}
						else if (partition_method == other)
						{
							if (agent_count % number_partitions == 0)
							{
								add_Sporo_agent_internal(current_Sporo_agent, Sporo_start_state);
							}
							++agent_count;
						}
					}
				}
				else
				{
					printf("Warning: agent name undefined - '%s'\n", agentname);
					exit(0);
				}
				agentname[0] = '\0';
				FLAME_in_xagent = 0;
				in_Sporo_agent = 0;
				
			}
			if(strcmp(buffer, "name") == 0) FLAME_in_name = 1;
			if(strcmp(buffer, "/name") == 0) FLAME_in_name = 0;
			if(strcmp(buffer, "id") == 0) { in_id = 1; }
			if(strcmp(buffer, "/id") == 0) { in_id = 0; }
			if(strcmp(buffer, "iter") == 0) { in_iter = 1; }
			if(strcmp(buffer, "/iter") == 0) { in_iter = 0; }
			if(strcmp(buffer, "FP") == 0) { in_FP = 1; }
			if(strcmp(buffer, "/FP") == 0) { in_FP = 0; }
			if(strcmp(buffer, "K0") == 0) { in_K0 = 1; }
			if(strcmp(buffer, "/K0") == 0) { in_K0 = 0; }
			if(strcmp(buffer, "TH0") == 0) { in_TH0 = 1; }
			if(strcmp(buffer, "/TH0") == 0) { in_TH0 = 0; }
			if(strcmp(buffer, "R0") == 0) { in_R0 = 1; }
			if(strcmp(buffer, "/R0") == 0) { in_R0 = 0; }
			if(strcmp(buffer, "coordx") == 0) { in_coordx = 1; }
			if(strcmp(buffer, "/coordx") == 0) { in_coordx = 0; }
			if(strcmp(buffer, "coordy") == 0) { in_coordy = 1; }
			if(strcmp(buffer, "/coordy") == 0) { in_coordy = 0; }
			if(strcmp(buffer, "xnbr_node0") == 0) { in_xnbr_node0 = 1; }
			if(strcmp(buffer, "/xnbr_node0") == 0) { in_xnbr_node0 = 0; }
			if(strcmp(buffer, "xnbr_node1") == 0) { in_xnbr_node1 = 1; }
			if(strcmp(buffer, "/xnbr_node1") == 0) { in_xnbr_node1 = 0; }
			if(strcmp(buffer, "xnbr_node2") == 0) { in_xnbr_node2 = 1; }
			if(strcmp(buffer, "/xnbr_node2") == 0) { in_xnbr_node2 = 0; }
			if(strcmp(buffer, "xnbr_node3") == 0) { in_xnbr_node3 = 1; }
			if(strcmp(buffer, "/xnbr_node3") == 0) { in_xnbr_node3 = 0; }
			if(strcmp(buffer, "xnbr_node4") == 0) { in_xnbr_node4 = 1; }
			if(strcmp(buffer, "/xnbr_node4") == 0) { in_xnbr_node4 = 0; }
			if(strcmp(buffer, "xnbr_node5") == 0) { in_xnbr_node5 = 1; }
			if(strcmp(buffer, "/xnbr_node5") == 0) { in_xnbr_node5 = 0; }
			if(strcmp(buffer, "xnbr_node6") == 0) { in_xnbr_node6 = 1; }
			if(strcmp(buffer, "/xnbr_node6") == 0) { in_xnbr_node6 = 0; }
			if(strcmp(buffer, "xnbr_node7") == 0) { in_xnbr_node7 = 1; }
			if(strcmp(buffer, "/xnbr_node7") == 0) { in_xnbr_node7 = 0; }
			if(strcmp(buffer, "xnbr_node8") == 0) { in_xnbr_node8 = 1; }
			if(strcmp(buffer, "/xnbr_node8") == 0) { in_xnbr_node8 = 0; }
			if(strcmp(buffer, "xnbr_node9") == 0) { in_xnbr_node9 = 1; }
			if(strcmp(buffer, "/xnbr_node9") == 0) { in_xnbr_node9 = 0; }
			if(strcmp(buffer, "xnbr_node10") == 0) { in_xnbr_node10 = 1; }
			if(strcmp(buffer, "/xnbr_node10") == 0) { in_xnbr_node10 = 0; }
			if(strcmp(buffer, "ynbr_node0") == 0) { in_ynbr_node0 = 1; }
			if(strcmp(buffer, "/ynbr_node0") == 0) { in_ynbr_node0 = 0; }
			if(strcmp(buffer, "ynbr_node1") == 0) { in_ynbr_node1 = 1; }
			if(strcmp(buffer, "/ynbr_node1") == 0) { in_ynbr_node1 = 0; }
			if(strcmp(buffer, "ynbr_node2") == 0) { in_ynbr_node2 = 1; }
			if(strcmp(buffer, "/ynbr_node2") == 0) { in_ynbr_node2 = 0; }
			if(strcmp(buffer, "ynbr_node3") == 0) { in_ynbr_node3 = 1; }
			if(strcmp(buffer, "/ynbr_node3") == 0) { in_ynbr_node3 = 0; }
			if(strcmp(buffer, "ynbr_node4") == 0) { in_ynbr_node4 = 1; }
			if(strcmp(buffer, "/ynbr_node4") == 0) { in_ynbr_node4 = 0; }
			if(strcmp(buffer, "ynbr_node5") == 0) { in_ynbr_node5 = 1; }
			if(strcmp(buffer, "/ynbr_node5") == 0) { in_ynbr_node5 = 0; }
			if(strcmp(buffer, "ynbr_node6") == 0) { in_ynbr_node6 = 1; }
			if(strcmp(buffer, "/ynbr_node6") == 0) { in_ynbr_node6 = 0; }
			if(strcmp(buffer, "ynbr_node7") == 0) { in_ynbr_node7 = 1; }
			if(strcmp(buffer, "/ynbr_node7") == 0) { in_ynbr_node7 = 0; }
			if(strcmp(buffer, "ynbr_node8") == 0) { in_ynbr_node8 = 1; }
			if(strcmp(buffer, "/ynbr_node8") == 0) { in_ynbr_node8 = 0; }
			if(strcmp(buffer, "ynbr_node9") == 0) { in_ynbr_node9 = 1; }
			if(strcmp(buffer, "/ynbr_node9") == 0) { in_ynbr_node9 = 0; }
			if(strcmp(buffer, "ynbr_node10") == 0) { in_ynbr_node10 = 1; }
			if(strcmp(buffer, "/ynbr_node10") == 0) { in_ynbr_node10 = 0; }
			
			index = 0;
			buffer[index] = '\0';
		}
		else if(c == '<')
		{
			buffer[index] = '\0';
			
			if(in_itno && FLAME_in_xagent == 0) *itno = atoi(buffer);
			if(FLAME_in_xagent == 1)
			{
				if(FLAME_in_name == 1)
				{
					strcpy(agentname, buffer);

					if(strcmp(agentname, "Sporo") == 0)
					{
						current_Sporo_agent = init_Sporo_agent();
						in_Sporo_agent = 1;
					}
					else
					{
						printf("Warning: agent name undefined - '%s'\n", agentname);
						exit(0);
					}
				}
				else if(in_Sporo_agent == 1)
				{
					if(in_id) { current_Sporo_agent->id = atoi(buffer); }
					if(in_iter) { current_Sporo_agent->iter = atoi(buffer); }
					if(in_FP) { current_Sporo_agent->FP = atof(buffer); }
					if(in_K0) { current_Sporo_agent->K0 = atof(buffer); }
					if(in_TH0) { current_Sporo_agent->TH0 = atof(buffer); }
					if(in_R0) { current_Sporo_agent->R0 = atof(buffer); }
					if(in_coordx) { j = 0;
						rc = read_float_static_array(buffer, index, &j, current_Sporo_agent->coordx, 11);
						if(rc != 0) { printf("Error: reading 'Sporo' agent variable 'coordx' of type 'float'\n"); exit(0); } }
					if(in_coordy) { j = 0;
						rc = read_float_static_array(buffer, index, &j, current_Sporo_agent->coordy, 11);
						if(rc != 0) { printf("Error: reading 'Sporo' agent variable 'coordy' of type 'float'\n"); exit(0); } }
					if(in_xnbr_node0) { j = 0;
						rc = read_float_dynamic_array(buffer, index, &j, &current_Sporo_agent->xnbr_node0);
						if(rc != 0) { printf("Error: reading 'Sporo' agent variable 'xnbr_node0' of type 'float_array'\n"); exit(0); } }
					if(in_xnbr_node1) { j = 0;
						rc = read_float_dynamic_array(buffer, index, &j, &current_Sporo_agent->xnbr_node1);
						if(rc != 0) { printf("Error: reading 'Sporo' agent variable 'xnbr_node1' of type 'float_array'\n"); exit(0); } }
					if(in_xnbr_node2) { j = 0;
						rc = read_float_dynamic_array(buffer, index, &j, &current_Sporo_agent->xnbr_node2);
						if(rc != 0) { printf("Error: reading 'Sporo' agent variable 'xnbr_node2' of type 'float_array'\n"); exit(0); } }
					if(in_xnbr_node3) { j = 0;
						rc = read_float_dynamic_array(buffer, index, &j, &current_Sporo_agent->xnbr_node3);
						if(rc != 0) { printf("Error: reading 'Sporo' agent variable 'xnbr_node3' of type 'float_array'\n"); exit(0); } }
					if(in_xnbr_node4) { j = 0;
						rc = read_float_dynamic_array(buffer, index, &j, &current_Sporo_agent->xnbr_node4);
						if(rc != 0) { printf("Error: reading 'Sporo' agent variable 'xnbr_node4' of type 'float_array'\n"); exit(0); } }
					if(in_xnbr_node5) { j = 0;
						rc = read_float_dynamic_array(buffer, index, &j, &current_Sporo_agent->xnbr_node5);
						if(rc != 0) { printf("Error: reading 'Sporo' agent variable 'xnbr_node5' of type 'float_array'\n"); exit(0); } }
					if(in_xnbr_node6) { j = 0;
						rc = read_float_dynamic_array(buffer, index, &j, &current_Sporo_agent->xnbr_node6);
						if(rc != 0) { printf("Error: reading 'Sporo' agent variable 'xnbr_node6' of type 'float_array'\n"); exit(0); } }
					if(in_xnbr_node7) { j = 0;
						rc = read_float_dynamic_array(buffer, index, &j, &current_Sporo_agent->xnbr_node7);
						if(rc != 0) { printf("Error: reading 'Sporo' agent variable 'xnbr_node7' of type 'float_array'\n"); exit(0); } }
					if(in_xnbr_node8) { j = 0;
						rc = read_float_dynamic_array(buffer, index, &j, &current_Sporo_agent->xnbr_node8);
						if(rc != 0) { printf("Error: reading 'Sporo' agent variable 'xnbr_node8' of type 'float_array'\n"); exit(0); } }
					if(in_xnbr_node9) { j = 0;
						rc = read_float_dynamic_array(buffer, index, &j, &current_Sporo_agent->xnbr_node9);
						if(rc != 0) { printf("Error: reading 'Sporo' agent variable 'xnbr_node9' of type 'float_array'\n"); exit(0); } }
					if(in_xnbr_node10) { j = 0;
						rc = read_float_dynamic_array(buffer, index, &j, &current_Sporo_agent->xnbr_node10);
						if(rc != 0) { printf("Error: reading 'Sporo' agent variable 'xnbr_node10' of type 'float_array'\n"); exit(0); } }
					if(in_ynbr_node0) { j = 0;
						rc = read_float_dynamic_array(buffer, index, &j, &current_Sporo_agent->ynbr_node0);
						if(rc != 0) { printf("Error: reading 'Sporo' agent variable 'ynbr_node0' of type 'float_array'\n"); exit(0); } }
					if(in_ynbr_node1) { j = 0;
						rc = read_float_dynamic_array(buffer, index, &j, &current_Sporo_agent->ynbr_node1);
						if(rc != 0) { printf("Error: reading 'Sporo' agent variable 'ynbr_node1' of type 'float_array'\n"); exit(0); } }
					if(in_ynbr_node2) { j = 0;
						rc = read_float_dynamic_array(buffer, index, &j, &current_Sporo_agent->ynbr_node2);
						if(rc != 0) { printf("Error: reading 'Sporo' agent variable 'ynbr_node2' of type 'float_array'\n"); exit(0); } }
					if(in_ynbr_node3) { j = 0;
						rc = read_float_dynamic_array(buffer, index, &j, &current_Sporo_agent->ynbr_node3);
						if(rc != 0) { printf("Error: reading 'Sporo' agent variable 'ynbr_node3' of type 'float_array'\n"); exit(0); } }
					if(in_ynbr_node4) { j = 0;
						rc = read_float_dynamic_array(buffer, index, &j, &current_Sporo_agent->ynbr_node4);
						if(rc != 0) { printf("Error: reading 'Sporo' agent variable 'ynbr_node4' of type 'float_array'\n"); exit(0); } }
					if(in_ynbr_node5) { j = 0;
						rc = read_float_dynamic_array(buffer, index, &j, &current_Sporo_agent->ynbr_node5);
						if(rc != 0) { printf("Error: reading 'Sporo' agent variable 'ynbr_node5' of type 'float_array'\n"); exit(0); } }
					if(in_ynbr_node6) { j = 0;
						rc = read_float_dynamic_array(buffer, index, &j, &current_Sporo_agent->ynbr_node6);
						if(rc != 0) { printf("Error: reading 'Sporo' agent variable 'ynbr_node6' of type 'float_array'\n"); exit(0); } }
					if(in_ynbr_node7) { j = 0;
						rc = read_float_dynamic_array(buffer, index, &j, &current_Sporo_agent->ynbr_node7);
						if(rc != 0) { printf("Error: reading 'Sporo' agent variable 'ynbr_node7' of type 'float_array'\n"); exit(0); } }
					if(in_ynbr_node8) { j = 0;
						rc = read_float_dynamic_array(buffer, index, &j, &current_Sporo_agent->ynbr_node8);
						if(rc != 0) { printf("Error: reading 'Sporo' agent variable 'ynbr_node8' of type 'float_array'\n"); exit(0); } }
					if(in_ynbr_node9) { j = 0;
						rc = read_float_dynamic_array(buffer, index, &j, &current_Sporo_agent->ynbr_node9);
						if(rc != 0) { printf("Error: reading 'Sporo' agent variable 'ynbr_node9' of type 'float_array'\n"); exit(0); } }
					if(in_ynbr_node10) { j = 0;
						rc = read_float_dynamic_array(buffer, index, &j, &current_Sporo_agent->ynbr_node10);
						if(rc != 0) { printf("Error: reading 'Sporo' agent variable 'ynbr_node10' of type 'float_array'\n"); exit(0); } }
				 }
			}
			index = 0;
			buffer[index] = '\0';
		}
		else
		{
			buffer[index] = c;
			if(index < 99999) index++;
			else
			{
				printf("Error: agent reading buffer too small\n");
				printf("%s\n", buffer);
				exit(0);
			}
		}
	}
	/* Close file */
	(void)fclose(file);

	return 0;
}

/** \fn void check_location_exists(char * location)
 * \brief Check that a directory exists.
 * \param location The directory location to check.
 */
int check_location_exists(char * location)
{
	/* Save a dummy file to directory and remove.
	 * This checks directory exists and is writable */
	FILE * file;
	char * buffer;

	buffer = (char *)malloc( (strlen(location) + strlen("dummy.temp") + 1) * sizeof(char));
	strcpy(buffer, location);
	strcat(buffer, "dummy.temp");

	if((file = fopen(buffer, "w")) == NULL) return 0;

	(void)fclose(file);
	/* Remove dummy file */
	remove(buffer);

	return 1;
}


/** \fn void readinitialstates(char * filename, int * itno, xmachine ** agent_list, double cloud_data[6], int flag)
 * \brief Read initial X-machine memory starting values from a file.
 * \param filename The path to the file.
 * \param filelocation The directory of the location
 * \param itno Pointer to the iteration number.
 * \param agent_list List of agents in the model to be constructed by this function.
 * \param cloud_data Max and min x,y,z coordinates of agents.
 * \param partition_method Identifies partitioning method: 1 = geometric, 2 = round-robin
 * \param flag Flag for whether to check space partitions.
 */
void readinitialstates(char * filename, char * filelocation, int * itno, double cloud_data[],
					   int partition_method, int flag)
{
	/* Pointer to file */
	FILE *file;
	/* Char and char buffer for reading file to */
	char c = '\0';
	char buffer[100000];
	char FLAME_location[10000];
	char FLAME_format[10000];
	char FLAME_type[10000];
	FLAME_output * current_FLAME_output = NULL;

	/* Things for round-robin partitioning */
	int agent_count = 0;
	int number_partitions = 0;
	int geometric=1;
	int other=2;
	

	/* Cloud data array initialisation */
	# ifndef S_SPLINT_S
	cloud_data[0] = SPINF;
	cloud_data[1] = -SPINF;
	cloud_data[2] = SPINF;
	cloud_data[3] = -SPINF;
	cloud_data[4] = SPINF;
	cloud_data[5] = -SPINF;
	# endif

	/* Temporary node and head of associated agent list to allow adding agents */
	node_information temp_node;

	/* Pointer to x-memory for initial state data */
	/*xmachine * current_xmachine;*/
	/* Variables for checking tags */
	int reading = 1;
	int i = 0;
	int rc;
	int in_tag = 0;
	int in_itno = 0;
	int FLAME_in_imports = 0;
	int FLAME_in_import = 0;
	int FLAME_in_outputs = 0;
	int FLAME_in_output = 0;
	int FLAME_in_location = 0;
	int FLAME_in_format = 0;
	int FLAME_in_type = 0;
	int FLAME_in_time = 0;
	int FLAME_in_period = 0;
	int FLAME_in_phase = 0;
	int FLAME_in_name = 0;

	/* Initialise environment vars */
	FLAME_environment_variable_NNODES = 0;
	FLAME_environment_variable_WIDTH = 0.0;
	FLAME_environment_variable_DT = 0.0;
	FLAME_environment_variable_NOISE = 0.0;
	FLAME_environment_variable_DELTA = 0.0;
	FLAME_environment_variable_KEL = 0.0;
	FLAME_environment_variable_KI = 0.0;
	FLAME_environment_variable_KTH = 0.0;
	FLAME_environment_variable_SIDEBOX = 0.0;
	FLAME_environment_variable_SIDEX = 0;
	FLAME_environment_variable_SIDEY = 0;
	

	/* Open config file to read-only */
	if((file = fopen(filename, "r"))==NULL)
	{
		printf("Error: opening initial states '%s'\n", filename);
		exit(0);
	}
	
	/* Initialise variables */
    *itno = 0;





	if(partition_method==geometric) printf("xml: Geometric partitioning\n");
	else if(partition_method==other) printf("xml: Round-robin partitioning\n");
	else printf("xml: Error - invalid partitioning method\n");


	/* Set p_xmachine to the agent list passed in then new agents are added to this list
	 * Will be set to agent list for specific node is space partitions are already known (flag=1)
	 */
	/* If we're reading without knowing partitions already then use the dummy current node (it's not in the list of nodes)*/
	if (flag == 0) current_node = &temp_node;

	printf("Reading initial data file: %s\n", filename);
	/* Read file until end of xml */
	while(reading==1)
	{
		/* Get the next char from the file */
		c = (char)fgetc(file);

		/* If the end of a tag */
		if(c == '>')
		{
			/* Place 0 at end of buffer to make chars a string */
			buffer[i] = 0;

			if(strcmp(buffer, "states") == 0) reading = 1;
			if(strcmp(buffer, "/states") == 0) reading = 0;
			if(strcmp(buffer, "itno") == 0) in_itno = 1;
			if(strcmp(buffer, "/itno") == 0) in_itno = 0;
			if(strcmp(buffer, "imports") == 0) FLAME_in_imports = 1;
			if(strcmp(buffer, "/imports") == 0) FLAME_in_imports = 0;
			if(strcmp(buffer, "import") == 0)
			{
				/*FLAME_location[0] = '\0';*/
				strcpy(FLAME_location, filelocation);
				FLAME_format[0] = '\0';
				FLAME_type[0] = '\0';

				FLAME_in_import = 1;
			}
			if(strcmp(buffer, "/import") == 0)
			{
				if(strcmp("agent", FLAME_type) == 0 || strcmp("environment", FLAME_type) == 0)
				{
					if(strcmp("xml", FLAME_format) == 0)
					{
						if(strcmp("agent", FLAME_type) == 0) readAgentXML(FLAME_location, cloud_data, partition_method, flag, number_partitions, agent_count, itno);
						if(strcmp("environment", FLAME_type) == 0) readEnvironmentXML(FLAME_location);
					}
					else
					{
						printf("Error: import format '%s' is unsupported\n", FLAME_format);
						exit(0);
					}
				}
				else
				{
					printf("Error: import type '%s' is not agent or environment\n", FLAME_type);
					exit(0);
				}

				FLAME_in_import = 0;
			}
			if(strcmp(buffer, "location") == 0) FLAME_in_location = 1;
			if(strcmp(buffer, "/location") == 0) FLAME_in_location = 0;
			if(strcmp(buffer, "format") == 0) FLAME_in_format = 1;
			if(strcmp(buffer, "/format") == 0) FLAME_in_format = 0;
			if(strcmp(buffer, "type") == 0) FLAME_in_type = 1;
			if(strcmp(buffer, "/type") == 0) FLAME_in_type = 0;
			if(strcmp(buffer, "outputs") == 0) FLAME_in_outputs = 1;
			if(strcmp(buffer, "/outputs") == 0) FLAME_in_outputs = 0;
			if(strcmp(buffer, "output") == 0)
			{
				if(FLAME_in_outputs == 1)
				{
					current_FLAME_output = add_FLAME_output(&FLAME_outputs);
	
					FLAME_in_output = 1;
				}
			}
			if(strcmp(buffer, "/output") == 0)
			{
				if(FLAME_in_outputs == 1)
				{
					if(current_FLAME_output->type == -1)
					{
						printf("Error: an output type has not been set\n");
						exit(0);
					}
					if(current_FLAME_output->format == -1)
					{
						printf("Error: an output format has not been set\n");
						exit(0);
					}
					if(current_FLAME_output->location == NULL)
					{
						printf("Error: an output location has not been set\n");
						exit(0);
					}
					/* If type is xml check if location exists */
					if(current_FLAME_output->type == 0)
					{
						rc = check_location_exists(current_FLAME_output->location);
						if(rc == 0)
						{
							printf("Error: location directory '%s' does not exist\n", current_FLAME_output->location);
							exit(0);
						}
					}
					/* Period has to be larger than 0 */
					if(current_FLAME_output->period < 1)
					{
						printf("Error: output period is less than 1: '%d'\n", current_FLAME_output->period);
						exit(0);
					}
					/* Phase cannot be equal or larger than period */
					if(current_FLAME_output->phase >= current_FLAME_output->period)
					{
						printf("Error: output phase is more or equal to period: '%d'\n", current_FLAME_output->phase);
						exit(0);
					}
	
					FLAME_in_output = 0;
				}
			}
			if(strcmp(buffer, "time") == 0) FLAME_in_time = 1;
			if(strcmp(buffer, "/time") == 0) FLAME_in_time = 0;
			if(strcmp(buffer, "period") == 0) FLAME_in_period = 1;
			if(strcmp(buffer, "/period") == 0) FLAME_in_period = 0;
			if(strcmp(buffer, "phase") == 0) FLAME_in_phase = 1;
			if(strcmp(buffer, "/phase") == 0) FLAME_in_phase = 0;
			if(strcmp(buffer, "name") == 0) FLAME_in_name = 1;
			if(strcmp(buffer, "/name") == 0) FLAME_in_name = 0;

			/* End of tag and reset buffer */
			in_tag = 0;
			i = 0;
		}
		/* If start of tag */
		else if(c == '<')
		{
			/* Place /0 at end of buffer to end numbers */
			buffer[i] = 0;
			/* Flag in tag */
			in_tag = 1;

			if(in_itno) *itno = atoi(buffer);
			if(FLAME_in_imports == 1)
			{
				if(FLAME_in_import == 1)
				{
					if(FLAME_in_location == 1) strcat(FLAME_location, buffer);
					if(FLAME_in_format == 1) strcpy(FLAME_format, buffer);
					if(FLAME_in_type == 1) strcpy(FLAME_type, buffer);
				}
			}
			if(FLAME_in_outputs == 1)
			{
				if(FLAME_in_output == 1)
				{
					if(FLAME_in_type == 1)
					{
						if(strcmp("snapshot", buffer) == 0) current_FLAME_output->type = 0;
						else if(strcmp("agent", buffer) != 0)
						{
							printf("Error: output type is not 'snapshot' or an 'agent': '%s'\n", buffer);
							exit(0);
						}
					}
					if(FLAME_in_name == 1)
					{
						if(strcmp("Sporo", buffer) == 0) current_FLAME_output->type = 1;
						else 
						{
							printf("Error: output name is not an agent name: '%s'\n", buffer);
							exit(0);
						}
					}
					if(FLAME_in_format == 1)
					{
						if(strcmp("xml", buffer) == 0) current_FLAME_output->format = 0;
						else
						{
							printf("Error: output format is unsupported: '%s'\n", buffer);
							exit(0);
						}
					}
					if(FLAME_in_location == 1)
					{
						current_FLAME_output->location = (char *)malloc( (strlen(filelocation) + strlen(buffer) + 1) * sizeof(char));
						strcpy(current_FLAME_output->location, filelocation);
						strcat(current_FLAME_output->location, buffer);
					}
					if(FLAME_in_time == 1)
					{
						if(FLAME_in_period == 1) current_FLAME_output->period = atoi(buffer);
						if(FLAME_in_phase == 1)  current_FLAME_output->phase = atoi(buffer);
					}
				}
			}
			/* Reset buffer */
			i = 0;
		}
		/* If in tag put read char into buffer */
		else if(in_tag == 1)
		{
			buffer[i] = c;
			i++;
		}
		/* If in data read char into buffer */
		else
		{
			buffer[i] = c;
			i++;
		}
	}

	/* Close the file */
	(void)fclose(file);

	/* Also try and read environment and agents from 0.xml */
	readEnvironmentXML(filename);
	readAgentXML(filename, cloud_data, partition_method, flag, number_partitions, agent_count, itno);

	/* If outputs is empty add default snapshot for every iteration */
	if(FLAME_outputs == NULL)
	{
		current_FLAME_output = add_FLAME_output(&FLAME_outputs);
		current_FLAME_output->type   = 0; /* snapshot */
		current_FLAME_output->format = 0; /* xml */
		current_FLAME_output->location = (char *)malloc( (strlen(filelocation) +  1) * sizeof(char));
		strcpy(current_FLAME_output->location, filelocation); /* location = 0.xml location */
		current_FLAME_output->period = 1; /* every iteration */
		current_FLAME_output->phase  = 0; /* no phase */
	}
	
	/* Print output settings to CLI */
	for(current_FLAME_output = FLAME_outputs; current_FLAME_output != NULL; current_FLAME_output = current_FLAME_output->next)
	{
		printf("output: type='");
		if(current_FLAME_output->type == 0) printf("snapshot");
		else if(current_FLAME_output->type == 1) printf("agent' name='Sporo");
		else printf("undefined");
		printf("' format='");
		if(current_FLAME_output->format == 0) printf("xml");
		else printf("undefined");
		printf("' location='%s'", current_FLAME_output->location);
		printf(" period='%d' phase='%d'\n", current_FLAME_output->period, current_FLAME_output->phase);
	}

	/* Make sure cloud data makes sense if any of x,y,z coords were all the same */
	if ( partition_method == geometric ) {
		if ( cloud_data[0] == cloud_data[1] ) {
			cloud_data[0] = -SPINF;
			cloud_data[1] = SPINF;
		}
		if ( cloud_data[2] == cloud_data[3] ) {
			cloud_data[2] = -SPINF;
			cloud_data[3] = SPINF;
		}
		if ( cloud_data[4] == cloud_data[5] ) {
			cloud_data[4] = -SPINF;
			cloud_data[5] = SPINF;
		}
	}
}

/** \fn void write_int_static_array(FILE *file, $name * temp)
 * \brief Writes int.
 */
void write_int_static_array(FILE *file, int * temp, int size)
{
	int i;
	char data[1000];

	fputs("{", file);
	for(i=0; i<size; i++)
	{
		sprintf(data, "%i", temp[i]);
		fputs(data, file);
		if(i < size-1) fputs(", ", file);
	}
	fputs("}", file);
}

/** \fn void write_float_static_array(FILE *file, float * temp)
 * \brief Writes float.
 */
void write_float_static_array(FILE *file, float * temp, int size)
{
	int i;
	char data[1000];

	fputs("{", file);
	for(i=0; i<size; i++)
	{
		sprintf(data, "%f", temp[i]);
		fputs(data, file);
		if(i < size-1) fputs(", ", file);
	}
	fputs("}", file);
}

/** \fn void write_double_static_array(FILE *file, double * temp)
 * \brief Writes double.
 */
void write_double_static_array(FILE *file, double * temp, int size)
{
	int i;
	char data[1000];

	fputs("{", file);
	for(i=0; i<size; i++)
	{
		sprintf(data, "%f", temp[i]);
		fputs(data, file);
		if(i < size-1) fputs(", ", file);
	}
	fputs("}", file);
}

/** \fn void write_char_static_array(FILE *file, char * temp)
 * \brief Writes char.
 */
void write_char_static_array(FILE *file, char * temp, int size)
{
	int i;
	char data[1000];

	//fputs("{", file);
	for(i=0; i<size; i++)
	{
		sprintf(data, "%c", temp[i]);
		fputs(data, file);
		//if(i < size-1) fputs(", ", file);
	}
	//fputs("}", file);
}

/** \fn void write_int_dynamic_array(FILE *file, int_array* temp)
 * \brief Writes int.
 */
void write_int_dynamic_array(FILE *file, int_array * temp)
{
	int i;
	char data[1000];

	fputs("{", file);
	for(i=0; i<(*temp).size; i++)
	{
		sprintf(data, "%i", (*temp).array[i]);
		fputs(data, file);
		if(i < ((*temp).size-1)) fputs(", ", file);
	}
	fputs("}", file);
}

/** \fn void write_float_dynamic_array(FILE *file, float_array* temp)
 * \brief Writes float.
 */
void write_float_dynamic_array(FILE *file, float_array * temp)
{
	int i;
	char data[1000];

	fputs("{", file);
	for(i=0; i<(*temp).size; i++)
	{
		sprintf(data, "%f", (*temp).array[i]);
		fputs(data, file);
		if(i < ((*temp).size-1)) fputs(", ", file);
	}
	fputs("}", file);
}

/** \fn void write_double_dynamic_array(FILE *file, double_array* temp)
 * \brief Writes double.
 */
void write_double_dynamic_array(FILE *file, double_array * temp)
{
	int i;
	char data[1000];

	fputs("{", file);
	for(i=0; i<(*temp).size; i++)
	{
		sprintf(data, "%f", (*temp).array[i]);
		fputs(data, file);
		if(i < ((*temp).size-1)) fputs(", ", file);
	}
	fputs("}", file);
}

/** \fn void write_char_dynamic_array(FILE *file, char_array* temp)
 * \brief Writes char.
 */
void write_char_dynamic_array(FILE *file, char_array * temp)
{
	int i;
	char data[1000];

	for(i=0; i<(*temp).size; i++)
	{
		sprintf(data, "%c", (*temp).array[i]);
		fputs(data, file);
	}
}



void write_Sporo_agent(FILE *file, xmachine_memory_Sporo * current)
{
	char data[1000];
	fputs("<xagent>\n" , file);
	fputs("<name>Sporo</name>\n", file);
		fputs("<id>", file);
	sprintf(data, "%i", current->id);
	fputs(data, file);
	fputs("</id>\n", file);
		fputs("<iter>", file);
	sprintf(data, "%i", current->iter);
	fputs(data, file);
	fputs("</iter>\n", file);
		fputs("<FP>", file);
	sprintf(data, "%f", current->FP);
	fputs(data, file);
	fputs("</FP>\n", file);
		fputs("<K0>", file);
	sprintf(data, "%f", current->K0);
	fputs(data, file);
	fputs("</K0>\n", file);
		fputs("<TH0>", file);
	sprintf(data, "%f", current->TH0);
	fputs(data, file);
	fputs("</TH0>\n", file);
		fputs("<R0>", file);
	sprintf(data, "%f", current->R0);
	fputs(data, file);
	fputs("</R0>\n", file);
		fputs("<coordx>", file);
	write_float_static_array(file, current->coordx, 11);
	fputs("</coordx>\n", file);
		fputs("<coordy>", file);
	write_float_static_array(file, current->coordy, 11);
	fputs("</coordy>\n", file);
		fputs("<xnbr_node0>", file);
	write_float_dynamic_array(file, &current->xnbr_node0);
	fputs("</xnbr_node0>\n", file);
		fputs("<xnbr_node1>", file);
	write_float_dynamic_array(file, &current->xnbr_node1);
	fputs("</xnbr_node1>\n", file);
		fputs("<xnbr_node2>", file);
	write_float_dynamic_array(file, &current->xnbr_node2);
	fputs("</xnbr_node2>\n", file);
		fputs("<xnbr_node3>", file);
	write_float_dynamic_array(file, &current->xnbr_node3);
	fputs("</xnbr_node3>\n", file);
		fputs("<xnbr_node4>", file);
	write_float_dynamic_array(file, &current->xnbr_node4);
	fputs("</xnbr_node4>\n", file);
		fputs("<xnbr_node5>", file);
	write_float_dynamic_array(file, &current->xnbr_node5);
	fputs("</xnbr_node5>\n", file);
		fputs("<xnbr_node6>", file);
	write_float_dynamic_array(file, &current->xnbr_node6);
	fputs("</xnbr_node6>\n", file);
		fputs("<xnbr_node7>", file);
	write_float_dynamic_array(file, &current->xnbr_node7);
	fputs("</xnbr_node7>\n", file);
		fputs("<xnbr_node8>", file);
	write_float_dynamic_array(file, &current->xnbr_node8);
	fputs("</xnbr_node8>\n", file);
		fputs("<xnbr_node9>", file);
	write_float_dynamic_array(file, &current->xnbr_node9);
	fputs("</xnbr_node9>\n", file);
		fputs("<xnbr_node10>", file);
	write_float_dynamic_array(file, &current->xnbr_node10);
	fputs("</xnbr_node10>\n", file);
		fputs("<ynbr_node0>", file);
	write_float_dynamic_array(file, &current->ynbr_node0);
	fputs("</ynbr_node0>\n", file);
		fputs("<ynbr_node1>", file);
	write_float_dynamic_array(file, &current->ynbr_node1);
	fputs("</ynbr_node1>\n", file);
		fputs("<ynbr_node2>", file);
	write_float_dynamic_array(file, &current->ynbr_node2);
	fputs("</ynbr_node2>\n", file);
		fputs("<ynbr_node3>", file);
	write_float_dynamic_array(file, &current->ynbr_node3);
	fputs("</ynbr_node3>\n", file);
		fputs("<ynbr_node4>", file);
	write_float_dynamic_array(file, &current->ynbr_node4);
	fputs("</ynbr_node4>\n", file);
		fputs("<ynbr_node5>", file);
	write_float_dynamic_array(file, &current->ynbr_node5);
	fputs("</ynbr_node5>\n", file);
		fputs("<ynbr_node6>", file);
	write_float_dynamic_array(file, &current->ynbr_node6);
	fputs("</ynbr_node6>\n", file);
		fputs("<ynbr_node7>", file);
	write_float_dynamic_array(file, &current->ynbr_node7);
	fputs("</ynbr_node7>\n", file);
		fputs("<ynbr_node8>", file);
	write_float_dynamic_array(file, &current->ynbr_node8);
	fputs("</ynbr_node8>\n", file);
		fputs("<ynbr_node9>", file);
	write_float_dynamic_array(file, &current->ynbr_node9);
	fputs("</ynbr_node9>\n", file);
		fputs("<ynbr_node10>", file);
	write_float_dynamic_array(file, &current->ynbr_node10);
	fputs("</ynbr_node10>\n", file);

	fputs("</xagent>\n", file);
}


void FLAME_write_xml(char * location, int iteration_number, int * output_types, int output_type_size)
{
	/* Pointer to file */
	FILE *file;
	char data[1000];
	
		sprintf(data, "%s%i.xml", location, iteration_number);
	
	if((file = fopen(data, "w"))==NULL)
	{
		printf("Error: cannot open file '%s' for writing\n", data);
		exit(0);
	}
	
	
	fputs("<states>\n", file);
	if(FLAME_integer_in_array(0, output_types, output_type_size))
	{
		fputs("<itno>", file);
		sprintf(data, "%i", iteration_number);
		fputs(data, file);
		fputs("</itno>\n", file);
		fputs("<environment>\n" , file);
			fputs("<NNODES>", file);
		sprintf(data, "%i", FLAME_environment_variable_NNODES);
		fputs(data, file);
		fputs("</NNODES>\n", file);
			fputs("<WIDTH>", file);
		sprintf(data, "%f", FLAME_environment_variable_WIDTH);
		fputs(data, file);
		fputs("</WIDTH>\n", file);
			fputs("<DT>", file);
		sprintf(data, "%f", FLAME_environment_variable_DT);
		fputs(data, file);
		fputs("</DT>\n", file);
			fputs("<NOISE>", file);
		sprintf(data, "%f", FLAME_environment_variable_NOISE);
		fputs(data, file);
		fputs("</NOISE>\n", file);
			fputs("<DELTA>", file);
		sprintf(data, "%f", FLAME_environment_variable_DELTA);
		fputs(data, file);
		fputs("</DELTA>\n", file);
			fputs("<KEL>", file);
		sprintf(data, "%f", FLAME_environment_variable_KEL);
		fputs(data, file);
		fputs("</KEL>\n", file);
			fputs("<KI>", file);
		sprintf(data, "%f", FLAME_environment_variable_KI);
		fputs(data, file);
		fputs("</KI>\n", file);
			fputs("<KTH>", file);
		sprintf(data, "%f", FLAME_environment_variable_KTH);
		fputs(data, file);
		fputs("</KTH>\n", file);
			fputs("<SIDEBOX>", file);
		sprintf(data, "%f", FLAME_environment_variable_SIDEBOX);
		fputs(data, file);
		fputs("</SIDEBOX>\n", file);
			fputs("<SIDEX>", file);
		sprintf(data, "%i", FLAME_environment_variable_SIDEX);
		fputs(data, file);
		fputs("</SIDEX>\n", file);
			fputs("<SIDEY>", file);
		sprintf(data, "%i", FLAME_environment_variable_SIDEY);
		fputs(data, file);
		fputs("</SIDEY>\n", file);
			fputs("</environment>\n" , file);
	}
	
	
	if(FLAME_integer_in_array(0, output_types, output_type_size) || FLAME_integer_in_array(1, output_types, output_type_size))
	{
		current_xmachine_Sporo_holder = Sporo_start_state->agents;
			while(current_xmachine_Sporo_holder)
			{
				write_Sporo_agent(file, current_xmachine_Sporo_holder->agent);

				current_xmachine_Sporo_holder = current_xmachine_Sporo_holder->next;
			}
	}
	
	fputs("</states>\n" , file);

	/* Close the file */
	(void)fclose(file);
}

/** \fn void saveiterationdata(int iteration_number)
 * \brief Save X-machine memory to a file.
 * \param iteration_number The current iteration number.
 */
void saveiterationdata(int iteration_number)
{
	FLAME_output * current_FLAME_output;
	FLAME_output * current_FLAME_output2;
	int output_types[1000];
	int output_type_size = 0;

	/* For each output */
	for(current_FLAME_output = FLAME_outputs; current_FLAME_output != NULL; current_FLAME_output = current_FLAME_output->next)
	{
		/* If period/phase equates to current iteration */
		if(iteration_number%current_FLAME_output->period == current_FLAME_output->phase)
		{
			/* If output has already been handled */
			if(current_FLAME_output->flag == 0)
			{
				/* Check snapshot */
				if(current_FLAME_output->type == 0)
				{
					//snapshot_location = current_FLAME_output->location;
					output_types[0] = 0;
					FLAME_write_xml(current_FLAME_output->location, iteration_number, output_types, 1);
					/* Set flag to 1 and every output to same location */
					current_FLAME_output->flag = 1;
					for(current_FLAME_output2 = FLAME_outputs; current_FLAME_output2 != NULL; current_FLAME_output2 = current_FLAME_output2->next)
					{
						if(strcmp(current_FLAME_output->location, current_FLAME_output2->location) == 0)
						{
							current_FLAME_output2->flag = 1;
						}
					}
				}
				if(current_FLAME_output->type == 1)
				{
					/* Reinitialise agent size */
					output_type_size = 0;
					output_types[0] = 1;
					
					current_FLAME_output->flag = 1;
					for(current_FLAME_output2 = FLAME_outputs; current_FLAME_output2 != NULL; current_FLAME_output2 = current_FLAME_output2->next)
					{
						if(current_FLAME_output2->flag == 0 && strcmp(current_FLAME_output->location, current_FLAME_output2->location) == 0)
						{
							output_types[++output_type_size] = current_FLAME_output2->type;
							
							current_FLAME_output2->flag = 1;
						}
					}
					
					FLAME_write_xml(current_FLAME_output->location, iteration_number, output_types, output_type_size);
				}
			}
		}
	}
	/* Set flags back to 0 */
	for(current_FLAME_output = FLAME_outputs; current_FLAME_output != NULL; current_FLAME_output = current_FLAME_output->next)
	{
		current_FLAME_output->flag = 0;
	}
}

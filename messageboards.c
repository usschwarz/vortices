/**
 * \file  messageboards.c
 * \brief Holds message board functions.
 */

#include "header.h"



/* Box filtering functions */




union pu_state 
{
    m_state *ptr;
    void *ptr_anon;
};

/** \fn void add_state_message(int id, float coordx, float coordy)
 * \brief Add state message by calling internal and processing.
 * \param id Message variable.
 * \param coordx Message variable.
 * \param coordy Message variable.
 */
void add_state_message(int id, float coordx[], float coordy[])
{
    int rc;
	m_state msg;
    
    msg.id = id;
    memcpy(msg.coordx, coordx, 11*sizeof(float));
    memcpy(msg.coordy, coordy, 11*sizeof(float));
    
    
    rc = MB_AddMessage(b_state, &msg);
    #ifdef ERRCHECK
    if (rc != MB_SUCCESS)
    {
       fprintf(stderr, "ERROR: Could not add message to 'state' board\n");
       switch(rc) {
           case MB_ERR_INVALID:
               fprintf(stderr, "\t reason: 'state' board has not been created?\n");
               break;
           case MB_ERR_MEMALLOC:
               fprintf(stderr, "\t reason: out of memory\n");
               break;
           case MB_ERR_LOCKED:
               fprintf(stderr, "\t reason: 'state' board is locked\n");
               break;
           case MB_ERR_INTERNAL:
               fprintf(stderr, "\t reason: internal error. Recompile libmoard in debug mode for more info \n");
               break;
	       default:
               fprintf(stderr, "\t MB_AddMessage returned error code: %d (see libmboard docs for details)\n", rc);
               break;
	   }
	      
	   
       exit(rc);
    }
    #endif
}

inline static m_state* getInternalMessage_state(void)
{
    static m_state *msg_prev = NULL;
    union pu_state msg_pu;
    int rc;
    
    /* deallocate previously returned message */
    if (msg_prev != NULL) 
    {
        free(msg_prev);
    }
    else 
    {
        rc = MB_Iterator_Rewind(i_state); 
        #ifdef ERRCHECK
        if (rc != MB_SUCCESS)
        {
            fprintf(stderr, "ERROR: Could not rewind 'state' Iterator\n");
            switch(rc) {
                case MB_ERR_INVALID:
                    fprintf(stderr, "\t reason: 'state' Iterator has not been created?\n");
                    break;
	            default:
                    fprintf(stderr, "\t MB_Iterator_Rewind returned error code: %d (see libmboard docs for details)\n", rc);
                    break;
	        }
	       
	       
       	   exit(rc);
        }
        #endif
    }
    
    /* get next message from iterator */
    rc = MB_Iterator_GetMessage(i_state, &(msg_pu.ptr_anon));
    #ifdef ERRCHECK
    if (rc != MB_SUCCESS)
    {
       fprintf(stderr, "ERROR: Could not get message from 'state' Iterator\n");
       switch(rc) {
           case MB_ERR_INVALID:
               fprintf(stderr, "\t reason: 'state' Iterator has not been created?\n");
               break;
           case MB_ERR_MEMALLOC:
               fprintf(stderr, "\t reason: out of memory\n");
               break;
	       default:
               fprintf(stderr, "\t MB_Iterator_GetMessage returned error code: %d (see libmboard docs for details)\n", rc);
               break;
	       }
	       
	       
       	   exit(rc);
    }
    #endif
    
    /* store pointer so memory can be deallocated later */
    msg_prev = msg_pu.ptr;
    
    return msg_pu.ptr;
}

/** \fn xmachine_message_state * get_first_state_message()
 * \brief Get the first state message in the state message list.
 * \return The first message in the list.
 */
m_state * get_first_state_message()
{
	return getInternalMessage_state();
}

/** \fn xmachine_message_state * get_next_state_message(xmachine_message_state * current)
 * \brief Get the next state message in the state message list after the current message.
 * \param current The current message in the list.
 * \return The next message in the list.
 */
m_state * get_next_state_message(m_state * current)
{
	return getInternalMessage_state();
}


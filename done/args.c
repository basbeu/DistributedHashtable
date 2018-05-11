/**
 * @file args.c
 * @brief
 *
 * @date 08 Mai 2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "args.h"
 
/**
 * @brief parse optional arguments
 * @param argv ref to array of string to check for argument, with last elem == NULL
 * @param arg_tag string of char repsenting the argument to parse
 * @return 1 if -- is detected, -1 if an error occured, else 0
 */
int parse_arg(char ***argv, const char * arg_tag, size_t* arg_value){
	if(!strncmp((*argv)[0], "--", 2)){
		++(*argv);
		return 1;
	}else if(!strncmp((*argv)[0], arg_tag, strlen(arg_tag))){
		int j = sscanf((*argv)[1],"%zu", arg_value);
		(*argv)+=2;
		
		if(j == -1){
			return j;
		}
	}
	
	return 0;
}

args_t *parse_opt_args(size_t supported_args, char ***rem_argv){
	args_t* args = calloc(1, sizeof(args_t));
	
	if(args != NULL){
		
		int parsing_state = 0;
		if (supported_args & TOTAL_SERVERS){
			parsing_state = parse_arg(rem_argv,"-n",&args->N);
		}
		
		if (supported_args & PUT_NEEDED && parsing_state == 0){
			parsing_state = parse_arg(rem_argv,"-w",&args->W);
		}
		
		if (supported_args & GET_NEEDED && parsing_state == 0){
			parsing_state = parse_arg(rem_argv,"-r",&args->R);			
		}
		
		if(!strncmp((*rem_argv)[0], "--", 2)){
			++(*rem_argv);
		}
		
		if(parsing_state == -1){
			free(args);
			return NULL;
		}
	}
	
	return args;
}

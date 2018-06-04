#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for memset()

#include "system.h"
#include "client.h"
#include "network.h"
#include "node.h"
#include "config.h"
#include "util.h"

#define REQUIRED_ARGS_GET 1
#define REQUIRED_ARGS_PUT 2

error_code get(client_t client, int argc, char* argv[]);
error_code put(client_t client, int argc, char* argv[]);
static void print_time(const struct timespec* p_time_start, const struct timespec* p_time_end);

int main(int argc, char* argv[])
{
    
    printf("************** Scénario équilibré : R = W = N/2 + 1 **************\n");
    
    for(int n = 10; n < 21; ++ n){
		
		//***************** INITIALIZE ARGS ****************
		
		int w = n/2 + 1;
		int r = w;
		
		char** put_argv;
		int put_argc = 7;
		char** get_argv;
		int get_argc = 6;
		
		client_t client_put;
		client_t client_get;
		put_argv = calloc(10, sizeof(char*));
		get_argv = calloc(10, sizeof(char*));
		
		for(size_t i = 0; i < 10; ++ i){
			put_argv[i] = calloc(MAX_MSG_ELEM_SIZE, sizeof(char));
			get_argv[i] = calloc(MAX_MSG_ELEM_SIZE, sizeof(char));
		}

		printf("N = %d, R = %d, W = %d\n", n, w, r);
		
		//*********** SET ARGS ******************
		
		put_argv[0] = "Put";
		put_argv[1] = "-n";
		put_argv[3] = "-w";
		put_argv[5] = "abra";
		put_argv[6] = "cadabra";
		get_argv[0] = "Get";
		get_argv[1] = "-n";
		get_argv[3] = "-r";
		get_argv[5] = "abra";
		sprintf(put_argv[2], "%d", n);
		sprintf(put_argv[4], "%d", w);
		sprintf(get_argv[2], "%d", n);
		sprintf(get_argv[4], "%d", r);
		
		//*********** INITIALIZE CLIENTS *************
		 if(client_init((client_init_args_t) {
			&client_put, REQUIRED_ARGS_PUT, TOTAL_SERVERS | PUT_NEEDED,
			(size_t) put_argc, &put_argv
			}) != ERR_NONE) {
			printf("FAIL\n");
			return 1;
		}
		
		if(client_init((client_init_args_t) {
		&client_get, REQUIRED_ARGS_GET, TOTAL_SERVERS | GET_NEEDED,
		(size_t) get_argc, &get_argv
		}) != ERR_NONE) {
			printf("FAIL\n");
			return 1;
		}
		
		//************ CALCULATE TIME ****************
		
		struct timespec time_start, time_end;
		int time_start_ret = clock_gettime(CLOCK_MONOTONIC, &time_start);
		error_code err = put(client_put, put_argc, put_argv);
		//M_EXIT_IF_ERR(err, "Put fail");
		err = get(client_get, get_argc, get_argv);
		//M_EXIT_IF_ERR(err, "Get fail");

		int time_end_ret = clock_gettime(CLOCK_MONOTONIC, &time_end);
		printf("Total time : ");
		print_time(&time_start, &time_end);
		
		/*for(size_t i = 0; i < 10; ++ i){
			if(put_argv[i] != NULL){
				free(put_argv[i]);
			}
			if(get_argv[i] != NULL){
				free(get_argv[i]);
			}			
		}
		free(*put_argv);
		free(*get_argv);*/
	}
   
    printf("*********************************************************\n");
    
    
    printf("******************Scénario PUT uniquement ***************\n");
    
    for(size_t w = 1; w < 11; ++ w){
		
		//***************** INITIALIZE ARGS ****************
		
		
		char** put_argv;
		int put_argc = 7;
		
		client_t client_put;

		put_argv = calloc(10, sizeof(char*));
		
		for(size_t i = 0; i < 10; ++ i){
			put_argv[i] = calloc(MAX_MSG_ELEM_SIZE, sizeof(char));
		}

		printf("N = 20, W = %zu\n", w);
		
		//*********** SET ARGS ******************
		
		put_argv[0] = "Put";
		put_argv[1] = "-n";
		put_argv[2] = "20";
		put_argv[3] = "-w";
		put_argv[5] = "abra";
		put_argv[6] = "cadabra";
		
		sprintf(put_argv[4], "%zu", w);
		
		//*********** INITIALIZE CLIENTS *************
		 if(client_init((client_init_args_t) {
			&client_put, REQUIRED_ARGS_PUT, TOTAL_SERVERS | PUT_NEEDED,
			(size_t) put_argc, &put_argv
			}) != ERR_NONE) {
			printf("FAIL\n");
			return 1;
		}
		
		
		//************ CALCULATE TIME ****************
		
		struct timespec time_start, time_end;
		int time_start_ret = clock_gettime(CLOCK_MONOTONIC, &time_start);
		error_code err = put(client_put, put_argc, put_argv);
		//M_EXIT_IF_ERR(err, "Put fail");

		int time_end_ret = clock_gettime(CLOCK_MONOTONIC, &time_end);
		printf("Total time : ");
		print_time(&time_start, &time_end);
		
		/*for(size_t i = 0; i < 10; ++ i){
			free(put_argv[i]);
		}
		free(put_argv);*/
	}
   
    printf("****************************************************\n");
    
    printf("************** Scénario GET uniquement *************\n");
    
    for(size_t r = 1; r < 11; ++ r){
		
		//***************** INITIALIZE ARGS ****************
		
		char** get_argv;
		int get_argc = 6;
		
		client_t client_get;
		get_argv = calloc(10, sizeof(char*));
		
		for(size_t i = 0; i < 10; ++ i){
			get_argv[i] = calloc(MAX_MSG_ELEM_SIZE, sizeof(char));
		}

		printf("N = 20, R = %zu\n", r);
		
		//*********** SET ARGS ******************
		
		get_argv[0] = "Get";
		get_argv[1] = "-n";
		get_argv[2] = "20";
		get_argv[3] = "-r";
		get_argv[5] = "abra";
		
		sprintf(get_argv[4], "%zu", r);
		
		//*********** INITIALIZE CLIENTS *************

		
		if(client_init((client_init_args_t) {
		&client_get, REQUIRED_ARGS_GET, TOTAL_SERVERS | GET_NEEDED,
		(size_t) get_argc, &get_argv
		}) != ERR_NONE) {
			printf("FAIL\n");
			return 1;
		}
		
		//************ CALCULATE TIME ****************
		
		struct timespec time_start, time_end;
		int time_start_ret = clock_gettime(CLOCK_MONOTONIC, &time_start);
		error_code err = get(client_get, get_argc, get_argv);
		//M_EXIT_IF_ERR(err, "Get fail");

		int time_end_ret = clock_gettime(CLOCK_MONOTONIC, &time_end);
		printf("Total time : ");
		print_time(&time_start, &time_end);
		/*for(size_t i = 0; i < 10; ++ i){
			free(get_argv[i]);
		}
		free(get_argv);*/
	}
   
    printf("*********************************************************\n");
    
    printf("********** TAILLE DES CLES VARIABLES N = 20, W = R = 2 ********\n");
    
    char** keys;
	keys = calloc(20, sizeof(char*));
		
	for(size_t i = 0; i < 20; ++ i){
		keys[i] = calloc(i+2, sizeof(char));
	}
	
	keys[0] = "a";
	keys[1] = "ab";
	keys[2] = "abc";
	keys[3] = "abcd";
	keys[4] = "abcde";
	keys[5] = "abcdef";
	keys[6] = "abcdefg";
	keys[7] = "abcdefgh";
	keys[8] = "abcdefghi";
	keys[9] = "abcdefghij";
	keys[10] = "abcdefghijk";
	keys[11] = "abcdefghijkl";
	keys[12] = "abcdefghijklm";
	keys[13] = "abcdefghijklmn";
	keys[14] = "abcdefghijklmno";
	keys[15] = "abcdefghijklmnop";
	keys[16] = "abcdefghijklmnopq";
	keys[17] = "abcdefghijklmnopqr";
	keys[18] = "abcdefghijklmnopqrs";
	keys[19] = "abcdefghijklmnopqrst";
    
    for(size_t n = 0; n < 20; ++ n){
		
		
		//***************** INITIALIZE ARGS ****************
		
		char** put_argv;
		int put_argc = 7;
		char** get_argv;
		int get_argc = 6;
		
		client_t client_put;
		client_t client_get;
		put_argv = calloc(10, sizeof(char*));
		get_argv = calloc(10, sizeof(char*));
		
		for(size_t i = 0; i < 10; ++ i){
			put_argv[i] = calloc(MAX_MSG_ELEM_SIZE, sizeof(char));
			get_argv[i] = calloc(MAX_MSG_ELEM_SIZE, sizeof(char));
		}

		printf("Clé de taille %zu : %s\n", n+1, keys[n]);
		
		//*********** SET ARGS ******************
		
		put_argv[0] = "Put";
		put_argv[1] = "-n";
		put_argv[2] = "20";
		put_argv[3] = "-w";
		put_argv[4] = "2";
		put_argv[5] = keys[n];
		put_argv[6] = "value";
		
		get_argv[0] = "Get";
		get_argv[1] = "-n";
		get_argv[2] = "20";
		get_argv[3] = "-r";
		get_argv[4] = "2";
		get_argv[5] = keys[n];
		
		//*********** INITIALIZE CLIENTS *************
		 if(client_init((client_init_args_t) {
			&client_put, REQUIRED_ARGS_PUT, TOTAL_SERVERS | PUT_NEEDED,
			(size_t) put_argc, &put_argv
			}) != ERR_NONE) {
			printf("FAIL\n");
			return 1;
		}
		
		if(client_init((client_init_args_t) {
		&client_get, REQUIRED_ARGS_GET, TOTAL_SERVERS | GET_NEEDED,
		(size_t) get_argc, &get_argv
		}) != ERR_NONE) {
			printf("FAIL\n");
			return 1;
		}
		
		//************ CALCULATE TIME ****************
		
		struct timespec time_start, time_end;
		int time_start_ret = clock_gettime(CLOCK_MONOTONIC, &time_start);
		error_code err = put(client_put, put_argc, put_argv);
		//M_EXIT_IF_ERR(err, "Put fail");
		err = get(client_get, get_argc, get_argv);
		//M_EXIT_IF_ERR(err, "Get fail");

		int time_end_ret = clock_gettime(CLOCK_MONOTONIC, &time_end);
		printf("Total time : ");
		print_time(&time_start, &time_end);
		
		/*for(size_t i = 0; i < 10; ++ i){
			free(put_argv[i]);
			free(get_argv[i]);
		}
		free(put_argv);
		free(get_argv);*/
	}
	
	/*for(size_t i = 0; i < 10; ++ i){
		free(keys[i]);
	}*/
	
	free(keys);
	
	
   
    printf("*********************************************************\n");
    
     printf("********** TAILLE DES VALEURS VARIABLES N = 20, W = R = 2 ********\n");
    
    char** values;
	values = calloc(20, sizeof(char*));
		
	for(size_t i = 0; i < 20; ++ i){
		values[i] = calloc(i+2, sizeof(char));
	}
	
	values[0] = "a";
	values[1] = "ab";
	values[2] = "abc";
	values[3] = "abcd";
	values[4] = "abcde";
	values[5] = "abcdef";
	values[6] = "abcdefg";
	values[7] = "abcdefgh";
	values[8] = "abcdefghi";
	values[9] = "abcdefghij";
	values[10] = "abcdefghijk";
	values[11] = "abcdefghijkl";
	values[12] = "abcdefghijklm";
	values[13] = "abcdefghijklmn";
	values[14] = "abcdefghijklmno";
	values[15] = "abcdefghijklmnop";
	values[16] = "abcdefghijklmnopq";
	values[17] = "abcdefghijklmnopqr";
	values[18] = "abcdefghijklmnopqrs";
	values[19] = "abcdefghijklmnopqrst";
    
    for(size_t n = 0; n < 20; ++ n){
		
		
		//***************** INITIALIZE ARGS ****************
		
		char** put_argv;
		int put_argc = 7;
		char** get_argv;
		int get_argc = 6;
		
		client_t client_put;
		client_t client_get;
		put_argv = calloc(10, sizeof(char*));
		get_argv = calloc(10, sizeof(char*));
		
		for(size_t i = 0; i < 10; ++ i){
			put_argv[i] = calloc(MAX_MSG_ELEM_SIZE, sizeof(char));
			get_argv[i] = calloc(MAX_MSG_ELEM_SIZE, sizeof(char));
		}

		printf("Valeur de taille %zu : %s\n", n+1, values[n]);
		
		//*********** SET ARGS ******************
		
		put_argv[0] = "Put";
		put_argv[1] = "-n";
		put_argv[2] = "20";
		put_argv[3] = "-w";
		put_argv[4] = "2";
		put_argv[5] = "key";
		put_argv[6] = values[n];
		
		get_argv[0] = "Get";
		get_argv[1] = "-n";
		get_argv[2] = "20";
		get_argv[3] = "-r";
		get_argv[4] = "2";
		get_argv[5] = "key";
	
		
		//*********** INITIALIZE CLIENTS *************
		 if(client_init((client_init_args_t) {
			&client_put, REQUIRED_ARGS_PUT, TOTAL_SERVERS | PUT_NEEDED,
			(size_t) put_argc, &put_argv
			}) != ERR_NONE) {
			printf("FAIL\n");
			return 1;
		}
		
		if(client_init((client_init_args_t) {
		&client_get, REQUIRED_ARGS_GET, TOTAL_SERVERS | GET_NEEDED,
		(size_t) get_argc, &get_argv
		}) != ERR_NONE) {
			printf("FAIL\n");
			return 1;
		}
		
		//************ CALCULATE TIME ****************
		
		struct timespec time_start, time_end;
		int time_start_ret = clock_gettime(CLOCK_MONOTONIC, &time_start);
		error_code err = put(client_put, put_argc, put_argv);
		//M_EXIT_IF_ERR(err, "Put fail");
		err = get(client_get, get_argc, get_argv);
		//M_EXIT_IF_ERR(err, "Get fail");

		int time_end_ret = clock_gettime(CLOCK_MONOTONIC, &time_end);
		printf("Total time : ");
		print_time(&time_start, &time_end);
		
		/*for(size_t i = 0; i < 10; ++ i){
			free(put_argv[i]);
			free(get_argv[i]);
		}
		free(put_argv);
		free(get_argv);*/
	}
	
	/*for(size_t i = 0; i < 10; ++ i){
		free(keys[i]);
	}*/
	
	free(values);
	
	
   
    printf("*********************************************************\n");

}

error_code get(client_t client, int argc, char* argv[]){
	    
    pps_value_t value = NULL;

    error_code err = network_get(client, argv[0], &value);

    if(err == ERR_NONE) {
        printf("OK %s\n", value);
    } else {
        return err;
    }

    free_const_ptr(value);
    client_end(&client);

    return err;
}

error_code put(client_t client, int argc, char* argv[]){
	
    error_code err = network_put(client, argv[0], argv[1]);

    if(err == ERR_NONE) {
        printf("OK\n");
    } else {
        return err;
    }
    client_end(&client);

    return err;
}

static void print_time(const struct timespec* p_time_start,
                       const struct timespec* p_time_end)
{
    long nsec = p_time_end->tv_nsec - p_time_start->tv_nsec;
    while (nsec < 0) nsec += 1000000000;
    printf("%ld%09ld\n", p_time_end->tv_sec - p_time_start->tv_sec, nsec);
    printf("------------------------------\n");
}

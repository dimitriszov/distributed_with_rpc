/************************************************************/
/*         Εργαστήριο Κατανεμημένα Συστήματα                */
/* Ονοματεπώνυμο: Ζωβοϊλης Δημήτριος-Μάριος                 */
/* ΑΜ: 19390064                                             */
/* Το αρχείο αυτό αποτελεί την υλοποίηση του RPC client και */
/* την πλευρά του server στην σύνδεση server-socket         */
/************************************************************/
#include "server_funcs.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <pthread.h>
#include <unistd.h>


typedef struct {
    int sockfd;
    char *host;
} Thread_data;


void error(char *msg);
void *server_prog_1(void *arg);


/**********************************/
/*          driver code           */
/**********************************/
int main (int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen, i;
    struct sockaddr_in serv_addr, cli_addr;
    pthread_t thread[50];
    char *host;
    Thread_data *thread_data;
    
    /* check that the program was called corectly */
    if (argc < 3) {
        printf ("usage: %s server_host port_number\n", argv[0]);
        exit (1);
    }
    
    /* open the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd < 0) 
        error("ERROR opening socket");
    
    /* get the host and the port number from the arguments */
    host = argv[1];
    portno = atoi(argv[2]);
    
    /* setup the struct sockaddr_in */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    
    /* bind the socket */
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd, 5); // wait for 5 connections
    
    /* for every connection open a thread */
    for (i=0;;)  {
        printf("Waiting for a connection...\n");
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) 
            error("ERROR on accept");
        
        /* pass the needed data to the struct Thread_data */
        thread_data = (Thread_data*)malloc(sizeof(Thread_data));
        thread_data->sockfd = newsockfd;
        thread_data->host = host;
        
        /* create the thread while calling server_prog_1 and passing thread_data */
        pthread_create(&(thread[i++]), NULL, server_prog_1, (void *)thread_data);
        
    }
    exit (0);
}


/**********************************/
/*        useful functions        */
/**********************************/

/*
 * The function is used in case of an error.
 * It just exits the execution of the program
 * and print the error message.
 */
void error(char *msg)
{
    perror(msg);
    exit(1);
}


/*
 * The function is called by each thread and is
 * responsible for calling the RPC server to do the
 * calculation that the client requested. 
 */
void *server_prog_1(void *arg)
{
    Thread_data *thread_data = (Thread_data*) arg;
    int choice, n;
    int so = thread_data->sockfd;
    char *host = thread_data->host;
    char str[100];
    
    
    CLIENT *clnt;
    double  *result_1;
    int_array  avg_1_arg;
    intpair  *result_2;
    int_array  max_min_1_arg;
    double_array  *result_3;
    mult_array  mult_1_arg;
    
    #ifndef DEBUG
    /* create the rpc client */
    clnt = clnt_create (host, SERVER_PROG, AVG_VERS, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror (host);
        exit (1);
    }
    #endif /* DEBUG */
    do {
        /* receive the users choice */
        n = recv(so, str, 100, 0);
        choice = atoi(str);
        
        /* 
         * depending the choice of the user a diferent
         * function  will be called on the rpc server 
         */
        if(choice == 1) { // average value of int array
            int len, i = 0;
            char *token;
            const char s[2] = " ";
            char output[50];
            
            /* receive the length of the array */
            n = recv(so, str, 100, 0);
            len = atoi(str);
            avg_1_arg.arr.arr_len = len;
            avg_1_arg.arr.arr_val = malloc(len*sizeof(int));
            
            /* receive the array into a string */
            n = recv(so, str, 100, 0);
            
            /* 
             * We will use strtok to convert the string
             * str that contains the int array into an
             * actual int array. That way doing any 
             * calculations will be much easier.
             */
            i = 0;
            // get the first token
            token = strtok(str, s);
            // walk through other tokens
            while(token != NULL) {
                avg_1_arg.arr.arr_val[i] = atoi(token);
                i++;
                token = strtok(NULL, s);
            }
            
            /* call the rpc server to calculate the average value */
            result_1 = avg_1(&avg_1_arg, clnt);
            if (result_1 == (double *) NULL) {
                clnt_perror (clnt, "call failed");
            }
            
            /* copy average into a string and send it to the client */
            snprintf(output, 50, "%lf", *result_1);
            send(so, output, 50, 0);
            
        } else if(choice == 2) { // max and min value of an int array
            int len, i = 0;
            char *token;
            const char s[2] = " ";
            char output[50];
            
            /* receive the length of the array */
            n = recv(so, str, 100, 0);
            len = atoi(str);
            max_min_1_arg.arr.arr_len = len;
            max_min_1_arg.arr.arr_val = malloc(len*sizeof(int));
            
            /* receive the array into a string */
            n = recv(so, str, 100, 0);
            
            /* 
             * We will use strtok to convert the string
             * str that contains the int array into an
             * actual int array. That way doing any 
             * calculations will be much easier.
             */
            i = 0;
            // get the first token
            token = strtok(str, s);
            // walk through other tokens
            while(token != NULL) {
                max_min_1_arg.arr.arr_val[i] = atoi(token);
                i++;
                token = strtok(NULL, s);
            }
            
            /* call the rpc server to calculate max and min values */
            result_2 = max_min_1(&max_min_1_arg, clnt);
            if (result_2 == (intpair *) NULL) {
                clnt_perror (clnt, "call failed");
            }
            
            /* copy max and min into a string and send them to the client */
            snprintf(output, 50, "%d %d", result_2->num1, result_2->num2);
            send(so, output, 50, 0);
            
        } else if(choice == 3) { // calculate the product of double * array
            int len, i;
            double a;
            char *token;
            const char s[2] = " ";
            char output[500];
            
            /* receive the length of the array */
            n = recv(so, str, 100, 0);
            len = atoi(str);
            mult_1_arg.arr.arr_len = len;
            mult_1_arg.arr.arr_val = malloc(len*sizeof(int));
            
            /* receive the array into a string */
            n = recv(so, str, 100, 0);
            
            /* 
             * We will use strtok to convert the string
             * str that contains the int array into an
             * actual int array. That way doing any 
             * calculations will be much easier.
             */
            i = 0;
            // get the first token
            token = strtok(str, s);
            // walk through other tokens
            while(token != NULL) {
                mult_1_arg.arr.arr_val[i] = atoi(token);
                i++;
                token = strtok(NULL, s);
            }
            
            /* receive the double and save it */
            n = recv(so, str, 100, 0);
            sscanf(str, "%lf", &mult_1_arg.num);
            
            /* call the rpc server to calculate the product */
            result_3 = mult_1(&mult_1_arg, clnt);
            if (result_3 == (double_array *) NULL) {
                clnt_perror (clnt, "call failed");
            }
            
            /*
             * Since the result is a double array and
             * we need to return a string we will use
             * strcat to fill the string output with
             * the new array and thne send it to the 
             * client.
             */
            // to avoid having other characters to the string
            output[0] = '\0';
            for(i = 0; i < len; i++) {
                // first copy the double to str
                snprintf(str, 100, "%lf ", result_3->arr.arr_val[i]);
                // then append output with the new double in str
                strcat(output, str);
            }
            // finally send the output to the client
            send(so, output, 500, 0);
            
        } else { // exit
            choice = 4;
        }
    } while (choice < 4);
    
    close(so);
    pthread_exit(NULL);
    #ifndef	DEBUG
    clnt_destroy (clnt);
    #endif	 /* DEBUG */
}

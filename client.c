/************************************************************/
/*         Εργαστήριο Κατανεμημένα Συστήματα                */
/* Ονοματεπώνυμο: Ζωβοϊλης Δημήτριος-Μάριος                 */
/* ΑΜ: 19390064                                             */
/* Το αρχείο αυτό αποτελεί την υλοποίηση του client         */
/************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>


void error(char *msg);


/**********************************/
/*          driver code           */
/**********************************/
int main(int argc, char *argv[])
{
    int sockfd, portno, n, t, done;
    int choice;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char str[100];
    
    /* check that the program was called corectly */
    if (argc < 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }
    
    /* open the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    
    /* get the server */
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    portno = atoi(argv[2]); // get the port number
    serv_addr.sin_port = htons(portno);
    
    /* Try to connect with the server */
    printf("Trying to connect...\n");
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    printf("Connected.\n");
    
    done = 0;
    do {
        /* display the choices */
        printf("Dia8esimes epiloges:\n");
        printf("\t1: Ypologismos mesou orou dianysmatos.\n");
        printf("\t2: Ypologismos megistis-elaxisths timhs dianysmatos.\n");
        printf("\t3: Ypologismos ginomenoy dianysmatos me ari8mo.\n");
        printf("\t4: Ejodos.\n");
        printf("Dwse thn epilogh sou:\n");
        printf("> ");
        
        /* get users choice and send it to the server */
        fgets(str, 100, stdin);
        send(sockfd, str, strlen(str), 0);
        choice = atoi(str);
        
        /* 
         * depending the choice of the user a diferent
         * function  will be called on the rpc server 
         */
        if(choice == 1) { // average value of int array
            /* get the length of the array */
            printf("Dwse to mhkos toy dianysmatos:\n");
            printf("> ");
            fgets(str, 100, stdin);
            send(sockfd, str, strlen(str), 0);
            
            /* get the nums of the array */
            printf("Dwse to dianysma, xwrismeno me kena:\n");
            printf("> ");
            fgets(str, 100, stdin);
            send(sockfd, str, strlen(str), 0);
            
            /* receive the average value and print it */
            recv(sockfd, str, 50, 0);
            printf("Mesh timh:\n");
            printf("%s\n", str);
            
        } else if(choice == 2) { // max and min value of an int array
            char *token;
            const char s[2] = " ";
            int max, min;
            
            /* get the length of the array */
            printf("Dwse to mhkos toy dianysmatos:\n");
            printf("> ");
            fgets(str, 100, stdin);
            send(sockfd, str, strlen(str), 0);
            
            /* get the nums of the array */
            printf("Dwse to dianysma, xwrismeno me kena:\n");
            printf("> ");
            fgets(str, 100, stdin);
            send(sockfd, str, strlen(str), 0);
            
            /* receive the max and min value and print them */
            recv(sockfd, str, 50, 0);
            printf("Max Min: %s\n", str);
            
        } else if(choice == 3) { // calculate the product of double * array
            char new_arr[500];
            
            /* get the length of the array */
            printf("Dwse to mhkos toy dianysmatos:\n");
            printf("> ");
            fgets(str, 100, stdin);
            send(sockfd, str, strlen(str), 0);
            
            /* get the nums of the array */
            printf("Dwse to dianysma, xwrismeno me kena:\n");
            printf("> ");
            fgets(str, 100, stdin);
            send(sockfd, str, strlen(str), 0);
            
            /* read the double */
            printf("Dwse ton pragmatiko ari8mo a:\n");
            printf("> ");
            fgets(str, 100, stdin);
            send(sockfd, str, strlen(str), 0);
            
            /* receive the new array and print it */
            recv(sockfd, new_arr, 500, 0);
            printf("Neos pinakas:\n");
            printf("%s\n", new_arr);
            
        } else { // client exits
            done = 1;
        }
    } while (!done);
    
    /* Close the socket and exit. */
    close(sockfd);
    return 0;
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
    exit(0);
}

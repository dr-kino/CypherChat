#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "chat_definitions.h"
#include "string.h"
#include "cypher_lib.h"

// Global variables
volatile sig_atomic_t flag = 0;
int sockfd = 0;
char nickname[LENGTH_NAME] = {};

void catch_ctrl_c_and_exit(int sig) {
    flag = 1;
}

void recv_msg_handler() {
    char receiveMessage[LENGTH_SEND] = {};
    char receiveMessage_tmp[LENGTH_SEND] = {};
    char message_dec[LENGTH_SEND] = {};
    char *message_enc;
    int messageLength = 0;
    int count = 0;
    char key[9];
    char iv[9];

    strcpy(key,"12345678");

    while (1) {
        int receive = recv(sockfd, receiveMessage, LENGTH_SEND, 0);

        for (count = 0; receiveMessage[count] != '\0'; ++count) {
            if ((receiveMessage[count + 1] == '\357') &&
                (receiveMessage[count + 2] == '\274') &&
                (receiveMessage[count + 3] == '\232')) {

                printf("%c", receiveMessage[count]);
                printf("%c", receiveMessage[count+1]);
                printf("%c", receiveMessage[count+2]);
                printf("%c", receiveMessage[count+3]);
                
                count = count + 4;
                break;
            }
            printf("%c", receiveMessage[count]);
        }

        if (receive > 0) {
            str_trim_lf(receiveMessage_tmp, LENGTH_MSG);
            for (messageLength = count; receiveMessage[messageLength] != '\0'; ++messageLength)
            {
                if ((receiveMessage[messageLength + 1] == ' ') &&
                    (receiveMessage[messageLength + 2] == 'f') &&
                    (receiveMessage[messageLength + 3] == 'r') &&
                    (receiveMessage[messageLength + 4] == 'o') &&
                    (receiveMessage[messageLength + 5] == 'm') &&
                    (receiveMessage[messageLength + 6] == ' ')) {

                    receiveMessage_tmp[messageLength - count] = receiveMessage[messageLength];
                    break;
                }
                receiveMessage_tmp[messageLength - count] = receiveMessage[messageLength];
            }

            // Call cypher lib: Decrypt
            strcpy(iv,"initvect");
            DES_CBC(1,receiveMessage_tmp,key,iv,message_dec);
            printf("%s\n", message_dec);
            str_overwrite_stdout();
        } else if (receive == 0) {
            break;
        } else { 
            // -1 
        }
        // Cleanup variable
        for (messageLength = 0; messageLength < LENGTH_SEND; ++messageLength) { receiveMessage[messageLength] = 0; }
    }
}

void send_msg_handler() {
    char message[LENGTH_MSG] = {};
    char message_tmp[LENGTH_MSG] = {};
    char message_enc[LENGTH_MSG] = {};
    int messageLength = 0;
    char key[9];
    char iv[9];

    strcpy(key,"12345678");

    while (1) {
        str_overwrite_stdout();
        while (fgets(message, LENGTH_MSG, stdin) != NULL) {
            str_trim_lf(message, LENGTH_MSG);
            if (strlen(message) == 0) {
                str_overwrite_stdout();
            } else {
                break;
            }
        }

        // Call cypher lib: encrypt
        strcpy(iv,"initvect");
        DES_CBC(0,message,key,iv,message_enc);
        send(sockfd, message_enc, LENGTH_MSG, 0);
        if (strcmp(message, "exit") == 0) {
            break;
        }
        // Cleanup variable
        for (messageLength = 0; messageLength < LENGTH_MSG; ++messageLength) { message_enc[messageLength] = 0; }      
    }
    catch_ctrl_c_and_exit(2);
}

int main()
{
    signal(SIGINT, catch_ctrl_c_and_exit);

    // Naming
    printf("Please enter your name: ");
    if (fgets(nickname, LENGTH_NAME, stdin) != NULL) {
        str_trim_lf(nickname, LENGTH_NAME);
    }
    if (strlen(nickname) < 2 || strlen(nickname) >= LENGTH_NAME-1) {
        printf("\nName must be more than one and less than thirty characters.\n");
        exit(EXIT_FAILURE);
    }

    // Create socket
    sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sockfd == -1) {
        printf("Fail to create a socket.");
        exit(EXIT_FAILURE);
    }

    // Socket information
    struct sockaddr_in server_info, client_info;
    int s_addrlen = sizeof(server_info);
    int c_addrlen = sizeof(client_info);
    memset(&server_info, 0, s_addrlen);
    memset(&client_info, 0, c_addrlen);
    server_info.sin_family = PF_INET;
    server_info.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_info.sin_port = htons(8888);

    // Connect to Server
    int err = connect(sockfd, (struct sockaddr *)&server_info, s_addrlen);
    if (err == -1) {
        printf("Connection to Server error!\n");
        exit(EXIT_FAILURE);
    }
    
    // Names
    getsockname(sockfd, (struct sockaddr*) &client_info, (socklen_t*) &c_addrlen);
    getpeername(sockfd, (struct sockaddr*) &server_info, (socklen_t*) &s_addrlen);
    printf("Connect to Server: %s:%d\n", inet_ntoa(server_info.sin_addr), ntohs(server_info.sin_port));
    printf("You are: %s:%d\n", inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port));

    send(sockfd, nickname, LENGTH_NAME, 0);

    pthread_t send_msg_thread;
    if (pthread_create(&send_msg_thread, NULL, (void *) send_msg_handler, NULL) != 0) {
        printf ("Create pthread error!\n");
        exit(EXIT_FAILURE);
    }

    pthread_t recv_msg_thread;
    if (pthread_create(&recv_msg_thread, NULL, (void *) recv_msg_handler, NULL) != 0) {
        printf ("Create pthread error!\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        if(flag) {
            printf("\nBye\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}
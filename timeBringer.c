#include<stdio.h>
#include<string.h>    // for strlen
#include<sys/socket.h>
#include<arpa/inet.h> // for inet_addr
#include<unistd.h>    // for write
#include <ctype.h>
int main(int argc, char *argv[])
{
    int socket_desc , new_socket , c;
    struct sockaddr_in server , client;
    char * message;
    char rc_message [1000];
    
    
    //char * charDate2 = charDate;
    int PATH_MAX = 100;
    FILE *fp;
    int status;
    char path[PATH_MAX];
    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        puts("Could not create socket");
        return 1;
    }
        
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8889);
     
    // Bind
    if(bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        puts("Bind failed");
        return 1;
    }
    puts("Socket is binded");
     
    // Listen
    listen(socket_desc, 3);
     
    // Accept and incoming connection
    puts("Waiting for incoming connections...");
    
    c = sizeof(struct sockaddr_in);
    new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c) ;
    while(1){
        puts("Connection accepted");
        message = "Hello client, enter the time format you want:\n\n";
        write(new_socket, message, strlen(message));
        recv(new_socket, rc_message, 1000, 0);
        puts("Reply received as:\n");
        puts(rc_message);
        
        char firstPart[] = "\"+";//to add in front of input string to make string correct format
        char lastPart[] = "\"";//to add back of input string
        char * firstPart2 = firstPart;
        char * lastPart2 = lastPart;
        char *token = strtok(rc_message, "\r");//erase the \r from end of the string
        //printf( "token %s\n", token ); //printing the token
        
        //check if there is any space to control correctness of input
        char ch = ' ';
        int i=0;
        int count = 0;
        while (ch != '\0') {
            ch = rc_message[i];
            if (isspace(ch))
                count++;
  
            i++;
        }
        if(count != 0){
            char *token2 = strtok(token, " ");
            char charDate2[] = "date ";
            //printf( "%s\n", token2 ); //printing the token
            token2 = strtok(NULL, " ");            
            strcat(firstPart2, token2);
            //printf( "%s\n", firstPart2 ); //printing the token
            strcat(firstPart2, lastPart2);
            //printf( "%s\n", firstPart2 ); //printing the token
            strcat(charDate2, firstPart2);
            fp = popen(charDate2, "r");
            if (fp == NULL){
                printf("Please enter a valid format");
            }else{  
        
                while (fgets(path, PATH_MAX, fp) != NULL){
                    //check if there is left any % in input to control correctness
                    char * ptr = NULL;
                    int    ch = '%';
                    ptr = strchr( path, ch );
                    if(ptr == NULL){
                        write(new_socket, path, strlen(path));
                    }else{
                        char token3[] = "INCORRECT REQUEST\n";
                        
                        write(new_socket, token3, strlen(token3));
                    }
                }
            }   
            status = pclose(fp);
            if (status == -1) {
                //Error reported by pclose()
            }else {
                /* Use macros described under wait() to inspect `status' in order
                to determine success/failure of command executed by popen()*/
            }
        }else{
            char err2[] = "INCORRECT REQUEST\n";
            
            write(new_socket, err2 , strlen(err2));
        }
        
    }
    close(new_socket);
    close(socket_desc);
    close(c);
    return 0;
}


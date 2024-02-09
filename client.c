    #include <stdio.h>
    #include <stdlib.h>
    #include <errno.h>
    #include <string.h>
    #include <fcntl.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>

    #define FIFO_CLIENT_SERVER "CLIENT_SERVER_FIFO"
    #define FIFO_SERVER_CLIENT "SERVER_CLIENT_FIFO"

    int main()
    {
        char sir[300], mesaj[50];
        int buffer, fd_client_server, fd_server_client, flag_logare, marime, receptez;

        printf("\nSe contecteaza la server...\n");
        fd_client_server = open(FIFO_CLIENT_SERVER, O_WRONLY);
        printf("\nComanda: ");

        while (gets(sir), !feof(stdin)) 
        {  
            fd_server_client = open(FIFO_SERVER_CLIENT, O_RDONLY);
            if ((buffer = write(fd_client_server, sir, strlen(sir))) == -1)
                perror("Problema la scriere in FIFO!");
            else
            {
                read(fd_server_client, &receptez, sizeof(int));
                marime = read(fd_server_client, mesaj, receptez);
                mesaj[marime] = '\0';
                if (strcmp(mesaj, "Serverul a fost inchis cu succes.")==0) 
                {
                    printf("\n%s\n", mesaj);
                    printf("\n");
                    close(fd_server_client);
                    return 0;
                }
                else 
                {
                    printf("\n%s\n", mesaj);
                    close(fd_server_client);
                }   
            }
        printf("\nComanda: "); 
        }
    return 0;
    }




    #include <stdio.h>
    #include <stdlib.h>
    #include <errno.h>
    #include <string.h>
    #include <fcntl.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <sys/wait.h>

    #define FIFO_CLIENT_SERVER "CLIENT_SERVER_FIFO"
    #define FIFO_SERVER_CLIENT "SERVER_CLIENT_FIFO"

    int main()
    {
        char sir[300], mesaj[100], user[100], usernamelist[100];
        char* comanda;
        int fd_client_server, fd_server_client, fd_login;
        int buffer, marime, flag_user;
        int flag_logare = 0;

        FILE *in_file = fopen("users.txt", "r");

        mknod(FIFO_CLIENT_SERVER, S_IFIFO | 0666, 0);
        mknod(FIFO_SERVER_CLIENT, S_IFIFO | 0666, 0);
        
        printf("Astept conectarea la client...\n");
        fd_client_server = open(FIFO_CLIENT_SERVER, O_RDONLY);
        printf("Conectarea a avut loc cu succes.\n");

        do 
        {
            fd_server_client = open(FIFO_SERVER_CLIENT, O_WRONLY);
            if (-1 == (buffer = read(fd_client_server, sir, 300)))
                    perror("Eroare la citirea din FIFO.");
            else 
            {
                sir[buffer] = '\0';
                if (in_file == NULL)
                {
                    printf("Eroare la verificarea fisierului 'users'.\n");
                    exit(1);
                }
                else 
                {
                    comanda = strtok(sir,":");
                    if(strcmp(comanda, "login ")==0 && flag_logare == 1)
                    {
                        strcpy(mesaj, "Sunteti deja logat la un cont.");
                        marime=strlen(mesaj);
                        printf("S-au scris in FIFO %d bytes\n", marime);
                        write(fd_server_client, &marime, sizeof(int));
                        write(fd_server_client, mesaj, strlen(mesaj));
                    }
                    else 
                        if(strcmp(comanda, "login ") == 0 && flag_logare == 0)
                        {
                            comanda = strtok(NULL, " ");
                            int pid_logare, pipe_logare[2];
                            if(-1 == pipe(pipe_logare) )
                            {
                                perror("Eroare la crearea canalului anonim");  exit(1);
                            }
                            if(-1 == ( pid_logare = fork() ))
                            {
                                perror("Eroare la crearea unui proces fiu");  exit(2);
                            }
                            if(pid_logare) // proces parinte pentru login
                            {
                                write(pipe_logare[1],comanda,strlen(comanda));
                                close(pipe_logare[1]);                     
                                wait(NULL);
                                read(pipe_logare[0], &flag_user, 4);
                                
                                if(flag_user == 1) // verificam logarea
                                {
                                    flag_logare = 1;
                                    strcpy(mesaj, "Logarea a avut loc cu succes.");
                                    marime=strlen(mesaj);
                                    printf("S-au scris in FIFO %d bytes\n", marime);
                                    write(fd_server_client, &marime, sizeof(int));
                                    write(fd_server_client, mesaj, strlen(mesaj));
                                }
                                else
                                {
                                    strcpy(mesaj, "Logarea a esuat.");
                                    marime=strlen(mesaj);
                                    printf("S-au scris in FIFO %d bytes\n", marime);
                                    write(fd_server_client, &marime, sizeof(int));
                                    write(fd_server_client, mesaj, strlen(mesaj));
                                }
                            }
                            else // proces copil pentru login
                            {
                                if(-1 == (fd_login = read(pipe_logare[0], user, 100)))
                                {
                                    perror("Eroare citire din pipe.\n");
                                    exit(1);
                                }

                                user[fd_login] = '\0';    
                                close(pipe_logare[0]);                    

                                while(fgets(usernamelist, sizeof(usernamelist), in_file)) // verific user-ul
                                {                                                         
                                    usernamelist[strlen(usernamelist)-1]='\0';
                                    if(strcmp(usernamelist, user) == 0)
                                    {
                                        flag_user = 1;
                                        break;
                                    }
                                    else 
                                        flag_user = 0;
                                }

                                write(pipe_logare[1], &flag_user, 4);
                                fseek(in_file, 0, SEEK_SET );
                                strcpy(usernamelist, "");
                                strcpy(user, "");
                                strcpy(sir, "");
                                flag_user = 0;
                                exit(0);
                            } 
                        } 
                        else 
                            if(strcmp(comanda, "logout") == 0 && flag_logare == 0)
                            {
                                strcpy(mesaj, "Nu sunteti logat.");
                                marime=strlen(mesaj);
                                printf("S-au scris in FIFO %d bytes\n", marime);
                                write(fd_server_client, &marime, sizeof(int));
                                write(fd_server_client, mesaj, strlen(mesaj));
                            }
                            else
                                if(strcmp(comanda, "logout") == 0 && flag_logare == 1)
                                {
                                    flag_logare = 0;
                                    strcpy(mesaj, "V-ati delogat cu succes.");
                                    marime=strlen(mesaj);
                                    printf("S-au scris in FIFO %d bytes\n", marime);
                                    write(fd_server_client, &marime, sizeof(int));
                                    write(fd_server_client, mesaj, strlen(mesaj)); 
                                }
                                else 
                                    if(strcmp(comanda, "quit") == 0) 
                                    {
                                        buffer = 0;
                                        strcpy(mesaj, "Serverul a fost inchis cu succes.");
                                        marime=strlen(mesaj);
                                        printf("S-au scris in FIFO %d bytes\n", marime);
                                        write(fd_server_client, &marime, sizeof(int));
                                        write(fd_server_client, mesaj, strlen(mesaj)); 
                                    }
                                    else
                                    {
                                        strcpy(mesaj, "Comanda nu este valida.");
                                        marime=strlen(mesaj);
                                        printf("S-au scris in FIFO %d bytes\n", marime);
                                        write(fd_server_client, &marime, sizeof(int));
                                        write(fd_server_client, mesaj, strlen(mesaj));
                                    }        
                    strcpy(mesaj, "");
                } 
            }
            close(fd_server_client);
        } while (buffer > 0);
    }


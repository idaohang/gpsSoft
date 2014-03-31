#include <stdio.h>   /* Стандартные объявления ввода/вывода */
#include <string.h>  /* Объявления строковых функций */
#include <unistd.h>  /* Объявления стандартных функций UNIX */
#include <fcntl.h>   /* Объявления управления файлами */
#include <errno.h>   /* Объявления кодов ошибок */
#include <termios.h> /* Объявления управления POSIX-терминалом */
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
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
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    
    
    
    
	int fileDesc;
	char str[1000] = {0};
	unsigned int i = 0;
	char c = '\0';
	fileDesc = open_port();

		printf("start\n");
		while(1)
		{
			read(fileDesc, &c, 1);
			if(c != '\n') 
			{
				str[i] = c;
				i++;
			}
			if(c == '\n')
			{
				i=0;
			//	printf("\nUART message: %s\n",str);
                bzero(buffer,256);
				buffer = show(str);
                printf(buffer);                
                n = write(sockfd,buffer,strlen(buffer));
                if (n < 0)
                    error("ERROR writing to socket");
                    bzero(buffer,256);
                    n = read(sockfd,buffer,255);
                    if (n < 0)
                        error("ERROR reading from socket");
                printf("%s\n",buffer);
			}
		}	
	close(fileDesc);
    
    
    
    
    close(sockfd);
    return 0;
}




char *show(char *);

open_port(void)
{
	int fd; /* Файловый дескриптор для порта */
	struct termios options;

  	fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
  	if (fd == -1)
 	{
 	  	/*
    		* Could not open the port.
    		*/

    		perror("open_port: Unable to open /dev/ttyf1 - ");
  	}
  	else
    	fcntl(fd, F_SETFL, 0);
  
 	tcgetattr(fd, &options);
  	cfsetispeed(&options, B9600);
  	cfsetospeed(&options, B9600);
	options.c_cflag |= (CLOCAL | CREAD);	
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
	
	options.c_cflag &= ~( ICANON | ECHO | ECHOE |ISIG );
    	options.c_iflag &= ~(IXON | IXOFF | IXANY );
    	options.c_oflag &= ~OPOST;
	tcflush(fd, TCIFLUSH);
  	return (fd);
}


char *show(char *s) 
{
	char *str;
	str = s;
	size_t strLen = strlen(str);
	//char inputType[6] = {0};
	//printf("\nString0: %s\n", str);
	//printf("\nString0: %s size: %d\n", inputType, (int)strlen(inputType));
	char *inputType = (char *) malloc(6*sizeof(char));
	strncpy(inputType, str, 6);
	//printf("\nString1: %s length: %d\n", inputType, (int)strlen(inputType));

	if (!strcmp(inputType,"$GNRMC") || !strcmp(inputType, "$GPRMC"))
	    {
	        char *time = (char *) malloc(10*sizeof(char));
            char *beliveFlag = malloc(1);
	        char *longtude = malloc(9);
	        char *NS = malloc(1);
	        char *latitude = malloc(10);
	        char *WE = malloc(1);

	        int i=0;
	        int j = 0;
	        int delimiterCounter = 0;
	        char *buf = (char *) malloc(21*sizeof(char));
	        char output[30];
	
	        //printf("\nString0: %s\n", str);

	        //printf("Int1: %d\n", (int)strLen);
	        while(i<(int)strLen)
	        {
        		buf[j]=str[i];
		        j++;
		
		        if (str[i]=='$')
		        {
			        delimiterCounter = 0;
		        }
		        if (str[i]==',' && delimiterCounter < 7)
		        //if (str[i]==',' && (delimiterCounter < 7))
		        {
		   	        //buf[j-1] = '\0';
			        delimiterCounter++;
           		    //printf("BUF5 %s  j=%d delimit=%d\n", buf, j, delimiterCounter);
			        switch(delimiterCounter)
			        {
			            case(2):
                            	//sprintf(time,"%s", buf);
                            	strncpy(time, buf, j);
                            	//printf("time: %s\n", time);
			    	            break;
			            case(3):
                            	strncpy(beliveFlag, buf, j);
                            	//printf("beliveFlag: %s\n", beliveFlag);
                            	break;
                	    case(4):
                            	strncpy(longtude, buf, j);
                            	//printf("longtude: %s\n", longtude);
                            	break;
                	    case(5):
                            	strncpy(NS, buf, j);
                            	//printf("NS: %s\n", NS);
                            	break;
                	    case(6):
                            	strncpy(latitude, buf, j);
                            	//printf("latutide: %s  j=%d\n", latitude,j);
				        //puts(latitude);
                            	break;
                	    case(7):
                            	strncpy(WE, buf, j);
                            	//printf("WE: %s\n", WE);
                            	break;
                	    default:    break;
              		}
			        //memset(buf, 0, (int)strlen(buf));
			        j=0;
		        }
		        i++;
	        }
		    memset(str, 0, (int)strlen(str));
    	    sprintf(output,"%c%s%c%s\n",NS[0],longtude,WE[0],latitude);
	        free(inputType);
	        free(buf);
	        free(time);
	        free(beliveFlag);
	        free(longtude);
	        free(NS);
	        free(latitude);
	        free(WE);
	    }
	return output;
}




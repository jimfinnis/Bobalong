#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



FILE *infile;
int sockfd;
struct sockaddr_in serv_addr;

void udp_tx_str(char *buffer,int size)
{
    int n;
    
    n = sendto(sockfd, buffer,strlen(buffer), 0, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    if (n < 0) 
    {
	perror("ERROR writing to socket");
        exit(1);
    }
}

void udp_rx_str(char *buffer,int size)
{
    int offset=0;
    int n;
    while(offset<size)
    {
        n = read(sockfd,buffer+offset,1);
        //printf("got data at %d = %c (%X)\n",offset,buffer[offset],buffer[offset]);
        if(buffer[offset]==0&&offset>0)
        {
    	    break;
        }
        if (n < 0) 
        {
          perror("ERROR reading to socket");
    	  exit(1);
        }
        offset=offset+n;
    }
    printf("got data %s (%d bytes strlen %d)\n",buffer,offset,(int)strlen(buffer));
}
/*Setup connection to the serial port*/
void udp_init(int portno,char *host)
{
    //struct hostent *server;

    /* Create the UDP socket */
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        fprintf(stderr, "Failed to create socket\n");
        perror("socket");
        exit(EXIT_FAILURE);
    }
          
    memset(&serv_addr, 0, sizeof(serv_addr));             /* Clear struct */
    serv_addr.sin_family = AF_INET;                        /* Internet/IP */
    serv_addr.sin_addr.s_addr = INADDR_ANY;    /* IP address */
    serv_addr.sin_port = htons(portno);                      /* server port */
       
    struct hostent *remote_host = gethostbyname("127.0.0.1");
    memcpy(&serv_addr.sin_addr.s_addr,remote_host->h_addr,remote_host->h_length);
     
}

void read_data(int maxlen, char *buf)
{
    char *ret;
    ret = fgets(buf,maxlen,infile);
    if(ret==NULL)
    {
        printf("no data\n");
        fclose(infile);
        exit(0);
    }
    
}
                 
int main(int argc,char **argv)
{
    char buf[255];
    long msgtime,lasttime=0;
    long replay_speed=1;
           
    if(argc==3)
    {
        infile=fopen(argv[1],"r");
        
        if(infile==NULL)
        {
            fprintf(stderr,"Unable to open file %s\n",argv[1]);
            exit(EXIT_FAILURE);
        }
        
        replay_speed=atol(argv[2]);
    }
    else
    {
        fprintf(stderr,"usage: replay_log filename speedfactor\n");
        exit(EXIT_FAILURE);
    }
    
    udp_init(4321,"127.0.0.1");
    
    while(1)
    {
        memset(buf,0,255); //clear the buffer
        read_data(254,buf); 
        printf("Got data:\n %s\n",buf);
        
        
        sscanf(buf,"time=%ld",&msgtime);
        
        printf("message time = %ld lasttime = %ld delaying %ld clock time=",msgtime,lasttime,(msgtime-lasttime)/replay_speed);
        printf("%s",ctime(&msgtime));
        
        fflush(stdout);
        
        if(lasttime!=0)
        {
            double sleep_time = ((double) (msgtime-lasttime) / (double) replay_speed) * 1000000;
            
            printf("sleep_time = %lf\n",sleep_time/1000000);
            usleep(sleep_time);
            //sleep((msgtime-lasttime)/replay_speed);
        }
       
        lasttime=msgtime;

        udp_tx_str(buf,strlen(buf));	
    }
    return 0;
}                                           
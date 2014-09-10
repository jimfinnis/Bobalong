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
#include <errno.h>
#include "uart.h"

int uart_fd;
int inbound_udp_fd;
int outbound_udp_fd;
struct sockaddr_in inbound_addr;
struct sockaddr_in outbound_addr;


char compute_checksum(char *data,int length)
{
    char computed_checksum=0;

    for (int i = 0; i < length; i++)
    {
        computed_checksum = (char)computed_checksum ^ data[i];
    }

    return computed_checksum;
}

/* checks if a string's checksum is correct
 * assuming string ends with checksum followed by \n
 * returns 0 if checksums aren't the same, 1 if they are
 */
char verify_checksum(char *data,int length)
{
    char checksum_str[3];
    char received_checksum;
    char computed_checksum = compute_checksum(data,length-4);
    checksum_str[0]=data[length-3];
    checksum_str[1]=data[length-2];
    checksum_str[2]=0;
    sscanf(checksum_str,"%hhX",&received_checksum);

    if (computed_checksum==received_checksum)
    {
        return 1;
    }

    printf("Checksum failed, got %X(%s), expected %X\n",received_checksum,checksum_str,computed_checksum);
    return 0;
}



void udp_tx_str(int fd,char *buffer,int size,struct sockaddr_in *where)
{
    int n;
    n = sendto(fd, buffer,strlen(buffer), 0, (struct sockaddr *) where, sizeof(*where));

    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }
}

void udp_rx_str(int fd,char *buffer,int size)
{
    int n;
    uint8_t is_local=0;
    struct sockaddr_in src_addr;
    socklen_t src_addr_len=sizeof(src_addr);
    n=recvfrom(fd,buffer,size,0,(struct sockaddr *)&src_addr,&src_addr_len);

    if ((src_addr.sin_addr.s_addr&0x000000FF)!=0x0000007F)
    {
        printf("Not a locally originating packet, ignoring");
        printf(" (from: %s)\n",inet_ntoa(src_addr.sin_addr));
        is_local=0;
        buffer[0]=0;
    }
    else
    {
        is_local=1;
    }

    //n = read(fd,buffer+offset,1500);
    /*if(buffer[offset+n]==0)
    {

        break;
    }

    if(buffer[offset]==0&&offset>0)
    {
        break;
    } */
    if (n < 0)
    {
        perror("ERROR reading to socket");
        exit(1);
    }

    // printf("UDP: %s",buffer);
}
/*Setup connection to the serial port*/
int udp_init(int portno,char *host, uint8_t should_bind,struct sockaddr_in *serv_addr)
{
    //struct hostent *server;
    int fd;

    /* Create the UDP socket */
    if ((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        fprintf(stderr, "Failed to create socket\n");
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(serv_addr, 0, sizeof(*serv_addr));             /* Clear struct */
    serv_addr->sin_family = AF_INET;                        /* Internet/IP */
    serv_addr->sin_addr.s_addr = INADDR_ANY;    /* IP address */
    serv_addr->sin_port = htons(portno);                      /* server port */

    if (!should_bind)
    {
        /*int broadcast = 1;
        if (setsockopt(fd, SOL_SOCKET, SO_BROADCAST, (void *) &broadcast,sizeof(broadcast)) < 0){
             fprintf(stderr, "Failed to set broadcast");
             exit(EXIT_FAILURE);
         }*/
        struct hostent *remote_host = gethostbyname("127.0.0.1");
        memcpy(&serv_addr->sin_addr.s_addr,remote_host->h_addr,remote_host->h_length);
    }

    if (should_bind)
    {
        if (bind(fd, (struct sockaddr *) serv_addr,sizeof(*serv_addr)) < 0)
        {
            perror("ERROR on binding UDP socket");
        }
    }

    return fd;
}

void read_data(int maxlen, char *buf)
{
    uint8_t c;
    int i=0;
    c=uart_rx(uart_fd);

    //read the rest of the string until we get a line terminator or hit the maximum string length.
    while (c!=10&&c!=13&&i<maxlen)
    {
        buf[i]=(char)c;
        c=uart_rx(uart_fd);
        i++;
    }

    //terminate the string properly
    buf[i]=0;
}

int main(int argc,char **argv)
{
    char uart_buf[1500];
    char udp_buf[1500];
    char inbound_udp_buf[1500];
    char filename[255];
    int status=0;
    int i=0;
    char c=0;
    fd_set read_fds;
    int max_fd=0;
    uint8_t newline_found=0;
    int inport,outport;
    char checksum_str[3];
    time_t now = time(NULL);
    struct tm *now_tm = gmtime(&now);
    sprintf(filename,"%4d-%02d-%02d_%02d%02d.log",now_tm->tm_year+1900,now_tm->tm_mon+1,now_tm->tm_mday,now_tm->tm_hour,now_tm->tm_min);
    FILE *logfile = fopen(filename,"a");

    if (logfile==NULL)
    {
        fprintf(stderr,"Can't open logfile\n");
        exit(EXIT_FAILURE);
    }

    if (argc==5)
    {
        uart_fd=uart_init(atoi(argv[2]),argv[1]);
        inport=atoi(argv[3]);
        outport=atoi(argv[4]);
    }
    else
    {
        fprintf(stderr,"usage: ser2udp devicename baudrate inport outport\n");
        fprintf(stderr,"Data read from the serial port will be sent to outport, data arriving on inport will be sent to the serial port. All data is sent to localhost.\n");
        exit(EXIT_FAILURE);
    }

    outbound_udp_fd=udp_init(outport,"127.0.0.1",0,&outbound_addr);
    inbound_udp_fd=udp_init(inport,"127.0.0.1",1,&inbound_addr);

    if (inbound_udp_fd>uart_fd)
    {
        max_fd = inbound_udp_fd;
    }
    else
    {
        max_fd = uart_fd;
    }

    max_fd++;
    FD_ZERO(&read_fds);
    FD_SET(inbound_udp_fd,&read_fds);
    FD_SET(uart_fd,&read_fds);
    memset(uart_buf,0,1500); //clear the buffer
    memset(udp_buf,0,1500); //clear the buffer
    memset(inbound_udp_buf,0,1500); //clear the buffer

    while (1)
    {
        FD_ZERO(&read_fds);
        FD_SET(inbound_udp_fd,&read_fds);
        FD_SET(uart_fd,&read_fds);
        status=select(max_fd,&read_fds, NULL, NULL,NULL);

        /*   tv.tv_sec=5;
           tv.tv_usec=0;*/
        if (status==-1)
        {
            perror("Select error");
            exit(EXIT_FAILURE);
        }

        //input available
        if (FD_ISSET(uart_fd,&read_fds))
        {
            c=uart_rx(uart_fd);
            uart_buf[i]=c;
            i++;

            if (c=='\n')//terminate at the new line
            {
                if (!verify_checksum(uart_buf,i))
                {
                    printf("Checksum failed, message: %s\n",uart_buf);
                    fprintf(logfile,"serial message checksum failed: %s\n",uart_buf);
                }
                else
                {
                    uart_buf[i-4]='\n'; //cut off the checksum
                    uart_buf[i-3]=0; //cut off the checksum
                    printf("serial: %s\n",uart_buf);
                    fprintf(logfile,"serial: %s\n",uart_buf);
                    udp_tx_str(outbound_udp_fd,uart_buf,i,&outbound_addr);
                }

                i=0;
                memset(uart_buf,0,1500); //clear the buffer
            }
        }

        if (FD_ISSET(inbound_udp_fd,&read_fds))
        {
            //n = read(fd,inbound_udp_buf,1);
            udp_rx_str(inbound_udp_fd,inbound_udp_buf,1500);
            //if(strcmp(inbound_udp_buf,last_inbound_udp_buf)==0)
            printf("UDP: %s\n",inbound_udp_buf);
            fprintf(logfile,"UDP: %s\n",inbound_udp_buf);

            if (strlen(inbound_udp_buf)>0)
            {
                newline_found=0;

                for (int j=0; j<strlen(inbound_udp_buf); j++)
                {
                    if (inbound_udp_buf[j]=='\n'||inbound_udp_buf[j]=='\r') //append checksum before newline character
                    {
                        snprintf(checksum_str,3,"%02hhX",compute_checksum(inbound_udp_buf,j));
                        uart_tx(uart_fd,' '); //put in a space so we don't confuse the checksum with the data
                        uart_tx(uart_fd,checksum_str[0]);
                        uart_tx(uart_fd,checksum_str[1]);
                        uart_tx(uart_fd,'\n');
                        newline_found=1;
                        break;
                    }
                    else
                    {
                        uart_tx(uart_fd,inbound_udp_buf[j]);
                    }
                }

                if (!newline_found)
                {
                    snprintf(checksum_str,3,"%02hhX",compute_checksum(inbound_udp_buf,strlen(inbound_udp_buf)));
                    uart_tx(uart_fd,' '); //put in a space so we don't confuse the checksum with the data
                    uart_tx(uart_fd,checksum_str[0]);
                    uart_tx(uart_fd,checksum_str[1]);
                    uart_tx(uart_fd,'\n');
                }
            }

            memset(inbound_udp_buf,0,1500); //clear the buffer
        }

        //
        fflush(logfile);
        //fflush(stdout);
        //udp_tx_str(buf,strlen(buf));
    } //end of while loop

    return 0;
}
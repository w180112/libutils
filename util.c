/*\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
  UTIL.C
    the common utilities of all files are saved in this file.
/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/

#include    "common.h"

/*------------------------------------------------------------------
 * GetStrTok
 *
 * input   : cpp - points to the searched string.
 *                 The pointed string will be modified permanently 
 *                 after running this procedure.
 *           delimiters - e.g. "." or "., " or just " "
 *
 * return  : token stored in cpp memory
 *
 * side-eff: cpp will go forward to the 1st char of
 *           next token.
 *------------------------------------------------------------------*/
char *GetStrTok(char **cpp, char *delimiters)
{
    char  *token=*cpp,*old_cpp;
    int   i,j;

    if (!(**cpp) || !delimiters[0])
        return NULL;

    for(j=0; delimiters[j];){ /* skip delimiter(s) until meet the 1st valid char */
        for(old_cpp=*cpp; **cpp && (**cpp)==delimiters[j]; (*cpp)++);
        if (!**cpp)
            return NULL;
        if (old_cpp != *cpp) /* *cpp has skipped the delimiter(s), and points to a new position of string */
            j = 0;
        else
            j++;
    }

    token = *cpp;
    for(i=0;;i++,(*cpp)++) {
    	for(j=0; delimiters[j]; j++) {
            if ((**cpp) == delimiters[j] /* end of token */ || !(**cpp) /* end of string */ ||
				(**cpp) == 0x0a || (**cpp) == 0x0d) {
                /*for(;**cpp && (**cpp)==delimiters[j]; (*cpp)++);*/
                if (**cpp && (**cpp)!=0x0a && (**cpp)!=0x0d)  (*cpp)++;
                /* move *cpp, otherwise token[i]='\0' will cause **cpp='\0' */
    			token[i] = '\0';
    			return token;
    		}
    	}
    }
    return NULL; /* Abnormal case */
}

/*----------------------------------------------------
 * DECODE_LIT_ENDIAN_U16()
 *---------------------------------------------------*/
U16  DECODE_LIT_ENDIAN_U16(U8 *message)
{
        char  cc[2];

        cc[1] = *message;  cc[0] = *(message+1);
        return *(U16*)cc;
}

/*----------------------------------------------------
 * DECODE_LIT_ENDIAN_U24
 *
 * Note: first incoming byte(biggest) saved in high mem
 *       small value <-> in low memory
 *           large value <-> in high memory
 * -+--+--+       +-------------
 *  |  |a | ----> |  |  |  |a |
 * -+--+--+       +-------------
 *                  0  1  2  3
 *---------------------------------------------------*/
U32  DECODE_LIT_ENDIAN_U24(U8 *message)
{
        char  cc[3];

        cc[2] = *(message+1);
        cc[1] = *(message+2);
        cc[0] = *(message+3);
        return *(U32*)cc;
}

/*----------------------------------------------------
 * DECODE_LIT_ENDIAN_U32()
 *
 * Note: first incoming byte(biggest) saved in high mem
 *       small value <-> in low memory
 *           large value <-> in high memory
 * -+--+--+       +-------------
 *  |  |a | ----> |  |  |  |a |
 * -+--+--+       +-------------
 *                  0  1  2  3
 *---------------------------------------------------*/
U32  DECODE_LIT_ENDIAN_U32(U8 *message)
{
        char  cc[4];

        cc[3] = *message;               cc[2] = *(message+1);
        cc[1] = *(message+2);   cc[0] = *(message+3);
        return *(U32*)cc;
}

/*----------------------------------------------------
 * DECODE_U16 :
 *
 * input  : mp
 * output : val
 *---------------------------------------------------*/
U8  *DECODE_U16(U16 *val, U8 *mp)
{
#       ifdef _BIG_ENDIAN
        *val = *(U16*)mp;
#       else
        *val = DECODE_LIT_ENDIAN_U16(mp);
#       endif

        return (mp+2);
}

/*----------------------------------------------------
 * DECODE_U24 :
 *
 * input  : mp
 * output : val
 *---------------------------------------------------*/
U8  *DECODE_U24(U32 *val, U8 *mp)
{
#       ifdef _BIG_ENDIAN
        *val = *(U32*)mp;
#       else
        *val = DECODE_LIT_ENDIAN_U32(mp);
#       endif

        return (mp+3);
}

/*----------------------------------------------------
 * DECODE_U32()
 *
 * input  : mp
 * output : val
 *---------------------------------------------------*/
U8  *DECODE_U32(U32 *val, U8 *mp)
{
#       ifdef _BIG_ENDIAN
        *val = *(U32*)mp;
#       else
        *val = DECODE_LIT_ENDIAN_U32(mp);
#       endif

        return (mp+4);
}

/*----------------------------------------------------
 * ENCODE_LIT_ENDIAN_U16
 *---------------------------------------------------*/
void ENCODE_LIT_ENDIAN_U16(U16 val, U8 *mp)
{
        U8 *cp=(U8*)&val;

        *mp++ = *(cp+1); /* biggest value will be put in the outgoing beginning addr */
        *mp = *cp;
}

/*---------------------------------------------------------------------
 * ENCODE_LIT_ENDIAN_U24
 *
 * Note: first outgoing byte(biggest) retrieved from lowest
 *       i.e. sent from high memory which used to save biggest value.
 *---------------------------------------------------------------------*/
void ENCODE_LIT_ENDIAN_U24(U32 val, U8 *mp)
{
        U8 *cp=(U8*)&val;

        *mp++ = *(cp+2); /* biggest value will be put in the outgoing beginning addr */
        *mp++ = *(cp+1);
        *mp = *cp;
}

/*----------------------------------------------------------------------
 * ENCODE_LIT_ENDIAN_U32
 *
 * Note: first outgoing byte(biggest) retrieved from lowest
 *       i.e. sent from high memory which used to save biggest value.
 *---------------------------------------------------------------------*/
void ENCODE_LIT_ENDIAN_U32(U32 val, U8 *mp)
{
        U8 *cp=(U8*)&val;

        *mp++ = *(cp+3); /* biggest value will be put in the outgoing beginning addr */
        *mp++ = *(cp+2);
        *mp++ = *(cp+1);
        *mp = *cp;
}

/*----------------------------------------------------
 * ENCODE_U16
 *
 * input  : val
 * output : mp
 *---------------------------------------------------*/
U8 *ENCODE_U16(U8 *mp, U16 val)
{
#       ifdef _BIG_ENDIAN
        *(U16*)mp = val;
#       else
        ENCODE_LIT_ENDIAN_U16(val,mp);
#       endif

        return (mp+2);
}

/*----------------------------------------------------
 * ENCODE_U24
 *
 * input  : val
 * output : mp
 *---------------------------------------------------*/
U8 *ENCODE_U24(U8 *mp, U32 val)
{
#       ifdef _BIG_ENDIAN
        *(U32*)mp = val;
#       else
        ENCODE_LIT_ENDIAN_U24(val,mp);
#       endif

        return (mp+3);
}

/*----------------------------------------------------
 * ENCODE_U32
 *
 * input  : val
 * output : mp
 *---------------------------------------------------*/
U8 *ENCODE_U32(U8 *mp, U32 val)
{
#       ifdef _BIG_ENDIAN
        *(U32*)mp = val;
#       else
        ENCODE_LIT_ENDIAN_U32(val,mp);
#       endif

        return (mp+4);
}

/*-----------------------------------------------
 * ADD_CARRY_FOR_CHKSUM
 *
 *----------------------------------------------*/
U16 ADD_CARRY_FOR_CHKSUM(U32 sum)
{
    U32  value;
    U8   carry;

    carry = (U8)(sum >> 16); //get carry
    value = sum & 0x0000ffff;
    sum = carry + value;

    carry = (U8)(sum >> 16);
    sum += carry;

    return sum;
}

/*-----------------------------------------------
 * CHECK_SUM
 *
 *----------------------------------------------*/
U16 CHECK_SUM(U32 sum)
{
    U32  value;
    U8   carry;

    carry = (U8)(sum >> 16); //get carry
    value = sum & 0x0000ffff;
    sum = carry + value;

    carry = (U8)(sum >> 16);
    sum += carry;

    return (U16)~sum;
}

/*------------------------------------------------------------
 * get_local_mac:
 *-----------------------------------------------------------*/
STATUS get_local_mac(U8 *mac, char *sif)
{
    struct ifreq    sifreq; //socket Interface request
    int                     fd;
    
    fd = socket(PF_INET,SOCK_STREAM,0);
    if (fd < 0){
       printf("error! can not open socket for getting mac\n");
       return ERROR;
    }
    
    strncpy(sifreq.ifr_name, sif, IF_NAMESIZE-1);
    if (ioctl(fd, SIOCGIFHWADDR, &sifreq) != 0){
        printf("error! ioctl failed when getting mac\n");
        close(fd);
        return ERROR;
    }
    memmove((void*)&mac[0], (void*)&sifreq.ifr_ifru.ifru_hwaddr.sa_data[0], 6);
    close(fd);

    return SUCCESS;
}

/*------------------------------------------------------------
 * get_local_ip:
 *
 * ioctl(sockfd, SIOCGIFFLAGS, &ifr);
 * if (ifr.ifr_flags & IFF_UP) {
 *     printf("eth0 is up!");
 * }
 *-----------------------------------------------------------*/
STATUS get_local_ip(char *ip_str, char *sif)
{
    struct ifreq    ifr; //socket Interface request
    struct in_addr  sin_addr;
    int             fd;
    
    fd = socket(PF_INET, SOCK_STREAM, 0);
    if (fd < 0){
        printf("error! can not open socket for getting ip\n");
        return ERROR;
    }
    
    strncpy(ifr.ifr_name, sif, IF_NAMESIZE-1);    
    if (ioctl(fd, SIOCGIFADDR, &ifr) < 0) {
        perror("ioctl SIOCGIFADDR error");
        close(fd);
        return ERROR;
    }
    
    sin_addr.s_addr = *(U32*)&ifr.ifr_addr.sa_data[2];
    char *sin_addr_str = inet_ntoa(sin_addr);
    strncpy(ip_str, sin_addr_str, strlen(sin_addr_str));
    close(fd);

    return SUCCESS;
}

/*------------------------------------------------------------
 * set_local_ip:
 *
 * ioctl(sockfd, SIOCSIFFLAGS, &ifr);
 * if (ifr.ifr_flags & IFF_UP) {
 *     printf("eth0 is up!");
 * }
 *-----------------------------------------------------------*/
STATUS set_local_ip(char *ip_str, char *sif) 
{ 
    struct ifreq ifr; //socket Interface request 
    struct sockaddr_in addr,mask; 
    int fd;
    STATUS ret = SUCCESS;
 
    fd = socket(AF_INET,SOCK_STREAM,0); 
    if (fd < 0){ 
        printf("error! can not open socket for getting ip\n"); 
        return ERROR; 
    } 
 
    bzero(&ifr,sizeof(ifr));  
    bzero(&addr,sizeof(addr));  
    bzero(&mask,sizeof(mask));  
 
    //sprintf(ifr.ifr_name, sif); 
    strncpy(ifr.ifr_name, sif, IF_NAMESIZE-1);  
    addr.sin_family = AF_INET; 
    addr.sin_addr.s_addr = inet_addr(ip_str); 
    //printf("addr.sin_addr.s_addr=%x\n",addr.sin_addr.s_addr); 
    memcpy(&ifr.ifr_addr, &addr, sizeof(struct sockaddr)); 
    if (ioctl(fd, SIOCSIFADDR, &ifr) < 0) { 
        perror("ioctl SIOCSIFADDR error"); 
        ret = ERROR;
        goto end;
    } 
 
    mask.sin_family = AF_INET; 
    mask.sin_addr.s_addr = inet_addr("255.255.0.0"); 
    memcpy(&ifr.ifr_addr, &mask, sizeof(struct sockaddr)); 
    if (ioctl(fd, SIOCSIFNETMASK, &ifr) < 0) { 
        perror("ioctl SIOCSIFNETMASK error"); 
        ret = ERROR;
        goto end; 
    } 
 
    ifr.ifr_flags |= IFF_UP |IFF_RUNNING;  
 
    //get the status of the device  
    if (ioctl(fd, SIOCSIFFLAGS, &ifr) < 0) { 
        perror("SIOCSIFFLAGS");
        ret = ERROR;
        goto end;
    }  
    if (ifr.ifr_flags & IFF_UP) { 
        printf("%s is up!\n",sif); 
    } 

end:
    close(fd);

    return ret; 
}
    
/********************************************************************
 * PRINT_MESSAGE()
 * Note: Print msg as wireshark-like format.
 ********************************************************************/
void PRINT_MESSAGE(unsigned char *msg, int len)
{
	int  row_cnt,rows,rest_bytes,hex_cnt,ch_cnt,cnt,xi,ci;
	
	if (NULL == msg){
		printf("PRINT_MESSAGE(): NULL message ?\n");
		return;
	}
	
	/*if ((len*5) > 2048){ // 5 format bytes for one raw data byte
		printf("Too large[len(%d) > max(%d)] to print out!\n",len,2048);
		return;
	}*/
	
	rest_bytes = len % 16;
	rows = len / 16;
	ci = xi = 0;
	
	for(row_cnt=0; row_cnt<rows; row_cnt++){
		/*------------- print label for each row --------------*/
		printf("%04x:  ",(row_cnt+1)<<4);
		
		/*------------- print hex-part --------------*/
		for(hex_cnt=1; hex_cnt<=8; hex_cnt++){
		    if (hex_cnt < 8)
		        printf("%02x ",msg[xi++]); /* Must be unsigned, otherwise garbage displayed */
		    else
		        printf("%02x",msg[xi++]); /* Must be unsigned, otherwise garbage displayed */
		}
		
		/* delimiters space for each 8's Hex char */
		printf("  ");
		
		for(hex_cnt=9; hex_cnt<=16; hex_cnt++){
		    if (hex_cnt < 16)
		        printf("%02x ",msg[xi++]);
		    else
		        printf("%02x",msg[xi++]);
		}
		
		/* delimiters space bet. Hex and Character row */
		printf("    ");
		
		/*------------- print character-part --------------*/
		for(ch_cnt=1; ch_cnt<=16; ch_cnt++,ci++){
			if (msg[ci]>0x20 && msg[ci]<=0x7e){
		    	printf("%c",msg[ci]);
		    }
		    else{
		    	printf(".");
			}
		}
		printf("\n");
	} //for
	
	/*================ print the rest bytes(hex & char) ==================*/
	if (rest_bytes == 0) {
		printf("\n");
		return;
	}
	
	/*------------- print label for last row --------------*/
	printf("%04x:  ",(row_cnt+1)<<4);
		
	/*------------- print hex-part(rest) --------------*/
	if (rest_bytes < 8){
	    for(hex_cnt=1; hex_cnt<=rest_bytes; hex_cnt++){
	        printf("%02x ",msg[xi++]);
	    }
	
	    /* fill in the space for 16's Hex-part alignment */
	    for(cnt=rest_bytes+1; cnt<=8; cnt++){ /* from rest_bytes+1 to 8 */
	        if (cnt < 8)
	            printf("   ");
	        else
	            printf("  ");
	    }
	
	    /* delimiters bet. hex and char */
	    printf("  ");
	
	    for(cnt=9; cnt<=16; cnt++){
	        if (cnt < 16)
	            printf("   ");
	        else
	            printf("  ");
	    }
	    printf("    ");
	}
	else if (rest_bytes == 8){
	    for(hex_cnt=1; hex_cnt<=rest_bytes; hex_cnt++){
	        if (hex_cnt < 8)
	            printf("%02x ",msg[xi++]);
	        else
	            printf("%02x",msg[xi++]);
	    }
	    printf("  ");
	
	    for(cnt=9; cnt<=16; cnt++){
	        if (cnt < 16)
	            printf("   ");
	        else
	            printf("  ");
	    }
	    printf("    ");
	}
	else{ /* rest_bytes > 8 */
	    for(hex_cnt=1; hex_cnt<=8; hex_cnt++){
	        if (hex_cnt < 8)
	            printf("%02x ",msg[xi++]);
	        else
	            printf("%02x",msg[xi++]);
	    }
	
	    /* delimiters space for each 8's Hex char */
	    printf("  ");
	
	    for(hex_cnt=9; hex_cnt<=rest_bytes; hex_cnt++){ /* 9 - rest_bytes */
	        if (hex_cnt < 16)
	            printf("%02x ",msg[xi++]);
	        else
	            printf("%02x",msg[xi++]);
	    }
	
	    for(cnt=rest_bytes+1; cnt<=16; cnt++){
	        if (cnt < 16)
	            printf("   ");
	        else
	            printf("  ");
	    }
	    /* delimiters space bet. Hex and Character row */
	    printf("    ");
	} /* else */
	
	/*------------- print character-part --------------*/
	for(ch_cnt=1; ch_cnt<=rest_bytes; ch_cnt++,ci++){
		if (msg[ci]>0x20 && msg[ci]<=0x7e){
	        printf("%c",msg[ci]);
	    }
	    else
	        printf(".");
	}
	printf("\n");
}

/*----------------------------------------------------
 * OSS_PRINT()
 *---------------------------------------------------*/
void OSS_PRINT(const char *fmt, ...)
{
	va_list  ap;
	int		 len;
	char     buf[255],tmp[1024];

	va_start(ap,fmt);
	vsprintf(buf,fmt,ap);
	
	len = strlen(buf);
    if (buf[len-1] == '\n'){
        buf[len] = '\r';
        buf [len+1] = '\0';	   
    }
       
	sprintf(tmp,"%s",buf);
	write(1,tmp,len);
	va_end(ap);	
}
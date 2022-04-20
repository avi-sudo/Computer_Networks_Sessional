#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>

/* ******************************************************************
 ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: SLIGHTLY MODIFIED
 FROM VERSION 1.1 of J.F.Kurose

   This code should be used for PA2, unidirectional or bidirectional
   data transfer protocols (from A to B. Bidirectional transfer of data
   is for extra credit and is not required).  Network properties:
   - one way network delay averages five time units (longer if there
       are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
       or lost, according to user-defined probabilities
   - packets will be delivered in the order in which they were sent
       (although some can be lost).
**********************************************************************/

#define BIDIRECTIONAL 1 /* change to 1 if you're doing extra credit */
/* and write a routine called B_output */

/* a "msg" is the data unit passed from layer 5 (teachers code) to layer  */
/* 4 (students' code).  It contains the data (characters) to be delivered */
/* to layer 5 via the students transport level protocol entities.
        */

FILE *fptr;

struct pkt
{
    char data[4];
};

/* a packet is the data unit passed from layer 4 (students code) to layer */
/* 3 (teachers code).  Note the pre-defined packet structure, which all   */
/* students must follow. */
struct frm
{
    int seqnum;
    int acknum;
    char checksum[32];
    char payload[4];
    int type;//type 0=data,1=ack,2=data-ack
};

int AsequenceNumber;
int BsequenceNumber;
bool AaCK,BaCK;
struct frm ArecentFrame;
struct frm BrecentFrame;
int BacknowledgeNumber;
int AacknowledgeNumber;
int counter=0,Bcounter=0;
int BOutstandingACK=0,AOutstandingACK=0;
char in[100];
int steps;
char* convert(int d)
{
    int b=0,i=1,r=0;
    char arr[100]="",ar2[100];
    if(d==0){
        itoa(0,ar2,10);
        strcat(arr,ar2);
        return arr;
    }
    while(d>0)
    {
        r=d%2;
        itoa(r,ar2,10);
        strcat(arr,ar2);
        d=d/2;

    }
    strrev(arr);
    return arr;
}
char* input(struct frm frame)
{
    char bn[100]="";
    int s=frame.seqnum,ac=frame.acknum,type=frame.type,i;

    for(i=0;i<strlen(frame.payload);i++){
        char* bt=convert(toascii(frame.payload[i]));
        strcat(bn,bt);
    }
    strcat(bn,convert(s));
    strcat(bn,convert(ac));
    strcat(bn,convert(type));
    return bn;
}

char* CRC(char input[100], char polynomial2[100])
{
    char polynomial[100];
    strcpy(polynomial,polynomial2);
    if(steps==1){
        printf("Input Bit String: %s\n",input);
        printf("Generator Polynomial: %s\n",polynomial2);
        fprintf(fptr,"Input Bit String: %s\n",input);
        fprintf(fptr,"Generator Polynomial: %s\n",polynomial2);
	}

    int i,j,k,l,polynomialLength,msgLength;
    polynomialLength=strlen(polynomial);
	msgLength=strlen(input);
	char aw[polynomialLength],remainder[100],polynomial1[100],temporary[100],quotient[100];


	strcpy(polynomial1,polynomial);
	for (i=0;i<polynomialLength-1;i++) {
		input[msgLength+i]='0';
	}
	for (i=0;i<polynomialLength;i++){
	 temporary[i]=input[i];
	}

    //printf("%d:%d\n",msgLength,polynomialLength);
    for(k=1;k<=polynomialLength;k++){
        l=l-1;
        //printf("%c",polynomial[l]);
    }
	for (i=0;i<msgLength;i++) {
		quotient[i]=temporary[0];

		if(quotient[i]=='1'){
		 for (j=0;j<polynomialLength;j++){
            polynomial[j]=polynomial1[j];
		 }

        }
		 else{
		 for (j=0;j<polynomialLength;j++){
		   polynomial[j]='0';
         }
        }
       // for(k=0;k<polynomialLength;k++){
            //printf("%d\n",k);
        //}
        for (k=0;k<msgLength;k++){
            //printf("%c",quotient[k]);
            //printf("\n");
        }
		for (j=polynomialLength-1;j>0;j--) {
			if(temporary[j]!=polynomial[j]){
			   remainder[j-1]='1';
			}
			 else
                remainder[j-1]='0';
		}

		remainder[polynomialLength-1]=input[i+polynomialLength];
		strcpy(temporary,remainder);

	}
	strcpy(remainder,temporary);

	for (i=0;i<polynomialLength-1;i++){
	 aw[i]=remainder[i];
	}
	aw[polynomialLength-1]=0;
    if(steps==1){
        printf("Calculated CRC: %s\n",aw);
        fprintf(fptr,"Calculated CRC: %s\n",aw);
	}
    //printf("\n");
	 return aw;
}

char* divide(char input[100],char polynomial2[100])
{
    char polynomial[100];
    strcpy(polynomial,polynomial2);
    if(steps==1){
        printf("Input Bit String: %s\n",input);
        printf("Generator Polynomial: %s\n",polynomial2);
        fprintf(fptr,"Input Bit String: %s\n",input);
        fprintf(fptr,"Generator Polynomial: %s\n",polynomial2);
	}
    int i,j,polynomialLength,msgLength,k,l;
    polynomialLength=strlen(polynomial);
	msgLength=strlen(input);
	char aw[polynomialLength],remainder[100],polynomial1[100],temporary[100],quotient[100];

	//printf("%d:%d\n",msgLength,polynomialLength);
	strcpy(polynomial1,polynomial);

    for (i=0;i<polynomialLength;i++){
	  temporary[i]=input[i];
    }

    //printf("%d:%d\n",msgLength,polynomialLength);
    for(k=1;k<=polynomialLength;k++){
        l=l-1;
        //printf("%c",polynomial[l]);
    }
	for (i=0;i<=(msgLength-polynomialLength);i++) {
		quotient[i]=temporary[0];
		if(quotient[i]=='1'){
		 for (j=0;j<polynomialLength;j++){
            polynomial[j]=polynomial1[j];
		 }

        }else{
		 for (j=0;j<polynomialLength;j++){
            polynomial[j]='0';
		 }
        }

        // for(k=0;k<polynomialLength;k++){
            //printf("%s\n",quotient);
        //}

        for (k=0;k<msgLength;k++){
            //printf("%c",quotient[k]);
            //printf("\n");
        }
        for (l=0;l<polynomialLength-1;l++){
            //printf("%c",remainder[l]);
            //printf("\n");
        }
		for (j=polynomialLength-1;j>0;j--) {
			if(temporary[j]!=polynomial[j]){
			 remainder[j-1]='1';
			}
            else
			 remainder[j-1]='0';
		}
		remainder[polynomialLength-1]=input[i+polynomialLength];
		strcpy(temporary,remainder);
	}
	strcpy(remainder,temporary);
	for (i=0;i<polynomialLength-1;i++){
        aw[i]=remainder[i];
	}
	aw[polynomialLength-1]=0;

	return aw;
}

bool check(char temp[100],char key[100])
{
    if(steps==1){
        printf("Error Detection Mechanism is started\n");
        fprintf(fptr,"Error Detection Mechanism is started\n");
    }
    char* b=divide(temp,key);
    int v=atoi(b);
    if(v!=0){
        if(steps==1){
            printf("Error is Detected\n");
            fprintf(fptr,"Error is Detected\n");
        }
        return false;
    }else {
        if(steps==1){
            printf("No error is occured\n");
            fprintf(fptr,"No error is occured\n");
        }
        return true;
    }
}


/********* FUNCTION PROTOTYPES. DEFINED IN THE LATER PART******************/
void starttimer(int AorB, float increment);
void stoptimer(int AorB);
void tolayer1(int AorB, struct frm frame);
void tolayer3(int AorB, char datasent[4]);

/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

/* called from layer 5, passed the data to be sent to other side */
void A_output(struct pkt packet)
{
    if(counter!=0 && AaCK==false){
        printf("In A_output: Sender A is not acknowledged yet........drop this %s data\n",packet.data);
        fprintf(fptr, "In A_output: Sender A is not acknowledged yet........drop this %s data\n\n",packet.data);
        return;
    }
    if(AOutstandingACK==1){
        printf("In A_output: Sender A sends the piggy Frame %s data\n",packet.data);
    fprintf(fptr, "In A_output: Sender A sends the piggy Frame %s data\n\n",packet.data);
    struct frm Frame;
    Frame.type=2;
    Frame.acknum=1-AacknowledgeNumber;
    Frame.seqnum=AsequenceNumber;
    strcpy(Frame.payload,packet.data);

    char arr[100];
    strcpy(arr,input(Frame));
    char* arr1=CRC(arr,in);
    strcpy(Frame.checksum,arr1);
    //printf("%s\n",Frame.checksum);

    ArecentFrame=Frame;
    AaCK=false;
    AOutstandingACK=0;
    tolayer1(0,Frame);
    counter++;
    starttimer(0,100.0);
    }
    else if(AOutstandingACK==0){
    printf("In A_output: Sender A sends the Frame %s data\n",packet.data);
    fprintf(fptr, "In A_output: Sender A sends the Frame %s data\n\n",packet.data);
    struct frm Frame;
    Frame.type=0;
    Frame.acknum=1;
    Frame.seqnum=AsequenceNumber;
    strcpy(Frame.payload,packet.data);

    char arr[100];
    strcpy(arr,input(Frame));
    char* arr1=CRC(arr,in);
    strcpy(Frame.checksum,arr1);
    //printf("%s\n",Frame.checksum);

    ArecentFrame=Frame;
    AaCK=false;
    tolayer1(0,Frame);
    counter++;
    starttimer(0,100.0);
    }
}

/* need be completed only for extra credit */
void B_output(struct pkt packet)
{
    if(Bcounter!=0 && BaCK==false){
        printf("In B_output: Sender B is not acknowledged yet........drop this %s data\n",packet.data);
        fprintf(fptr, "In B_output: Sender B is not acknowledged yet........drop this %s data\n\n",packet.data);
        return;
    }
    if(BOutstandingACK==1){
        printf("In B_output: Sender B sends the piggy Frame %s data\n",packet.data);
    fprintf(fptr, "In B_output: Sender B sends the piggy Frame %s data\n\n",packet.data);
    struct frm Frame;
    Frame.type=2;
    Frame.acknum=1-BacknowledgeNumber;
    Frame.seqnum=BsequenceNumber;
    strcpy(Frame.payload,packet.data);

    char arr[100];
    strcpy(arr,input(Frame));
    char* arr1=CRC(arr,in);
    strcpy(Frame.checksum,arr1);
    //printf("%s\n",Frame.checksum);

    BrecentFrame=Frame;
    BaCK=false;
    BOutstandingACK=0;
    tolayer1(1,Frame);
    Bcounter++;
    starttimer(1,100.0);
    }
    else if(BOutstandingACK==0){
    printf("In B_output: Sender B sends the Frame %s data\n",packet.data);
    fprintf(fptr, "In B_output: Sender B sends the Frame %s data\n\n",packet.data);
    struct frm Frame;
    Frame.type=0;
    Frame.acknum=1;
    Frame.seqnum=BsequenceNumber;
    strcpy(Frame.payload,packet.data);

    char arr[100];
    strcpy(arr,input(Frame));
    char* arr1=CRC(arr,in);
    strcpy(Frame.checksum,arr1);
    //printf("%s\n",Frame.checksum);

    BrecentFrame=Frame;
    BaCK=false;
    tolayer1(1,Frame);
    Bcounter++;
    starttimer(1,100.0);
    }

}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(struct frm frame)
{
    if(frame.type==1){

    char arr2[100];
    strcpy(arr2,input(frame));
    strcat(arr2,frame.checksum);
    //printf("%s\n",arr2);
    if(!check(arr2,in)){
        printf("In A_input: Acknowledged frame got Corrupted......drop\n");
        fprintf(fptr, "In A_input: Acknowledged frame got Corrupted.......drop\n\n");
        return;
    }
    if(frame.acknum!=AsequenceNumber){
        printf("In A_input: Ignoring Acknowledgement..........drop\n");
        fprintf(fptr, "In A_input: Ignoring Acknowledgement.......drop\n\n");
        return;
    }

    printf("In A_input: Sender A receives valid acknowledged frame\n");
    fprintf(fptr, "In A_input: Sender A receives valid acknowledged frame\n\n");
    stoptimer(0);
    AaCK=true;
    AsequenceNumber=1-AsequenceNumber;
    }
    else if(frame.type==0){
        char arr2[100];
        strcpy(arr2,input(frame));
        strcat(arr2,frame.checksum);
        //printf("%s\n",arr2);
        if(!check(arr2,in)){
        printf("In A_input: Receiver A receives Corrupted frame %s data.......Drop\n",frame.payload);
        fprintf(fptr, "In A_input: Receiver A receives Corrupted frame %s data.......Drop\n\n",frame.payload);

        struct frm ACKframe;
        ACKframe.type=1;
        ACKframe.seqnum=1;
        ACKframe.acknum=1-AacknowledgeNumber;
        //strcpy(ACKframe.payload,"\0\0\0\0");
        int i;
        for(i=0;i<4;i++){
            ACKframe.payload[i]=0;
        }

        char arr[100];
        strcpy(arr,input(ACKframe));
        printf("In A_input: Receiver A sends negative acknowledgement(NACK)\n");
        fprintf(fptr, "In A_input: Receiver A sends negative acknowledgement(NACK)\n\n");

        char* arr1=CRC(arr,in);
        strcpy(ACKframe.checksum,arr1);
        //printf("%s\n",ACKpacket.checksum);


        AOutstandingACK=0;
        tolayer1(0,ACKframe);

        return;
    }
    if(frame.seqnum!=AacknowledgeNumber){
        printf("In A_input: Receiver A discards duplicate frame for %s data......Drop\n",frame.payload);
        fprintf(fptr, "In A_input: Receiver A discards duplicate frame for %s data......Drop\n\n",frame.payload);
        struct frm ACKframe;
        ACKframe.type=1;
        ACKframe.seqnum=1;
        ACKframe.acknum=1-AacknowledgeNumber;
        //strcpy(ACKframe.payload,"\0\0\0\0");
        int i;
        for(i=0;i<4;i++){
            ACKframe.payload[i]=0;
        }

        char arr[100];
        strcpy(arr,input(ACKframe));
        printf("In A_input: Receiver A sends negative acknowledgement(NACK)\n");
        fprintf(fptr, "In A_input: Receiver A sends negative acknowledgement(NACK)\n\n");

        char* arr1=CRC(arr,in);
        strcpy(ACKframe.checksum,arr1);
        //printf("%s\n",ACKpacket.checksum);

        AOutstandingACK=0;
        tolayer1(0,ACKframe);
        return;
    }


    printf("In A_input: Receiver A receives valid frame %s data\n",frame.payload);
    fprintf(fptr, "In A_input: Receiver A receives valid frame %s data\n\n",frame.payload);

    AOutstandingACK=1;

    printf("In A_input: Receiver A waits for Sending the acknowledged frame for %s data\n",frame.payload);
    fprintf(fptr, "In A_input: Receiver A waits for Sending the acknowledged frame for %s data\n\n",frame.payload);
    //tolayer3(0,ACKpacket);
    tolayer3(0,frame.payload);
    AacknowledgeNumber=1-AacknowledgeNumber;
    }
    else if(frame.type==2){
        //AOutstandingACK=1;
        char arr2[100];
        strcpy(arr2,input(frame));
        strcat(arr2,frame.checksum);
        if(!check(arr2,in)){
        printf("In A_input: Receiver A receives Corrupted piggy frame %s data.......Drop\n",frame.payload);
        fprintf(fptr, "In A_input: Receiver A receives Corrupted piggy frame %s data.......Drop\n\n",frame.payload);

        struct frm ACKframe;
        ACKframe.type=1;
        ACKframe.seqnum=1;
        ACKframe.acknum=1-AacknowledgeNumber;
       // strcpy(ACKframe.payload,"\0\0\0\0");
       int i;
        for(i=0;i<4;i++){
            ACKframe.payload[i]=0;
        }

        char arr[100];
        strcpy(arr,input(ACKframe));
        printf("In A_input: Receiver A sends negative acknowledgement(NACK)\n");
        fprintf(fptr, "In A_input: Receiver A sends negative acknowledgement(NACK)\n\n");

        char* arr1=CRC(arr,in);
        strcpy(ACKframe.checksum,arr1);
        //printf("%s\n",ACKpacket.checksum);

        AOutstandingACK=0;
        tolayer1(0,ACKframe);

        return;
    }

    if(frame.seqnum!=AacknowledgeNumber){
        printf("In A_input: Receiver A discards duplicate piggy frame for %s data......Drop\n",frame.payload);
        fprintf(fptr, "In A_input: Receiver A discards duplicate piggy frame for %s data......Drop\n\n",frame.payload);
        struct frm ACKframe;
        ACKframe.type=1;
        ACKframe.seqnum=1;
        ACKframe.acknum=1-AacknowledgeNumber;
        //strcpy(ACKframe.payload,"\0\0\0\0");
        int i;
        for(i=0;i<4;i++){
            ACKframe.payload[i]=0;
        }

        char arr[100];
        strcpy(arr,input(ACKframe));
        printf("In A_input: Receiver A sends negative acknowledgement(NACK)\n");
        fprintf(fptr, "In A_input: Receiver A sends negative acknowledgement(NACK)\n\n");
        char* arr1=CRC(arr,in);
        strcpy(ACKframe.checksum,arr1);
        //printf("%s\n",ACKpacket.checksum);

        AOutstandingACK=0;
        tolayer1(0,ACKframe);
        return;
    }
    printf("In A_input: Receiver A receives valid piggy frame %s data\n",frame.payload);
    fprintf(fptr, "In A_input: Receiver A receives valid piggy frame %s data\n\n",frame.payload);

    AOutstandingACK=1;

    printf("In A_input: Receiver A waits for Sending the acknowledged frame for %s data\n",frame.payload);
    fprintf(fptr, "In A_input: Receiver A waits for Sending the acknowledged frame for %s data\n\n",frame.payload);
    //tolayer3(0,ACKpacket);
    tolayer3(0,frame.payload);
    AacknowledgeNumber=1-AacknowledgeNumber;

    if(frame.acknum!=AsequenceNumber){
        printf("In A_input: Ignoring piggy Acknowledgement..........drop\n");
        fprintf(fptr, "In A_input: Ignoring piggy Acknowledgement.......drop\n\n");
        return;
    }


    printf("In A_input: Receiver A receives acknowledge for previous frame\n");
    fprintf(fptr,"In A_input: Receiver A receives acknowledge for previous frame\n");


    stoptimer(0);
    AaCK=true;
    AsequenceNumber=1-AsequenceNumber;
    }


}

/* called when A's timer goes off */
void A_timerinterrupt(void)
{

    if(AaCK==true){
        printf("In A_timerinterrupt: Sender A is already acknowledged....so ignoring this event\n");
        fprintf(fptr, "In A_timerinterrupt: Sender A is already acknowledged....so ignoring this event\n\n");
        return;
    }
    printf("In A_timerinterrupt: Sender A retransmits the last frame %s data\n",ArecentFrame.payload);
    fprintf(fptr, "In A_timerinterrupt: Sender A retransmits the last frame %s data\n\n",ArecentFrame.payload);
    tolayer1(0,ArecentFrame);
    starttimer(0,20.0);
}

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init(void)
{
    AaCK=false;
    AsequenceNumber=0;
    AacknowledgeNumber=0;
}

/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(struct frm frame)
{
    if(frame.type==0){
            //BOutstandingACK=1;
        char arr2[100];
        strcpy(arr2,input(frame));
        strcat(arr2,frame.checksum);
        //printf("%s\n",arr2);
        if(!check(arr2,in)){
        printf("In B_input: Receiver B receives Corrupted frame %s data.......Drop\n",frame.payload);
        fprintf(fptr, "In B_input: Receiver B receives Corrupted frame %s data.......Drop\n\n",frame.payload);

        struct frm ACKframe;
        ACKframe.type=1;
        ACKframe.seqnum=1;
        ACKframe.acknum=1-BacknowledgeNumber;
        //strcpy(ACKframe.payload,"\0\0\0\0");
        int i;
        for(i=0;i<4;i++){
            ACKframe.payload[i]=0;
        }

        char arr[100];
        strcpy(arr,input(ACKframe));

        printf("In B_input: Receiver B sends negative acknowledgement(NACK)\n");
        fprintf(fptr, "In B_input: Receiver B sends negative acknowledgement(NACK)\n\n");

        char* arr1=CRC(arr,in);
        strcpy(ACKframe.checksum,arr1);

        BOutstandingACK=0;
        tolayer1(1,ACKframe);

        return;
    }
    if(frame.seqnum!=BacknowledgeNumber){
        printf("In B_input: Receiver B discards duplicate frame for %s data......Drop\n",frame.payload);
        fprintf(fptr, "In B_input: Receiver B discards duplicate frame for %s data......Drop\n\n",frame.payload);
        struct frm ACKframe;
        ACKframe.type=1;
        ACKframe.seqnum=1;
        ACKframe.acknum=1-BacknowledgeNumber;
        //strcpy(ACKframe.payload,"\0\0\0\0");
        int i;
        for(i=0;i<4;i++){
            ACKframe.payload[i]=0;
        }

        char arr[100];
        strcpy(arr,input(ACKframe));
        printf("In B_input: Receiver B sends negative acknowledgement(NACK)\n");
        fprintf(fptr, "In B_input: Receiver B sends negative acknowledgement(NACK)\n\n");
        char* arr1=CRC(arr,in);
        strcpy(ACKframe.checksum,arr1);

        BOutstandingACK=0;
        tolayer1(1,ACKframe);
        return;
    }
    printf("In B_input: Receiver B receives valid frame %s data\n",frame.payload);
    fprintf(fptr, "In B_input: Receiver B receives valid frame %s data\n\n",frame.payload);

    BOutstandingACK=1;
    printf("In B_input: Receiver B waits for Sending the acknowledged frame for %s data\n",frame.payload);
    fprintf(fptr, "In B_input: Receiver B waits for Sending the acknowledged frame for %s data\n\n",frame.payload);

    //tolayer3(1,ACKpacket);

    tolayer3(1,frame.payload);
    BacknowledgeNumber=1-BacknowledgeNumber;

    }
    else if(frame.type==1){
        char arr2[100];
        strcpy(arr2,input(frame));
        strcat(arr2,frame.checksum);
        //printf("%s\n",arr2);
        if(!check(arr2,in)){
        printf("In B_input: Acknowledged frame got Corrupted......drop\n");
        fprintf(fptr, "In B_input: Acknowledged frame got Corrupted.......drop\n\n");
        return;
    }
    if(frame.acknum!=BsequenceNumber){
        printf("In B_input: Ignoring Acknowledgement..........drop\n");
        fprintf(fptr, "In B_input: Ignoring Acknowledgement.......drop\n\n");
        return;
    }
    //printf("%d-----%d\n",packet.acknum,AsequenceNumber);
    printf("In B_input: Sender B receives valid acknowledged frame\n");
    fprintf(fptr, "In B_input: Sender B receives valid acknowledged frame\n\n");
    stoptimer(1);
    BaCK=true;
    BsequenceNumber=1-BsequenceNumber;
    }
    else if(frame.type==2){
        //BOutstandingACK=1;
        char arr2[100];
        strcpy(arr2,input(frame));
        strcat(arr2,frame.checksum);
        //printf("%s\n",arr2);
        if(!check(arr2,in)){
        printf("In B_input: Receiver B receives Corrupted frame %s data.......Drop\n",frame.payload);
        fprintf(fptr, "In B_input: Receiver B receives Corrupted frame %s data.......Drop\n\n",frame.payload);

        struct frm ACKframe;
        ACKframe.type=1;
        ACKframe.seqnum=1;
        ACKframe.acknum=1-BacknowledgeNumber;
        //strcpy(ACKframe.payload,"\0\0\0\0");
        int i;
        for(i=0;i<4;i++){
            ACKframe.payload[i]=0;
        }

        char arr[100];
        strcpy(arr,input(ACKframe));
        printf("In B_input: Receiver B sends negative acknowledgement(NACK)\n");
        fprintf(fptr, "In B_input: Receiver B sends negative acknowledgement(NACK)\n\n");
        char* arr1=CRC(arr,in);
        strcpy(ACKframe.checksum,arr1);

        BOutstandingACK=0;
        tolayer1(1,ACKframe);

        return;
    }

    if(frame.seqnum!=BacknowledgeNumber){
        printf("In B_input: Receiver B discards duplicate piggy frame for %s data......Drop\n",frame.payload);
        fprintf(fptr, "In B_input: Receiver B discards duplicate piggy frame for %s data......Drop\n\n",frame.payload);
        struct frm ACKframe;
        ACKframe.type=1;
        ACKframe.seqnum=1;
        ACKframe.acknum=1-BacknowledgeNumber;
        //strcpy(ACKframe.payload,"\0\0\0\0");
        int i;
        for(i=0;i<4;i++){
            ACKframe.payload[i]=0;
        }

        char arr[100];
        strcpy(arr,input(ACKframe));
        printf("In B_input: Receiver B sends negative acknowledgement(NACK)\n");
        fprintf(fptr, "In B_input: Receiver B sends negative acknowledgement(NACK)\n\n");
        char* arr1=CRC(arr,in);
        strcpy(ACKframe.checksum,arr1);

        BOutstandingACK=0;
        tolayer1(1,ACKframe);
        return;
    }

    printf("In B_input: Receiver B receives valid piggy frame %s data\n",frame.payload);
    fprintf(fptr, "In B_input: Receiver B receives valid piggy frame %s data\n\n",frame.payload);

    BOutstandingACK=1;

    printf("In B_input: Receiver B waits for Sending the acknowledged frame for %s data\n",frame.payload);
    fprintf(fptr, "In B_input: Receiver B waits for Sending the acknowledged frame for %s data\n\n",frame.payload);

    //tolayer3(1,ACKpacket);

    tolayer3(1,frame.payload);
    BacknowledgeNumber=1-BacknowledgeNumber;

    if(frame.acknum!=BsequenceNumber){
        printf("In B_input: Ignoring piggy Acknowledgement..........drop\n");
        fprintf(fptr, "In B_input: Ignoring piggy Acknowledgement.......drop\n\n");
        return;
    }


    printf("In B_input: Receiver B receives acknowledge for previous frame\n");
    fprintf(fptr,"In B_input: Receiver B receives acknowledge for previous frame\n");


    stoptimer(1);
    BaCK=true;
    BsequenceNumber=1-BsequenceNumber;
    }
}

/* called when B's timer goes off */
void B_timerinterrupt(void)
{
    //printf("  B_timerinterrupt: B doesn't have a timer. ignore.\n");

    if(BaCK==true){
        printf("In B_timerinterrupt: Sender B is already acknowledged....so ignoring this event\n");
        fprintf(fptr, "In B_timerinterrupt: Sender B is already acknowledged....so ignoring this event\n\n");
        return;
    }
    printf("In B_timerinterrupt: Sender B retransmits the last frame %s data\n",BrecentFrame.payload);
    fprintf(fptr, "In B_timerinterrupt: Sender B retransmits the last frame %s data\n\n",BrecentFrame.payload);
    tolayer1(1,BrecentFrame);
    starttimer(1,20.0);
}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init(void)
{
    BaCK=false;
    BacknowledgeNumber=0;
    BsequenceNumber=0;
}
//fclose(fptr);

/*****************************************************************
***************** NETWORK EMULATION CODE STARTS BELOW ***********
The code below emulates the layer 3 and below network environment:
    - emulates the tranmission and delivery (possibly with bit-level corruption
        and packet loss) of packets across the layer 3/4 interface
    - handles the starting/stopping of a timer, and generates timer
        interrupts (resulting in calling students timer handler).
    - generates message to be sent (passed from later 5 to 4)

THERE IS NOT REASON THAT ANY STUDENT SHOULD HAVE TO READ OR UNDERSTAND
THE CODE BELOW.  YOU SHOLD NOT TOUCH, OR REFERENCE (in your code) ANY
OF THE DATA STRUCTURES BELOW.  If you're interested in how I designed
the emulator, you're welcome to look at the code - but again, you should have
to, and you defeinitely should not have to modify
******************************************************************/

struct event
{
    float evtime;       /* event time */
    int evtype;         /* event type code */
    int eventity;       /* entity where event occurs */
    struct frm *frmptr; /* ptr to packet (if any) assoc w/ this event */
    struct event *prev;
    struct event *next;
};
struct event *evlist = NULL; /* the event list */

/* possible events: */
#define TIMER_INTERRUPT 0
#define FROM_LAYER3 1
#define FROM_LAYER1 2

#define OFF 0
#define ON 1
#define A 0
#define B 1

int TRACE = 1;     /* for my debugging */
int nsim = 0;      /* number of messages from 5 to 4 so far */
int nsimmax = 0;   /* number of msgs to generate, then stop */
float time = 0.000;
float lossprob;    /* probability that a packet is dropped  */
float corruptprob; /* probability that one bit is packet is flipped */
float lambda;      /* arrival rate of messages from layer 5 */
int ntolayer1;     /* number sent into layer 3 */
int nlost;         /* number lost in media */
int ncorrupt;      /* number corrupted by media*/

void init();
void generate_next_arrival(void);
void insertevent(struct event *p);

int main()
{


    //FILE *fptr;

    // opening file in writing mode
    fptr=fopen("1605006.doc","w");
    //fptr=fopen("a.txt","w");

    // exiting program
    if(fptr == NULL) {
        printf("Error!");
        exit(1);
    }
    struct event *eventptr;
    struct frm frm2give;
    struct pkt pkt2give;

    int i, j;
    char c;

    init();
    A_init();
    B_init();

    while (1)
    {
        eventptr = evlist; /* get next event to simulate */
        if (eventptr == NULL)
            goto terminate;
        evlist = evlist->next; /* remove this event from event list */
        if (evlist != NULL)
            evlist->prev = NULL;
        if (TRACE >= 2)
        {
            printf("\nEVENT time: %f,", eventptr->evtime);
            printf("  type: %d", eventptr->evtype);
            if (eventptr->evtype == 0)
                printf(", timerinterrupt  ");
            else if (eventptr->evtype == 1)
                printf(", fromlayer3 ");
            else
                printf(", fromlayer1 ");
            printf(" entity: %d\n", eventptr->eventity);
        }
        time = eventptr->evtime; /* update time to next event time */
        if (eventptr->evtype == FROM_LAYER3)
        {
            if (nsim < nsimmax)
            {
                if (nsim + 1 < nsimmax)
                    generate_next_arrival(); /* set up future arrival */
                /* fill in msg to give with string of same letter */
                j = nsim % 26;
                for (i = 0; i < 4; i++)
                    pkt2give.data[i] = 97 + j;
                pkt2give.data[3] = 0;
                if (TRACE > 2)
                {
                    printf("          MAINLOOP: data given to student: ");
                    for (i = 0; i < 4; i++)
                        printf("%c", pkt2give.data[i]);
                    printf("\n");
                }
                nsim++;
                if (eventptr->eventity == A)
                    A_output(pkt2give);
                else
                    B_output(pkt2give);
            }
        }
        else if (eventptr->evtype == FROM_LAYER1)
        {
            frm2give.seqnum = eventptr->frmptr->seqnum;
            frm2give.acknum = eventptr->frmptr->acknum;
            //pkt2give.checksum = eventptr->pktptr->checksum;
            strcpy(frm2give.checksum,eventptr->frmptr->checksum);
            frm2give.type=eventptr->frmptr->type;
            for (i = 0; i < 4; i++)
                frm2give.payload[i] = eventptr->frmptr->payload[i];
            if (eventptr->eventity == A) /* deliver packet by calling */
                A_input(frm2give); /* appropriate entity */
            else
                B_input(frm2give);
            free(eventptr->frmptr); /* free the memory for packet */
        }
        else if (eventptr->evtype == TIMER_INTERRUPT)
        {
            if (eventptr->eventity == A)
                A_timerinterrupt();
            else
                B_timerinterrupt();
        }
        else
        {
            printf("INTERNAL PANIC: unknown event type \n");
        }
        free(eventptr);
    }

terminate:
    fclose(fptr);
    printf(
        " Simulator terminated at time %f\n after sending %d msgs from layer5\n",
        time, nsim);
}

void init() /* initialize the simulator */
{
    int i;
    float sum, avg;
    float jimsrand();

    printf("-----  Sliding Window Protocol Network Simulator Version 1.1 -------- \n\n");
    char dv[100];
    printf("Enter the Polynomial Generator: ");
    scanf("%s",&dv);
    strcpy(in,dv);
    printf("Enter the CRC steps: ");
    scanf("%d",&steps);
    printf("Enter the number of messages to simulate: ");
    scanf("%d",&nsimmax);
    printf("Enter  packet loss probability [enter 0.0 for no loss]:");
    scanf("%f",&lossprob);
    printf("Enter packet corruption probability [0.0 for no corruption]:");
    scanf("%f",&corruptprob);
    printf("Enter average time between messages from sender's layer5 [ > 0.0]:");
    scanf("%f",&lambda);
    printf("Enter TRACE:");
    scanf("%d",&TRACE);

    srand(9999); /* init random number generator */
    sum = 0.0;   /* test random number generator for students */
    for (i = 0; i < 1000; i++)
        sum = sum + jimsrand(); /* jimsrand() should be uniform in [0,1] */
    avg = sum / 1000.0;
    if (avg < 0.25 || avg > 0.75)
    {
        printf("It is likely that random number generation on your machine\n");
        printf("is different from what this emulator expects.  Please take\n");
        printf("a look at the routine jimsrand() in the emulator code. Sorry. \n");
        exit(1);
    }

    ntolayer1 = 0;
    nlost = 0;
    ncorrupt = 0;

    time = 0.0;              /* initialize time to 0.0 */
    generate_next_arrival(); /* initialize event list */
}

/****************************************************************************/
/* jimsrand(): return a float in range [0,1].  The routine below is used to */
/* isolate all random number generation in one location.  We assume that the*/
/* system-supplied rand() function return an int in therange [0,mmm]        */
/****************************************************************************/
float jimsrand(void)
{
    double mmm = RAND_MAX;
    float x;                 /* individual students may need to change mmm */
    x = rand() / mmm;        /* x should be uniform in [0,1] */
    return (x);
}

/********************* EVENT HANDLINE ROUTINES *******/
/*  The next set of routines handle the event list   */
/*****************************************************/

void generate_next_arrival(void)
{

    double x, log(), ceil();
    struct event *evptr;
    float ttime;
    int tempint;

    if (TRACE > 2)
        printf("          GENERATE NEXT ARRIVAL: creating new arrival\n");

    x = lambda * jimsrand() * 2; /* x is uniform on [0,2*lambda] */
    /* having mean of lambda        */
    evptr = (struct event *)malloc(sizeof(struct event));
    evptr->evtime = time + x;
    evptr->evtype = FROM_LAYER3;
    if (BIDIRECTIONAL && (jimsrand() > 0.5))
        evptr->eventity = B;
    else
        evptr->eventity = A;
    insertevent(evptr);
}

void insertevent(struct event *p)
{
    struct event *q, *qold;

    if (TRACE > 2)
    {
        printf("            INSERTEVENT: time is %lf\n", time);
        printf("            INSERTEVENT: future time will be %lf\n", p->evtime);
    }
    q = evlist;      /* q points to header of list in which p struct inserted */
    if (q == NULL)   /* list is empty */
    {
        evlist = p;
        p->next = NULL;
        p->prev = NULL;
    }
    else
    {
        for (qold = q; q != NULL && p->evtime > q->evtime; q = q->next)
            qold = q;
        if (q == NULL)   /* end of list */
        {
            qold->next = p;
            p->prev = qold;
            p->next = NULL;
        }
        else if (q == evlist)     /* front of list */
        {
            p->next = evlist;
            p->prev = NULL;
            p->next->prev = p;
            evlist = p;
        }
        else     /* middle of list */
        {
            p->next = q;
            p->prev = q->prev;
            q->prev->next = p;
            q->prev = p;
        }
    }
}

void printevlist(void)
{
    struct event *q;
    int i;
    printf("--------------\nEvent List Follows:\n");
    for (q = evlist; q != NULL; q = q->next)
    {
        printf("Event time: %f, type: %d entity: %d\n", q->evtime, q->evtype,
               q->eventity);
    }
    printf("--------------\n");
}

/********************** Student-callable ROUTINES ***********************/

/* called by students routine to cancel a previously-started timer */
void stoptimer(int AorB /* A or B is trying to stop timer */)
{
    struct event *q, *qold;

    if (TRACE > 2)
        printf("          STOP TIMER: stopping timer at %f\n", time);
    /* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next)  */
    for (q = evlist; q != NULL; q = q->next)
        if ((q->evtype == TIMER_INTERRUPT && q->eventity == AorB))
        {
            /* remove this event */
            if (q->next == NULL && q->prev == NULL)
                evlist = NULL;          /* remove first and only event on list */
            else if (q->next == NULL) /* end of list - there is one in front */
                q->prev->next = NULL;
            else if (q == evlist)   /* front of list - there must be event after */
            {
                q->next->prev = NULL;
                evlist = q->next;
            }
            else     /* middle of list */
            {
                q->next->prev = q->prev;
                q->prev->next = q->next;
            }
            free(q);
            return;
        }
    printf("Warning: unable to cancel your timer. It wasn't running.\n");
}

void starttimer(int AorB /* A or B is trying to start timer */, float increment)
{
    struct event *q;
    struct event *evptr;

    if (TRACE > 2)
        printf("          START TIMER: starting timer at %f\n", time);
    /* be nice: check to see if timer is already started, if so, then  warn */
    /* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next)  */
    for (q = evlist; q != NULL; q = q->next)
        if ((q->evtype == TIMER_INTERRUPT && q->eventity == AorB))
        {
            printf("Warning: attempt to start a timer that is already started\n");
            return;
        }

    /* create future event for when timer goes off */
    evptr = (struct event *)malloc(sizeof(struct event));
    evptr->evtime = time + increment;
    evptr->evtype = TIMER_INTERRUPT;
    evptr->eventity = AorB;
    insertevent(evptr);
}

/************************** TOLAYER3 ***************/
void tolayer1(int AorB, struct frm frame)
{
    struct frm *myfrmptr;
    struct event *evptr, *q;
    float lastime, x;
    int i;

    ntolayer1++;

    /* simulate losses: */
    if (jimsrand() < lossprob)
    {
        nlost++;
        if (TRACE > 0){
            printf("          TOLAYER1: frame being lost\n");
            fprintf(fptr,"          TOLAYER1: frame being lost\n\n");
        }
        return;
    }

    /* make a copy of the packet student just gave me since he/she may decide */
    /* to do something with the packet after we return back to him/her */
    myfrmptr = (struct frm *)malloc(sizeof(struct frm));
    myfrmptr->seqnum = frame.seqnum;
    myfrmptr->acknum = frame.acknum;
    //mypktptr->checksum = packet.checksum;
    strcpy(myfrmptr->checksum,frame.checksum);
    myfrmptr->type=frame.type;
    for (i = 0; i < 4; i++)
        myfrmptr->payload[i] = frame.payload[i];
    if (TRACE > 2)
    {
        printf("          TOLAYER1: seq: %d, ack %d, check: %s ", myfrmptr->seqnum,
               myfrmptr->acknum, myfrmptr->checksum);
        for (i = 0; i < 4; i++)
            printf("%c", myfrmptr->payload[i]);
        printf("\n");
    }

    /* create future event for arrival of packet at the other side */
    evptr = (struct event *)malloc(sizeof(struct event));
    evptr->evtype = FROM_LAYER1;      /* packet will pop out from layer3 */
    evptr->eventity = (AorB + 1) % 2; /* event occurs at other entity */
    evptr->frmptr = myfrmptr;         /* save ptr to my copy of packet */
    /* finally, compute the arrival time of packet at the other end.
       medium can not reorder, so make sure packet arrives between 1 and 10
       time units after the latest arrival time of packets
       currently in the medium on their way to the destination */
    lastime = time;
    /* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next) */
    for (q = evlist; q != NULL; q = q->next)
        if ((q->evtype == FROM_LAYER1 && q->eventity == evptr->eventity))
            lastime = q->evtime;
    evptr->evtime = lastime + 1 + 9 * jimsrand();

    /* simulate corruption: */
    if (jimsrand() < corruptprob)
    {
        ncorrupt++;
        if ((x = jimsrand()) < .75)
            myfrmptr->payload[0] = 'Z'; /* corrupt payload */
        else if (x < .875)
            myfrmptr->seqnum = 999999;
        else
            myfrmptr->acknum = 999999;
        if (TRACE > 0){
            printf("          TOLAYER3: frame being corrupted\n");
            fprintf(fptr,"          TOLAYER3: frame being corrupted\n\n");
        }
    }

    if (TRACE > 2)
        printf("          TOLAYER3: scheduling arrival on other side\n");
    insertevent(evptr);
}

void tolayer3(int AorB, char datasent[4])
{
    int i;
    if (TRACE > 2)
    {
        printf("          TOLAYER3: data received: ");
        for (i = 0; i < 4; i++)
            printf("%c", datasent[i]);
        printf("\n");
    }
}

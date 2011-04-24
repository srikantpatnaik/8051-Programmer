/**orginally written by Srikant Patnaik**/
/**Present Version is modified by Karsten Albrektsen**/

/** This is a free software,and you can do whatever you want  **/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/io.h>
#include <string.h>

#define bit_set(value, bit)      ((value) |= (bit))
#define bit_clear(value, bit)    ((value) &= ~(bit))


void pport_enable(void);
unsigned char ascii2int(unsigned char msb,unsigned char lsb);
unsigned int  char2hexaddr(unsigned int *addr),test;
unsigned char char2hexdata(unsigned char upnib,unsigned char lownib);
void writeflash(unsigned long data,unsigned long address);
unsigned long  prog_enable(int clockType);
void chip_erase(void);
void close_communication(void);
unsigned long clock_command_1(unsigned long command) ;
unsigned long clock_command_2(unsigned long command) ;
unsigned long clock_command(unsigned long command) ;
void detectChipType();
void readLockBits();
void readLockBits_1();
void readLockBits_2();

unsigned char clockType =0;
unsigned char chipType =0;
char manufacture[] = "Unknown";
char chipName[] = "Unknown"; 
char lockbitsDescription[] = "       Unknown            "; 




unsigned long PROGRAM_ENABLE =       0xAC53FF69L;
unsigned long CHIP_ERASE =           0xAC80FFFFL;
unsigned long WRITE_PROGRAM =        0x40000000L;
unsigned long READ_PROGRAM =         0x20000000L;




unsigned long const ChipType_1_Read_Signature1_Bytes = 0x28000000L;
unsigned long const ChipType_1_Read_Signature2_Bytes = 0x28010000L;
unsigned long const ChipType_1_Read_Signature3_Bytes = 0x28020000L;

unsigned long const ChipType_2_Read_Signature1_Bytes = 0x28003000L;
unsigned long const ChipType_2_Read_Signature2_Bytes = 0x28003100L;
unsigned long const ChipType_2_Read_Signature3_Bytes = 0x28003200L;

unsigned long Read_Signature1_Bytes = 0;
unsigned long Read_Signature2_Bytes = 0;
unsigned long Read_Signature3_Bytes = 0;


unsigned long Read_LOCK_BITS =        0x24000000L;




unsigned long WRITE_NO_LOCK_BITS =    0xACE70000L;
unsigned long WRITE_LB1_LOCK_BITS =   0xACE60000L; 
unsigned long WRITE_LB2_LOCK_BITS =   0xACE50000;  
unsigned long WRITE_LB3_LOCK_BITS =   0xACE30000;  


unsigned long ChipType_1_WRITE_NO_LOCK_BITS =    0xACE00000L;
unsigned long ChipType_1_WRITE_LB1_LOCK_BITS =   0xACE10000L; 
unsigned long ChipType_1_WRITE_LB2_LOCK_BITS =   0xACE20000L;  
unsigned long ChipType_1_WRITE_LB3_LOCK_BITS =   0xACE30000L;  


unsigned long ChipType_2_WRITE_NO_LOCK_BITS =    0xACE70000L;
unsigned long ChipType_2_WRITE_LB1_LOCK_BITS =   0xACE60000L; 
unsigned long ChipType_2_WRITE_LB2_LOCK_BITS =   0xACE50000;  
unsigned long ChipType_2_WRITE_LB3_LOCK_BITS =   0xACE30000;  










unsigned char SCK=0x40;
unsigned char MOSI=0x20;
unsigned char MISO_BITNR=6;
unsigned char MISO_INV=0x00;
unsigned char RESET=0x10;
unsigned char CHIP_ENABLE=0x8;

unsigned char ENABLE;
unsigned char outbyte;

int portAddress; 
int portStatus;




int getBit(int number, int position) {
	unsigned int bitmask = 1 << position;
	return (number & bitmask) ? 1 : 0;
}


void readLockBits(){

	prog_enable(0);

	if(chipType == 1){
	    readLockBits_1();
	}

	if(chipType == 2){
   	    readLockBits_2();
	}


}


//89s51, 89s52
void readLockBits_1(){
	unsigned long readBack = 0;
	readBack = clock_command(Read_LOCK_BITS);
	int lastbyte =readBack % 0x100;

	printf("Lockbits : 0x%x\n",lastbyte );

	printf("Lockbit 1: %d\n",getBit(lastbyte,2) );
	printf("Lockbit 2: %d\n",getBit(lastbyte,3) );
	printf("Lockbit 3: %d\n",getBit(lastbyte,4) );
        int b1 = getBit(lastbyte,2);
        int b2 = getBit(lastbyte,3);
        int b3 = getBit(lastbyte,4);

        if(!b1 && ! b2)
            strcpy(lockbitsDescription,"Mode 1, no lock protection");
        if(!b1 &&  b2)
            strcpy(lockbitsDescription,"Mode 2, lock bit 1 activated");
        if(b1 &&  !b2)
            strcpy(lockbitsDescription,"Mode 3, lock bit 2 activated");
        if(b1 &&  b2)
            strcpy(lockbitsDescription,"Mode 4, lock bit 3 activated");


}

//89s8253
void readLockBits_2(){
	unsigned long readBack = 0;
	readBack = clock_command(Read_LOCK_BITS);
	int lastbyte =readBack % 0x100;

	printf("Lockbits : 0x%x\n",lastbyte );

	printf("Lockbit 1: %d\n",getBit(lastbyte,0) );
	printf("Lockbit 2: %d\n",getBit(lastbyte,1) );
	printf("Lockbit 3: %d\n",getBit(lastbyte,2) );
        int b1 = getBit(lastbyte,0);
        int b2 = getBit(lastbyte,1);
        int b3 = getBit(lastbyte,2);

        if(b1 &&  b2 && b3)
            strcpy(lockbitsDescription,"Mode 1, no lock protection");
        if(!b1 &&  b2  && b3)
            strcpy(lockbitsDescription,"Mode 2, lock bit 1 activated");
        if(!b1 &&  !b2 && b3)
            strcpy(lockbitsDescription,"Mode 3, lock bit 2 activated");
        if(!b1 &&  !b2 && !b3)
            strcpy(lockbitsDescription,"Mode 4, lock bit 3 activated");

}



void writeLockBits(unsigned long lockBit){
	unsigned long readBack = 0;
	readBack = clock_command(lockBit);
	int lastbyte =readBack % 0xff;
	printf("Reply : 0x%x\n",lastbyte );

}







void verifyFlash(){
	FILE *temp;
	unsigned int k;
        char fileName[] ="bin";
        int line = 0; 
	unsigned int n =0;
        unsigned long readBack = 0;
        

        if((temp=fopen(fileName,"r"))!=NULL){
  	  while((k=fgetc(temp))!=EOF){
                line++;
		printf("%02x ",k );
                if(line > 20){
                    printf("\n");
                    line =0; 
                }
               readBack = clock_command(READ_PROGRAM + n*0x100);
               unsigned int byte =readBack % 256;
               if(k != byte){
                  printf("\n\nVerify error location %d  file: %02x    flash: %02x ",n,k,byte );
		  fclose(temp);
     		  printf("\n\n");
                  return; 
	       } 
               n++;
	 }
	}else{
           printf("File not found");
        }
      fclose(temp);
     printf("\n\n");
     printf("Verify OK");

}


void readFlash(){
	int chars =0;
	int n =0;
        int FFCount =0;  

	unsigned long readBack = 0;
	  while(FFCount < 6){
	   readBack = clock_command(READ_PROGRAM + n*0x100);
	   int lastbyte =readBack % 256;
           if(lastbyte == 0xFF){
             FFCount++;
           }else{
             FFCount =0;
  	   }

	   printf("%02x ",lastbyte );
	   chars++;
	   if(chars > 20){
	     chars=0;
	     printf("\n");
	  }
         n++; 
       }
	printf("\n");
	printf("Bytes count: %d",n);
	printf("\n");
}




void readFile(){
	FILE *temp;
	unsigned int k;
        char fileName[] ="bin";
        int line = 0; 

        if((temp=fopen(fileName,"r"))!=NULL){
  	  while((k=fgetc(temp))!=EOF){
                line++;
		printf("%02x ",k );
                if(line > 20){
                    printf("\n");
                    line =0; 
                }

                   
	 }
	}else{
           printf("File not found");
        }
      fclose(temp);
     printf("\n\n");

}

void writeFile(){
	FILE *temp;
	unsigned int k;
        char fileName[] ="test";
        if((temp=fopen(fileName,"w"))!=NULL){
           fputc(1, temp);
           fputc(2, temp);
	}else{
           printf("File could not be created");
        }
      fclose(temp);
}


long readAndConvertFile(char * fileName,int program){

	FILE *hex,*bin,*buf;
	unsigned int line=0;
	unsigned char a,d,e,data;
	unsigned char b,c,upnib,lownib;
	unsigned int address,k,addr[4];
        long programSize =0;
	long adressHigh = 0;
        char binFileName[] ="bin";


	system("clear");


	 if((hex=fopen(fileName,"r"))==NULL) {
	    printf("\nFile not found: %s\n",fileName);
	    exit(1);
	  }


        
       if (! program) {
        if((bin=fopen(binFileName,"w"))!=NULL){
            printf("File %s opened for write\n ",binFileName);
            
	}else{
           printf("File could not be created");
        }
       }



	/////////////////////////////////////////////



	while((k=fgetc(hex))!=EOF)
	{if(k==':')
	line++;}//total no of lines in the hex file
	rewind(hex);
	line=line-1;//skipping the last line to be printed

	///////////////////


	while(line--)
	{
	while(fgetc(hex)!=':');
	{
		a=fgetc(hex);
		c=fgetc(hex);
		b=ascii2int(a,c);

		for(c=0;c<4;c++)
		{
		  addr[c]=fgetc(hex);
                  if ( program) 
		    printf("%c",addr[c]);
		}


		address=char2hexaddr(addr);




		if ( program)
  		   printf(":");


		for(c=0;c<2;c++)//skipping the next line information
		  fgetc(hex);


		 for(c=0;c<b;c++)//1byte of data to be converted to the hexadecimal no
		 {
			unsigned  int address_flash;

			upnib=fgetc(hex);
			lownib=fgetc(hex);
                        if ( program)  
 			  printf("%c%c",upnib,lownib);
			data=char2hexdata(upnib,lownib);
			address_flash=address;
			address_flash+=c;

		        if(address_flash > adressHigh){
		             adressHigh = address_flash;
			}
			programSize++;


		        if ( program)
 			  writeflash(data,address_flash);//writes the data to the flash
                        else{ 
			 fseek ( bin , address_flash , SEEK_SET );
                         fputc(data, bin);
			}
			
	          }


		for(e=0;e<2;e++)//skipping the checksum
		  a=fgetc(hex);


		if ( program){ 
                  usleep(30000);
		  printf("\n");
		}

	}


	address=0;

	}

       if (! program) fclose(bin);

       printf("\nprogramSize %ld",programSize);
       printf("\nadressHigh %ld\n",adressHigh);
	
       return programSize;



}


void printHeader(){
   printf("\n  ***********ISP BASED FLASH PROGRAMMER FOR AT89S51 AND AT89S52***********\n\n");
     printf("  ***************  ORIGINALLY WRITTEN BY SRIKANT PATNAIK   ***************\n");
     printf("  **********  PRESENT VERSION MODIFIED BY KARSTEN ALBREKTSEN  ************\n\n");
   printf("\n");
}





main(int argc,char *argv[]){

int job = 0;

  pport_enable();
  detectChipType();

if(argc == 1){

  int choose = 0;

  while(choose != 4){ 

         readLockBits();
         bit_clear(outbyte, RESET);
         outb(outbyte,portAddress);

         bit_set(outbyte, CHIP_ENABLE);
         outb(outbyte,portAddress);





	 system("clear");
         printHeader();

        printf("\n  Chip: %s - %s\n",manufacture,chipName);
        printf("  %s\n",lockbitsDescription);

 	 printf("\n--------Choose action------------\n");
	 printf("1 Read flash\n");
  	 printf("2 Erase flash\n");
	 printf("3 Write lock bit\n");
	 printf("4 Exit\n");

	scanf("%d", &choose);

        printf("\n");

	if(choose == 4){
          return 0;
	}


         bit_clear(outbyte, CHIP_ENABLE);
         outb(outbyte,portAddress);

	prog_enable(0);

	if(choose == 1){
	  readFlash();
	}
	if(choose == 2){
           chip_erase();
	}
	if(choose == 3){
	  system("clear");
	  printf("\n");
 	  printf("\n--------Choose lock level------------\n");
  	  printf("1 Lock bit 1\n");
  	  printf("2 Lock bit 2\n");
  	  printf("3 Lock bit 3\n");
  	  printf("4 Cancel\n");
	  scanf("%d", &choose);
          printf("\n");
	  if(choose == 1)
       	     writeLockBits(WRITE_LB1_LOCK_BITS);
	  if(choose == 2)
       	     writeLockBits(WRITE_LB2_LOCK_BITS);
	  if(choose == 3)
       	     writeLockBits(WRITE_LB3_LOCK_BITS);

	}

	getchar() ;
	printf("\nPress ENTER to continue\n");
        while( getchar() != '\n' );
	system("clear");

   }


}


if(argc == 2){


  int choose = 0;
  long programSize = 0;
  
  programSize = readAndConvertFile(argv[1],0);


  while(choose != 6){ 

         readLockBits();

         bit_clear(outbyte, RESET);
         outb(outbyte,portAddress);
         bit_set(outbyte, CHIP_ENABLE);
         outb(outbyte,portAddress);


         system("clear");
         printHeader();

        printf("\n  Chip: %s - %s\n",manufacture,chipName);
        printf("  %s\n",lockbitsDescription);
        printf("\n  FileName: %s - program size: %ld bytes ",argv[1],programSize);
                
 
         printf("\n");
 	 printf("\n--------Choose action------------\n");
  	 printf("1 Write hexfile to flash\n");
	 printf("2 Verify\n");
	 printf("3 Read flash\n");
  	 printf("4 Erase flash\n");
	 printf("5 Write lock bit\n");
	 printf("6 Exit\n");
	 //printf("0 Test\n");


	scanf("%d", &choose);
        printf("\n");

	if(choose == 6){
          return 0;
	}


         bit_clear(outbyte, CHIP_ENABLE);
         outb(outbyte,portAddress);



	prog_enable(0);


	if(choose == 0){
//	   readFile();
	  detectChipType();


	}


	if(choose == 1){
  	   chip_erase();
           readAndConvertFile(argv[1],1);
	}

	if(choose == 2){
          readAndConvertFile(argv[1],0);
	  verifyFlash();
	}

	if(choose == 3){
	  readFlash();
	}

	if(choose == 4){
           chip_erase();
	}

	if(choose == 5){
         
	  system("clear");
	  printf("\n");
 	  printf("\n--------Choose lock level------------\n");
  	  printf("1 Lock bit 1\n");
  	  printf("2 Lock bit 2\n");
  	  printf("3 Lock bit 3\n");
  	  printf("4 Cancel\n");
	  scanf("%d", &choose);
          printf("\n");
	  if(choose == 1)
       	     writeLockBits(WRITE_LB1_LOCK_BITS);
	  if(choose == 2)
       	     writeLockBits(WRITE_LB2_LOCK_BITS);
	  if(choose == 3)
       	     writeLockBits(WRITE_LB3_LOCK_BITS);
	}

	getchar() ;
	printf("\nPress ENTER to continue\n");
        while( getchar() != '\n' );
	system("clear");

   }




}





if(argc == 3){

 if (strcmp(argv[2],"-r")==0){
   job = 1;
 }

}

printf("\nJob %i\n ", job);

}




unsigned char ascii2int(unsigned char msb,unsigned char lsb)

{
unsigned char a;

if((msb>47)&&(msb<58))
msb=msb-48;
else
msb=(msb-65)+10;

if((lsb>47)&&(lsb<58))
lsb=lsb-48;
else
lsb=(lsb-65)+10;

lsb=1*lsb;
msb=16*msb;
a=lsb+msb;
return a;
}
//////////////////////////////////////////////////////////////////////////////////////////

unsigned int char2hexaddr(unsigned int *addr)
{
unsigned char i,temp;
unsigned int address=0,j;
for(i=0,j=12;i<4;i++)

{temp=0;
temp=addr[i];
if((temp>=48)&&(temp<=57))
temp=temp-48;
else
temp=(temp-65)+10;
address|=((unsigned int)temp<<j);

j=j-4;
}
return address;
}
///////////////////////////////////////////////////////////////////////////////////////////



unsigned char char2hexdata(unsigned char upnib,unsigned char lownib)

{
unsigned char data=0;


if((upnib>47)&&(upnib<58))
upnib=upnib-48;
else
upnib=(upnib-65)+10;

if((lownib>47)&&(lownib<58))
lownib=lownib-48;
else
lownib=(lownib-65)+10;

data|=upnib<<4;
data|=lownib<<0;

return data;
}
///////////////////////////////////////////////////////////////////////////////////////////

void writeflash(unsigned long data,unsigned long address)
{
	unsigned long readBack=0x0l;

	address=(0x00000000 |address)<<8;
	WRITE_PROGRAM= (address | WRITE_PROGRAM);

	WRITE_PROGRAM=(WRITE_PROGRAM | data);

	clock_command(WRITE_PROGRAM);

	usleep(700);//delay for byte write
	WRITE_PROGRAM=WRITE_PROGRAM & 0xFF000000;//to avoid adding up of previous address
}

//////////////////////////////////////////////////////////////////////////////////////////

void detectChipType(){
unsigned long readBack = 0;
unsigned char manufactureCode;
unsigned char chipNameCode;
unsigned char nameCode;

 bit_set(outbyte, RESET);
 outb(outbyte,portAddress);
 usleep(20000);


// Test for clockType 1
prog_enable(1);
readBack = clock_command_1(ChipType_1_Read_Signature2_Bytes);
int lastbyte =readBack % 256;
if ((lastbyte != 0xff)  && (readBack != 0)){
    clockType = 1;
}


// Test for clockType 2
prog_enable(2);
readBack = clock_command_2(ChipType_2_Read_Signature2_Bytes);
lastbyte =readBack % 256;
if ((lastbyte != 0xff)  && (readBack != 0)){
    clockType = 2;
}

printf("clockType testet to: %i\n",clockType);




prog_enable(0);

//Test for chipType1
readBack = clock_command(ChipType_1_Read_Signature2_Bytes);
lastbyte =readBack % 256;
printf("Manufactured: 0x%x\n",lastbyte );
if ((lastbyte != 0xff)  && (readBack != 0)){
  chipNameCode = lastbyte;
  Read_Signature1_Bytes = ChipType_1_Read_Signature1_Bytes;
  Read_Signature2_Bytes = ChipType_1_Read_Signature2_Bytes;
  Read_Signature3_Bytes = ChipType_1_Read_Signature3_Bytes;
  chipType = 1;	
}

//Test for chipType2
readBack = clock_command(ChipType_2_Read_Signature2_Bytes);
lastbyte =readBack % 256;
printf("Manufactured: 0x%x\n",lastbyte );
if ((lastbyte != 0xff)  && (readBack != 0)){
  chipNameCode = lastbyte;
  Read_Signature1_Bytes = ChipType_2_Read_Signature1_Bytes;
  Read_Signature2_Bytes = ChipType_2_Read_Signature2_Bytes;
  Read_Signature3_Bytes = ChipType_2_Read_Signature3_Bytes;
  chipType = 2;
}


printf("chipType testet to: %i\n",chipType);


//Get chip name
if(chipNameCode == 0x52){
  strcpy(chipName,"89S52");
 printf("Chip name: 89S52\n");
}else if(chipNameCode == 0x73){
  strcpy(chipName,"89S8253");
 printf("Chip name: 89S8253\n");
}else if(chipNameCode == 0x51){
  strcpy(chipName,"89S51");
 printf("Chip name: 89S51\n");


}else{
  printf("Chip name code: 0x%x\n",chipNameCode );
}






//Get Manufactured
readBack = clock_command(Read_Signature1_Bytes);
lastbyte =readBack % 256;
printf("Name: 0x%x\n",lastbyte );

if(lastbyte == 0x1e){
 printf("Manufactured by Atmel\n");
 strcpy(manufacture,"Atmel");
}else{
  strcpy(manufacture,"Unknown");
  printf("Manufactured: 0x%x\n",lastbyte );
}

}




void pport_enable(void) {


   if (ioperm(0x278,3,1)) {perror("ioperm error at 0x278");exit(1);}
   if (ioperm(0x378,3,1)) {perror("ioperm error at 0x378");exit(1);}
   if (ioperm(0x3BC,3,1)) {perror("ioperm error at 0x3BC");exit(1);}
   if (ioperm(0xEC00,3,1)) {perror("ioperm error at 0xEC00");exit(1);}
   if (ioperm(0xE880,3,1)) {perror("ioperm error at 0xE880");exit(1);}
   
   outb(0x80,0x278);   
   usleep(1000);               
   if (inb(0x278) == 0x80) 
      {
      portAddress= 0x278;   
      portStatus=portAddress+1;
      printf("Parallel port Found and Tested at LPT2(0x278)\n");
      return; 
      } 
   
     outb(0x80,0x378);   
     usleep(1000);               


   if (inb(0x378) == 0x80) 
      {
      portAddress= 0x378;   
      portStatus=portAddress+1;
      printf("Parallel port Found and Tested at LPT1(0x378)\n");
      return; 
      } 

   outb(0x80,0x3BC);
   usleep(1000);               

   if (inb(0x3BC) == 0x80) 
      {
      portAddress= 0x3BC;   
      portStatus=portAddress+1;
      printf("Parallel port Found and Tested at LPT3(0x3BC)\n");
      return; 
      } 

   outb(0x80,0xE880);
   usleep(1000);               

   if (inb(0xE880) == 0x80) 
      {
      portAddress= 0xE880;   
      portStatus=portAddress+1;
      printf("Parallel port Found and Tested at LPT4(0xE880)\n");
      return; 
      } 

   outb(0x80,0xEC00);
   usleep(1000);               

   if (inb(0xEC00) == 0x80) 
      {
      portAddress= 0xEC00;   
      portStatus=portAddress+1;
      printf("Parallel port Found and Tested at LPT4(0xEC00)\n");
      return; 
      } 


   printf("Parallel port NOT Found\n");
   exit(1);


  }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void close_communication(void)
{
	//resetting the parallel port values to all zeros
	usleep(100);
	outbyte=0;
	outb(outbyte,portAddress);
	outb(outbyte,portStatus);

	 if (ioperm(portAddress, 3, 0)) {perror(""); exit(1);}
	 else
 	 printf("\nClosing Parallel Port and making RESET LOW          : Done\n\n\n\n\n");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////


unsigned long prog_enable(int clockType )
{


unsigned long readBack = 0;
   int tries = 0;


 
   bit_clear(outbyte, ENABLE | SCK | MOSI | RESET);// Initialise programming lines.
   outb(outbyte,portAddress);
  
   usleep(30000);//sleep for 30ms
 
      bit_set(outbyte, RESET);
      outb(outbyte,portAddress);

              
      usleep(30000);//20ms delay to start 
   
   while (tries <3) {
      tries++;
       
       if(clockType ==0)      
        readBack = clock_command(PROGRAM_ENABLE);
      if(clockType ==1)      
	readBack = clock_command_1(PROGRAM_ENABLE);
      if(clockType ==2)      
	readBack = clock_command_2(PROGRAM_ENABLE);

       readBack = (readBack & 0x000000FFL);//looking for return byte as of AVR
       printf("prog_enable ReadBack: %#lx\n",readBack);
     readBack =0x69;
      if (readBack == 0x69){
         return readBack;
	}
    }	


 
return readBack;

  
}





////////////////////////////////////////////////////////////////////////////////////////////////////////






unsigned long clock_command(unsigned long command) {

	if(clockType == 1){
	  return clock_command_1( command);
	}

	if(clockType == 2){
	  return clock_command_2( command);
	}

}



/*
    89s52 
*/
unsigned long clock_command_1(unsigned long command) 
{

  unsigned long inBuf;

unsigned long readBack = 0x0L;
   unsigned char inbyte;  
   int mostSigBit;
   int i;


   mostSigBit = 31;
   for (i = mostSigBit; i >= 0; i--)
 {
      if ((command >> i) & 0x01L)
          bit_set(outbyte, MOSI);
      else
          bit_clear(outbyte, MOSI);
      
	bit_clear(outbyte, SCK);
      outb(outbyte,portAddress);

     usleep(2);
      bit_set(outbyte, SCK);
      outb(outbyte,portAddress);

      usleep(2);
      inbyte = inb(portStatus);
      inBuf = ((unsigned long) (inbyte ^ MISO_INV)) >> MISO_BITNR;
      inBuf = (inBuf & 0x00000001L) << i;
      readBack = readBack | inBuf;
   }
//  return SCK to lo
 
   bit_clear(outbyte, SCK);
   outb(outbyte,portAddress);
   return(readBack);

}






/*
    89s8253
*/

unsigned long clock_command_2(unsigned long command) 
{

   unsigned long inBuf;
   unsigned long readBack = 0x0L;
   unsigned char inbyte;  
   int mostSigBit;
   int i;
   
  




   mostSigBit = 31;
   for (i = mostSigBit; i >= 0; i--)
   {


      bit_set(outbyte, SCK);
      outb(outbyte,portAddress);
      usleep(2);


      if ((command >> i) & 0x01L){
          bit_set(outbyte, MOSI);
	}
      else{
          bit_clear(outbyte, MOSI);
	}

   

      usleep(2);
      inbyte = inb(portStatus);
      inBuf = ((unsigned long) (inbyte ^ MISO_INV)) >> MISO_BITNR;
      inBuf = (inBuf & 0x00000001L) << i;
      readBack = readBack | inBuf;

      usleep(2);
      bit_clear(outbyte, SCK);
      outb(outbyte,portAddress);

      bit_set(outbyte, SCK);
      outb(outbyte,portAddress);
      usleep(2);


   }
   

return(readBack);
}





//////////////////////////////////////////////////////////////////////////////////////


void chip_erase(void)
{
	clock_command(CHIP_ERASE);
	printf("Performing Chip Erase\n");
	usleep(600000);
	clock_command(PROGRAM_ENABLE);
	usleep(30000);
	printf("Chip Erase done\n");
}
//////////////////////////////////////////////////////////////////////////////////////


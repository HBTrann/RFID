#include "diemdanh.h"
#include <stdint.h> //khai bao thu vien uint8_t
#include <unistd.h>
#include <mysql.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>

#define channel 0

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW *row;

char *server = "localhost";
char *user = "TRAN";
char *password = "pi";
char *database = "RC522";

char the1[6] = {0x71,0xF6,0x3C,0x1C,0xA7};
char the2[6] = {0x81,0x03,0xC9,0x1C,0x57};
char the3[6] = {0x7D,0xA6,0xCF,0x10,0x04};
char the4[6] = {0x90,0x66,0xC9,0x21,0x1E};
char the5[6] = {0x73,0x66,0x09,0x1D,0x01};
char id[6];
char hoten[5][20]={"Vo Cong Thuc","Le Quoc An","Ho Bao Tran","Nguyen Van A","Tran Van B"};
char mssv[5][10]={"19146272","19146142","19146280","19146210","19146211"};
int i;

void kiemtra()
{
  if(id[i]==the1[i])
  {
    printf(" \n ");
    // hoten = "Vo Cong Thuc";
    // mssv = "19146272";
    printf("%s",hoten[0]);
    printf("%s",mssv[0]);
    //Connect to database
    conn = mysql_init(NULL);
    mysql_real_connect(conn,server,user,password,database,0,NULL,0);
    // Create sql command
    char cmd[200];
    sprintf(cmd,"insert into read_rc522(hoten,mssv) values ('%s','%s')", hoten[0], mssv[0]);
    // send SQL query
    mysql_query(conn, cmd);
    mysql_close(conn);
  }
  if(id[i]==the2[i])
  {
    printf(" \n ");
    printf("%s",hoten[1]);
    printf("%s",mssv[1]);
    //Connect to database
    conn = mysql_init(NULL);
    mysql_real_connect(conn,server,user,password,database,0,NULL,0);
    // Create sql command
    char cmd[200];
    sprintf(cmd,"insert into read_rc522(hoten,mssv) values ('%s','%s')", hoten[1], mssv[1]);
    // send SQL query
    mysql_query(conn, cmd);
    mysql_close(conn);
  }
  if(id[i]==the3[i])
  {
    printf(" \n ");
    printf("%s ",hoten[2]);
    printf("%s ",mssv[2]);
    //Connect to database
    conn = mysql_init(NULL);
    mysql_real_connect(conn,server,user,password,database,0,NULL,0);
    // Create sql command
    char cmd[200];
    sprintf(cmd,"insert into read_rc522(hoten,mssv) values ('%s','%s')", hoten[2], mssv[2]);
    // send SQL query
    mysql_query(conn, cmd);
    mysql_close(conn);
  }
  if(id[i]==the4[i])
  {
    printf(" \n ");
    printf("%s",hoten[3]);
    printf("%s",mssv[3]);
    //Connect to database
    conn = mysql_init(NULL);
    mysql_real_connect(conn,server,user,password,database,0,NULL,0);
    // Create sql command
    char cmd[200];
    sprintf(cmd,"insert into read_rc522(hoten,mssv) values ('%s','%s')", hoten[3], mssv[3]);
    // send SQL query
    mysql_query(conn, cmd);
    mysql_close(conn);
  }
  if(id[i]==the5[i])
  {
    printf(" \n ");
    printf("%s",hoten[4]);
    printf("%s",mssv[4]);
    //Connect to database
    conn = mysql_init(NULL);
    mysql_real_connect(conn,server,user,password,database,0,NULL,0);
    // Create sql command
    char cmd[200];
    sprintf(cmd,"insert into read_rc522(hoten,mssv) values ('%s','%s')", hoten[4], mssv[4]);
    // send SQL query
    mysql_query(conn, cmd);
    mysql_close(conn);
  }
}
  
int main()
{
  unsigned char s;
  wiringPiSPISetup(channel, 8000000);
  MFRC522_init();

  while(1)
  {
    if (RFID_findCard(0x52,&s)==MI_OK)
    {
       if ( RFID_anticoll(id)==MI_OK)
       {
         for(i=0;i<5;i++)  
         {
            printf("%d ",id[i]);
            if(i==4)
            {  
               kiemtra(); 
            }
         }
            printf("\n");
       } 
       else printf("ERR\n");
    
    }
    sleep(1);
    
  }

  return 0;
}

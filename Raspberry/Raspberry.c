 #ifdef RaspberryPi
//  sudo gcc test.c -o helloB -l wiringPi -DRaspberryPi -I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient
 #include <stdio.h> //for printf
 #include <stdint.h> //uint8_t definitions
 #include <stdlib.h> //for exit(int);
 #include <string.h> //for errno
 #include <errno.h> //error output
 #include <time.h> 
 #include <wiringPi.h> 
 #include <wiringSerial.h> 
 #include "mysql/mysql.h"

 char device1[] = "/dev/ttyACM0"; // HX711
 char device2[] = "/dev/ttyACM1"; // ZigBee
 //char device3[]= "/dev/ttyACM2";
 //char device4[]= "/dev/ttyACM3";

 MYSQL * conn;
 MYSQL_RES * res;
 MYSQL_ROW row;

 char * server = "192.168.0.3";
 char * user = "root";
 char * password = "1234";
 char * database = "checkers";
 #define W_size 40
 int id = 4910060;
 int fd1;
 int fd2;
 int fd3;
 int fd4;
 unsigned long baud = 9600;

 char RFID = 'N';
 char RFIDInput = 'K';
 int flag = 0;
 char weight[W_size];
 int main(void);
 void loop(void);
 void setup(void);
 void excute(char * data);
 char * timeToString(struct tm * t);

 void setup() {
   fflush(stdout);

   if ((fd1 = serialOpen(device1, baud)) < 0) {
     fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
     exit(1); //error : device1
   }

   if ((fd2 = serialOpen(device2, baud)) < 0) {
     fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
     exit(1); //error : device2
   }

   if (wiringPiSetup() == -1) {
     fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
     exit(1); //error : can't start wiringPi
   }

   if (!(conn = mysql_init((MYSQL * ) NULL))) {
     printf("init fail\n");
     exit(1);
   }

   if (!mysql_real_connect(conn, server, user, password, NULL, 3306, NULL, 0)) {
     printf("connect error.\n"); //DB (MYSQL*, host, id, pw, null, port, 0)
     fprintf(stderr, "Failed to connect to databases : Error : %s\n", mysql_error(conn));
     exit(1);
   }
   printf("connect success.\n");

   if (mysql_select_db(conn, database) != 0) {
     printf("select_db fail.\n");
     mysql_close(conn);

     exit(1);
   }

   printf("Program is running!\n");
   for(int i=0;i<W_size;i++)
     weight[i] = '\0';

 }

 void loop() {
   while (RFID != 'Y') { // RFID 의 초기값은 N
    // RFIDInput = serialGetchar(fd2); // 지그비를 통해 RFID tag를 읽어온다.
     if (RFIDInput == 'K') {
       printf("RFID 읽기 완료 \n");
       RFID = 'Y';
       serialPutchar(fd1, 'Y');
       flag = 1;
       printf("get weight !!\n");
     }
   }
   while (flag == 1) {
     
     for (int i = 0; i < W_size; i++) {
          char ch = serialGetchar(fd1);
          printf("%c\n",ch);
          if (ch == '/') {
               weight[i] = '\0';
               break;
          }
          weight[i] = ch;
     }
     printf("DB 담기\n");
     excute(weight);
     fflush(stdout);
   }
 }

 int main(void) {
   setup();
   while (1) {
     loop();
   }
   return 0;
 }

void excute(char * data) {
	printf("excute 진입\n");
	char temp[100];
	int i, index;
	char weight_A[5];
	char weight_B[5];
	char weight_C[5];
	char weight_D[5];
	char weight_E[5];
	char head_tail[] = ",";
	char endTail[] = ");";
	char ch;
	for (char chk = 'A', i = 1, index = 0; index < 5; chk++, index++, i++) {
		int j = 0;
		char data_change[W_size];
		while (weight[i] != chk+1 && weight[i] != '/') {
			data_change[j++] = weight[i++];
		}
		data_change[j] = '\0';
		switch (index) {
		case 0:
			sprintf(weight_A, "%s", data_change);
			break;
		case 1:
			sprintf(weight_B, "%s", data_change);
			break;
		case 2:
			sprintf(weight_C, "%s", data_change);
			break;
		case 3:
			sprintf(weight_D, "%s", data_change);
			break;
		case 4:
			sprintf(weight_E, "%s", data_change);
			break;
		default:
			break;
		}
	}
	printf("%s %s %s %s %s\n", weight_A, weight_B, weight_C, weight_D, weight_E);

	printf("DB 담기 시작\n");
	char head[] = "INSERT INTO plate_weight VALUES(";
	sprintf(temp, "%s%s%s%s%s%s%s%s%s%s%s%s%s", head, id, head_tail, weight_A, head_tail, weight_B, head_tail, weight_C, head_tail, weight_D, head_tail, weight_E, endTail);
	printf("temp 담기\n");
	printf("%s\n", temp);
	//mysql_query(conn, temp);
	printf("DB 담기 완료\n");
}

char * timeToString(struct tm * t) {
     static char s[20];

     sprintf(s, "%04d%02d%02d%02d%02d%02d",
          t -> tm_year + 1900, t -> tm_mon + 1, t -> tm_mday,
          t -> tm_hour, t -> tm_min, t -> tm_sec
     );
     return s;
}

#endif //#ifdef RaspberryPi

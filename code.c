 #ifdef RaspberryPi

 #include < stdio.h > //for printf
 #include < stdint.h > //uint8_t definitions
 #include < stdlib.h > //for exit(int);
 #include < string.h > //for errno
 #include < errno.h > //error output
 #include < time.h > 
 #include < wiringPi.h > 
 #include < wiringSerial.h > 
 #include "mysql/mysql.h"

 char device1[] = "/dev/ttyACM0"; // RFID
 char device2[] = "/dev/ttyACM1"; // HX711
 //char device3[]= "/dev/ttyACM2";
 //char device4[]= "/dev/ttyACM3";

 MYSQL * conn;
 MYSQL_RES * res;
 MYSQL_ROW row;

 char * server = "192.168.0.3";
 char * user = "root";
 char * password = "12345678";
 char * database = "weight";
 const int W_size = 40;
 int id = 1;
 int fd1;
 int fd2;
 int fd3;
 int fd4;
 unsigned long baud = 9600;

 char RFID = 'N';
 char RFIDInput = 'F';
 int flag = 0;
 char weight[W_size];
 int main(void);
 void loop(void);
 void setup(void);
 void excute(char * data);
 char * timeToString(struct tm * t);

 void setup() {
   fflush(stdout);
   /*	if ((fd4 = serialOpen (device4, baud)) < 0){
   		printf (stderr, "Unable to open serial device: %s\n", strerror(errno)) ;
   		exit(1); //error : device4
   	}

   	if ((fd3 = serialOpen (device3, baud)) < 0) {
   		fprintf (stderr, "Unable to open serial device: %s\n", strerror(errno)) ;
   		exit(1); //error : device3
   	}
   */

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

 }

 void loop() {

   while (RFID != 'Y') {
     RFIDInput = serialGetchar(fd1);
     if (RFIDInput == 'K') {
       printf("%c\n", RFIDInput);
       RFID = 'Y';
       serialPutchar(fd2, 'Y');
       flag = 1;

     }
     if (RFIDInput == 'N') {
       RFID = 'N';
       serialPutchar(fd2, 'N');
       printf("%c\n", RFID);
     }
   }
   while (flag == 1) {
     char ch = serialGetchar(fd2);
     if (ch == 'Z')
       flag = 2;
   }
   printf("get weight !!\n");
   while (flag == 2) {
     char ch = serialGetchar(fd2);
     for (i = 0; i < W_size; i++) {
       if (weight[i] == '/') {
         weight[i] = '\0';
         break;
       }
       weight[i] = serialGetchar(fd2);
     }
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
   struct tm * t;
   time_t timer;
   timer = time(NULL);
   t = localtime( & timer);
   timeToString(t);

   char chk = weight[0];
   char temp[W_size];

   char head_tail[] = ",";
   char endTail[] = ");";
   int i = 0;
   char * timenow = timeToString(t);

   char data_change[W_size];
   for (i = 0; i < W_size; i++) {
     if (weight[i] == '\0') {
       break;
     }
     data_change[i] = weight[i + 1];
   }

   if (chk == 'A') {
     char head[] = "INSERT INTO weight VALUES(";
     sprintf(temp, "%s%s%s%s%s", head, id, head_tail, data_change, endTail);

   } else if (chk == 'B') {
     char head2[] = "INSERT INTO weight VALUES(1, ";
     sprintf(temp, "%s%s%s%s%s", head2, timenow, head_tail, data_change, endTail);
   } else if (chk == 'C') {
     char head3[] = "INSERT INTO weight VALUES(1, ";
     sprintf(temp, "%s%s%s%s%s", head3, timenow, head_tail, data_change, endTail);
   } else if (chk == 'D') {
     char head3[] = "INSERT INTO weight VALUES(1, ";
     sprintf(temp, "%s%s%s%s%s", head3, timenow, head_tail, data_change, endTail);
   } else if (chk == 'E') {
     char head3[] = "INSERT INTO weight VALUES(1, ";
     sprintf(temp, "%s%s%s%s%s", head3, timenow, head_tail, data_change, endTail);
   }

   printf("%s", temp);

   mysql_query(conn, temp);
 }

 char * timeToString(struct tm * t) {
   static char s[20];

   sprintf(s, "%04d%02d%02d%02d%02d%02d",
     t - > tm_year + 1900, t - > tm_mon + 1, t - > tm_mday,
     t - > tm_hour, t - > tm_min, t - > tm_sec
   );

   return s;
 }

 #endif //#ifdef RaspberryPi

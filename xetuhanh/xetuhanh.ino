#include<Arduino_FreeRTOS.h>
#include<task.h>
#include<queue.h>
#include<SoftwareSerial.h>

QueueHandle_t frontQueue;
QueueHandle_t rightQueue;
QueueHandle_t leftQueue;
QueueHandle_t commandQueue;

SoftwareSerial bluetooth(2,3);

int trig=4;
int echoRight=5;
int echoLeft=7;
int echoFront=6;

int rightmotor1=9;
int rightmotor2=10;
int leftmotor1=11;
int leftmotor2=12;

void setup(){
  // @author : TranGiaLuong - CT030433 - KMA : gmail - gialuong9820@gmail.com
  
  //=========== active pin and serial ==========
  Serial.begin(9600);
  bluetooth.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echoRight, INPUT);
  pinMode(echoFront, INPUT);
  pinMode(echoLeft, INPUT);
  pinMode(rightmotor1, OUTPUT);
  pinMode(rightmotor2, OUTPUT);
  pinMode(leftmotor1, OUTPUT);
  pinMode(leftmotor2, OUTPUT);
  //===========================================
  
  //================ create queue ================
  char message[4];
  frontQueue = xQueueCreate(1,sizeof(message));
  rightQueue = xQueueCreate(1,sizeof(message));
  leftQueue = xQueueCreate(1,sizeof(message));
  commandQueue = xQueueCreate(5,sizeof(message));
  //============================================

  //============== create task ===================
  xTaskCreate(hdsr04,"khoangcach", 200,NULL,1,NULL);
  xTaskCreate(start,"chay", 200,NULL,1,NULL);
  xTaskCreate(command,"cmd",200,NULL,1,NULL);
  vTaskStartScheduler();
  //==============================================
}

void loop(){}

void start(void *param){
  char num[4];
  int leftspace;
  int rightspace;
  int frontspace;
  while(1){
    
    //===== get command from queue =========
    char cmd[4];
    xQueueReceive(commandQueue,cmd,50);
    //======================================
    
    //======== L398N Controller ============
    switch(cmd[0]){
      case '1':
        xQueuePeek(frontQueue, num, ( TickType_t )0);
        frontspace = atoi(num);
        Serial.println(frontspace);
        if(frontspace>40){
          front(); 
        }else stp();
        break;
      case '2':
        back();
        break;
      case '3':
        xQueuePeek(leftQueue, num, ( TickType_t )0);
        leftspace = atoi(num);
        Serial.println(leftspace);
        if(leftspace>25){
          left();    
        }else stp();
        break;
      case '4':
        xQueuePeek(rightQueue, num, ( TickType_t )0);
        rightspace = atoi(num);
        Serial.println(rightspace);
        if(rightspace>25){
          right();
        }else stp();
        break;
      default:
        stp();
    }
    //=====================================
    vTaskDelay(20/ portTICK_PERIOD_MS);
  }
}


void command(void *param){
  while(1){
    if(bluetooth.available()){
      char cmd;
      cmd = bluetooth.read();
//      Serial.write(cmd);
      if(cmd=='1'){
        xQueueSend(commandQueue,"1",50);
      }else
      if(cmd=='2'){
        xQueueSend(commandQueue,"2",50); 
      }else
      if(cmd=='3'){
        xQueueSend(commandQueue,"3",50); 
      }else
      if(cmd=='4'){
        xQueueSend(commandQueue,"4",50); 
      }
    }else{
      xQueueSend(commandQueue,"nt",50);
    }
    vTaskDelay(40/ portTICK_PERIOD_MS);
  }
}


void hdsr04(void *param){
  char message[4];
  unsigned long duration1;
  unsigned long duration2;
  unsigned long duration3;
  
  int distance1;
  int distance2;
  int distance3;
  while(1){
    //   right hc sr04
    
    digitalWrite(trig,LOW);
    vTaskDelay(2/portTICK_PERIOD_MS);
    digitalWrite(trig,HIGH);
    vTaskDelay(10/portTICK_PERIOD_MS);
    digitalWrite(trig,LOW);
    duration1 = pulseIn(echoRight,HIGH);
    distance1 = int(duration1/2/29.412);
//    Serial.print("1: ");
//    Serial.println(distance1);
    sprintf(message,"%d",distance1);
    if(distance1>0){
      xQueueOverwrite(rightQueue,message);
    }
  
    vTaskDelay(20/portTICK_PERIOD_MS);
    
    // front hc-sr04
    
    digitalWrite(trig,LOW);
    vTaskDelay(2/portTICK_PERIOD_MS);
    digitalWrite(trig,HIGH);
    vTaskDelay(10/portTICK_PERIOD_MS);
    digitalWrite(trig,LOW);
    duration2 = pulseIn(echoFront,HIGH);
    distance2 = int(duration2/2/29.412);
//    Serial.print("2: ");
//    Serial.println(distance2);
    sprintf(message,"%d",distance2);
    if(distance2>0){
      xQueueOverwrite(frontQueue,message);
    }
    vTaskDelay(20/portTICK_PERIOD_MS);
    // left
    digitalWrite(trig,LOW);
    vTaskDelay(2/portTICK_PERIOD_MS);
    digitalWrite(trig,HIGH);
    vTaskDelay(10/portTICK_PERIOD_MS);
    digitalWrite(trig,LOW);
    duration3 = pulseIn(echoLeft,HIGH);
    distance3 = int(duration3/2/29.412);
//    Serial.print("3: ");
//    Serial.println(distance3);
    sprintf(message,"%d",distance3);
    if(distance3>0){
      xQueueOverwrite(leftQueue,message);
    }
    vTaskDelay(20/portTICK_PERIOD_MS);
  }
}

void back(){
  digitalWrite(leftmotor1, HIGH);
  digitalWrite(leftmotor2, LOW);
  digitalWrite(rightmotor1, HIGH);
  digitalWrite(rightmotor2, LOW);
}
void front(){
  digitalWrite(leftmotor1, LOW);
  digitalWrite(leftmotor2, HIGH);
  digitalWrite(rightmotor1, LOW);
  digitalWrite(rightmotor2, HIGH);
}
void right(){
  digitalWrite(leftmotor1, LOW);
  digitalWrite(leftmotor2, HIGH);
  digitalWrite(rightmotor1, HIGH);
  digitalWrite(rightmotor2, LOW);
}
void left(){
  digitalWrite(leftmotor1, HIGH);
  digitalWrite(leftmotor2, LOW);
  digitalWrite(rightmotor1, LOW);
  digitalWrite(rightmotor2, HIGH);
}

void stp(){
  digitalWrite(leftmotor1, LOW);
  digitalWrite(leftmotor2, LOW);
  digitalWrite(rightmotor1, LOW);
  digitalWrite(rightmotor2, LOW);
}

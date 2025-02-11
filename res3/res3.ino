#include <LiquidCrystal.h>

#include <LCD_1602_RUS.h>
#include <PCF8574.h>    
#include <Wire.h>

#include <Adafruit_PWMServoDriver.h>    // Подключаем библиотеку Adafruit_PWMServoDriver
 
 
//#define SERVO_1 0                       // Номер порта (0 - 15)


#define pinX    A2  // ось X джойстика
#define pinY    A1  // ось Y джойстика
#define swPin    8  // кнопка джойстика

// Объявляем все важные переменные как глобальные

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40); // Установка адреса I2C 0x40


PCF8574 POutput(0x20);
PCF8574 PInput(0x24);

LCD_1602_RUS lcd1(0x25,16,2);
LCD_1602_RUS lcd2(0x27,16,2);

/*bool sysNaliv = true;
bool sysMoove = false;*/

bool vyborNaliv = 0;
bool execNaliv = 0;

int currOperation = 0;
int menuLevel=0;
int curStakan=0;

int st1=0;
int st2=0;
int st3=0;
int st4=0;

int specM = -1430;
int koeff = 11;
int flowDelay=0;

int rec1[3]={50,0,0};
int rec2[3]={0,10,0};
int rec3[3]={0,0,40};
int rec4[3]={80,20,0};
int rec5[3]={30,0,50};
int rec6[3]={35,45,10};


long int flow_count = 0;
float flow_max = 50;

int joyButt=0;
int menuPosition=0;
int tmpMenuPosition=0;

int wtTODO = 0;


void closeAll()
{

 Serial.println("closeAll");

    POutput.digitalWrite(4, HIGH);
    digitalWrite(4+8, HIGH);
  delay(500);
  POutput.digitalWrite(0, HIGH);
  digitalWrite(0+8, HIGH);
  delay(500);
  POutput.digitalWrite(1, HIGH);
  digitalWrite(1+8, HIGH);
    delay(500);
  POutput.digitalWrite(2, HIGH);
  digitalWrite(2+8, HIGH);
    delay(500);
  POutput.digitalWrite(3, HIGH);
  digitalWrite(3+8, HIGH);
    delay(500);
  


 /* POutput.digitalWrite(5, HIGH);
    delay(100);
  POutput.digitalWrite(6, HIGH);
    delay(100);
  POutput.digitalWrite(7, HIGH);
    delay(100);
*/
  
}

void flow () // Interrupt function
{
   flow_count++;
     //  if (flow_count>=flow_max) {  closeAll();  delay (100);}

}


void setup() {
  // put your setup code here, to run once:

// подключения

    Wire.begin();    
 
    Serial.begin(9600);

// джойстик
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  
  pinMode(swPin, INPUT);
  digitalWrite(swPin, HIGH);

  pinMode (2,INPUT); // Расходомер
  attachInterrupt(digitalPinToInterrupt(2), flow, RISING); // Setup Interrupt

  pwm.begin();                          // Инициализация
  pwm.setPWMFreq(60);                   // Частота следования импульсов 60 Гц

 pinMode (5,OUTPUT); // мотор внизу
 digitalWrite(5, LOW);
 pinMode (6,OUTPUT); // мотор внизу
 digitalWrite(6, LOW);



lcd1.init();       
lcd1.noBacklight();
lcd1.clear();

lcd2.init();     
lcd2.noBacklight();
lcd2.clear();

delay(300);


  PInput.pinMode(0, INPUT);     
  PInput.pinMode(1, INPUT);      
  PInput.pinMode(2, INPUT);       
  PInput.pinMode(3, INPUT);      
  PInput.pinMode(4, INPUT);     
  PInput.pinMode(5, INPUT);      
  PInput.pinMode(6, INPUT);      
  PInput.pinMode(7, INPUT);

  POutput.pinMode(0, OUTPUT);     
  POutput.pinMode(1, OUTPUT);      
  POutput.pinMode(2, OUTPUT);       
  POutput.pinMode(3, OUTPUT);      
  POutput.pinMode(4, OUTPUT);     
  POutput.pinMode(5, OUTPUT);      
  POutput.pinMode(6, OUTPUT);      
  POutput.pinMode(7, OUTPUT);



  POutput.digitalWrite(0, HIGH);
  POutput.digitalWrite(1, HIGH);
  POutput.digitalWrite(2, HIGH);
  POutput.digitalWrite(3, HIGH);
  POutput.digitalWrite(4, HIGH);


  pinMode(8, OUTPUT);     
  pinMode(9, OUTPUT);     
  pinMode(10, OUTPUT);     
    pinMode(11, OUTPUT);     
      pinMode(12, OUTPUT);     


  pinMode(3, INPUT);  

digitalWrite(8, HIGH);
digitalWrite(9, HIGH);
digitalWrite(10, HIGH);
digitalWrite(11, HIGH);
digitalWrite(12, HIGH);




  /*PInput.digitalWrite(0, HIGH);    
  PInput.digitalWrite(1, HIGH);     
  PInput.digitalWrite(2, HIGH);      
  PInput.digitalWrite(3, HIGH);*/


  POutput.digitalWrite(5, LOW);
  POutput.digitalWrite(6, LOW);
  POutput.digitalWrite(7, LOW);

  
lcd1.backlight();// Включаем подсветку дисплея
lcd2.backlight();// Включаем подсветку дисплея

lcd1.setCursor(1, 0);
lcd1.print("Приветствие");

delay(1000);

nosikMove(1);

delay(2000);

/*do
{
int xxx = digitalRead(3);
lcd2.setCursor(1, 0);
if(xxx == HIGH) 
      lcd2.print("HIGH");
  else 
  lcd2.print("LOW");

delay (300);
}while(1);
*/


/*

nosikMove(-1500);

delay (2000);

nosikMove(-1500);

delay (2000);

nosikMove(-1500);

delay (2000);

nosikMove(4800);


*/

  drawMenu();

}




void nosikMove(int newPos)
{

Serial.println("nosikMove");
Serial.println(newPos);
int xxx;


if(newPos == 1)
{
  
  digitalWrite(5, HIGH);
  
  do
  {

    xxx = digitalRead(3);
    readJoyButt();
    delay(10);

  } while(xxx == HIGH && joyButt==0);
    digitalWrite(5, LOW);

    return;
}

if(newPos == -1)
{
 digitalWrite(6, HIGH);
  do
  {
   
    xxx = digitalRead(3);
    readJoyButt();
    //digitalWrite(6, HIGH);
    delay(10);
  } while(xxx == HIGH && joyButt==0);
   
   
    digitalWrite(6, LOW);

    return;
}




if (newPos>0)
{



digitalWrite(5, HIGH);
delay(newPos);
digitalWrite(5, LOW);
}
else
{
digitalWrite(6, HIGH);
delay(-newPos);
digitalWrite(6, LOW);
}


return;


uint16_t ii1 = pwm.getPWM(0);

delay(200);

for (uint16_t pulselen = ii1; pulselen > newPos; pulselen--) 
  {
    pwm.setPWM(0, 0, pulselen);
     delay(20);
  }
  delay(250);

  for (uint16_t pulselen = ii1; pulselen < newPos; pulselen++) 
  {
    pwm.setPWM(0, 0, pulselen);
     delay(20);
  }

  delay(250);


}

void readJoyButt()
{
  
  int X = analogRead(pinX);              // считываем значение оси Х
  int Y = analogRead(pinY);              // считываем значение оси Y


    if(X>800)
      {
          joyButt=1;

          while(X>800) { delay (20);  X = analogRead(pinX);} 
                       
          return;
      }

if(Y>800)

      {
          joyButt=10;
          while(Y>800) { delay (20);  Y = analogRead(pinY);} 
          return;
      }

    if(Y<200)

      {
          joyButt=200;
          while(Y<200) { delay (20);  Y = analogRead(pinY);} 
          return;
      }


if(X<200)

      {
          joyButt=2;
          while(X<200) { delay (20);  X = analogRead(pinX);} 
          return;
      }



    if(X>450&&X<570&&Y>450&&Y<570)

      {
          joyButt=0;
          return;
      }

return;

}


////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////                   ////////////////////////////////////////////////////
/////////////////////////////                   ///////////////////////////////////////////////////
/////////////////////////////       ///////////////////////////////////////////////////////////////
/////////////////////////////       ///////////////////////////////////////////////////////////////
////////////////////////////                 ///////////////////////////////////////////////////////
///////////////////////////////////             ///////////////////////////////////////////////////
///////////////////////////////////////         ////////////////////////////////////////////////////
//////////////////////////////////////          ////////////////////////////////////////////////////
////////////////////////////                   /////////////////////////////////////////////////////
////////////////////////////                 ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


void nalitStakan(int r1, int r2, int r3)
{

long int alarm_count;



do
  {

      switch (currOperation)
      {
        case 0:
              {

                // вкл 1

                              Serial.println("ВКЛ 1");
                              Serial.println(joyButt);

                 do {readJoyButt();delay(20);} while (joyButt==0);

                currOperation=1;
                flow_count = 0;
                flow_max = r1*koeff;
                if(flow_max<1){currOperation=2;break;}
                flow_max += flowDelay;
                alarm_count = 0;
                lcd1.setCursor(0, 0);
                lcd1.print("Наливается...");              
                lcd1.setCursor(2, 1);
                lcd1.print("Шаг1");
                /*POutput.digitalWrite(3, LOW); 
                digitalWrite(3+8, LOW);*/

                delay(500);
                POutput.digitalWrite(0, LOW); 
                digitalWrite(0+8, LOW);

                delay(500);     
                POutput.digitalWrite(4, LOW); 
                digitalWrite(4+8, LOW);

                //delay(100);
                break;    
            
              }

        case 1:
              {
                        
                        // вЫкл 1

                          alarm_count++;
                          //lcd2.setCursor(2, 0);
                          //lcd2.print(flow_count);
                              Serial.println(flow_count);
                              Serial.print(" alarm ");
                              Serial.print(alarm_count);

                          if(flow_count>=flow_max) {currOperation=2; closeAll(); break;}
                          if(alarm_count>=1000) {Serial.println("выход из стакана 1");currOperation=2; closeAll(); break;} // {currOperation=2;closeAll();lcd2.setCursor(2, 0);lcd2.print("Ошибка расход"); flow_count=0;  wtTODO = 0;delay(5000); return;break;}
                          delay(20);
                          break;

              }
      case 2:
              {
                        
                // вкл 2


                while (joyButt==0) {readJoyButt();delay(20);}

                currOperation=3;
                flow_count = 0;
                flow_max = r2*koeff;
                if(flow_max<1){currOperation=4;break;}
                flow_max += flowDelay;
                alarm_count = 0;
                lcd1.setCursor(0, 0);
                lcd1.print("Наливается...");              
                lcd1.setCursor(2, 1);
                lcd1.print("Шаг2");

                /*POutput.digitalWrite(3, LOW); 
                digitalWrite(3+8, LOW);*/
                delay(500);
                POutput.digitalWrite(1, LOW); 
                digitalWrite(1+8, LOW);
                delay(500);
                POutput.digitalWrite(4, LOW); 
                digitalWrite(4+8, LOW);
                //delay(100);
                break;    

              }


       case 3:
              {

                          // выкл 2

                          alarm_count++;
                          //lcd2.setCursor(2, 0);
                          //lcd2.print(flow_count);
                          if(flow_count>=flow_max) {currOperation=4; closeAll(); break;}
                          if(alarm_count>=1000) {currOperation=4; closeAll(); break;} //{currOperation=2;closeAll();lcd2.setCursor(2, 0);lcd2.print("Ошибка расход"); flow_count=0;  wtTODO = 0;delay(5000); return;break;}
                          delay(20);
                          break;

              }

      case 4:
              {
                        

                // вкл 3

                while (joyButt==0) {readJoyButt();delay(20);}

                currOperation=5;
                flow_count = 0;
                flow_max = r3 * koeff;
                if(flow_max<1){currOperation=6;break;}
                flow_max += flowDelay;
                alarm_count = 0;
                lcd1.setCursor(0, 0);
                lcd1.print("Наливается...");              
                lcd1.setCursor(2, 1);
                lcd1.print("Шаг4");

                /*POutput.digitalWrite(3, LOW); 
                digitalWrite(3+8, LOW);
                delay(500);*/
                POutput.digitalWrite(2, LOW); 
                digitalWrite(2+8, LOW);
                delay(500);
                POutput.digitalWrite(4, LOW); 
                digitalWrite(4+8, LOW);
                //delay(100);
                break;    

              }


       case 5:
              {
                          alarm_count++;
                          //lcd2.setCursor(2, 0);
                          //lcd2.print(flow_count);
                          if(flow_count>=flow_max) {currOperation=6; closeAll(); break;}
                          if(alarm_count>=1000){currOperation=6; closeAll(); break;} // {currOperation=2;closeAll();lcd2.setCursor(2, 0);lcd2.print("Ошибка расход"); flow_count=0;  wtTODO = 0;delay(5000); return;break;}
                          delay(20);
                          break;

              }


      case 6:
              {
                        
                currOperation=7;
                
                lcd1.setCursor(0, 0);
                lcd1.print("Диспенсер...");              
                lcd1.setCursor(2, 1);
                lcd1.print("Шаг6");
                //POutput.digitalWrite(3, LOW); 

                delay(1000);
                //POutput.digitalWrite(7, LOW); 

                //POutput.digitalWrite(3, HIGH); 

                //delay(100);
                break;    

              }


       case 7:
              {
                         // delay(5000);

                          currOperation=0; 
                          closeAll();  
                          flow_count=0; // wtTODO = 0;
                          delay(1000); 
                          return;
                          break;
                          
              }



      default:
              {
                        
//                          wtTODO = 0;
                          return;
                          break;

              }



      }

  }while(true);



}


void sysNalivStart()
{

// Надо поочерёдно налить в каждый стакан

// Двигаем носик по очереди в каждый стакан. Проверяем, есть ли он там. Если есть, то дальше.
// По выбранному меню для этого стакана трижды поочерёдно открываем соответствующий клапан. Включаем насос. Ждём налива, выключаем всё
// Пауза. Потом открываем клапан на носике


// Подвинуть на точку 0
// Если стакан там есть и он не 0, то 

/*nosikMove(-2000);

delay (2000);

nosikMove(-2000);

delay (2000);

nosikMove(-2000);

delay (2000);

nosikMove(6000);

*/

int sensorStatus;
 sensorStatus = PInput.digitalRead(3);
 if(sensorStatus == LOW)
 {

      switch(st1)
      {
        case 0: break;
        case 1: nalitStakan(rec1[0], rec1[1], rec1[2]); break;
        case 2: nalitStakan(rec2[0], rec2[1], rec2[2]); break;
        case 3: nalitStakan(rec3[0], rec3[1], rec3[2]); break;
        case 4: nalitStakan(rec4[0], rec4[1], rec4[2]); break;
        case 5: nalitStakan(rec5[0], rec5[1], rec5[2]); break;
        case 6: nalitStakan(rec6[0], rec6[1], rec6[2]); break;
        default: break;
      };
 }

// Подвинуть на точку 1
// Если стакан там есть и он не 0, то 

nosikMove(specM);

sensorStatus = PInput.digitalRead(2);
 if(sensorStatus == LOW)
 {

switch(st2)
{
  case 0: break;
  case 1: nalitStakan(rec1[0], rec1[1], rec1[2]); break;
  case 2: nalitStakan(rec2[0], rec2[1], rec2[2]); break;
  case 3: nalitStakan(rec3[0], rec3[1], rec3[2]); break;
  case 4: nalitStakan(rec4[0], rec4[1], rec4[2]); break;
  case 5: nalitStakan(rec5[0], rec5[1], rec5[2]); break;
  case 6: nalitStakan(rec6[0], rec6[1], rec6[2]); break;
  default: break;
};

 }

// Подвинуть на точку 2
// Если стакан там есть и он не 0, то 

nosikMove(specM);


sensorStatus = PInput.digitalRead(1);
 if(sensorStatus == LOW)
 {

switch(st3)
{
  case 0: break;
  case 1: nalitStakan(rec1[0], rec1[1], rec1[2]); break;
  case 2: nalitStakan(rec2[0], rec2[1], rec2[2]); break;
  case 3: nalitStakan(rec3[0], rec3[1], rec3[2]); break;
  case 4: nalitStakan(rec4[0], rec4[1], rec4[2]); break;
  case 5: nalitStakan(rec5[0], rec5[1], rec5[2]); break;
  case 6: nalitStakan(rec6[0], rec6[1], rec6[2]); break;
  default: break;
};
 }
// Подвинуть на точку 3
// Если стакан там есть и он не 0, то 

nosikMove(specM);

sensorStatus = PInput.digitalRead(0);
 if(sensorStatus == LOW)
 {
switch(st4)
{
  case 0: break;
  case 1: nalitStakan(rec1[0], rec1[1], rec1[2]); break;
  case 2: nalitStakan(rec2[0], rec2[1], rec2[2]); break;
  case 3: nalitStakan(rec3[0], rec3[1], rec3[2]); break;
  case 4: nalitStakan(rec4[0], rec4[1], rec4[2]); break;
  case 5: nalitStakan(rec5[0], rec5[1], rec5[2]); break;
  case 6: nalitStakan(rec6[0], rec6[1], rec6[2]); break;
  default: break;
};
 }
// Подвинуть на точку 0 !!!!!!!
// угловой двигатель дёргается при включении !!!
nosikMove(1);


wtTODO = 0;


}

void sysMooveStart() 
{

 int sensorStatus = 0;

lcd2.setCursor(0,1);lcd2.print("...1...");
nosikMove(4*specM);

do { readJoyButt();   
     sensorStatus = PInput.digitalRead(3);
     delay(20);
  }  while (joyButt==0 && sensorStatus == LOW);

delay(2000);
  
lcd2.setCursor(0,1);lcd2.print("...2...");
nosikMove(specM);

delay(2000);

do { readJoyButt();   
     sensorStatus = PInput.digitalRead(2);
     delay(20);
  }  while (joyButt==0 && sensorStatus == LOW);


lcd2.setCursor(0,1);lcd2.print("...3...");
nosikMove(specM);
delay(1000);


do { readJoyButt();   
     sensorStatus = PInput.digitalRead(1);
     delay(20);
  }  while (joyButt==0 && sensorStatus == LOW);


lcd2.setCursor(0,1);lcd2.print("...4...");
nosikMove(specM);
delay(2000);

do { readJoyButt();   
     sensorStatus = PInput.digitalRead(0);
     delay(20);
  }  while (joyButt==0 && sensorStatus == LOW);



nosikMove(1);

wtTODO = 0;


}


////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////                 ////////////////////////////////////////////////////
/////////////////////////////                   ///////////////////////////////////////////////////
/////////////////////////////       ///////////////////////////////////////////////////////////////
/////////////////////////////       ///////////////////////////////////////////////////////////////
////////////////////////////                 ///////////////////////////////////////////////////////
////////////////////////////                    ///////////////////////////////////////////////////
////////////////////////////       /////        ////////////////////////////////////////////////////
////////////////////////////       /////        ////////////////////////////////////////////////////
////////////////////////////                   /////////////////////////////////////////////////////
//////////////////////////////                 ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////




void drawMenu()
{

if (tmpMenuPosition == 0)
{

  lcd1.clear();
  lcd2.clear();

  switch(menuLevel)
  {
    case 0:
        lcd1.setCursor(0, 0);
        lcd1.print("Выбор ПАК ");

        lcd2.setCursor(2, 0);
        lcd2.print("1 = Налив ");
        lcd2.setCursor(2, 1);
        lcd2.print("2 = Выдача");

    break;

  case 1:
        lcd1.setCursor(0, 0);
        lcd1.print("ПАК 1. Изготовл.");

        lcd2.setCursor(2, 0);
        lcd2.print("1 = Меню  ");
        lcd2.setCursor(2, 1);
        lcd2.print("2 = Исполн");

        break;
        case 2:
            curStakan = 0;

            lcd1.setCursor(2, 0);lcd1.print("1ст = ");
            lcd1.setCursor(9, 0);lcd1.print(st1);
            lcd1.setCursor(2, 1);lcd1.print("2ст = ");
            lcd1.setCursor(9, 1);lcd1.print(st2);
            lcd2.setCursor(2, 0);lcd2.print("3ст = ");
            lcd2.setCursor(9, 0);lcd2.print(st3);

            lcd2.setCursor(2, 1);lcd2.print("4ст = ");
            lcd2.setCursor(9, 1);lcd2.print(st4);
        break;

        case 3:
            
            lcd1.setCursor(2, 0);lcd1.print("1.ГазиВода");
            lcd1.setCursor(2, 1);lcd1.print("2.МятСироп");
            lcd2.setCursor(2, 0);lcd2.print("3.Апельсок");
            lcd2.setCursor(2, 1);lcd2.print(">>Следующие");
            
        break;

        case 4:
            
            lcd1.setCursor(2, 0);lcd1.print("4.Л_Мятн");
            lcd1.setCursor(2, 1);lcd1.print("5.Л_ЗАпел");
            lcd2.setCursor(2, 0);lcd2.print("6.Л_Тройн");
            lcd2.setCursor(2, 1);lcd2.print(">>Следующие");
            
        break;


  }
  tmpMenuPosition = 100;
}

  readJoyButt();
  
  switch (joyButt)
  {
      case 0:
         delay(10);
      break;

      case 1:
        tmpMenuPosition+=1;
        if(menuLevel==0&&tmpMenuPosition>2)tmpMenuPosition=1;
        if(menuLevel==1&&tmpMenuPosition>2)tmpMenuPosition=1;
        if(menuLevel==2&&tmpMenuPosition>4)tmpMenuPosition=1;
        if(menuLevel==3&&tmpMenuPosition>4)tmpMenuPosition=1;
        if(menuLevel==4&&tmpMenuPosition>4)tmpMenuPosition=1;


        if(menuLevel==0 || menuLevel==1 )
        { 
            if(tmpMenuPosition==1) {lcd2.setCursor(0, 0); lcd2.print("+");lcd2.setCursor(0, 1); lcd2.print(" ");} 
            if(tmpMenuPosition==2) {lcd2.setCursor(0, 0); lcd2.print(" ");lcd2.setCursor(0, 1); lcd2.print("+");}
        }

          if(menuLevel==2 || menuLevel==3|| menuLevel==4)
        { 
          if(tmpMenuPosition==1) {lcd1.setCursor(0, 0); lcd1.print("*");lcd1.setCursor(0, 1); lcd1.print(" "); lcd2.setCursor(0, 0); lcd2.print(" ");lcd2.setCursor(0, 1); lcd2.print(" ");}
          if(tmpMenuPosition==2) {lcd1.setCursor(0, 0); lcd1.print(" ");lcd1.setCursor(0, 1); lcd1.print("*"); lcd2.setCursor(0, 0); lcd2.print(" ");lcd2.setCursor(0, 1); lcd2.print(" ");}
          if(tmpMenuPosition==3) {lcd1.setCursor(0, 0); lcd1.print(" ");lcd1.setCursor(0, 1); lcd1.print(" "); lcd2.setCursor(0, 0); lcd2.print("*");lcd2.setCursor(0, 1); lcd2.print(" ");}
          if(tmpMenuPosition==4) {lcd1.setCursor(0, 0); lcd1.print(" ");lcd1.setCursor(0, 1); lcd1.print(" "); lcd2.setCursor(0, 0); lcd2.print(" ");lcd2.setCursor(0, 1); lcd2.print("*");}
        }


      break;

      case 2:
        tmpMenuPosition-=1;
        if(menuLevel==0&&tmpMenuPosition<1)tmpMenuPosition=2;
        if(menuLevel==1&&tmpMenuPosition<1)tmpMenuPosition=2;
        if(menuLevel==2&&tmpMenuPosition<1)tmpMenuPosition=4;
        if(menuLevel==3&&tmpMenuPosition<1)tmpMenuPosition=4;
        if(menuLevel==4&&tmpMenuPosition<1)tmpMenuPosition=4;
        if(tmpMenuPosition<1)tmpMenuPosition=1;

        if(menuLevel==0 || menuLevel==1)
        { 
            if(tmpMenuPosition==1) {lcd2.setCursor(0, 0); lcd2.print("+");lcd2.setCursor(0, 1); lcd2.print(" ");} 
            if(tmpMenuPosition==2) {lcd2.setCursor(0, 0); lcd2.print(" ");lcd2.setCursor(0, 1); lcd2.print("+");}
        }

        if(menuLevel==2 || menuLevel==3|| menuLevel==4)
        { 
          if(tmpMenuPosition==1) {lcd1.setCursor(0, 0); lcd1.print("*");lcd1.setCursor(0, 1); lcd1.print(" "); lcd2.setCursor(0, 0); lcd2.print(" ");lcd2.setCursor(0, 1); lcd2.print(" ");}
          if(tmpMenuPosition==2) {lcd1.setCursor(0, 0); lcd1.print(" ");lcd1.setCursor(0, 1); lcd1.print("*"); lcd2.setCursor(0, 0); lcd2.print(" ");lcd2.setCursor(0, 1); lcd2.print(" ");}
          if(tmpMenuPosition==3) {lcd1.setCursor(0, 0); lcd1.print(" ");lcd1.setCursor(0, 1); lcd1.print(" "); lcd2.setCursor(0, 0); lcd2.print("*");lcd2.setCursor(0, 1); lcd2.print(" ");}
          if(tmpMenuPosition==4) {lcd1.setCursor(0, 0); lcd1.print(" ");lcd1.setCursor(0, 1); lcd1.print(" "); lcd2.setCursor(0, 0); lcd2.print(" ");lcd2.setCursor(0, 1); lcd2.print("*");}
        }


      break;

      case 10:

      menuPosition = tmpMenuPosition;
      switch(menuLevel) 
          {
              case 0:
              if (menuPosition==1)
              {menuLevel = 1; tmpMenuPosition=0;}
              else
              {wtTODO = 2; lcd1.clear(); lcd2.clear(); lcd1.setCursor(0,0);lcd1.print("Auto: Выдача");currOperation=0;delay(1000);return;}
              break;
              case 1:
              if (menuPosition==1)
              {menuLevel = 2; tmpMenuPosition=0;}
              else
              {wtTODO = 1; lcd1.clear(); lcd2.clear(); lcd1.setCursor(0,0);lcd1.print("Auto: Налив ");currOperation=0;delay(1000);return;}
              break;
              case 2:
                curStakan = menuPosition; 
                menuLevel = 3;
                tmpMenuPosition=0;

              break;

              case 3:
                  
                 if(menuPosition == 1||menuPosition == 2||menuPosition == 3)
                 { 
                  switch(curStakan)
                    {
                        case 1: st1 = menuPosition; break;
                        case 2: st2 = menuPosition; break;
                        case 3: st3 = menuPosition; break;
                        case 4: st4 = menuPosition; break;
                        default:st4 = curStakan; break;
                    }
                 menuLevel = 2;    
                 } else
                 {menuLevel = 4;};  

                
              tmpMenuPosition=0;
              
              break;

              case 4:
                  
                 if(menuPosition == 1||menuPosition == 2||menuPosition == 3)
                 { 
                  switch(curStakan)
                    {
                        case 1: st1 = menuPosition+3; break;
                        case 2: st2 = menuPosition+3; break;
                        case 3: st3 = menuPosition+3; break;
                        case 4: st4 = menuPosition+3; break;
                        default:st4 = curStakan; break;
                    }
                 menuLevel = 2;    
                 } else
                 {menuLevel = 3;};  

               
              tmpMenuPosition=0;
              
              break;

             
          }



      /*
      if (menuPosition == 1) sysNaliv = 1;
      if (menuPosition == 2) sysMoove = 1;   */
        menuPosition = 0;
        tmpMenuPosition = 0;
        break;

       case 200:
                
                switch(menuLevel)
                {
                      case 0: break;
                      case 1: menuLevel=0; break;
                      case 2: menuLevel=1; break;
                      case 3: menuLevel=2; break;
                      case 4: menuLevel=2; break;
                      default:     menuLevel=0; break;                  
                }
                
                
                
                if (menuLevel < 0) menuLevel = 0;
                delay(500);
                tmpMenuPosition = 0;
        break;


  }


}

void loop() {
  // put your main code here, to run repeatedly:



  if (wtTODO == 0)
        drawMenu();

  

  if (wtTODO == 1)
  {

    //lcd1.setCursor(0,0);lcd1.print("InLoop: Налив");
    //delay(1000);
        //currOperation=0;
        sysNalivStart();

        tmpMenuPosition=0;
        wtTODO = 0;

  }
  else;
  //closeAll();

  
  if (wtTODO == 2)
     {     
         lcd1.clear();
         lcd2.clear();
         lcd1.setCursor(0,1);lcd1.print("Выдача");
         sysMooveStart();
         tmpMenuPosition=0;
         wtTODO = 0;

         delay(100);

     }

}


// 88888888888888888888888888888888888888888888888888888888888888888
// 8888888888888888888888             888888888888888888888888888888
// 8888888888888888888      8888888       88888888888888888888888888
// 8888888888888888888      8888888       88888888888888888888888888
// 88888888888888888888                 8888888888888888888888888888
// 888888888888888888       888888888       888888888888888888888888
// 888888888888888888        88888888      8888888888888888888888888
// 8888888888888888888888              88888888888888888888888888888
// 88888888888888888888888888888888888888888888888888888888888888888
// 88888888888888888888888888888888888888888888888888888888888888888
// 88888888888888888888888888888888888888888888888888888888888888888















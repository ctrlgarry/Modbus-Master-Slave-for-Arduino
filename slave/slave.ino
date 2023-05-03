//RS-485 Modbus Slave (Arduino UNO)
//Circuit Digest
#include<ModbusRtu.h>       //Library for using Modbus in Arduino
#include<LiquidCrystal.h>   //Library for using 16x2 LCD display
#include <Servo.h>          //Library for using Servo Motor
#define led1 2              //Define as 2 led1 
#define led2 5              //Define as 5 led2
LiquidCrystal lcd(8,9,10,11,12,13);   //initizlize lcd object with pins (RS,E,D4,D5,D6,D7) for class liquid crystal
Servo servo;                          //Initilize servo object for class Servo
Modbus bus;                          //Define Object bus for class modbus 
uint16_t modbus_array[] = {0,0,0};    //первоначально в массив записываем нулевые значения
                      
void setup()
{
  lcd.begin(16,2);                //режим 16x2 для ЖК дисплея
  lcd.print("RS-485 Modbus");     //приветственное сообщение
  lcd.setCursor(0,1); 
  lcd.print("Arduino Slave");
  delay(5000);
  lcd.clear();
  
  pinMode(led1,OUTPUT);           //Led1 на вывод данных
  pinMode(led2,OUTPUT);           //Led2 на вывод данных
  servo.attach(6);                //Servo PWM pin 6 (контакт, к которому подключен сервомотор)
  bus = Modbus(1,1,4);            //Modbus slave ID as 1 and 1 connected via RS-485 and 4 connected to DE & RE pin of RS-485 Module
//адрес ведомого равен 1, используется связь через интерфейс RS-485 (вторая 1), 4 – номер контакта Arduino, к которому подключены контакты DE & RE модуля RS-485
  bus.begin(9600);                //Modbus slave baudrate at 9600 (скорость 9600 бод)
}
void loop()
{
   bus.poll(modbus_array,sizeof(modbus_array)/sizeof(modbus_array[0]));       //Used to receive or write value from Master 
  
  if (modbus_array[0] == 0)    //Depends upon value in modubus_array[0] written by Master Modbus
  {
    digitalWrite(led1,LOW);    //выключаем первый светодиод
    lcd.setCursor(0,0);
    lcd.print("L1:OFF");
  }
  else
  {
     digitalWrite(led1,HIGH);  // включаем первый светодиод
     lcd.setCursor(0,0);
     lcd.print("L1:ON");        
  } 
 if (modbus_array[1] == 0)    //Depends upon value in modbus_array[1] written by Master Modbus
  {
    digitalWrite(led2,LOW);   //LED OFF if 0
    lcd.setCursor(8,0);
    lcd.print("L2:OFF");
  }
  else
  {
     digitalWrite(led2,HIGH);  //LED ON if value other than 0
     lcd.setCursor(9,0);
     lcd.print("L2:ON");
  }
  
  int pwm = modbus_array[2]; //Depends upon value in modbus_array[1] written by Master Modbus
 
  servo.write(pwm);          // поворачиваем сервомотор на угол pwm (от 0 до 180 градусов), принятый  от Modbus Master
  lcd.setCursor(0,1);        
  lcd.print("Servo angle:");
  lcd.print(pwm);              //выводим значение угла на экран ЖК дисплея 16х2
  delay(200); 
  lcd.clear();
}
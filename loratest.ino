int incomingByte = 0; // for incoming serial data
unsigned long timer;
int INTERVAL = 180000; //定時更新
int INTERVAL2 = 2000; //每隔一段時間觀察淹水狀態
char lora_string[100];
int id = 0001;
int times = 0;
int level = 0; //原淹水等級
int level2 = -1;//改變的淹水等級
int level3 = -1;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(115200);

  pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  pinMode(12, OUTPUT);  //for rest EK-S76SXB
  digitalWrite(12, LOW);
  delay(100);
  digitalWrite(12, HIGH);
  delay(2000);

  
  delay(5000);
  
    while (Serial1.available() > 0) {
      // read the incoming byte:
      incomingByte = Serial1.read();

      // say what you got:
      //Serial.print("I received: ");
      //Serial.println(incomingByte, DEC);
      Serial.write(incomingByte);
    }
    Serial.println();
    Serial1.print("mac join abp");  //join mode of LoRaWAN (use abp)
    Serial.println("mac join abp");

    timer = millis();

}

void loop() {
  // put your main code here, to run repeatedly:

    //ucnf.toCharArray(ucnfch,30);

    int sensor1 = analogRead(A0);
    int sensor2 = analogRead(A1);
    int sensor3 = analogRead(A2);

    
  
    while (Serial1.available() > 0) {
      // read the incoming byte:
      incomingByte = Serial1.read();

      // say what you got:
      //Serial.print("I received: ");
      //Serial.println(incomingByte, DEC);
      Serial.write(incomingByte);
    }
    
     
     if( sensor1 >= 3000 && sensor2 >= 3000 && sensor3 >= 3000 )
      level = 0;
     else if( 100 <= sensor1 && sensor1 < 3000 && sensor2 >= 3000 && sensor3 >= 3000 )
      level = 1;
     else if( 100 <= sensor1 && sensor1 < 3000 && 100 <= sensor2 && sensor2 < 3000 && sensor3 >= 3000 )
      level = 2;
     else if( 100 <= sensor1 && sensor1 < 3000 && 100 <= sensor2 && sensor2 < 3000 && 100 <= sensor3 && sensor3 < 3000 )
      level = 3;
     else
      level = 4;

      
      
      if(level2 == level || times == 0 && level != level3){ 
        if(millis() >= timer + INTERVAL2){
          timer += INTERVAL2;
            times ++;  
            Serial.print("times：");
            Serial.println(times); 
            
            Serial.print("原淹水狀態："); 
            Serial.println(level); 

            Serial.print("改變後："); 
            Serial.println(level2); 

            Serial.print("存放：");
            Serial.println(level3);
            
            Serial.print("sensor1：");
            Serial.println(sensor1);

            Serial.print("sensor2：");
            Serial.println(sensor2);
            
            Serial.print("sensor3：");
            Serial.println(sensor3);
          }
      }
      else{
            level2 = level;
            times = 0;
          }

       if(times == 5){
          if(level == 4){
              sprintf(lora_string,"mac tx ucnf 2 %04d%02d%02d",id, 4 ,0);
           }
          else{
              sprintf(lora_string,"mac tx ucnf 2 %04d%02d",id, level);
           }
            
            Serial1.print(lora_string);  //tx now
            Serial.println(lora_string); 
            level3 = level2;
            times = 6;
         }

         if(millis() >= timer + INTERVAL){
          timer += INTERVAL; 
            sprintf(lora_string,"mac tx ucnf 2 %04d%02d",id, level);
          }
     
}

#include "mbed.h"
Serial pc( USBTX, USBRX );
DigitalIn  Switch(SW3);
DigitalOut redLED(LED1);
DigitalOut greenLED(LED2);
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
char table[11] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00};
char tableP[11]= {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF, 0x00};
AnalogOut Aout(DAC0_OUT);
AnalogIn Ain(A0);

int main(){
  float a=0,b=0,c=0;
  int count=0;
  int i=0;
  int flag=0;

    for(i=0;i<430;i++){
        a = Ain;
        pc.printf("%1.3f\r\n", a);
        wait(1./430);
    }  
  while(1){
  //  Aout = Ain;

    float k;
    if(count!=0){
        for( k=0; k<2; k+=0.05){
            Aout = 0.5 + 0.5*sin(k*3.1415926);
            wait(1./(count*50));
        }
    }

    if( Switch == 1 ){
      greenLED = 0;
      redLED = 1;
      display = table[10];
    }
    else{

        redLED = 0;
        greenLED = 1;
        if(flag==0){
            int initial=0;
            for (i=0; i < 500; i++){
                b=a;
                a = Ain;
              //  pc.printf("%1.3f\r\n", a);
                wait(0.002);
                
                if(initial!=2){
                    flag=(a>b?2:3);
                    initial++;
                }
                if(a>b && flag==3){
                    count++;
                    flag=2;
                }
                if(a<b && flag==2){
                    count++;
                    flag=3;
                }
            }
            count/=2;
            flag=1;
        }
        
        display = table[count/100];
        wait(0.8);
        display = table[(count%100)/10];
        wait(0.8);
        display = tableP[count%10];
        wait(0.8);
    }
  }
}

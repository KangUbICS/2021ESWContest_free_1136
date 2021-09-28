#include <avr/pgmspace.h>

#include <avr/interrupt.h> 

#include <avr/io.h>

 

int A = 14;

int B = 15;

int C = 16;

int D = 17;

int Data_Red = 18;

int Data_Green = 19;

int Clk = 20;

int LE = 21;

int OE = 2;

const int __attribute__((progmem)) string[][16]={

  0x00, 0x60, 0x18f0, 0x1998, 0x1b0c, 0x7b0c, 0x7b0c, 0x1b0c, 0x1998, 0x18f0, 0x60, 0x0, 0xc0, 0xc0, 0x1fc0, 0x1fc0,  //안

  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

  0x00, 0x40, 0x30e0, 0x31f0, 0x31b0, 0x3318, 0x3318, 0x360c, 0x360c, 0x3000, 0x00, 0x1fc0, 0x18c0, 0x1fc0, 0x1fc0, //심

  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

  0x00, 0x60, 0x60, 0x3fc, 0x1bfc, 0x1800, 0x18f0, 0x19f8, 0x7b9c, 0x7b0c, 0x1b0c, 0x1b0c, 0x1b9c, 0x19f8, 0xf0, 0x00, //하

  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

  0x00, 0x00, 0xd810, 0xd838, 0xd838, 0xd86c, 0xd86c, 0xdec6, 0xdec6, 0xd8c6, 0xd983, 0xd983, 0xd983, 0x00, 0x00, 0x00, //세

  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

  0x00, 0x3e0, 0xff8, 0xc18, 0x180c, 0x180c, 0x180c, 0xc18, 0xff8, 0x7f0, 0x360, 0x360, 0x3ffe, 0x3ffe, 0x00, 0x00, //요

  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

};

 

unsigned char Dot_char_cnt=0;//도트매트릭스에 표시 될 문자 카운팅!

unsigned char flag_cnt=0; //도트매트릭스에 표시 될 색상 카운팅!

unsigned char move_motion=0;   

unsigned int Move_cnt=16;

unsigned int Move_cnt2=0;

unsigned int str_speed_cnt=0;

bool flag_Oe=0;

//타이머 인터럽트 함수

ISR(TIMER2_OVF_vect){  

  static bool output = HIGH; 

  digitalWrite(5, OUTPUT);

  output = !output;

  static unsigned int string_moving_speed_value=500;

  static unsigned int cnt=0;

  if(flag_Oe)

  {

    digitalWrite(OE, LOW);

    cnt++;

    if(cnt>=0)

    {

     flag_Oe=0;

      cnt=0;

      digitalWrite(OE, HIGH);

    }

  }

  

  str_speed_cnt++;

  if(str_speed_cnt>string_moving_speed_value) //500ms 문자가 머문 후 시프트 시작!

  {

    str_speed_cnt=0;

    Move_cnt--;

    Move_cnt2++;

    if(Move_cnt==0||Move_cnt2==16){

      Move_cnt=16;

      Move_cnt2=0;

      move_motion++;

      if(move_motion>1){

        move_motion=0;

        Dot_char_cnt+=2;      

        flag_cnt++;

        if(flag_cnt==3)

        {

          flag_cnt=0; 

        }

      }

    }

  }

  TCNT2=0xe7;

  

  

} 

void setup() {

  pinMode(A, OUTPUT);

  pinMode(B, OUTPUT);

  pinMode(C, OUTPUT);

  pinMode(D, OUTPUT);

  pinMode(Data_Red, OUTPUT);

  pinMode(Data_Green, OUTPUT);

  pinMode(Clk, OUTPUT);

  pinMode(LE, OUTPUT);

  pinMode(OE, OUTPUT);

  pinMode(1, OUTPUT);

  

  TCCR2A=0x00;

  TCCR2B=0x04;

  TIMSK2=0x01;

  TCNT2=0xe7;

  SREG=0x80;

}

void row_dynamic(unsigned char str_cnt) //행의 어드레스 접근

{

    switch(str_cnt)//ROW SHIFT!

    {

    case 0:digitalWrite(A, LOW); digitalWrite(B, LOW); digitalWrite(C, LOW); digitalWrite(D, LOW); break;         //1행 LED

    case 1:digitalWrite(A, HIGH); digitalWrite(B, LOW); digitalWrite(C, LOW); digitalWrite(D, LOW); break;        //2행 LED

    case 2:digitalWrite(A, LOW); digitalWrite(B, HIGH); digitalWrite(C, LOW); digitalWrite(D, LOW); break;        //3행 LED

    case 3:digitalWrite(A, HIGH); digitalWrite(B, HIGH); digitalWrite(C, LOW); digitalWrite(D, LOW); break;       //4행 LED

    case 4:digitalWrite(A, LOW); digitalWrite(B, LOW); digitalWrite(C, HIGH); digitalWrite(D, LOW); break;        //5행 LED

    case 5:digitalWrite(A, HIGH); digitalWrite(B, LOW); digitalWrite(C, HIGH); digitalWrite(D, LOW); break;       //6행 LED

    case 6:digitalWrite(A, LOW); digitalWrite(B, HIGH); digitalWrite(C, HIGH); digitalWrite(D, LOW); break;       //7행 LED

    case 7:digitalWrite(A, HIGH); digitalWrite(B, HIGH); digitalWrite(C, HIGH); digitalWrite(D, LOW); break;      //8행 LED

    case 8:digitalWrite(A, LOW); digitalWrite(B, LOW); digitalWrite(C, LOW); digitalWrite(D, HIGH); break;        //9행 LED

    case 9:digitalWrite(A, HIGH); digitalWrite(B, LOW); digitalWrite(C, LOW); digitalWrite(D, HIGH); break;       //10행 LED

    case 10:digitalWrite(A, LOW); digitalWrite(B, HIGH); digitalWrite(C, LOW); digitalWrite(D, HIGH); break;      //11행 LED

    case 11:digitalWrite(A, HIGH); digitalWrite(B, HIGH); digitalWrite(C, LOW); digitalWrite(D, HIGH); break;     //12행 LED

    case 12:digitalWrite(A, LOW); digitalWrite(B, LOW); digitalWrite(C, HIGH); digitalWrite(D, HIGH); break;      //13행 LED

    case 13:digitalWrite(A, HIGH); digitalWrite(B, LOW); digitalWrite(C, HIGH); digitalWrite(D, HIGH); break;     //14행 LED

    case 14:digitalWrite(A, LOW); digitalWrite(B, HIGH); digitalWrite(C, HIGH); digitalWrite(D, HIGH); break;     //15행 LED

    case 15:digitalWrite(A, HIGH); digitalWrite(B, HIGH); digitalWrite(C, HIGH); digitalWrite(D, HIGH); break;    //16행 LED

    }     

}

void shift_Register(unsigned char out) //데이터 입력함수

{

  unsigned char clk=0;          

  for(clk=0;clk<8;clk++){ //8비트 데이터를 1비트씩 시프트 레지스터에 입력        

    if(out&(0x80>>clk))

      {

        switch(flag_cnt){

         case 0:digitalWrite(Data_Green, LOW);digitalWrite(Data_Red, HIGH);break;    //Red

         case 1:digitalWrite(Data_Green, HIGH);digitalWrite(Data_Red, LOW);break;    //Green

         case 2:digitalWrite(Data_Green, HIGH);digitalWrite(Data_Red, HIGH);break;   //Orange

        }   

        digitalWrite(Data_Green, LOW);digitalWrite(Data_Red, HIGH);

      } 

     else

     {

       digitalWrite(Data_Green, LOW);digitalWrite(Data_Red, LOW);

     }  

        digitalWrite(Clk, HIGH);

        digitalWrite(Clk, LOW); 

    }  

}

void ActivePulse()

{    

  digitalWrite(LE, HIGH); 

  digitalWrite(LE, LOW); //래치 출력

  digitalWrite(OE, LOW);

 flag_Oe=1;

}

void dot1_display_shift(unsigned char first)

{

  static unsigned int i_cnt=0;    

  unsigned int buff1[16]={0};       //Dot1 in

  unsigned int buff5[16]={0};       //Dot1 out        

  unsigned char high1=0;            //Dot1

  unsigned char low1=0;             //Dot1            

  register unsigned int i=0;

  

  for(i_cnt=0;i_cnt<16;i_cnt++){     //좌에서 우로 시프트          

      if(move_motion==0)  

      {  

        buff1[i_cnt]=pgm_read_word(&string[first][i_cnt])<<Move_cnt;  // 1st String Dot in

      }    

      if(move_motion==1)  

      {  

        buff1[i_cnt]=pgm_read_word(&string[first][i_cnt])>>Move_cnt2;  // 1st String Dot out

      }       

  }                  

  for(i=0;i<16;i++)

    {

      buff5[i]|=buff1[i]; 

      high1=(buff5[i]>>8);

      low1=(buff5[i]&0xff);                 

      shift_Register(high1);   

      shift_Register(low1);                                                  

      row_dynamic(i);

      ActivePulse();

    }

}

void loop() {

  dot1_display_shift(Dot_char_cnt); 

}

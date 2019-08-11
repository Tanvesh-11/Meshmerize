// p 1.4, d  2.35; p =1.4, d = 0.006 i=6;d=10=0.008;
//float kp = 1.4;
//float ki = 0.007;
//float kd = 20;

int value = 0;
String binary = "00000000";
const int l1 = 4, l2 = 5, pl = 6, r1 = 8, r2 = 7, pr = 9, Junction_Pulse = 3, UART_EN = 2;
float x, y, error_pwm = 0, error_sum = 0, previous_error = 0;

float p_error = 0, i_error = 0, d_error = 0, total_error = 0;
float average_pwm = 100, left_pwm = 0, right_pwm = 0;

float kp = 1.4;
float ki = 0.007;
float kd = 25;
int i = 0, array_size = 20;
int sum[20];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(l1, OUTPUT);
  pinMode(l2, OUTPUT);
  pinMode(pl, OUTPUT);
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(pr, OUTPUT);
  pinMode(Junction_Pulse, INPUT);
  pinMode(UART_EN, OUTPUT);
  for (int i = 0; i < array_size; i++)
  {
    sum[i] = 0;
  }
  digitalWrite(UART_EN, HIGH);
}

void loop() {
  // Uncomment for UART Digital Output..
  /*
   digitalWrite(2,LOW);
  delay(1);
if(Serial.available()>0)
{
  value=Serial.read();

}
digitalWrite(2,HIGH);
binary = String(value,BIN);
int zeros=String(value,BIN).length();
String myString;
for(int i=0;i<(8-zeros);i++)
{
  myString=myString+"0";
}
Serial.println(value);
Serial.print("          ");
Serial.println(myString+binary);     //values would be from right to left..
*/
  x = 0;
  for (int k = 0; k <= 5; k++)
  {
    x = analogRead(A0) + x;
  }
  x = x / 6;
  y = map(x, 20, 670, 0, 70);
  error_pwm = y - 35;
  sum[i] = error_pwm;
  error_sum = 0;
  for (int n = 0; n < array_size; n++)
  {
    error_sum = sum[n] + error_sum;
  }

  p_error = kp * error_pwm;
  i_error = ki * error_sum;
  d_error = kd * (error_pwm - previous_error);
  total_error = p_error + i_error + d_error;
  previous_error = error_pwm;
  left_pwm = (average_pwm + total_error);
  right_pwm = (average_pwm - total_error);
  left_pwm = constrain(left_pwm, 0, 255);
  right_pwm = constrain(right_pwm, 0, 255);
  analogWrite(pl, left_pwm);
  analogWrite(pr, right_pwm);

  digitalWrite(l1, HIGH);
  digitalWrite(l2, LOW);
  digitalWrite(r1, HIGH);
  digitalWrite(r2, LOW);

  //  Serial.print(error_pwm); //1
  //  Serial.print("         ");
  //  Serial.print(p_error);     //2
  //  Serial.print("         ");
  //  Serial.print(d_error);        //3
  //  Serial.print("         ");
  //  Serial.print(i_error);       //4
  //  Serial.print("         ");
  //  Serial.print(error_sum);
  //  Serial.print("         ");
  Serial.println(total_error);    //5
  //  Serial.print("         ");
  //  Serial.print(left_pwm);        //6
  //  Serial.print("         ");
  //  Serial.println(right_pwm);     //7
  i++;
  if (i > array_size)
  {
    i = 0;
  }

}

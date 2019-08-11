int value = 0;
String binary = "00000000";
const int l1 = 4, l2 = 5, pl = 6, r1 = 8, r2 = 7, pr = 9, Junction_Pulse = 3, UART_EN = 2;
float x, y, error_pwm = 0, error_sum = 0, previous_error = 0;
const int left_sensor = 10, right_sensor = 11, middle_sensor = 12 , lsa_2 = A5, lsa_4 = A4, lsa_5 = A3, lsa_7 = A2 ,lsa_6 =A1;
int Junction = 0;
float p_error = 0, i_error = 0, d_error = 0, total_error = 0;
float average_pwm = 100, left_pwm = 0, right_pwm = 0, rotational_pwm = 70;
boolean lsv = 0, rsv = 0, msv = 0, lsa_2v = 0, lsa_4v = 0, lsa_5v = 0, lsa_7v = 0 , lsa_6v=0;      //left_sensor_value(lsv)
const int switchh=2;
float kp = 1.4;
float ki = 0.008;
float kd = 20;
int i = 0, array_size = 20;
int sum[20];
String stringone, stringtwo;
int a;
int Led=13;
int count;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Starting...");
  pinMode(l1, OUTPUT);
  pinMode(l2, OUTPUT);
  pinMode(pl, OUTPUT);
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(pr, OUTPUT);
  pinMode(Junction_Pulse, INPUT);
  pinMode(UART_EN, OUTPUT);
  pinMode(left_sensor, INPUT);
  pinMode(right_sensor, INPUT);
  pinMode(middle_sensor, INPUT);
  pinMode(lsa_2, INPUT);
  pinMode(lsa_4, INPUT);
  pinMode(lsa_5, INPUT);
  pinMode(lsa_7, INPUT);
  pinMode(lsa_6, INPUT);
  pinMode(Led, OUTPUT);
  
  for (int i = 0; i < array_size; i++)
  {
    sum[i] = 0;
  }
  digitalWrite(UART_EN, HIGH);

}

void loop() {

  go_straight();
  a = analogRead(A0);
  //    Serial.println(a);

  if (digitalRead(Junction_Pulse) == HIGH || analogRead(A0) > 700)
  {
    Stop();
    delay(70);
    read_sensor_values();
    Junction = detect_Junction();
    if (Junction == 1 || Junction == 3 || Junction == 4)
    {
      Slight_left();
    }

    else if (Junction == 6 || Junction == 5)
    {
      Slight_left();
    }
    else if (/*analogRead(A0) > 690 ||*/ Junction == 7)
    {
      U_right();
    }
    else if (Junction == 2)
    {
      sharp_right();
    }
    else if (Junction == 8)
    {
      Straight();
    }
      else if (Junction ==9)
    {
     Stop();
     delay(20);
     St();
     delay(500);
     digitalWrite(Led,HIGH);
     Serial.println("STOOPPP");
     Stop();
     delay(20000);
     Short_path();
    
  }
}

  Serial.println(stringone);
}

void Stop()
{
  //  Serial.println("STOP");
  digitalWrite(l1, LOW);
  digitalWrite(l2, LOW);
  digitalWrite(r1, LOW);
  digitalWrite(r2, LOW);
  delay(5);
}

int detect_Junction()
{
  //  Serial.println("identifying Junction");
  int i = 0;
  
  if (lsa_2v && lsa_4v && lsa_5v && lsa_7v && rsv && lsv )      //T junction
  {
    i = 1;
  }
   else if(lsa_2v & lsa_4v  && lsa_5v && lsa_7v && !lsv && !msv && !rsv)
  {
    i=9;
    
  }
 
  else if (!lsa_2v  &&( lsa_5v || lsa_7v) && lsa_6v &&  lsv && msv && rsv )        // Right
  {
    i = 2;
  }
  else if (lsa_2v && lsa_4v  && !lsa_7v && rsv)        // Left
  {
    i = 3;
  }
  else if (lsa_2v && lsa_4v  && !lsa_7v && !rsv )          // left and (right 135)
  {
    i = 4;
  }
  else if (!lsa_2v  && lsa_5v && lsa_7v && !lsv && rsv )         //right and (left 135)
  {
    i = 5;
  }
   else if ((!lsv && !rsv && msv) || ((digitalRead(Junction_Pulse) == HIGH) && (digitalRead(lsa_4) == LOW || digitalRead(lsa_5) == LOW  ) && (digitalRead(lsa_2) == HIGH || digitalRead(lsa_7) == HIGH) && digitalRead(lsa_6) ==LOW)) //Y JUNCTION
  {
    i = 6;
  }
 
  else if (!lsa_2v && !lsa_4v && !lsa_5v && !lsa_7v || analogRead(A0) > 670 )
  {
    i = 7;
  }
  else if (!lsa_2v  && lsa_5v && lsa_7v && lsv && !msv)        // St
  {
    i = 8;
  }
 
  Serial.println("Junction is :-  ");
  Serial.println(i);


  return i;

}
int Read_LSA()
{
  //  Serial.print("Reading LSA    ");
  x = 0;
  for (int k = 0; k <= 5; k++)
  {
    x = analogRead(A0) + x;
  }
  x = x / 6;
  y = map(x, 20, 630, 0, 70);
  //  Serial.println(y - 35);
  return (y - 35);
}
void read_sensor_values()
{
  //  Serial.println("Reading Sensors");
  lsa_2v = digitalRead(lsa_2);             //LSA
  lsa_4v = digitalRead(lsa_4);
  lsa_5v = digitalRead(lsa_5);
  lsa_7v = digitalRead(lsa_7);
  lsa_6v=  digitalRead(lsa_6);
  lsv = digitalRead(left_sensor);          //read IR
  rsv = digitalRead(right_sensor);
  msv = digitalRead(middle_sensor);
}


void go_straight()
{

  error_pwm = Read_LSA();
  //Serial.println(error_pwm);
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
  i++;
  if (i > array_size)
  {
    i = 0;
  }
 
}

void Straight()
{
  int x = Read_LSA();
  analogWrite(pl, average_pwm );
  analogWrite(pr, average_pwm);
  digitalWrite(l1, HIGH);
  digitalWrite(l2, LOW);
  digitalWrite(r1, HIGH);
  digitalWrite(r2, LOW);
  delay(100);
  while (1)
  {
    analogWrite(pl, average_pwm);
    analogWrite(pr, average_pwm);
    digitalWrite(l1, HIGH);
    digitalWrite(l2, LOW);
    digitalWrite(r1, HIGH);
    digitalWrite(r2, LOW);
    x = Read_LSA();
    if ((digitalRead(Junction_Pulse) == LOW) && (x > -10) && (x < 15))
    {
      break;
    }
  }
  stringone += "S";
}
void Slight_left()
{
  //  Serial.println("Taking Slight LEFT");
  int x = Read_LSA();
  analogWrite(pl, 120);
  analogWrite(pr, 120);
  digitalWrite(l1, LOW);
  digitalWrite(l2, LOW);
  digitalWrite(r1, HIGH);
  digitalWrite(r2, LOW);
  delay(500);
  while (1)
  {
    analogWrite(pl, 120);
    analogWrite(pr, 120);
    digitalWrite(l1, LOW);
    digitalWrite(l2, LOW);
    digitalWrite(r1, HIGH);
    digitalWrite(r2, LOW);
    x = Read_LSA();
    if ((digitalRead(Junction_Pulse) == LOW) && (x > -10) && (x < 15))
    {
      break;
    }
  }
  stringone += "L";
}

void U_right()
{
  //  Serial.println("Taking U RIGHT");
  analogWrite(6, 110);
  analogWrite(9, 110);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  digitalWrite(8, LOW);
  digitalWrite(7, HIGH);
  delay(500);
  int x = Read_LSA();
  while (1)
  {
    analogWrite(6, 110);
    analogWrite(9, 110);
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(8, LOW);
    digitalWrite(7, HIGH);
    x = Read_LSA();
    if ((digitalRead(Junction_Pulse) == LOW) && (x > -10) && (x < 10))
    {
      break;
    }
  }
  stringone += "U";
}
void sharp_right()
{
  //  Serial.println("Taking Sharp RIGHT");
  analogWrite(6, 130);
  analogWrite(9, 130);
  digitalWrite(l1, HIGH);
  digitalWrite(l2, LOW);
  digitalWrite(r1, LOW);
  digitalWrite(r2, LOW);
  delay(750);
  int x = Read_LSA();
  while (1)
  {
    analogWrite(6,130);
    analogWrite(9, 130);
    digitalWrite(l1, HIGH);
    digitalWrite(l2, LOW);
    digitalWrite(r1, LOW);
    digitalWrite(r2, LOW);
    x = Read_LSA();
    if ((digitalRead(Junction_Pulse) == LOW) && (x > -10) && (x < 10))
    {
      break;
    }
  }
  stringone += "R";
}


void St()
{
  analogWrite(6, 120);
  analogWrite(9, 120);

  digitalWrite(l1, HIGH);
  digitalWrite(l2, LOW);
  digitalWrite(r1, HIGH);
  digitalWrite(r2, LOW);
  
}

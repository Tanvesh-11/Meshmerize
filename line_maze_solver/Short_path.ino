
void Short_path()
{

    stringtwo=stringone;
    stringtwo.replace("LUS","R");
    stringtwo.replace("LUL","S");
    stringtwo.replace("SUL","R");
    stringtwo.replace("LUR","U");
    stringtwo.replace("RUL","U");
    stringtwo.replace("LUS","R");
    stringtwo.replace("LUL","S");
    stringtwo.replace("SUL","R");
    stringtwo.replace("LUR","U");
    stringtwo.replace("RUL","U");
    stringtwo.replace("LUS","R");
    stringtwo.replace("LUL","S");
    stringtwo.replace("SUL","R");
    stringtwo.replace("LUR","U");
    stringtwo.replace("RUL","U");
    Serial.println(stringtwo);
    count=-1;
    
    while(1)
    {
      final_();
        if(count>50)
        {
          break;
        }
    }
}

void final_()
{
  go_straight();
  a = analogRead(A0);
  
  if (digitalRead(Junction_Pulse) == HIGH || analogRead(A0) > 700)
  {
    Stop();
    count++;
    delay(70);
    char J = stringtwo.charAt(count);
        if(J=="S")
       {
        Straight();
       }
        else if(J=="L")
       {
        Slight_left();
       }
        else if(J=="R")
       {
        sharp_right();
       }
       else if(J=="U")
       {
        U_right();
       }
      else if(J=="W")
      {
        Stop(); 
      }
    
    
  }

}

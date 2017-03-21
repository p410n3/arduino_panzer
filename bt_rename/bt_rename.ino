//Connect to rx3 and tx3 to get this to work
void setup()  
{
  // Open serial communications and wait for port to open:
  Serial3.begin(9600);
  
}

void loop() // run over and over
{
  Serial3.write("AT");
  delay(2000);

  Serial3.write("AT+NAMEpalone-bt");
  
  delay(2000);
  
  Serial3.write("AT+PIN1337");
  delay(2000);
}

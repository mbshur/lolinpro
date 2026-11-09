
void raporzamanlayicivoid()
{
  if(raporsayaci < millis())
  {
    raporsayaci = raporzamanlayici + millis();
    // raporlama yap
    Serial.println("raporladım");
  }
}
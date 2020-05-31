# ArduinoTestProject-DS3231_Real_Time_Clock

this is how to set and get the time form DS3231 RTC module

1st you need to to add the library (DS3231-1.0.2.zip) to your arduino IDE

then you need to execute set project : DS3231_Set_Clock.ino

input the current datetime with format : YYMMDDwHHMMSS (YY = year, MM = Date, DD = date, Dw = day of week, HH = hour, MM = minute, SS = second, example : 2005317235959)

then you can read the time by using project : DS3231_Clock_Test.ino
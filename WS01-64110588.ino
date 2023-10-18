float P = 0.0;  // ประกาศตัวแปร P เป็น float และกำหนดค่าเริ่มต้นเป็น 0.0

void setup() {
  // ส่วน setup สำหรับการเริ่มต้นโปรแกรม รันครั้งเดียว
  Serial.begin(9600);  // เริ่มการสื่อสารผ่าน Serial ที่ความเร็ว 9600 bps
}

void loop() { 
  Serial.printf("%f %f %d\n",sin(P),cos(P+6),0);
  P = P + 0.3;
  delay(50);
}

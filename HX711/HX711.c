#include <HX711.h>

double a_Offset = -8062.83;
double a_correct = 0.00303;
double b_Offset = -131681.18;
double b_correct = 0.00306;
double c_Offset = -94007.92;
double c_correct = 0.00306;
double d_Offset = -126868.67;
double d_correct = 0.00223;
double e_Offset = -35743.56;
double e_correct = 0.00315;
HX711 hx_a(A3, A2, 128, a_correct);
HX711 hx_b(9, 6, 128, b_correct);
HX711 hx_c(5, 3, 128, c_correct);
HX711 hx_d(A1, A0, 128, d_correct);
HX711 hx_e(A5, A4, 128, e_correct);

int a_sum1 = 0, b_sum1 = 0, c_sum1 = 0, d_sum1 = 0, e_sum1 = 0;
int flag = 0;
void setup() {
  Serial.begin(9600);
}
void loop()
{
  if (flag == 2)
    weightPrint();

  if (Serial.available() && flag == 0) {
    flag = 1;
  }
  else if (flag == 1) {
    weightCorrect();
    flag = 2;
    Serial.write('Z');
  }

}
void weightPrint() {
  a_sum1 = 0, b_sum1 = 0, c_sum1 = 0, d_sum1 = 0, e_sum1 = 0;
  a_sum1 = hx_a.bias_read();
  Serial.print("A");
  Serial.print(a_sum1);

  b_sum1 = hx_b.bias_read();
  Serial.print("B");
  Serial.print(b_sum1);

  c_sum1 = hx_c.bias_read();
  Serial.print("C");
  Serial.print(c_sum1);

  d_sum1 = hx_d.bias_read();
  Serial.print("D");
  Serial.print(d_sum1);

  e_sum1 = hx_e.bias_read();
  Serial.print("E");
  Serial.print(e_sum1);
  delay(3000);
}
void weightCorrect() { // 무게 보정

  hx_a.set_offset(a_Offset);
  hx_b.set_offset(b_Offset);
  hx_c.set_offset(c_Offset);
  hx_d.set_offset(d_Offset);
  hx_e.set_offset(e_Offset);

  do {
    a_sum1 = hx_a.bias_read();
    b_sum1 = hx_b.bias_read();
    c_sum1 = hx_c.bias_read();
    d_sum1 = hx_d.bias_read();
    e_sum1 = hx_e.bias_read();
    if (a_sum1 != 0) {
      if (a_sum1 > 0)
        for (int i = a_sum1; i > 0; i--)
          a_Offset += (10000 / 104);
      else
        for (int i = a_sum1; i < 0; i++)
          a_Offset -= (10000 / 104);
    }
    if (b_sum1 != 0) {
      if (b_sum1 > 0)
        for (int i = b_sum1; i > 0; i--)
          b_Offset += (10000 / 101);
      else
        for (int i = b_sum1; i < 0; i++)
          b_Offset -= (10000 / 101);
    }
    if (c_sum1 != 0) {
      if (c_sum1 > 0)
        for (int i = c_sum1; i > 0; i--)
          c_Offset += 100;
      else
        for (int i = c_sum1; i < 0; i++)
          c_Offset -= 100;
    }
    if (d_sum1 != 0) {
      if (d_sum1 > 0)
        for (int i = d_sum1; i > 0; i--)
          d_Offset += (10000 / 99);
      else
        for (int i = d_sum1; i < 0; i++)
          d_Offset -= (10000 / 99);
    }
    if (e_sum1 != 0) {
      if (e_sum1 > 0)
        for (int i = e_sum1; i > 0; i--)
          e_Offset += (10000 / 101);
      else
        for (int i = e_sum1; i < 0; i++)
          e_Offset -= (10000 / 101);
    }
    hx_a.set_offset(a_Offset);
    hx_b.set_offset(b_Offset);
    hx_c.set_offset(c_Offset);
    hx_d.set_offset(d_Offset);
    hx_e.set_offset(e_Offset);
    delay(1000);
  } while (a_sum1 != 0 || b_sum1 != 0 || c_sum1 != 0 || d_sum1 != 0 || e_sum1 != 0);
}

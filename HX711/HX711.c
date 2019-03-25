#include <HX711.h>

double a_Offset = 274656.84;
double a_correct = 0.0023;
double b_Offset = -236480.81;
double b_correct = 0.00235;
double c_Offset = -75999.82;
double c_correct = 0.00238;
double d_Offset = -136521.81;
double d_correct = 0.0025;
double e_Offset = 69528.19;
double e_correct = 0.002355;
HX711 hx_a(A0, A1, 128, a_correct);
HX711 hx_b(6, 9, 128, b_correct);
HX711 hx_c(A2, A3, 128, c_correct);
HX711 hx_d(A4, A5, 128, d_correct);
HX711 hx_e(3, 5, 128, e_correct);

int a_sum1 = 0, b_sum1 = 0, c_sum1 = 0, d_sum1 = 0, e_sum1 = 0;
int flag = 0;

void setup() {
  Serial.begin(9600);
  weightCorrect();
}

void loop() {

  weightPrint();

  //if (Serial.available() && flag == 0) {
  //  flag = 1;
  //}
  //else if (flag == 1) {
  //  weightCorrect();
  //  flag = 2;
  //  Serial.write('Z');
  //}

}
void weightPrint() {
  a_sum1 = 0, b_sum1 = 0, c_sum1 = 0, d_sum1 = 0, e_sum1 = 0;
  a_sum1 = hx_a.bias_read();
  Serial.print("A");
  if (a_sum1 < 0)
    a_sum1 -= a_sum1;
  Serial.print(a_sum1);

  b_sum1 = hx_b.bias_read();
  Serial.print("B");
  if (b_sum1 < 0)
    b_sum1 -= b_sum1;

  Serial.print(b_sum1);

  c_sum1 = hx_c.bias_read();
  Serial.print("C");
  if (c_sum1 < 0)
    c_sum1 -= c_sum1;

  Serial.print(c_sum1);

  d_sum1 = hx_d.bias_read();
  Serial.print("D");
  if (d_sum1 < 0)
    d_sum1 -= d_sum1;

  Serial.print(d_sum1);

  e_sum1 = hx_e.bias_read();
  Serial.print("E");
  if (e_sum1 < 0)
    e_sum1 -= e_sum1;

  Serial.println(e_sum1);
  delay(5000);
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

  } while (a_sum1 != 0 || b_sum1 != 0 || c_sum1 != 0 || d_sum1 != 0 || e_sum1 != 0);
}

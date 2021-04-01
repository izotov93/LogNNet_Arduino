#include "LogNNet.h"

float Sh[P+1]; 
int i = 1;
byte Y[S+1];

void Hidden_Layer_Calculating(byte Y[S+1]) {
  float W1 = 0;      
  Sh[0] = 1;
  for (int j = 1; j <= P; j++) {
    Sh[j] = 0;
    for (int i = 0; i <= S; i++) {
      W1 = A * sin(i / float(S) * PI / B);
      for (int k = 2; k <= j; k++)
        W1 = 1 - (r * W1 * W1);
      Sh[j] = Sh[j] + Y[i]/255.0 * W1;   
    }
  }
}

void Hidden_Layer_Normalization() {
  Sh[0] = 1;
  for (int j = 1; j <= P; j++) 
    Sh[j] = ((Sh[j] - minH[j-1]/100.0) / 
        (maxH[j-1]/100.0 - minH[j-1]/100.0)) 
            - 0.5 - meanH[j-1]/10000.0;
}

byte Output_Layer() {
  float Sout[M];
  for (int j = 0; j < M; j++) {
    Sout[j] = 0;
    for (int i = 0; i <= P; i++) 
      Sout[j] = Sout[j] + Sh[i] * W2[i][j] / 100.0;
    Sout[j] = 1 / (1 + exp(-1 * Sout[j]));
  }
  byte digit = 0;
  for (int i = 0; i < M; i++) {
    if (Sout[i] > Sout[digit])
      digit = i;
  }
  return digit;
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  if (Serial.available() > 0) {
    Y[i] = Serial.read(); 
    i++;
    if (i == S+1) { 
      Y[0] = 255;
      i = 1;
      Hidden_Layer_Calculating(Y);       
      Hidden_Layer_Normalization();
      byte Digit = Output_Layer(); 
      Serial.print(String(Digit));  
    }
  }
}

void setup() {
  pinMode (9, OUTPUT);
  pinMode (10, OUTPUT);
  analogWrite(9, LOW);
  analogWrite(10, LOW);
}

void go() {
  digitalWrite(10, 255);
  digitalWrite(9, 0);
}

void stop() {
  digitalWrite(9, 0);
  digitalWrite(10, 0);
}

void loop() {
  const float power_mult = 16.f / 5.f;

  const int sensor = analogRead(A0);

  const int zero_time_on = 80 * power_mult;
  const int full_time_on = 40 * power_mult;
  const float m_time_on = (full_time_on - zero_time_on) / 1024.f;
  const int time_on = m_time_on * sensor + zero_time_on;

  go();
  delay(time_on);
  stop();

  const int zero_time_off = 1000;
  const int full_time_off = 429;
  const float m_time_off = (full_time_off - zero_time_off) / 1024.f;
  const int time_off = m_time_off * sensor + zero_time_off;

  delay(time_off - time_on);
}


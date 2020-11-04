const int bottom = 8;
const int top = 9;
const int tr = 10;
const int br = 11;
const int tl = 13;
const int bl = 12;

enum GameState {
  IN_RESET, GAME
};

GameState state = IN_RESET;


int resetTime = 500;

int ballY = 0;
int ballX = 0;

int ballVX = 0;
int ballVY = 0;
int paddle1 = 0;
int paddle2 = 0;

const int paddleWidth = 255;

void bounce(int& x, int& dx) {
  if (x < 0) {
    x = 0;
    dx *= -1;
  } else if (x > 1024) {
    x = 1024;
    dx *= -1;
  }
}

void updateBall() {
  ballX += ballVX;
  ballY += ballVY;

  bounce(ballX, ballVX);

  if (ballY < 0) {
    if (paddle2 <= ballX && ballX < paddle2 + paddleWidth) {
      bounce(ballY, ballVY);
    } else reset();
  } else if (ballY > 1024) {
    if (paddle1 <= ballX && ballX < paddle1 + paddleWidth) {
      bounce(ballY, ballVY);
    } else reset();
  }
}

void initSegment(int seg) {
  pinMode(seg, OUTPUT);
  digitalWrite(seg, HIGH);
}

void setup() {
  // initialize the LED pin as an output:
  pinMode(5, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  initSegment(top);
  initSegment(bottom);
  initSegment(tr);
  initSegment(br);
  initSegment(tl);
  initSegment(bl);

  Serial.begin(9600);
  reset();
}



void show(const int digit, const int segment) {
  digitalWrite(segment, LOW);
  digitalWrite(digit, HIGH);
  delay(1);
  digitalWrite(digit, LOW);
  digitalWrite(segment, HIGH);
}

int xToDigit(int x) {
  return 2 + x / 256;
}

int xyToSegment(int x, int y) {
  if ((x / 128) % 2 == 0) {
    if (y < 512) return tl;
    return bl;
  }
  if (y < 512) return tr;
  return br;
}

void resetting() {
  resetTime -= 50;

  show(2, bottom);
  show(2, top);
  show(3, bottom);
  show(3, top);
  show(4, bottom);
  show(4, top);
  show(5, bottom);
  show(5, top);

  if (resetTime <= 0) {
    state = GAME;
  }

  delay(100);
}

void playGame() {
  paddle1 = analogRead(0);
  paddle2 = analogRead(1);

  show(xToDigit(paddle1), bottom);
  show(xToDigit(paddle1+paddleWidth), bottom);
  show(xToDigit(paddle2), top);
  show(xToDigit(paddle2+paddleWidth), top);

  updateBall();

  show(xToDigit(ballX), xyToSegment(ballX, ballY));

}

void reset() {
  state = IN_RESET;
  resetTime = 500;
  ballX = 0;
  ballY = 0;
  ballVX = 7;
  ballVY = 19;
}


void loop() {
  switch (state) {
    case IN_RESET: resetting(); break;
    case GAME: playGame(); break;
  }
  delay(5);
}

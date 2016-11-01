printf("hello \" world");


int x;
int y = 1, z = 2;
int a = 2;
int b = 3;
int c = 4;

x = y + z;

x = a;
x = a++;
x = -a++;
x = -a++ * b;
x = -a++ * b - c;
x = 10 < -a++ * b - c;
x = 1 == 10 < -a++ * b - c;
{
  x = 1;
  {
    int y;
  }

}

// main() {
x = 5;
if (x < 10)
  x = 1;
else if (x > 5) {
  x = 2;
}
else {
  x = 3;
}

for (;;) {
  x = x + 2;
  break;
}

do {
  break;
  while (x < 10) x--;
} while (0);
// }

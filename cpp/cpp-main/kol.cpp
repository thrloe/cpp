#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

int main() {
  for (int num = 10000; num <= 99999; num++) {
    int digits[5];
    int n = num;
    for (int i = 4; i >= 0; i--) {
      digits[i] = n % 10;
      n /= 10;
    }

    for (int pos = 0; pos < 5; pos++) {
      int four_digit = 0;
      int power = 3;
      for (int i = 0; i < 5; i++) {
        if (i != pos) {
          four_digit += digits[i] * pow(10, power);
          power--;
        }
      }

      if (four_digit * 9 == num) {
        cout << "число: " << num << endl;
        cout << "позиция " << pos + 1 << endl;
        cout << "оставшееся: " << four_digit << endl;
        cout << four_digit << " * 9 = " << four_digit * 9 << endl;
        cout << "-----------------" << endl;
      }
    }
  }
  return 0;
}

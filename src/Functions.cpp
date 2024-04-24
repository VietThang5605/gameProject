#include "Functions.h"

void logError(const char* msg, const char* error) {
  std::cout << msg << ' ' << error << std::endl;
}

string IntToString(int x) {
  string res = "";
  while (x > 0) {
    res += char('0' + x % 10);
    x /= 10;
  }
  int sz = (int)res.size();
  for (int i = 0; i < sz / 2; i++) {
    char tmp = res[i];
    res[i] = res[sz - i - 1];
    res[sz - i - 1] = tmp;
  }
  return res;
}

string TimeToString(double x) {
  string res = "";
  int num = (int)x;
  if (num < 10) {
    res += char('0' + num);
  }
  else {
    while (num > 0) {
      res += char('0' + num % 10);
      num /= 10;
    }
    int sz = (int)res.size();
    for (int i = 0; i < sz / 2; i++) {
      char tmp = res[i];
      res[i] = res[sz - i - 1];
      res[sz - i - 1] = tmp;
    }
  }
  res += '.';
  num = (int)(x * 10);
  res += char('0' + num % 10);
  return res;
}


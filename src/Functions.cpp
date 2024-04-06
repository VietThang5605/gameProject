#include "Functions.h"

void logError(const char* msg, const char* error) {
  std::cout << msg << ' ' << error << std::endl;
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
    string tmp = res;
    int sz = (int)res.size();
    for (int i = 0; i < sz; i++)
      res[i] = tmp[sz - i - 1];
  }
  res += '.';
  num = (int)(x * 10);
  res += char('0' + num % 10);
  return res;
}
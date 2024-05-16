#include "Utilities.h"

void logError(const char* msg, const char* error) {
  std::cerr << msg << ' ' << error << std::endl;
}

string IntToString(int x) {
  if (x == 0)
    return "0";
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

string TimeToString(int Timer, int ConvertType) {
  string res = "";
  if (ConvertType == Skill_TimeConvertType) {
    double realTime = Timer / (FPS * 1.0);
    int num = (int)realTime;
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
    num = (int)(realTime * 10);
    res += char('0' + num % 10);
  }
  else if (ConvertType == Countdown_TimeConvertType) {
    double Minute = Timer / (60 * FPS * 1.0);
    int Second = Timer % (60 * 40) / 40;
    int num = (int)Minute;
    if (num < 10) {
      res += '0';
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
    res += ':';
    if (Second < 10)
      res += '0';
    res += IntToString(Second);
  }
  return res;
}


#ifndef SMS_DATABASE
#define SMS_DATABASE

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

enum MessageType
{
  MESSAGE_HAM = 0,
  MESSAGE_SPAM
};

struct Sms
{
  MessageType type;
  string message;
};

class SmsDatabase
{

public:
  SmsDatabase();
  ~SmsDatabase();

  bool ExtractData(string filename);
  void PrintData(void);

  vector<Sms>& GetData();

private:
  vector<Sms> data;

};

#endif //SMS_DATABASE
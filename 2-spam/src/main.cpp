#include "SmsDatabase.h"
#include <iostream>

using namespace std;

int main(void)
{
  SmsDatabase database;
  if (!database.ExtractData("..//res//spam.csv"))
  {
    return 1;
  }

  database.PrintData();
  /*
  vector<Sms> data1 = database.GetData();
  data1.pop_back();
  cout << data1[data1.size() - 1].message << endl;

  vector<Sms> data2 = database.GetData();
  cout << data2[data2.size() - 1].message << endl;
  */

  return 0;
}
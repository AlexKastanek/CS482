#include "SmsDatabase.h"
#include <iostream>

using namespace std;

int main(void)
{
  SmsDatabase database;
  database.ExtractData("..//res//spam.csv");

  database.PrintData();

  return 0;
}
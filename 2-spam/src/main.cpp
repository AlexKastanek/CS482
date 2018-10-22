#include "SmsDatabase.h"
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
  SmsDatabase database;
  vector<Sms> data;
  unordered_map<string, int> hamMap;
  unordered_map<string, int> spamMap;
  //ifstream inSpamFile;
  ofstream outHamProbabilityFile, outSpamProbabilityFile;
  int hamCount = 0, spamCount = 0;
  int commandArgCount = 7;
  string resourcesDirPath = "..//res//";
  string inSpamFilePath, outHamProbabilityFilePath, outSpamProbabilityFilePath;

  //get command line
  if (argc != commandArgCount)
  {
    cout << "ERROR: invalid number of command line arguments" << endl;
    return 1;
  }
  for (int i = 1; i < argc; i++)
  {
    string command = argv[i];
    if (command == "-i")
    {
      string inSpamFileName;

      i++;
      inSpamFileName = argv[i];
      inSpamFilePath = resourcesDirPath + inSpamFileName;
    }
    else if (command == "-os")
    {
      string outSpamProbabilityFileName;

      i++;
      outSpamProbabilityFileName = argv[i];
      outSpamProbabilityFilePath = resourcesDirPath + outSpamProbabilityFileName;
    }
    else if (command == "-oh")
    {
      string outHamProbabilityFileName;

      i++;
      outHamProbabilityFileName = argv[i];
      outHamProbabilityFilePath = resourcesDirPath + outHamProbabilityFileName;
    }
    else
    {
      cout << "ERROR: invalid command line entry" << endl;
      cout << "List of commands for training program:" << endl;
      cout << "-i   <spam.csv file>" << endl;
      cout << "-os  <output spam probability file>" << endl;
      cout << "-oh  <output ham probability file>" << endl;
      cout << "***Each of the above commands must be included in the command line" 
           << endl;

      return 2;
    }
  }

  //get data from input file
  if (!database.ExtractData(inSpamFilePath))
  {
    return 3;
  }

  database.PrintData();

  //for each message, if the message is ham, increment value of each word in the ham map
  //if the message is spam, increment value of each word in the spam map
  data = database.GetData();
  for (int i = 0; i < data.size(); i++)
  {
    if (data[i].type == MESSAGE_HAM)
    {
      for (int j = 0; j < data[i].words.size(); j++)
      {
        hamMap[data[i].words[j]]++;
        hamCount++;
      }
    }
    else if (data[i].type == MESSAGE_SPAM)
    {
      for (int j = 0; j < data[i].words.size(); j++)
      {
        spamMap[data[i].words[j]]++;
        spamCount++;
      }
    }
  }

  /* output data to files */

  //open files and check if opened properly
  outHamProbabilityFile.open(outHamProbabilityFilePath);
  if (!outHamProbabilityFile.is_open())
  {
    cout << "ERROR: Could not open ham probability output file" << endl;
    return 4;
  }
  outSpamProbabilityFile.open(outSpamProbabilityFilePath);
  if (!outSpamProbabilityFile.is_open())
  {
    cout << "ERROR: Could not open spam probability output file" << endl;
    outHamProbabilityFile.close();
    return 5;
  }
  
  //output number of ham words
  outHamProbabilityFile << hamCount << endl;

  //output each ham word and number of ocurrences of ham word
  unordered_map<string, int>::iterator i;
  for (i = hamMap.begin(); i != hamMap.end(); i++)
  {
    outHamProbabilityFile << i->first << " " << i->second << endl;
  }

  outHamProbabilityFile.close();

  //output number of spam words
  outSpamProbabilityFile << spamCount << endl;

  //output each spam word and number of ocurrences of spam word
  for (i = spamMap.begin(); i != spamMap.end(); i++)
  {
    outSpamProbabilityFile << i->first << " " << i->second << endl;
  }

  outSpamProbabilityFile.close();

  /*
  unordered_map<string, int>::iterator i;
  cout << "HAM WORDS:" << endl;
  for (i = hamMap.begin(); i != hamMap.end(); i++)
  {
    cout << "(" << i->first << ", " << i->second << ")" << endl;
  }
  cout << endl;
  cout << "SPAM WORDS: " << endl;
  for (i = spamMap.begin(); i != spamMap.end(); i++)
  {
    cout << "(" << i->first << ", " << i->second << ")" << endl;
  }
  */

  return 0;
}
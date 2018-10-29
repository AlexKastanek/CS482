#include "SmsDatabase.h"
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

struct SmsClassification {
  MessageType type;
  double probSpam;
  double probHam;
  string message;
};

struct SmsWord {
  string word;
  int occurrences;
};

int main(int argc, char* argv[])
{
  SmsDatabase database;
  vector<Sms> testingData;
  vector<SmsWord> spamWords;
  vector<SmsWord> hamWords;
  vector<SmsClassification> classifications;
  unordered_map<string, double> spamProbs;
  unordered_map<string, double> hamProbs;
  double totalSpamProb;
  double totalHamProb;
  int spamWordAmount = 0;
  int hamWordAmount = 0;
  int commandArgCount = 9;
  ifstream fin;
  string resourcesDirPath = "..//res//";
  string inDataFilePath, inSpamProbFilePath, inHamProbFilePath, 
         outClassFilePath;

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
      string inDataFileName;

      i++;
      inDataFileName = argv[i];
      inDataFilePath = resourcesDirPath + inDataFileName;
    }
    else if (command == "-is")
    {
      string inSpamProbFileName;

      i++;
      inSpamProbFileName = argv[i];
      inSpamProbFilePath = resourcesDirPath + inSpamProbFileName;
    }
    else if (command == "-ih")
    {
      string inHamProbFileName;

      i++;
      inHamProbFileName = argv[i];
      inHamProbFilePath = resourcesDirPath + inHamProbFileName;
    }
    else if (command == "-o")
    {
      string outClassFileName;

      i++;
      outClassFileName = argv[i];
      outClassFilePath = resourcesDirPath + outClassFileName;
    }
    else
    {
      cout << "ERROR: invalid command line entry" << endl;
      cout << "List of commands for classify program:" << endl;
      cout << "-i   <testing dataset file>" << endl;
      cout << "-is  <input spam probability file>" << endl;
      cout << "-ih  <input ham probability file>" << endl;
      cout << "-o   <output classification file>" << endl;
      cout << "***Each of the above commands must be included in the command line" 
           << endl;

      return 2;
    }
  }

  //get data from testing dataset
  if (!database.ExtractData(inDataFilePath))
  {
    return 3;
  }
  //database.PrintData();
  testingData = database.GetData();

  /*get data from probability files*/

  //spam probability file
  fin.open(inSpamProbFilePath);
  if (!fin.is_open())
  {
    cout << "ERROR: could not open spam probability file: "
         << inSpamProbFilePath 
         << endl;
    return 4;
  }
  //get the amount of words
  fin >> spamWordAmount;
  //get each word
  while (true)
  {
    SmsWord spamWord;
    fin >> spamWord.word;
    fin >> spamWord.occurrences;
    if (fin.eof())
    {
      break;
    }
    else
    {
      spamWords.push_back(spamWord);
    }
  }
  fin.close();

  //ham probability file
  fin.open(inHamProbFilePath);
  if (!fin.is_open())
  {
    cout << "ERROR: could not open ham probability file: "
         << inHamProbFilePath 
         << endl;
    return 5;
  }
  //get the amount of words
  fin >> hamWordAmount;
  //get each word
  while (true)
  {
    SmsWord hamWord;
    fin >> hamWord.word;
    fin >> hamWord.occurrences;
    if (fin.eof())
    {
      break;
    }
    else
    {
      hamWords.push_back(hamWord);
    }
  }
  fin.close();

  //cout << spamWords.size() << endl;
  //cout << hamWords.size() << endl;
  /*
  for (int i = 0; i < spamWords.size(); i++)
  {
    cout << spamWords[i].word << ", " << spamWords[i].occurrences << endl;
  }
  //*/
  /*
  for (int i = 0; i < hamWords.size(); i++)
  {
    cout << hamWords[i].word << ", " << hamWords[i].occurrences << endl;
  }
  //*/

  //calculate probability any given message is ham or spam
  int totalMessages = spamWordAmount + hamWordAmount;
  totalSpamProb = 
    (double) ((double) spamWordAmount) / 
             ((double) totalMessages);
  totalHamProb = 
    (double) ((double) hamWordAmount) / 
             ((double) totalMessages);

  cout << totalSpamProb << endl;
  cout << totalHamProb << endl;

  //put data from probability files into probability maps
  for (int i = 0; i < spamWords.size(); i++)
  {
    spamProbs[spamWords[i].word] = 
      (double) (((double)spamWords[i].occurrences) / 
                ((double)spamWordAmount));
  }
  for (int i = 0; i < hamWords.size(); i++)
  {
    hamProbs[hamWords[i].word] = 
      (double) (((double)hamWords[i].occurrences) / 
                ((double)hamWordAmount));
  }

  /*
  for (int i = 0; i < spamWords.size(); i++)
  {
    cout << spamWords[i].word 
         << ", " 
         << spamProbs[spamWords[i].word]
         << endl;
  }
  //*/

  //get the probability for each message and choose if message is spam/ham
  //based on the probability
  for (int i = 0; i < testingData.size(); i++)
  {
    SmsClassification classification;
    double probGivenSpam, probGivenHam;

    //copy over the message
    classification.message = testingData.message;

    //get the probability the message occured given the message is spam or ham
    for (int j = 0; j < testingData.words.size(); i++)
    {
      double spamProb = spamProbs[testingData.words[j]];
      double hamProb = hamProbs[testingData.words[j]];

      if (spamProb == 0)
      {
        spamProb = 0.0000001;
      }
      if (hamProb == 0)
      {
        hamProb = 0.0000001;
      }

      if (j == 0)
      {
        probGivenSpam = spamProb;
        probGivenHam = hamProb;
      }
      else
      {
        probGivenSpam *= spamProb;
        probGivenHam *= hamProb;
      }
    }

    //TODO: get the probability the message occured (probabilty word 1 occurred * probability word 2 occurred * ...)
  }

  //output to file

  return 0;
}
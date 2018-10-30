#include "SmsDatabase.h"
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

double NaiveBayes(double prior, double likelihood, double evidence);

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
  ofstream fout;
  string resourcesDirPath = "";
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

  cout << inDataFilePath << endl;
  cout << inSpamProbFilePath << endl;
  cout << inHamProbFilePath << endl;
  cout << outClassFilePath << endl;

  fout.open(outClassFilePath);
  if (!fout.is_open())
  {
    cout << "ERROR: could not open classification file" << endl;
    return 7;
  }
  //fout.close();

  cout << "check 1" << endl;

  //get data from testing dataset
  if (!database.ExtractData(inDataFilePath))
  {
    return 3;
  }
  cout << "check 2" << endl;

  //database.PrintData();
  testingData = database.GetData();

  cout << "check 3" << endl;

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
    double probGivenSpam, probGivenHam, probTotal;
    double probSpamGivenMessage, probHamGivenMessage;

    //copy over the message
    classification.message = testingData[i].message;

    /*
    cout << "********* NEW MESSAGE *********" << endl;
    if (testingData[i].type == MESSAGE_SPAM)
    {
      cout << "SPAM: ";
    }
    else
    {
      cout << "HAM: ";
    }
    cout << testingData[i].message << endl;
    */

    //for each word in the message, get probability of the word occurring
    //and get the probability of the word occurring given message is spam or ham
    //then add the probabilities to the message's respective probabilities
    for (int j = 0; j < testingData[i].words.size(); j++)
    {
      double spamProb = spamProbs[testingData[i].words[j]];
      double hamProb = hamProbs[testingData[i].words[j]];
      double prob = ((spamProb * spamWordAmount) + (hamProb * hamWordAmount)) 
                    / (spamWordAmount + hamWordAmount);

      //cout << testingData[i].words[j] << ":" << endl;

      if (spamProb == 0)
      {
        spamProb = 0.0000001;
      }
      if (hamProb == 0)
      {
        hamProb = 0.0000001;
      }
      if (prob == 0)
      {
        prob = 0.0000001;
      }

      /*
      if (j == 0)
      {
        probGivenSpam = spamProb;
        probGivenHam = hamProb;
        probTotal = prob;
      }
      else
      {
        probGivenSpam *= spamProb;
        probGivenHam *= hamProb;
        probTotal *= prob;
      }
      */

      if (j == 0)
      {
        probSpamGivenMessage = NaiveBayes(
          totalSpamProb,
          spamProb,
          prob);

        probHamGivenMessage = NaiveBayes(
          totalHamProb,
          hamProb,
          prob);
      }
      else
      {
        probSpamGivenMessage *= NaiveBayes(
          totalSpamProb,
          spamProb,
          prob);

        probHamGivenMessage *= NaiveBayes(
          totalHamProb,
          hamProb,
          prob);
      }

      /*
      cout << testingData[i].words[j] << ":" << endl 
           << "SPAM - " << spamProb 
           << ", HAM - " << hamProb 
           << ", TOTAL - " << prob << endl;
      */
    }
    /*
    cout << "Message:" << endl 
         << "SPAM - " << probGivenSpam 
         << ", HAM - " << probGivenHam 
         << ", TOTAL - " << probTotal << endl;
    */

    /*
    classification.probSpam = NaiveBayes(
      totalSpamProb,
      probGivenSpam,
      probTotal);

    classification.probHam = NaiveBayes(
      totalHamProb,
      probGivenHam,
      probTotal);
    */

    classification.probSpam = probSpamGivenMessage;
    classification.probHam = probHamGivenMessage;

    //cout << "p(SPAM | message): " << classification.probSpam << endl;
    //cout << "p(HAM | message): " << classification.probHam << endl;
    //cout << "Classification: ";
    if (classification.probSpam > classification.probHam)
    {
      classification.type = MESSAGE_SPAM;
      //cout << "SPAM" << endl;
    }
    else
    {
      classification.type = MESSAGE_HAM;
      //cout << "HAM" << endl;
    }

    classifications.push_back(classification);
  }

  //compare
  int correctCounter = 0;
  if (testingData.size() != classifications.size())
  {
    cout << "ERROR: results are of different sizes" << endl;
    return 6;
  }
  for (int i = 0; i < testingData.size(); i++)
  {
    if (testingData[i].type == classifications[i].type)
    {
      correctCounter++;
    }
    else
    {
      cout << endl << "Incorrect classification:" << endl;
      cout << "Message: " << classifications[i].message << endl;
      cout << "Type: ";
      if (testingData[i].type == MESSAGE_SPAM)
      {
        cout << "SPAM" << endl;
      }
      else
      {
        cout << "HAM" << endl;
      }
      cout << "Probability SPAM: " << classifications[i].probSpam << endl;
      cout << "Probability HAM: " << classifications[i].probHam << endl;
    }
  }
  cout << endl << "SCORE: " 
       << (double) (((double)correctCounter) / ((double)testingData.size()))
       << endl << (testingData.size() - correctCounter) << " out of "
       << testingData.size() << " guessed incorrectly" << endl;

  //output to file
  /*
  fout.open(outClassFilePath);
  if (!fout.is_open())
  {
    cout << "ERROR: could not open classification file" << endl;
    return 7;
  }
  */
  for (int i = 0; i < classifications.size(); i++)
  {
    if (classifications[i].type == MESSAGE_SPAM)
    {
      fout << "spam, ";
    }
    else
    {
      fout << "ham, ";
    }
    string message = classifications[i].message;
    message.pop_back();
    message.pop_back();
    //message.pop_back();
    //message.pop_back();
    fout << "\"" << message << "\",,," << endl;
  }
  fout.close();

  return 0;
}

double NaiveBayes(double prior, double likelihood, double evidence)
{
  double posterior;

  //cout << "Bayes: (" << prior << " * " << likelihood << ") / " << evidence << endl;

  posterior = (prior * likelihood) / evidence;
  return posterior;
}
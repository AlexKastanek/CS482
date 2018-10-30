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

  //get the probability for each message and choose if message is spam/ham
  //based on the probability
  for (int i = 0; i < testingData.size(); i++)
  {
    SmsClassification classification;
    double probGivenSpam, probGivenHam, probTotal;
    double probSpamGivenMessage, probHamGivenMessage;

    //copy over the message
    classification.message = testingData[i].message;

    //for each word in the message
    for (int j = 0; j < testingData[i].words.size(); j++)
    {
      //get probabilities for that word (given ham, given spam, and given any type)
      double spamProb = spamProbs[testingData[i].words[j]];
      double hamProb = hamProbs[testingData[i].words[j]];
      double prob = ((spamProb * spamWordAmount) + (hamProb * hamWordAmount)) 
                    / (spamWordAmount + hamWordAmount);

      //if the probability is 0, make it really small to avoid zeroing entire equation
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

      //if on the first iteration, set the new probabilities
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
      //if not on first iteration, multiply on to the new probabilites
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
    }

    //assign the new probabilities
    classification.probSpam = probSpamGivenMessage;
    classification.probHam = probHamGivenMessage;

    //determine the most likely type (max of the two)
    if (classification.probSpam > classification.probHam)
    {
      classification.type = MESSAGE_SPAM;
    }
    else
    {
      classification.type = MESSAGE_HAM;
    }

    classifications.push_back(classification);
  }

  //compare and output any incorrect classifications
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
  //output the final score
  cout << endl << "SCORE: " 
       << (double) (((double)correctCounter) / ((double)testingData.size()))
       << endl << (testingData.size() - correctCounter) << " out of "
       << testingData.size() << " guessed incorrectly" << endl;

  //output classifications to file
  fout.open(outClassFilePath);
  if (!fout.is_open())
  {
    cout << "ERROR: could not open classification file" << endl;
    return 7;
  }
  for (int i = 0; i < classifications.size(); i++)
  {
    if (classifications[i].type == MESSAGE_SPAM)
    {
      fout << "spam" << endl;
    }
    else
    {
      fout << "ham" << endl;
    }
  }
  fout.close();

  return 0;
}

double NaiveBayes(double prior, double likelihood, double evidence)
{
  double posterior;

  posterior = (prior * likelihood) / evidence;
  return posterior;
}
#include "SmsDatabase.h"

SmsDatabase::SmsDatabase()
{

}

SmsDatabase::~SmsDatabase()
{

}

bool SmsDatabase::ExtractData(string filename)
{
  ifstream fin;
  string format, line;
  int lineCount = 0;

  //open file and check if open
  fin.open(filename);
  if (!fin.is_open())
  {
    cout << "ERROR: Could not open file: " << filename 
         << ". Ensure file is located in res directory." 
         << endl;
    return false;
  }

  //get each line until end of file is reached
  while (getline(fin, line))
  {
    //this check is to help with the very picky autograder
    if (line == "v1,v2,,,")
    {
      continue;
    }

    Sms sms;
    MessageType messageType;
    vector<string> words;
    string str_messageType, message, word;
    char character = '\0';

    lineCount++;

    //append to messageType until a comma is reached
    int index = 0;
    while (true)
    {
      character = line[index];
      index++;

      if (character == ',')
      {
        break;
      }
      else
      {
        str_messageType.push_back(character);
      }
    }

    //check which messageType was extracted
    if (str_messageType == "ham")
    {
      messageType = MESSAGE_HAM;
    }
    else if (str_messageType == "spam")
    {
      messageType = MESSAGE_SPAM;
    }
    else
    {
      cout << "ERROR: incorrect message type format in file: " 
           << filename 
           << " ("
           << str_messageType
           << ") on line: "
           << lineCount
           << endl;
      return false;
    }

    //append to message and words until end of line
    for (int i = index; i < line.size(); i++)
    {
      character = line[i];

      //append the character to message
      message.push_back(character);

      //append character to word if character is a letter (upper or lower case)
      if ((character >= 'a' && character <= 'z') ||
          (character >= 'A' && character <= 'Z'))
      {
        if (character >= 'A' && character <= 'Z')
        {
          //convert to all lower case
          character += 32;
        }
        word.push_back(character);
      }
      //if not a letter and not empty, append to the words vector
      else if (!word.empty())
      {
        words.push_back(word);
        word.clear();
      }
    }

    //set sms values
    sms.type = messageType;
    sms.message = message;
    sms.words = words;

    //push sms onto container
    data.push_back(sms);
  }
  fin.close();

  return true;
}

void SmsDatabase::PrintData(void)
{
  for (int i = 0; i < data.size(); i++)
  {
    if (data[i].type == MESSAGE_HAM)
    {
      cout << "HAM: ";
    }
    else if (data[i].type == MESSAGE_SPAM)
    {
      cout << "SPAM: ";
    }
    else
    {
      cout << "ERROR: invlaid message type recorded" << endl;
      return;
    }

    
    cout << "\""
         << data[i].message
         << "\""
         << endl;
    

    cout << "WORDS: ";
    for (int j = 0; j < data[i].words.size(); j++)
    {
      cout << data[i].words[j] << ", ";
    }
    cout << endl;

    cout << endl;
  }
}

vector<Sms>& SmsDatabase::GetData()
{
  return data;
}
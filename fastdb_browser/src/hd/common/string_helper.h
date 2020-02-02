/***********************************************************
 * media transfer GmbH
 * 
 * Package:     TOSW Common
 * Module:      $RCSfile: string_helper.h,v $
 * Type:        C++ Source
 * Version:     $Revision: 1.1 $
 * Last changed:
 *   Date: $Date: 2003/11/24 14:48:15 $
 *   By:   $Author: kzerbe $
 * 
 ***********************************************************/
/** 
 * @file string_helper.h
 *  This file contains several string helper functions
 */
#ifndef __STRING_HELPER_H__
#define __STRING_HELPER_H__


#include <vector>
#include <algorithm>

/**
 * This function can be used to split a given string at the given delimiter. The
 * result is stored in the given vector.
 *
 * @param input     the string that should be splitted.
 * @param delimiter the delimiter where the split should take place.
 * @param results   the vector, where the results are stored.
 *
 * @return the number of delimiter found in the input string.
 */
template < class TStringType > 
int SplitString ( const TStringType & input, const TStringType & delimiter, std::vector < TStringType > & results )
{
  int iPos = 0;
  int newPos = -1;
  int sizeS2 = delimiter.size();
  int isize = input.size();
  std::vector < int > positions;
  newPos = input.find (delimiter, 0);
  if ( newPos < 0 )
  {
    return 0;
  }

  int numFound = 0;
  while ( newPos > iPos )
  {
    numFound++;
    positions.push_back(newPos);
    iPos = newPos;
    newPos = input.find (delimiter, iPos + sizeS2 + 1);
  }

  if( numFound>0 )
  {
    for ( int i = 0; i <= positions.size(); i++ )
    {
      TStringType s;
      if ( i == 0 )
      {
        s = input.substr( i, positions[i] );
      }
      int offset = positions[i - 1] + sizeS2;
      if ( offset < isize )
      {
        if ( i == positions.size() )
        {
          s = input.substr(offset);
        }
        else if ( i > 0 )
        {
          s = input.substr( positions[i - 1] + sizeS2, positions[i] - positions[i - 1] - sizeS2 );
        }
      }
      if ( s.size() > 0 )
      {
        results.push_back(s);
      }
    }
  }
  return numFound;
}


/**
 * This function can be used to split a given string at the given delimiter like SplitString.
 * The difference is that no empty vector will be returned and an empty string will be returned
 * in the vector if one delimiter succeeds one another.
 *
 * @param input     the string that should be splitted.
 * @param delimiter the delimiter where the split should take place.
 * @param results   the vector, where the results are stored.
 *
 * @return the number of delimiter found in the input string.
 */
template < class TStringType > 
int SplitString2 ( const TStringType & input, const TStringType & delimiter, std::vector < TStringType > & results )
{
  int iPos = -1;
  int newPos = -1;
  int sizeS2 = delimiter.size();
  int isize = input.size();
  std::vector < int > positions;
  newPos = input.find (delimiter, 0);
  if ( newPos < 0 )
  {
	results.push_back(input);
    return 0;
  }

  int numFound = 0;
  while ( newPos > iPos )
  {
    numFound++;
    positions.push_back(newPos);
    iPos = newPos;
    newPos = input.find (delimiter, iPos + sizeS2);
  }

  if( numFound>0 )
  {
    for ( int i = 0; i <= positions.size(); i++ )
    {
      TStringType s;
      if ( i == 0 )
      {
        s = input.substr( 0, positions[i] );
      }
	  else if (i == positions.size())
	  {
        int offset = positions[i - 1] + sizeS2;
        if ( offset < isize )
          s = input.substr(offset);
		else
		  s = "";
	  }
	  else
      {
        s = input.substr( positions[i - 1] + sizeS2, positions[i] - positions[i - 1] - sizeS2 );
      }
      results.push_back(s);
    }
  }
  return numFound;
}


/**
 * This function can be used to compare two dot seperated version numbers
 *
 * @param ver1 version nummber 1
 * @param ver2 version nummber 1 
 *
 * @return -1 if ver1 is lower than ver2<br>
 *         0  if ver1 is equal ver2<br>
 *         -1 if ver1 is higher than ver2
 */
template < class TStringType > 
int CompareVersions(const TStringType& ver1,const TStringType& ver2)
{
  // assume equality
  int retVal=0;

  std::vector<TStringType> verElements_1;
  std::vector<TStringType> verElements_2;

  if( 0==SplitString<TStringType>(ver1,_T("."),verElements_1) )
    verElements_1.push_back(ver1);

  if( 0==SplitString<TStringType>(ver2,_T("."),verElements_2) )
    verElements_2.push_back(ver2);

  std::vector<TStringType>::iterator iter1,iter2;

  iter1=verElements_1.begin();
  iter2=verElements_2.begin();
  
  while(iter1!=verElements_1.end() && iter2!=verElements_2.end())
  {
    if(atoi(iter1->c_str()) > atoi(iter2->c_str()) )
    {
      retVal=1;
      break;
    }
    
    if(atoi(iter1->c_str()) < atoi(iter2->c_str()))
    {
      retVal=-1;
      break;
    }

    iter1++;
    iter2++;
  }

  if( retVal==0 )
  {
    if(iter1!=verElements_1.end() && iter2==verElements_2.end())
      retVal=1;
    else if(iter1==verElements_1.end() && iter2!=verElements_2.end())
      retVal=-1;
  }
  
  return retVal;
}


inline bool isNotDigit(int c)
{
  return !isdigit(c);
}

/**
 * This function can be used to check, if the given sting is a valid 
 * dot seperated version number.
 *
 * @param version  the version string that should be checked.
 *
 * @return true if the version is valid, false otherwise.
 */
template < class TStringType > 
bool IsVersionValid(const TStringType& version)
{
  bool retVal=true;

  if( !version.empty() )
  {
    std::vector<TStringType> verElements;

    if( 0==SplitString<TStringType>(version,_T("."),verElements) )
      verElements.push_back(version);

    std::vector<TStringType>::iterator iter;

    for(iter=verElements.begin();iter!=verElements.end();iter++)
    {
      if( std::find_if(iter->begin(),iter->end(),isNotDigit)!=iter->end())
      {
        retVal=false;
        break;
      }
    }
  }
  else
    retVal=false;

  return retVal;
}

template < class TStringType > 
inline TStringType QuoteString(const TStringType& input)
{
  TStringType output;

  output=TStringType(_T("\""))+input+TStringType(_T("\""));

  return output;
}

/**
 * This function reads one line out of a file with no buffer overflow guaranteed.
 */
template < class TStringType > 
bool ReadLine(FILE* fp, TStringType& line)
{
  static TCHAR buffer[1024 + 1];
  
  static size_t noOfItems = sizeof(buffer) / sizeof(TCHAR) - 1;
  
  static size_t noOfItemsRead = 0;
  
  static unsigned int startOfLine;
  
  static bool doContinue;

  line.Empty();
  
  do
  {
    if (noOfItemsRead == 0)
    {
      noOfItemsRead = fread(buffer, sizeof(TCHAR), noOfItems, fp);
    
      doContinue = noOfItemsRead == noOfItems;
    
      if (noOfItemsRead < noOfItems && (noOfItemsRead == 0 || buffer[noOfItemsRead - 1] != _T('\n')))
        buffer[noOfItemsRead++] = _T('\n');
    
      startOfLine = 0;
    }
    
    for (unsigned int i = startOfLine; i < noOfItemsRead; ++i)
    {
      if (buffer[i] == _T('\n'))
      {
        buffer[i] = 0;
        
        line += (buffer + startOfLine);
        
        startOfLine = i + 1;

        return true;
      }
    }
    
    if (buffer[noOfItemsRead - 1] != 0)
    {
      buffer[noOfItemsRead] = 0;
      
      line += (buffer + startOfLine);
    }

    noOfItemsRead = 0;
  }
  while (doContinue);

  return false;
}

#endif //__STRING_HELPER_H__

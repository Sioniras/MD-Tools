//////////////////////////////////////////////////////////////////////////////
// A tool to compare the output of pycontacts.
// 
// (c) 2017 by Claus N.
//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <algorithm>
#include <sstream>
#include <iomanip>

#include "ContactList.h"

int main(int argc,char** argv)
{
	// Check for proper input
	if(argc < 4)
	{
		std::cout << "Please specify the minimum score and at least two contacts files." << std::endl;
		std::cout << "compcontacts [minimum score] [file1] [file2] [optional: add more files]" << std::endl;
		return 0;
	}
	
	std::vector<std::shared_ptr<ContactList> > list;
	const int minscore = std::atoi(argv[1]);	// Get minimum score
	const int columnpadding = 8;
	
	std::cout << "Loading contact files for analysis with a minimum score of " << minscore << "." << std::endl;
	
	// Create lists containing contact objects. The files are read from ContactList::loadFile.
	for(int i = 2;i < argc;i++)
	{
		std::string strargv(argv[i]);
		std::shared_ptr<ContactList> new_ptr(new ContactList(strargv));
		
		if(new_ptr->IsValid())
		{
			list.push_back(std::shared_ptr<ContactList>(new_ptr));
		}
		else
		{
			std::cerr << "Failed to parse file \"" << strargv << "\"! Exitting program." << std::endl;
			std::exit(1);
		}
	}
	
	std::cout << "Finished reading the contact files." << std::endl;
	
	// Obtain a full list of contacts (note that they were already sorted in ContactList::loadFile)
	std::vector<Contact> FullContactList;
	for(auto i = list.cbegin(); i != list.cend(); i++)
	{
		// Create a temporary vector to hold the previous result
		std::vector<Contact> tmp(FullContactList);
		FullContactList.clear();
		
		// Get the union
		std::set_union( (*i)->List().cbegin(), (*i)->List().cend(),
						tmp.cbegin(), tmp.cend(),
						std::back_inserter(FullContactList));
	}
	
	std::cout << "Found a total of " << FullContactList.size() << " unique contacts." << std::endl;
	std::cout << "The table contains the mean score for the trajectories containing the contact. Note that only contacts with a score of at least " << minscore << " are shown.\n" << std::endl;
	
	// Sort the complete list
	std::sort(FullContactList.begin(),FullContactList.end());
	
	// Prepare and write header
	std::stringstream str;
	str << std::setw(2*columnpadding+2) << "Contacts |";
	for(unsigned int i = 0;i < list.size();i++)
		str << " " << std::setw(columnpadding-1) << "file " << (i+1) << " |";
	str << "\n" << std::string(str.str().size(),'-');
	std::cout << str.str() << std::endl;
	
	// Helper definitions
	std::string s[list.size()];
	int cs[list.size()] = {0};
	int k = 0;
	int k2 = 0;
	double m = 0.0;
	
	// Now go through the entire list
	for(auto i = FullContactList.cbegin(); i != FullContactList.cend(); i++)
	{
		// Reset helper definitions
		str.str("");
		str << std::setw(columnpadding) << (*i).Left << " -" << std::setw(columnpadding) << (*i).Right + " |";
		k = 0;
		k2 = 0;
		m = 0.0;
		
		for(auto j = list.cbegin(); j != list.cend(); j++)
		{
			// Get iterator to the element in "j", if it is there
			auto it = std::find( (*j)->List().cbegin(), (*j)->List().cend(), (*i));
			
			// Check whether it was found in "j"
			if(it != (*j)->List().cend() && (*it).Mean >= minscore)
			{
				k++;
				cs[k2]++;
				m = (m < (*it).Mean)?(*it).Mean:m;
				str << " " << std::setw(columnpadding) << (*it).Mean << " |";
			}
			else
			{
				str << " " << std::setw(columnpadding) << "" << " |";
			}
			
			k2++;
		}
		
		// Add the result-string to the appropriate output-string, i.e. based on number of files in which the contact was found ("k")
		if(m >= minscore)
			s[k-1] += str.str() + "\n";
	}
	
	// Write contacts in order of ascending number of trajectories in which the contact was present
	for(unsigned int i = 0;i < list.size(); i++)
		std::cout << s[i];
	
	// Write total number of contacts
	std::cout << std::string(str.str().size(),'-') << std::endl;
	std::cout << std::setw(2*columnpadding+2) << "Total contacts |";
	for(unsigned int i = 0;i < list.size(); i++)
		std::cout << " " << std::setw(columnpadding) << cs[i] << " |";
	std::cout << std::endl;
}
//////////////////////////////////////////////////////////////////////////////

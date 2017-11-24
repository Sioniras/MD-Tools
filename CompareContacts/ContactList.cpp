//////////////////////////////////////////////////////////////////////////////
// ContactList class to load an output file from pycontacts.
// 
// (c) 2017 by Claus N.
//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include "ContactList.h"
//////////////////////////////////////////////////////////////////////////////
// Constructor
ContactList::ContactList(const std::string& _filename) : isValid(false), filename(_filename)
{
	this->loadFile();
}

// Destructor
ContactList::~ContactList()
{
	
}

// Copy constructor defined in terms of copy-assignment operator
ContactList::ContactList(const ContactList& _obj)
{
	(*this) = _obj;
}

// Copy-assignment
const ContactList& ContactList::operator=(const ContactList& _obj)
{
	std::cerr << "Copy-assignment of ContactList objects is not implemented yet." << std::endl;
	
	return (*this);
}
//////////////////////////////////////////////////////////////////////////////
// Private methods
//////////////////////////////////////////////////////////////////////////////
Contact ContactList::createContact(const std::string& _contact)
{
	auto i = _contact.cbegin();
	std::string left(this->getNext(i,_contact.cend()," -"));
	std::string right(this->getNext(i,_contact.cend()," -"));
	std::string type(this->getNext(i,_contact.cend()," "));
	std::string mean(this->getNext(i,_contact.cend()," "));
	std::string median(this->getNext(i,_contact.cend()," "));
	std::string hbond(this->getNext(i,_contact.cend()," "));
	
	return Contact(left,right,type,std::atof(mean.c_str()),std::atof(median.c_str()),std::atof(hbond.c_str()));
}

// Get the next value in a string
std::string ContactList::getNext(std::string::const_iterator& _i,const std::string::const_iterator& _end,const std::string& _delims)
{
	// Skip all delimiters
	while(_i != _end && _delims.find(*_i) != std::string::npos) {_i++;}
	
	// Get the string between the delimiters
	std::string tmp("");
	while(_i != _end && _delims.find(*_i) == std::string::npos)
	{
		tmp += (*_i);
		_i++;
	}
	
	return tmp;
}
//////////////////////////////////////////////////////////////////////////////
// Public methods
//////////////////////////////////////////////////////////////////////////////
bool ContactList::loadFile()
{
	// Open file for reading
	std::filebuf fb;
	if(fb.open(this->filename,std::ios::in))
	{
		std::istream is(&fb);
		std::string str;
		int i = 0;
		
		// Skip first line with the headers
		// TODO: Read header line
		std::getline(is,str);
		
		// Loop through all lines
		while(std::getline(is,str))
		{
			i++;
			this->contacts.push_back(this->createContact(str));
		}
		
		// Sort the list
		std::sort(this->contacts.begin(),this->contacts.end());
		
		// When the file is read, we are done
		std::cout << "Read " << i << " lines from file \"" << this->filename << "\"." << std::endl;
		this->isValid = true;
		
		fb.close();
	}
	
	return this->isValid;
}

bool ContactList::GetContact(unsigned int _i,Contact& _out)
{
	if(_i < this->contacts.size())
	{
		_out = this->contacts[_i];
		return true;
	}
	
	return false;
}
//////////////////////////////////////////////////////////////////////////////
// Non-member non-friend functions and operators
//////////////////////////////////////////////////////////////////////////////
bool operator==(const Contact& _l,const Contact& _r)
{
	return ((_l.Left.compare(_r.Left) == 0) && (_l.Right.compare(_r.Right) == 0));
}

bool operator<(const Contact& _l,const Contact& _r)
{
	return ((_l.Left + _l.Right) < (_r.Left + _r.Right));
}
//////////////////////////////////////////////////////////////////////////////

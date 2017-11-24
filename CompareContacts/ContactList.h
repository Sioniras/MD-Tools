//////////////////////////////////////////////////////////////////////////////
// ContactList class to load an output file from pycontacts.
// 
// (c) 2017 by Claus N.
//////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <string>

// Contact helper struct
struct Contact
{
	public:
		// Data members
		std::string Left;
		std::string Right;
		std::string Type;
		float Mean;
		float Median;
		float HBondPercentage;
	
		// Constructors / destructor
		Contact() {};
		Contact(const std::string& _l,const std::string& _r,const std::string& _t,float _mea,float _med,float _h)
			: Left(_l), Right(_r), Type(_t), Mean(_mea), Median(_med), HBondPercentage(_h) {};
		~Contact() {};
};

// Comparison operator for Contact structs
bool operator==(const Contact&,const Contact&);
bool operator<(const Contact&,const Contact&);

// ContactList class (list of Contact structs)
class ContactList
{	
	private:
		// Data members
		bool isValid;
		std::string filename;
		std::vector<Contact> contacts;
		
		// Private methods
		bool loadFile();
		Contact createContact(const std::string&);
		std::string getNext(std::string::const_iterator&,const std::string::const_iterator&,const std::string&);
	
	public:
		// Constructors / destructor
		ContactList(const std::string&);
		ContactList(const ContactList&);	// Copy-constructor
		~ContactList();
		
		// Operators
		const ContactList& operator=(const ContactList&);	// Copy-assignment
		
		// Public members
		bool IsValid() const {return this->isValid;};
		bool GetContact(unsigned int,Contact&);
		const std::vector<Contact>& List() {return contacts;}
};
//////////////////////////////////////////////////////////////////////////////

#include <string>

using namespace std;

class string_table {
 public:
	string_table(unsigned capacity);
        unsigned put(string name);
        string get(unsigned index);

 private:
        string *data;
	unsigned used;
	unsigned capacity; 
};

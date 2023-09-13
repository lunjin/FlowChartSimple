class string_table
{

 private:
        string *data;
	int used, capacity; 

 public:
	string_table(int capacity);
        int put(string name);
        string get(int index);
   
}

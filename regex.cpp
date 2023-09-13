#include <iostream>
#include <string>
#include <regex>
 
void show_matches(const std::string& in, const std::string& re)
{
    std::smatch m;
    std::regex_search(in, m, std::regex(re,std::regex::icase));
    if(!m.empty()) {
        std::cout << "input=[" << in << "], regex=[" << re << "]\n  "
                     "prefix=[" << m.prefix() << "]\n  smatch: ";
        for(std::size_t n = 0; n < m.size(); ++n)
            std::cout << "m[" << n << "]=[" << m[n] << "] ";
        std::cout << "\n  suffix=[" << m.suffix() << "]\n";
    } else {
        std::cout << "input=[" << in << "], regex=[" << re << "]: NO MATCH\n";
    }
}
 
int main()
{
      show_matches("abcdef", "abc");
    show_matches("abc", "ab|abc");    // left Alernative matched first
     // Match of the input against the left Alternative (a) followed
    // by the remained of the regex (c|bc) succeeds, which results
    // in m[1]="a" and m[4]="bc".
    // The skipped Alternatives (ab) and (c) leave their submatches
    // m[3] and m[5] empty.
    show_matches("abc", "((a)|(ab))((c)|(bc))");
}

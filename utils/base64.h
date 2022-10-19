
#ifndef BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A
#define BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A

#include <string>
#include <vector>

std::string base64_encode(unsigned char const* , size_t len);
std::string base64_decode(std::string const& s);
void bin_base64_decode(std::string const& encoded_string, std::vector<char> &target);

#endif /* BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A */

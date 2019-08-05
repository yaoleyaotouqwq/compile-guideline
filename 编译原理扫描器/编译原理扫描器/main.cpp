#include "stdafx.h"


int main()
{
	int max_len;
	word_list words[1000];
	max_len = outthe_cifa_list(words);
	outthe_yufa_list(words,max_len);
	outthe_yuyi_list(words, max_len);
	std::cin.get();
	return 0;
}


#ifdef _FB
#else

#include <string>

class Stream;

void jsonParser(std::string& json, int sessionid);

void jsonParser2(void * buf, int len, Stream& st);

#endif


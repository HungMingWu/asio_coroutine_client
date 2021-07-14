#include "client.h"
int main()
{
	client c;
	auto error = c.connect_to("192.168.190.69", 123);
	if (error)
		printf("%s\n", error.message().c_str());
	else
		printf("Success\n");
	return 0;
}
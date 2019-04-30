#include <stdio.h>
#include <unistd.h>

int main()
{
	printf( "user id\n");
	printf( "%d\n", getuid());
	printf( "%d\n", geteuid());

	printf( "group id \n");
	printf( "%d\n", getgid());
	printf( "%d\n", getegid());	
	
	return 0;
}

#include "expsolver.h"

using namespace gnilk;
static char *num2bin(unsigned int num, char *buffer, int maxlen) {
	int idxStart = 0;

	int i=0;
	for (i=31;i>=0;i--) {
		int bit = (num & (1<<i));
		if (bit != 0) break;
	}
	buffer[idxStart]='0';
	idxStart++;
	for(;i>=0;i--) {
		buffer[idxStart] = (num & (1<<i))?'1':'0';
		idxStart++;
	}
	buffer[idxStart]='\0';
	return buffer;
}

int main(int argc, char **argv) {
	double tmp;
	char binary[48];

	if (argc != 2) {
		printf("Usage: %s <expression>\n", argv[0]);
		printf("Solves normal expressions, like: '4+5*3/7'\n");
		printf("Prefix supported:\n");
		printf("  %% - binary\n");
		printf("  $/x - hex\n");
		return 0;
	}
	ExpSolver::Solve(&tmp, argv[1]);
	num2bin((int)tmp, binary, 48);
	printf("%d, 0x%.x, %%%s\n",(int)tmp,(int)tmp, binary);	
	return 0;
}
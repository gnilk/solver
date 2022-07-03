#include "expsolver.h"

using namespace gnilk;
static char *num2bin_grouped(unsigned int num, char *buffer, int maxlen) {
	int idxStart = 0;

	int i=0;
	for (i=31;i>=0;i--) {
		int bit = (num & (1<<i));
		if (bit != 0) break;
	}

    int groups = 1 + (i+1)/4;
    memset(buffer,' ', maxlen);

    idxStart = (groups * 4 + groups-1);
    buffer[idxStart]='\0';
    idxStart--;
    int bit = 0;
    for(int i=0;i<groups;i++) {
        for(int i=0;i<4;i++) {
            buffer[idxStart] = (num & (1 << bit)) ? '1' : '0';
            idxStart--;
            bit++;
        }
        if (idxStart > 0) {
            buffer[idxStart] = ' ';
            idxStart--;
        }
    }
    return buffer;
}

static char *num2bin(unsigned int num, char *buffer, int maxlen) {
    int idxStart = 0;

    int i=0;
    for (i=31;i>=0;i--) {
        int bit = (num & (1<<i));
        if (bit != 0) break;
    }

    buffer[idxStart]='0';
    idxStart++;
    int bitCount = 0;
    for(;i>=0;i--) {
        buffer[idxStart] = (num & (1<<i))?'1':'0';
        idxStart++;
    }
    buffer[idxStart]='\0';
    return buffer;
}
static int Usage(char *name) {
    printf("Usage: %s [options] <expression>\n", name);
    printf("Solves normal expressions, like: '4+5*3/7'\n");
    printf("Prefix supported:\n");
    printf("  %% - binary\n");
    printf("  $/x - hex\n");
    printf("Options:\n");
    printf(" --old   prints binary as a flat (ungrouped) string\n");
    printf("    -h   this stuff..\n");
    return 0;
}

int main(int argc, char **argv) {
	double tmp;
    bool printOld = false;
    char *expr = nullptr;

    for(int i=1;i<argc;i++) {
        if (!strcmp(argv[i],"--old")) {
            printOld = true;
        } else if (!strcmp(argv[i],"-h")) {
            return Usage(argv[0]);
        } else {
            expr = argv[i];
        }
    }
    if (expr == nullptr) {
        return Usage(argv[0]);
    }

	ExpSolver::Solve(&tmp, expr);

    if (printOld) {
        char binary[48];
        num2bin((int)tmp, binary, 48);
        printf("%d, 0x%.x, %%%s\n",(int)tmp,(int)tmp, binary);
    } else {
        char groupedBinary[64];
        num2bin_grouped((int)tmp, groupedBinary, 64);
        printf("%d, 0x%.x, %%%s\n",(int)tmp,(int)tmp, groupedBinary);
    }

    return 0;
}
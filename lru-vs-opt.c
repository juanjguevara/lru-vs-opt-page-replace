#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int gettotalframes();
int lru(int refindex, int frames, int table[], int ref[]);
int opt(int refindex, int frames, int table[], int ref[]);

int main()
{
	int refstr[20] = {0};
	int totalframes = gettotalframes();
	int lrutable[totalframes + 1][20];
	int opttable[totalframes + 1][20];
	int lrucurrent[totalframes];
	int optcurrent[totalframes];
	int replacepage = 0;
	int lrufaultflag = 0;
	int optfaultflag = 0;
	int lrufaults = 0;
	int optfaults = 0;
	int i = 0;
	int j = 0;

	srand(time(NULL));
	
	for(i = 0; i < totalframes; i++) {
		lrucurrent[i] = 0;
		optcurrent[i] = 0;
	}

	for(i = 0; i <= totalframes; i++) {
		for(j = 0; j < 20; j++) {
			lrutable[i][j] = 0;
			opttable[i][j] = 0;
		}
	}

	for(i = 0; i < 20; i++) {
		refstr[i] = (rand() % 5) + 1;
	}

	printf("\nnumber of frames: %d\nreference string: ", totalframes);
	for(i = 0; i < 20; i++) {
		printf("%d ", refstr[i]);
	}
	printf("\n\n");

	for(j = 0; j < 20; j++) {
		lrufaultflag = 1;
		optfaultflag = 1;

		for(i = 0; i < totalframes; i++) {
			if(lrucurrent[i] == refstr[j]) {
				lrufaultflag = 0;
			}

			if(optcurrent[i] == refstr[j]) {
				optfaultflag = 0;
			}
		}

		if(lrufaultflag) {
			replacepage = lru(j, totalframes, lrucurrent, refstr);

			for(i = 0; i < totalframes; i++) {
				if(lrucurrent[i] == replacepage) {
					lrucurrent[i] = refstr[j];
					break;
				}
			}

			lrutable[totalframes][j] = '*';
			lrufaults++;
		}

		if(optfaultflag) {
			replacepage = opt(j, totalframes, optcurrent, refstr);

			for(i = 0; i < totalframes; i++) {
				if(optcurrent[i] == replacepage) {
					optcurrent[i] = refstr[j];
					break;
				}
			}

			opttable[totalframes][j] = '*';
			optfaults++;
		}

		for(i = 0; i < totalframes; i++) {
			lrutable[i][j] = lrucurrent[i];
			opttable[i][j] = optcurrent[i];
		}
	}

	printf("REPORT: LRU\nFAULTS: %d\n\n  ", lrufaults);

	for(i = 0; i < 20; i++) {
		printf("%d ", refstr[i]);
	}
	printf("\n\n");

	for(i = 0; i < totalframes; i++) {
		printf("0 ");
		for(j = 0; j < 20; j++) {
			printf("%d ", lrutable[i][j]);
		}
		printf("\n");
	}

	printf("\n  ");
	for(j = 0; j < 20; j++) {
		lrutable[totalframes][j] == '*' ? printf("* ") : printf("  ");
	}
	printf("\n\n");

	printf("REPORT: OPT\nFAULTS: %d\n\n  ", optfaults);
	for(i = 0; i < 20; i++) {
		printf("%d ", refstr[i]);
	}
	printf("\n\n");

	for(i = 0; i < totalframes; i++) {
		printf("0 ");
		for(j = 0; j < 20; j++) {
			printf("%d ", opttable[i][j]);
		}
		printf("\n");
	}

	printf("\n  ");
	for(j = 0; j < 20; j++) {
		opttable[totalframes][j] == '*' ? printf("* ") : printf("  ");
	}
	printf("\n");

	return(0);
}

int gettotalframes()
{
	int frames = 0;

	printf("Please enter a number between 1 and 9: ");
	fflush(stdout);
	frames = getchar();
	frames -= 48;

	return frames;
}

int lru(int refindex, int frames, int table[], int ref[])
{
	int indexfound[frames];
	int lruindex = refindex;
	int lru = 0;
	int i = 0;
	int j = 0;
	
	for(i = 0; i < frames; i++) {
		indexfound[i] = 0;
	}

	for(i = 0; i < frames; i++) {
		if(table[i] == 0) {
			return lru;
		}
	}

	for(i = 0; i < refindex; i++) {
		for(j = 0; j < frames; j++) {
			if(table[j] == ref[i]) {
				indexfound[j] = i;
			}
		}
	}

	for(i = 0; i < frames; i++) {
		if(indexfound[i] < lruindex) {
			lruindex = indexfound[i];
		}
	}

	for(i = 0; i < frames; i++) {
		if(indexfound[i] == lruindex) {
			lru = table[i];
		}
	}

	return lru;
}

int opt(int refindex, int frames, int *table, int *ref)
{
	int indexfound[frames];
	int optindex = refindex;
	int opt = 0;
	int i = 0;
	int j = 0;
	
	for(i = 0; i < frames; i++) {
		indexfound[i] = 21;
	}

	for(i = 0; i < frames; i++) {
		if(table[i] == 0) {
			return opt;
		}
	}

	for(i = (refindex + 1); i < 20; i++) {
		for(j = 0; j < frames; j++) {
			if(table[j] == ref[i] && indexfound[j] == 21) {
				indexfound[j] = i;
			}
		}
	}

	for(i = 0; i < frames; i++) {
		if(indexfound[i] > optindex) {
			optindex = indexfound[i];
		}
	}

	for(i = 0; i < frames; i++) {
		if(indexfound[i] == optindex) {
			opt = table[i];
		}
	}

	return opt;
}

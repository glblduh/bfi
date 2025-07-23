#include <stdio.h>

#define TAPE_SIZE 30000
#define MAX_CELL_VALUE 255

int tape[TAPE_SIZE] = {0};
int pointerIndex = 0;

char commandList[50000] = {0};
int commandListRepetition[sizeof(commandList)/sizeof(char)] = {0};

int unpairedLoopIndex[1000] = {0};
int pairedStartLoopIndex[1000] = {0};
int pairedEndLoopIndex[sizeof(pairedStartLoopIndex)/sizeof(int)] = {0};
int pairedLoopCount = 0;

void interpret();

int main(int argc, char* argv[]) {

	if (argc < 2) {
		printf("No file to open\n");
		return 1;
	}

	FILE *srcf = fopen(argv[1], "r");

	if (srcf == NULL) {
		printf("Cannot open file\n");
		return 1;
	}

	char currentCommand;
	int unpairedLoopCount = 0;
	for (int i=0;i<(sizeof(commandList)/sizeof(char));i++) {
		currentCommand = fgetc(srcf);
		if (currentCommand == EOF) {
			break;
		}

		if (
		currentCommand != '>' && 
		currentCommand != '<' && 
		currentCommand != '+' && 
		currentCommand != '-' && 
		currentCommand != ',' && 
		currentCommand != '.' && 
		currentCommand != '[' && 
		currentCommand != ']' 
		) {
			i--;
			continue;
		}

		if (i > 0 && currentCommand == commandList[i-1] && currentCommand != '[' && currentCommand != ']') {
			commandListRepetition[i-1]++;
			i--;
			continue;
		}

		if (currentCommand == '[') {
			unpairedLoopIndex[unpairedLoopCount] = i;
			unpairedLoopCount++;
		}

		if (currentCommand == ']') {
			pairedStartLoopIndex[pairedLoopCount] = unpairedLoopIndex[unpairedLoopCount-1];
			pairedEndLoopIndex[pairedLoopCount] = i;
			unpairedLoopCount--;
			pairedLoopCount++;
		}

		commandList[i] = currentCommand;
		commandListRepetition[i]++;
	}

	fclose(srcf);

	if (unpairedLoopCount != 0) {
		printf("Imbalanced loop\n");
		return 1;
	}

	interpret();
	return 0;
}

void interpret() {
	if (commandList[0] == 0) {
		return;
	}

	for (int i=0;i<(sizeof(commandList)/sizeof(char));i++) {
		switch (commandList[i]) {
			case '>':
				pointerIndex += commandListRepetition[i];
				break;
			case '<':
				pointerIndex -= commandListRepetition[i];
				break;
			case '+':
				tape[pointerIndex] += commandListRepetition[i];
				if (tape[pointerIndex] > MAX_CELL_VALUE) tape[pointerIndex] -= MAX_CELL_VALUE;
				break;
			case '-':
				tape[pointerIndex] -= commandListRepetition[i];
				if (tape[pointerIndex] < 0) tape[pointerIndex] += MAX_CELL_VALUE;
				break;
			case ',':
				for (int j=0;j<commandListRepetition[i];j++) {
					tape[pointerIndex] = fgetc(stdin);
				}
				break;
			case '.':
				for (int j=0;j<commandListRepetition[i];j++) {
					fputc(tape[pointerIndex], stdout);
				}
				break;
			case '[':
				if (tape[pointerIndex] == 0) {
					for (int j=0;j<pairedLoopCount;j++) {
						if (i != pairedStartLoopIndex[j]) continue;
						i = pairedEndLoopIndex[j];
						break;
					}
				}
				break;
			case ']':
				if (tape[pointerIndex] != 0) {
					for (int j=0;j<pairedLoopCount;j++) {
						if (i != pairedEndLoopIndex[j]) continue;
						i = pairedStartLoopIndex[j];
						break;
					}
				}
				break;
		}

		if (pointerIndex > TAPE_SIZE) {
			pointerIndex -= TAPE_SIZE;
		}
		if (pointerIndex < 0) {
			pointerIndex += TAPE_SIZE;
		}
	}
	return;
}

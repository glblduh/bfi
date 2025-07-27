#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

char commandList[50000] = {0};
int commandListRepetition[sizeof(commandList)/sizeof(char)] = {0};

int interpret();

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

		commandList[i] = currentCommand;
		commandListRepetition[i]++;

		if (currentCommand == '[') {
			unpairedLoopCount++;
		}

		if (currentCommand == ']') {
			unpairedLoopCount--;
		}
	}

	fclose(srcf);

	if (unpairedLoopCount != 0) {
		printf("Imbalanced loop\n");
		return 1;
	}

	if (interpret() != 0) {
    return 1;
  }
	return 0;
}

int interpret() {
	if (commandList[0] == 0) {
		return 0;
	}

  char code[500000] = {0};
  int codeCount = 0;
  int unpairedLoopIndex[1000] = {0};
  int unpairedLoopCount = 0;
  for (int i=0;i<(sizeof(commandList)/sizeof(char));i++) {
		switch (commandList[i]) {
			case '>':
        code[codeCount] = 0x48;
        codeCount++;
        code[codeCount] = 0x83;
        codeCount++;
        code[codeCount] = 0xc7;
        codeCount++;
        code[codeCount] = commandListRepetition[i] & 0xFF;
        codeCount++;
				break;
			case '<':
        code[codeCount] = 0x48;
        codeCount++;
        code[codeCount] = 0x83;
        codeCount++;
        code[codeCount] = 0xef;
        codeCount++;
        code[codeCount] = commandListRepetition[i] & 0xFF;
        codeCount++;
				break;
			case '+':
        code[codeCount] = 0x80;
        codeCount++;
        code[codeCount] = 0x07;
        codeCount++;
        code[codeCount] = commandListRepetition[i] & 0xFF;
        codeCount++;
				break;
			case '-':
        code[codeCount] = 0x80;
        codeCount++;
        code[codeCount] = 0x2f;
        codeCount++;
        code[codeCount] = commandListRepetition[i] & 0xFF;
        codeCount++;
				break;
			case ',':
				break;
			case '.':
        for (int j=0;j<commandListRepetition[i];j++) {
          code[codeCount] = 0x48;
          codeCount++;
          code[codeCount] = 0x89;
          codeCount++;
          code[codeCount] = 0xfe;
          codeCount++;
          code[codeCount] = 0x57;
          codeCount++;
          code[codeCount] = 0xb8;
          codeCount++;
          code[codeCount] = 0x01;
          codeCount++;
          code[codeCount] = 0x00;
          codeCount++;
          code[codeCount] = 0x00;
          codeCount++;
          code[codeCount] = 0x00;
          codeCount++;
          code[codeCount] = 0xbf;
          codeCount++;
          code[codeCount] = 0x01;
          codeCount++;
          code[codeCount] = 0x00;
          codeCount++;
          code[codeCount] = 0x00;
          codeCount++;
          code[codeCount] = 0x00;
          codeCount++;
          code[codeCount] = 0xba;
          codeCount++;
          code[codeCount] = 0x01;
          codeCount++;
          code[codeCount] = 0x00;
          codeCount++;
          code[codeCount] = 0x00;
          codeCount++;
          code[codeCount] = 0x00;
          codeCount++;
          code[codeCount] = 0x0f;
          codeCount++;
          code[codeCount] = 0x05;
          codeCount++;
          code[codeCount] = 0x5f;
          codeCount++;
        }
				break;
			case '[':
        code[codeCount] = 0x80;
        codeCount++;
        code[codeCount] = 0x3f;
        codeCount++;
        code[codeCount] = 0x00;
        codeCount++;
        code[codeCount] = 0x0f;
        codeCount++;
        code[codeCount] = 0x84;
        codeCount++;
        code[codeCount] = 0x00;
        codeCount++;
        code[codeCount] = 0xff;
        codeCount++;
        code[codeCount] = 0xff;
        codeCount++;
        code[codeCount] = 0xff;
        codeCount++;

        unpairedLoopIndex[unpairedLoopCount] = codeCount;
        unpairedLoopCount++;
	 			break;
			case ']':
        int loopPairAddr = unpairedLoopIndex[unpairedLoopCount-1];

        code[codeCount] = 0x80;
        codeCount++;
        code[codeCount] = 0x3f;
        codeCount++;
        code[codeCount] = 0x00;
        codeCount++;
        code[codeCount] = 0x0f;
        codeCount++;
        code[codeCount] = 0x85;
        codeCount++;
        code[codeCount] = 0x00;
        codeCount++;
        code[codeCount] = 0x00;
        codeCount++;
        code[codeCount] = 0x00;
        codeCount++;
        code[codeCount] = 0x00;
        codeCount++;

        int offset = codeCount - loopPairAddr; 
        int negativeOffset = offset * -1;

        code[codeCount-4] = negativeOffset & 0xFF;
        code[codeCount-3] = (negativeOffset >> 8) & 0xFF;
        code[codeCount-2] = (negativeOffset >> 16) & 0xFF;
        code[codeCount-1] = (negativeOffset >> 24) & 0xFF;

        code[loopPairAddr-4] = offset & 0xFF;
        code[loopPairAddr-3] = (offset >> 8) & 0xFF;
        code[loopPairAddr-2] = (offset >> 16) & 0xFF;
        code[loopPairAddr-1] = (offset >> 24) & 0xFF;
        unpairedLoopCount--;
				break;
		}
	}

  code[codeCount] = 0xc3;
  codeCount++;

  void (*bin) (void *memory);

  bin = mmap(0, sizeof(code), PROT_READ|PROT_WRITE|PROT_EXEC, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
  if (bin == MAP_FAILED) {
    printf("Cannot allocate page memory\n");
    return 1;
  }

  memcpy(bin, code, sizeof(code));

  void *memory = malloc(10*1000*1000);

  bin(memory);

  free(memory);

	return 0;
}

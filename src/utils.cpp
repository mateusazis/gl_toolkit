#include <cstdio>
#include <cstring>

extern void readFile(const char* path, char* dest){
	dest[0] = '\0';
	const int MAX_SIZE = 2048;
	char line[300] = "";
	FILE* f = fopen(path, "r");
	while (fgets(line, MAX_SIZE, f)){
		strcat(dest, line);
	}
	fclose(f);
}
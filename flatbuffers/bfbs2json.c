/*
 * Copyright (C) 2016-2023 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdlib.h>
#include <stdio.h>

#include "reflection_json_printer.h"

static const size_t BLOCKSIZE = 32768;

void *alloc(void *ptr, size_t size)
{
	void *result = realloc(ptr, size);
	if (result == NULL) {
		fprintf(stderr, "Out of memory\n");
		exit(1);
	}
	return result;
}

static char *readfile(const char *filename, size_t *length)
{
	FILE *file;
	char *result;
	size_t size, pos, rsz;

	result = NULL;
	if (filename[0] == '-' &&  filename[1] == 0)
		filename = "/dev/stdin";

	file = fopen(filename, "r");
	if (file == NULL) {
		fprintf(stderr, "Can't open file: %s\n", filename);
		exit(1);
	}

	size = BLOCKSIZE;
	pos = 0;
	result = NULL;
	for(;;) {
		result = alloc(result, size);
		rsz = fread(&result[pos], 1, size - pos, file);
		if (rsz == 0 && ferror(file)) {
			fprintf(stderr, "Error while reading %s\n", filename);
			exit(1);
		}
		pos += rsz;
		if (pos < size) {
			fclose(file);
			result = alloc(result, pos + 1);
			result[pos] = 0;
			if (length != NULL)
				*length = pos;
			return result;
		}
		size += BLOCKSIZE;
	}
}

int main(int ac, char **av)
{
	char *buf;
	size_t sz;

	if (!*++av || !strcmp(*av,"-h") || !strcmp(*av,"--help") || av[1]) {
		fprintf(stderr, "usage: %s bfbs-file\n", av[-1]);
		return 1;
	}
	buf = readfile(*av, &sz);
	reflection_print_json(0, buf, sz);
	return 0;
}

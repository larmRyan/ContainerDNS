#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

void get_directories(char* parent_dir);
int get_container_count(char* parent_dir);
char* remove_spaces(char* input_string);
void get_dns_entry();
void write_to_hosts(FILE* read_path);
void remove_entry(char *file, char *ip);
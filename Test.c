#include <stdio.h>
#include <string.h>


void create_data(char *data, char *ip, char *url) {
  char *delim = "*";
  

  strncat(data, ip, 255);
  strncat(data, delim, 1);
  strncat(data, url, 255);
}

int main() {

  printf("Start\n");
  char ip[255] = "127.0.0.1";
  char url[255] = "google.com";
  char data[255];
  create_data(data, ip, url);
  printf("DATA: %s\n", data);
  return 0;

}
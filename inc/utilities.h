#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <iostream>


#define FRED        "\033[31m"
#define FGREEN      "\033[32m"
#define NONE        "\033[0m"

#define DEBUG() printf(FGREEN"DEBUG: Function: %s \tLine Number: %d", __FUNCTION__, __LINE__); printf(NONE"\n");

#define ERROR(x) printf(FRED"ERROR: Function: %s \tLine Number: %d \tError: %s", __FUNCTION__, __LINE__, x); printf(NONE"\n");
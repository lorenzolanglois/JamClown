#include <time.h>
#include <stdlib.h>

#include "engine.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));
    game();
    return 0;
}

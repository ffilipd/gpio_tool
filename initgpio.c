#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define EXPORT "/export"
#define GPIODIR "/sys/class/gpio"
#define GPIOPIN "/sys/class/gpio/gpio"
#define SUF_VAL "/value"
#define SUF_DIR "/direction"

#define OUT "out"
#define HIGH "1"
#define LOW "0"

char *pins[] = { "66", "67", "68", "69" }; // Edit this line according to your GPIO pins
char *direction[] = { "out", "out", "out", "out"}; // Set "in" or "out", following the same index as GPIO pins
char path[33];
int fd;

void exportPin(char **i) {
    snprintf(path, sizeof(path), "%s%s", GPIOPIN, EXPORT);     
    fd = open(path, O_WRONLY);
    if (fd == -1) {
        printf("Error opening /export\n");
        exit(1);
    }
    if (write(fd, *i, 2) != 2) {
        printf("Error writing /export\n");
        exit(1);
    }
    close(fd);
}

void setDirection(char **i, char d) {
    snprintf(path, sizeof(path), "%s%s%s",GPIOPIN, *i, SUF_DIR);
    fd = open(path, O_WRONLY);
    if (fd == -1) {
        printf("Error opening /direction\n");
        exit(1);
    }
    if (write(fd, *i, 3) != 3) {
        printf("Error writing /direction\n");
        exit(1);
    }
    close(fd);
}

void setPinValue(char **i, char v) {
    snprintf(path, sizeof(path), "%s%s%s",GPIOPIN, *i, SUF_VAL);
    fd = open(path, O_WRONLY);
    if (fd == -1) {
        printf("Error opening /value\n");
        exit(1);
    }
    if (write(fd, v, 1) != 1) {
        printf("Error writing /value\n");
        exit(1);
    }
    close(fd);
}
int main() {
    
    for (int i = 0; i < (sizeof(pins) / sizeof(*pins)); i++) {
        exportPin(&pins[i]);
        setDirection(&pins[i], &direction[i]);
        if (strcmp(direction[i], "out") == 0) {
            setPinValue(&pins[i], LOW);
        }

    }

    return(0);
}



#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#define EXPORT "/export"
#define GPIODIR "/sys/class/gpio"
#define GPIOPIN "/sys/class/gpio/gpio"
#define SUF_VAL "/value"
#define SUF_DIR "/direction"


char path[33];

int fd;

void exportPin(char *i) {
    snprintf(path, sizeof(path), "%s%s", GPIODIR, EXPORT);     
    fd = open(path, O_WRONLY);

    if (fd == -1) {
        printf("Error opening /export\n");
        exit(1);
    }
    if (write(fd, i, 2) != 2) {
        printf("Error writing /export\n");
        exit(1);
    }

    printf("init pin: %s\n", i);
    close(fd);
}

void setDirection(char *i, char *d) {
    
    snprintf(path, sizeof(path), "%s%s%s",GPIOPIN, i, SUF_DIR);
    fd = open(path, O_WRONLY);

    if (fd == -1) {
        printf("Error opening /direction\n");
        exit(1);
    }
    if (write(fd, d, 3) != 3) {
        printf("Error writing /direction\n");
        exit(1);
    }

    printf("pin: %s direction: %s\n", i, d);
    close(fd);
}

void setPinValue(char *i, char *v) {
    
    snprintf(path, sizeof(path), "%s%s%s",GPIOPIN, i, SUF_VAL);
    fd = open(path, O_WRONLY);

    if (fd == -1) {
        printf("Error opening /value\n");
        exit(1);
    }
    if (write(fd, v, 1) != 1) {
        printf("Error writing /value\n");
        exit(1);
    }

    printf("pin: %s value: %s\n", i, v);
    close(fd);
}
int main(int argc, char *argv[]) {
    
    while ((++argv)[0]) {
        if (argv[0][0] == '-'){
            switch (argv[0][1]) {
                default:
                    printf("unknown option -%c\n\n", argv[0][1]);
                    break;

                case 'i':
                    if (argv[1][0] != '-' && (int) isdigit(argv[1][0])){
                        exportPin(argv[1]);
                    } else {
                        printf("invalid value for -i '%s', integer was expected\n", argv[1]);
                    }
                    if (argv[2] && strcmp(argv[2], "in") == 0 || argv[2] && strcmp(argv[2], "out") == 0) {
                        setDirection(argv[1], argv[2]);
                    } else {
                        printf("invalid value for direction '%s', expected 'in' or 'out'\n", argv[2]); 
                    }
                    break;

                case 'd':
                    if (argv[1][0] != '-' && (int) isdigit(argv[1][0])) {
                        if (strcmp(argv[2], "in") == 0 || strcmp(argv[2], "out") == 0) {
                            setDirection(argv[1], argv[2]);
                        } else {
                            printf("invalid value for direction '%s', expected 'in' or 'out'\n", argv[2]);
                        }
                    } else {
                        printf("invalid pin value for -d '%s', integer was expected\n", argv[1]);
                    }
                    break;

                case 'v':
                    if (argv[1][0] != '-' && (int) isdigit(argv[1][0])) {
                        if (strcmp(argv[2], "1") == 0 || strcmp(argv[2], "0") == 0) {
                            setPinValue(argv[1], argv[2]);
                        } else {
                            printf("invalid value '%s', expected '1' or '0'\n", argv[2]);
                        }
                    } else {
                        printf("invalid value for -v '%s', integer was expected\n", argv[1]);
                    }
                    break;

                case 'h':
                    printf("\n\nUsage:\n");
                    printf("\tOPTIONS");
                    printf("\n\t\t-i Initialize GPIO pin. Expects pin number(int) as argument, optionally append direction('in'/'out') after pin number.\n");
                    printf("\n\t\t-d Set direction of GPIO pin. Expects pin number(int) as first argument, and direction('in'/'out') as second argument.\n");
                    printf("\n\t\t-v Set value of GPIO pin. Expects pin number(int) as first argument, and value(int '1'/'0') as second argument.\n\n");
                    printf("\tEXAMPLES");
                    printf("\n\t\t\"gpio -i 2\"\n");
                    printf("\n\t\t\"gpio -i 25 out\"\n");
                    printf("\n\t\t\"gpio -d 12 in\"\n");
                    printf("\n\t\t\"gpio -v 10 1\"\n");
                    break;
            }
        }
    }
    return(0);
}



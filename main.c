#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BRIGHTNESS_FILE "/sys/class/backlight/amdgpu_bl0/brightness"
#define MAX_BRIGHTNESS  ((1 << 8) - 1)

int read_current_brightness() {
    int   curbr  = -1;
    FILE *brfile = fopen(BRIGHTNESS_FILE, "r");

    if (brfile == 0x00) {
        fprintf(stderr, "Failed to open brightness file.\n");
        return -1;
    }

    if (fscanf(brfile, "%d", &curbr) != 1) {
        fprintf(stderr, "Failed to read brightness value.\n");
        curbr = -1;
        fclose(brfile);
    }
    fclose(brfile);
    return curbr;
}

int set_brightness(int nbr, bool flag) {
    FILE *brfile = fopen(BRIGHTNESS_FILE, "w");
    int   curbr, newbr;

    if (brfile == 0x00) {
        fprintf(stderr, "Failed to open brightness file.\n");
        return -1;
    }

    curbr = read_current_brightness();
    if (curbr == -1) {
        fprintf(stderr, "Failed to read current brightness.\n");
        fclose(brfile);
        return -1;
    }

    newbr = flag ? (curbr + nbr) : (curbr - nbr);
    if (flag)
        if ((MAX_BRIGHTNESS - newbr) <= 10) newbr = MAX_BRIGHTNESS;
    if (flag)
        if ((MAX_BRIGHTNESS - curbr) <= 10) newbr = MAX_BRIGHTNESS;
    fprintf(brfile, "%d", newbr);
    fclose(brfile);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("%d\n", MAX_BRIGHTNESS);
        printf("Usage: %s <option> <val>\n", argv[0]);
        printf("Options:\n");
        printf("  -i   Increase brightness\n");
        printf("  -d   Decrease brightness\n");
        return 1;
    }

    bool flag;
    if (strcmp(argv[1], "-d") == 0) {
        flag = false;
    } else if (strcmp(argv[1], "-i") == 0) {
        flag = true;
    } else {
        fprintf(stderr, "Invalid option: %s\n", argv[1]);
        return 1;
    }

    int val = atoi(argv[2]);
    if (val <= 0) {
        fprintf(stderr, "Invalid brightness value: %s\n", argv[2]);
        return 1;
    }

    if (set_brightness(val, flag) == -1) {
        fprintf(stderr, "Failed to set brightness.\n");
        exit(1);
    }
    return 0;
}
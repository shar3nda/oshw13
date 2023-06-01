#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BASE_FILENAME "a"
#define DIRECTORY_NAME "test_dir"

void error_and_exit(char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

int main(void) {
    char file_name[3] = BASE_FILENAME;
    char link_name[3] = "aa";
    int depth = 0;
    int fd;

    // Создаем директорию
    if(mkdir(DIRECTORY_NAME, 0755) < 0)
        error_and_exit("Failed to create directory");

    // Переходим в директорию
    if(chdir(DIRECTORY_NAME) < 0)
        error_and_exit("Failed to change directory");

    // Создаем файл
    if((fd = open(file_name, O_CREAT | O_RDWR, 0666)) < 0)
        error_and_exit("Failed to create file");
    close(fd);

    while(1) {
        // Создаем символическую ссылку
        if(symlink(file_name, link_name) < 0)
            error_and_exit("Failed to create symlink");

        // Проверяем, что мы не уперлись в ограничение рекурсии
        if((fd = open(link_name, O_RDWR)) < 0) {
            printf("Reached recursion depth: %d\n", depth);
            break;
        }
        close(fd);
        depth++;

        // Переходим по ссылке
        strcpy(file_name, link_name);
        // Генерируем имя следующей ссылки
        if(link_name[1] == 'z') {
            link_name[0]++;
            link_name[1] = 'a';
        } else {
            link_name[1]++;
        }
    }

    return EXIT_SUCCESS;
}

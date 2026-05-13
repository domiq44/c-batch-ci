#include "logs.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void grep_lines(FILE* f, const char* pattern) {
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), f)) {
        if (strstr(buffer, pattern)) {
            printf("%s", buffer);
        }
    }
}

static void print_upper(FILE* f) {
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), f)) {
        for (char* p = buffer; *p; p++) {
            *p = toupper((unsigned char)*p);
        }
        printf("%s", buffer);
    }
}

static int count_lines(FILE* f) {
    int count = 0;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), f)) {
        count++;
    }
    return count;
}

static int count_words(FILE* f) {
    int words = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), f)) {
        char* p = buffer;
        int in_word = 0;

        while (*p) {
            if ((*p == ' ') || (*p == '\t') || (*p == '\n')) {
                in_word = 0;
            } else if (!in_word) {
                in_word = 1;
                words++;
            }
            p++;
        }
    }
    return words;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        log_error("Usage: %s <fichier> <mode>", argv[0]);
        return 1;
    }

    const char* filename = argv[1];
    const char* mode = argv[2];

    // DEBUG AVANT LES INFO
    log_debug("Arguments reçus : fichier=%s, mode=%s", filename, mode);
    log_debug("Mode sélectionné : %s", mode);

    log_info("Lecture du fichier %s en mode %s", filename, mode);

    log_debug("Ouverture du fichier %s", filename);
    FILE* f = fopen(filename, "r");
    if (!f) {
        log_error("Impossible d'ouvrir le fichier %s", filename);
        return 2;
    }

    int result;

    if (strcmp(mode, "count") == 0) {
        log_debug("Début du mode count");
        result = count_lines(f);
        log_info("Nombre de lignes : %d", result);
    } else if (strcmp(mode, "words") == 0) {
        log_debug("Début du mode words");
        result = count_words(f);
        log_info("Nombre de mots : %d", result);
    } else if (strcmp(mode, "upper") == 0) {
        log_debug("Début du mode upper");
        print_upper(f);
        log_info("Conversion en majuscules terminée");
    } else if (strcmp(mode, "grep") == 0) {
        if (argc < 4) {
            log_error("Mode grep : motif manquant");
            fclose(f);
            return 4;
        }
        const char* pattern = argv[3];
        log_debug("Début du mode grep avec motif : %s", pattern);
        grep_lines(f, pattern);
        log_info("Filtrage grep terminé (motif = %s)", pattern);
    } else {
        log_error("Mode inconnu : %s", mode);
        fclose(f);
        return 3;
    }

    fclose(f);
    return 0;
}

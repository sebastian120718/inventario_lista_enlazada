#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NAME_LEN 40

struct Node {
    int id;
    char name[NAME_LEN];
    int stock;
    struct Node *next;
};

/* ---------- Helpers de entrada ---------- */
static void trim_newline(char *s) {
    size_t L = strlen(s);
    if (L && s[L-1] == '\n') s[L-1] = '\0';
}

static void read_line(char *buffer, int size) {
    if (fgets(buffer, size, stdin) == NULL) { buffer[0] = '\0'; return; }
    trim_newline(buffer);
}

/* Lee entero con validación; repite hasta que sea entero y >= min_allowed */
static int read_int_valid(const char *prompt, int min_allowed) {
    char buf[64];
    long val;
    char *end;
    while (1) {
        if (prompt) printf("%s", prompt);
        read_line(buf, sizeof(buf));
        if (buf[0] == '\0') { printf("Entrada vacía, intente de nuevo.\n"); continue; }
        val = strtol(buf, &end, 10);
        if (*end != '\0') { printf("Entrada no numérica, intente de nuevo.\n"); continue; }
        if (val < min_allowed) { printf("El número debe ser >= %d.\n", min_allowed); continue; }
        return (int)val;
    }
}

static struct Node *create_node(int id, const char *name, int stock) {
    struct Node *n = malloc(sizeof(struct Node));
    if (!n) return NULL;
    n->id = id;
    strncpy(n->name, name, NAME_LEN);
    n->name[NAME_LEN-1] = '\0';
    n->stock = stock;
    n->next = NULL;
    return n;
}

static void free_list(struct Node *head) {
    struct Node *p = head;
    while (p) {
        struct Node *tmp = p;
        p = p->next;
        free(tmp);
    }
}

/* Busca nodo por id; devuelve puntero o NULL */
static struct Node *find_node(struct Node *head, int id) {
    struct Node *p = head;
    while (p) {
        if (p->id == id) return p;
        p = p->next;
    }
    return NULL;
}

static bool insert_front(struct Node **head_ref, int id, const char *name, int stock) {
    if (find_node(*head_ref, id)) return false;
    struct Node *n = create_node(id, name, stock);
    if (!n) return false;
    n->next = *head_ref;
    *head_ref = n;
    return true;
}

/* Inserta al final; evita IDs duplicados */
static bool insert_back(struct Node **head_ref, int id, const char *name, int stock) {
    if (find_node(*head_ref, id)) return false;
    struct Node *n = create_node(id, name, stock);
    if (!n) return false;
    if (*head_ref == NULL) {
        *head_ref = n;
    } else {
        struct Node *p = *head_ref;
        while (p->next) p = p->next;
        p->next = n;
    }
    return true;
}

/* Elimina por id; devuelve true si se eliminó */
static bool remove_product(struct Node **head_ref, int id) {
    struct Node *cur = *head_ref;
    struct Node *prev = NULL;
    while (cur) {
        if (cur->id == id) {
            if (prev) prev->next = cur->next;
            else *head_ref = cur->next;
            free(cur);
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

/* Actualiza stock por id; devuelve true si encontrado */
static bool set_stock(struct Node *head, int id, int new_stock) {
    struct Node *n = find_node(head, id);
    if (!n) return false;
    n->stock = new_stock;
    return true;
}
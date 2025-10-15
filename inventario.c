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

static void list_products(struct Node *head) {
    if (!head) { printf("Inventario vacío.\n"); return; }
    printf("\nID\tNombre\t\t\tStock\n");
    printf("------------------------------------------------\n");
    struct Node *p = head;
    while (p) {
        printf("%d\t%-20s\t%d\n", p->id, p->name, p->stock);
        p = p->next;
    }
}

/* Menú (impresión) */
static void print_menu(void) {
    printf("\n--- MENU - Inventario ---\n");
    printf("1. Insertar producto al inicio\n");
    printf("2. Insertar producto al final\n");
    printf("3. Buscar por ID y mostrar datos\n");
    printf("4. Actualizar stock por ID\n");
    printf("5. Eliminar producto por ID\n");
    printf("6. Listar inventario completo\n");
    printf("0. Salir\n");
    printf("Elija una opción: ");
}

int main(void) {
    struct Node *head = NULL;
    char name[NAME_LEN];

    while (1) {
        print_menu();
        int opt = read_int_valid(NULL, 0);

        if (opt == 0) break;

        if (opt == 1 || opt == 2) {
            int id = read_int_valid("Ingrese ID (entero >= 0): ", 0);
            if (find_node(head, id)) { printf("Error: ID %d ya existe.\n", id); continue; }
            while (1) {
                printf("Ingrese nombre (no vacío, max %d caracteres): ", NAME_LEN-1);
                read_line(name, NAME_LEN);
                if (name[0] == '\0') { printf("Nombre vacío, intente de nuevo.\n"); continue; }
                break;
            }
            int stock = read_int_valid("Ingrese stock (entero >= 0): ", 0);
            bool ok = (opt == 1) ? insert_front(&head, id, name, stock) : insert_back(&head, id, name, stock);
            if (ok) printf("Producto insertado correctamente.\n");
            else printf("Error al insertar producto.\n");
        } else if (opt == 3) {
            int id = read_int_valid("Ingrese ID a buscar: ", 0);
            struct Node *n = find_node(head, id);
            if (!n) printf("Producto con ID %d no encontrado.\n", id);
            else printf("Id: %d\nNombre: %s\nStock: %d\n", n->id, n->name, n->stock);
        } else if (opt == 4) {
            int id = read_int_valid("Ingrese ID a actualizar: ", 0);
            if (!find_node(head, id)) { printf("Producto con ID %d no existe.\n", id); continue; }
            int new_stock = read_int_valid("Ingrese nuevo stock (>= 0): ", 0);
            if (set_stock(head, id, new_stock)) printf("Stock actualizado.\n");
            else printf("Error al actualizar stock.\n");
        } else if (opt == 5) {
            int id = read_int_valid("Ingrese ID a eliminar: ", 0);
            if (remove_product(&head, id)) printf("Producto eliminado.\n");
            else printf("No se encontró producto con ID %d.\n", id);
        } else if (opt == 6) {
            list_products(head);
        } else {
            printf("Opción inválida. Intente de nuevo.\n");
        }
    }

    free_list(head);
    printf("Saliendo. Memoria liberada.\n");
    return 0;
}
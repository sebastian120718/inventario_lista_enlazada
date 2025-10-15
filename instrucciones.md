// explicacion

Este programa de C gestiona un inventario mediante una lista enlazada. Hay un único archivo inventario.c que implementa las operaciones básicas que solicitaron y un menú por consola para interactuar con el usuario. El código usa una estructura struct Node con los campos id, name[40], stock y un puntero next, y libera la memoria al finalizar.

// Copilacion
gcc inventario.c -o inventario
./inventario

//operaciones

-Insertar producto al inicio.
-Insertar producto al final.
-Buscar producto por ID y mostrar sus datos.
-Actualizar stock por ID.
-Eliminar producto por ID.
-Listar el inventario completo (orden actual de la lista).
-Entrada validada: IDs y stock como enteros (>= 0)
-Evita IDs duplicados. Libera memoria al salir.

ejemplo

Ejecuta ./inventario.
Elige 2 (insertar al final): ID 101, Nombre Manzana, Stock 50.
Elige 1 (insertar al inicio): ID 50, Nombre Pan, Stock 30.
Elige 6 para listar — verás primero Pan y luego Manzana.
/* Data initializtion routines */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <time.h>

# include "global.h"
# include "rand.h"

/* Function to initialize a population randomly */
void initialize_pop (population *pop)
{
    int i;
    for (i=0; i<popsize; i++)
    {
        initialize_ind (&(pop->ind[i]));
    }
    return;
}

/* Function to initialize an individual randomly */
void initialize_ind (individual *ind)
{
    // --- RCMDVRP: inicialización de rutas aleatorias con restricciones ---
    int clientes[MAX_NODES];
    int i, j, pos = 0;
    int carga = 0;
    double riesgo = 0.0;
    int capacidad = b; // o la capacidad correspondiente
    double riesgo_max = theta; // o el parámetro correspondiente
    int n_cli = n_customers;

    // Copiar clientes
    for (i = 0; i < n_cli; i++) clientes[i] = set_R[i];
    // Barajar clientes (Fisher-Yates)
    for (i = n_cli - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = clientes[i]; clientes[i] = clientes[j]; clientes[j] = tmp;
    }
    pos = 0;
    carga = 0;
    riesgo = 0.0;
    for (i = 0; i < n_cli; i++) {
        int c = clientes[i];
        int demanda = dm[c];
        double riesgo_cliente = sigma[c][0]; // o el cálculo adecuado
        if ((carga + demanda > capacidad) || (riesgo + riesgo_cliente > riesgo_max)) {
            ind->route[pos++] = -1; // Separador de ruta
            carga = 0;
            riesgo = 0.0;
        }
        ind->route[pos++] = c;
        carga += demanda;
        riesgo += riesgo_cliente;
    }
    ind->route_length = pos;
    // Rellenar el resto con -1 si se desea
    for (; pos < MAX_NODES; pos++) ind->route[pos] = -1;
    return;
}

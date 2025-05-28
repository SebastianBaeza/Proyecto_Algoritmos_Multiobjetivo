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
    int clientes[MAX_NODES];
    int i;
    int j = 0;
    int pos = 0;
    int carga = 0;
    double riesgo = 0.0;
    int capacidad = b;
    double riesgo_max = theta;
    int n_veh = 1;

    int cliente_anterior = 0;
    int separador = -1;

    for (i = 0; i < n_customers; i++) {
        clientes[i] = set_R[i];
    }

    for (i = n_customers - 1; i > 0; i--) {
        j = rand() % (i + 1);
        int tmp = clientes[i]; 
        clientes[i] = clientes[j]; 
        clientes[j] = tmp;
    }

    for (i = 0; i < n_customers; i++) {
        if (n_veh <= (n_vehicles * n_depots)){
            int c = clientes[i];
            int demanda = dm[c];
            double riesgo_cliente = demanda * d[cliente_anterior][c];
            if ((carga + demanda > capacidad) || (riesgo + riesgo_cliente > riesgo_max)) {
                ind->route[pos++] = separador; 
                separador -= 1;
                carga = 0;
                riesgo = 0.0;
                n_veh++;
            }
            ind->route[pos++] = c;
            carga += demanda;
            riesgo += riesgo_cliente;
            cliente_anterior = c; 
        } else {
            ind->constr[0] += dm[clientes[i]];
        }
    }
    ind->route_length = pos;
    return;
}

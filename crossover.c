/* Crossover routines */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"


int valueinarray(int val, int *arr, size_t n) {
    size_t i;
    for(i = 0; i < n; i++) {
        if(arr[i] == val)
            return 1;
    }
    return 0;
}

/* Function to cross two individuals */
void crossover (individual *parent1, individual *parent2, individual *child1, individual *child2)
{
/*     if (nreal!=0)
    {
        realcross (parent1, parent2, child1, child2);
        }
        if (nbin!=0)
        {
            bincross (parent1, parent2, child1, child2);
            } */
    realcross (parent1, parent2, child1, child2);
    return;
}

/* Routine for real variable SBX crossover */
void realcross (individual *parent1, individual *parent2, individual *child1, individual *child2)
{
/*     printf("\n--- Performing Sinusoidal Motion crossover ---\n"); */
    int i, j;
    individual *parent1_n;
    individual *parent2_n;
    individual *child1_n;
    individual *child2_n;

    int indice1 = 0;
    int indice2 = 0;
    int counter;
    int internal_counter1 = 0;
    int internal_counter2 = 0;

    int carga = 0;
    double riesgo = 0.0;
    int capacidad = b; 
    double riesgo_max = theta;
    int child_route_pos = 0;
    separador = -1;
    int cliente_anterior = 0;
    int demanda;

    if (randomperc() <= pcross_real)
    {
        nrealcross++;
        parent1_n = (individual *)malloc(sizeof(individual));
        parent2_n = (individual *)malloc(sizeof(individual));
        child1_n = (individual *)malloc(sizeof(individual));
        child2_n = (individual *)malloc(sizeof(individual));
        parent1_n->route = (int *)malloc(MAX_NODES * sizeof(int));
        parent2_n->route = (int *)malloc(MAX_NODES * sizeof(int));
        child1_n->route = (int *)malloc(MAX_NODES * sizeof(int));
        child2_n->route = (int *)malloc(MAX_NODES * sizeof(int));

/*         // Copia rutas de los padres a los temporales */
/*         printf("Parent1 route: ");
        for (i = 0; i < parent1->route_length; i++) {
            printf("%d ", parent1->route[i]);
        }
        printf("\nParent2 route: ");
        for (i = 0; i < parent2->route_length; i++) {
            printf("%d ", parent2->route[i]);
        }
        printf("\n"); */

        for (i = 0; i < n_customers; i++) {
            if (parent1->route[internal_counter1] > 0) {
                parent1_n->route[i] = parent1->route[internal_counter1];
            } else {
                i--;
            }
            internal_counter1++;
        }
        for (i = 0; i < n_customers; i++) {
            if (parent2->route[internal_counter2] > 0) {
                parent2_n->route[i] = parent2->route[internal_counter2];
            } else {
                i--;
            }
            internal_counter2++;
        }

        for (i=0; i<nreal; i++)
        {
            if (randomperc()<=0.5 )
            /* if (randomperc()<=1.0) */
            {
                for (counter = 0; counter < n_customers; counter++)
                {
                    /* printf("counter=%d, parent1_n->route[counter]=%d, parent2_n->route[counter]=%d\n", counter, parent1_n->route[counter], parent2_n->route[counter]); */
                    if (valueinarray(parent1_n->route[counter],child1_n->route, n_customers) == 1){
                        child2_n->route[indice2] = parent1_n->route[counter];
/*                         printf("child2_n->route[%d] = %d\n", indice2, parent1_n->route[counter]); */
                        indice2++;
                    } else {
                        child1_n->route[indice1] = parent1_n->route[counter];
/*                         printf("child1_n->route[%d] = %d\n", indice1, parent1_n->route[counter]); */
                        indice1++;
                    }

                    if (valueinarray(parent2_n->route[counter],child1_n->route, n_customers) == 1){
                        child2_n->route[indice2] = parent2_n->route[counter];
/*                         printf("child2_n->route[%d] = %d\n", indice2, parent2_n->route[counter]); */
                        indice2++;
                    } else {
                        child1_n->route[indice1] = parent2_n->route[counter];
/*                         printf("child1_n->route[%d] = %d\n", indice1, parent2_n->route[counter]); */
                        indice1++;
                    }
                }

/*                 printf("child1_n route after assignment: ");
                for (j = 0; j < n_customers; j++) printf("%d ", child1_n->route[j]);
                printf("\nchild2_n route after assignment: ");
                for (j = 0; j < n_customers; j++) printf("%d ", child2_n->route[j]);
                printf("\n"); */
/* 
                // Construcción de la ruta final para child1 */
                child1->route_length = 0;
                child_route_pos = 0;
                carga = 0;
                riesgo = 0.0;
                cliente_anterior = 0;
                separador = -1;
                for (j = 0; j < n_customers; j++) {
                    cliente = child1_n->route[j];
                    demanda = dm[cliente];

                    if (carga + demanda > capacidad || riesgo + demanda * d[cliente_anterior][cliente] > riesgo_max) {
                        child1->route[child_route_pos++] = separador;
                        /* printf("child1->route[%d] = separador (%d)\n", child_route_pos-1, separador); */
                        separador -= 1; 
                        child1->route_length++;
                        carga = 0; 
                        riesgo = 0.0; 
                    }

                    child1->route[child_route_pos++] = cliente;
/*                     printf("child1->route[%d] = %d\n", child_route_pos-1, cliente); */
                    riesgo += demanda * d[cliente_anterior][cliente];
                    child1->route_length++;
                    carga += demanda;
                    cliente_anterior = cliente; 
                }
/*                 printf("\n"); */

/*                 // Construcción de la ruta final para child2 */
                carga = 0;
                riesgo = 0.0;
                child_route_pos = 0;
                child2->route_length = 0;
                separador = -1;
                cliente_anterior = 0;
                for (j = 0; j < n_customers; j++) {
                    cliente = child2_n->route[j];
                    demanda = dm[cliente];

                    if (carga + demanda > capacidad || riesgo + demanda * d[cliente_anterior][cliente] > riesgo_max) {
                        child2->route[child_route_pos++] = separador;
/*                         printf("child2->route[%d] = separador (%d)\n", child_route_pos-1, separador); */
                        separador -= 1;
                        child2->route_length++;
                        carga = 0;
                        riesgo = 0.0;
                    }

                    child2->route[child_route_pos++] = cliente;
/*                     printf("child2->route[%d] = %d\n", child_route_pos-1, cliente); */
                    riesgo += demanda * d[cliente_anterior][cliente];
                    child2->route_length++;
                    carga += demanda;
                    cliente_anterior = cliente; 
                }
            }
 /*            else
            {
                printf("No crossover for real variable %d, copying xreal\n", i);
                child1->xreal[i] = parent1->xreal[i];
                child2->xreal[i] = parent2->xreal[i];
            } */
            else
            {
                printf("No crossover performed, copying parent1 to child1 and parent2 to child2\n");
                child1->route_length = parent1->route_length;
                child2->route_length = parent2->route_length;
                for (i = 0; i < parent1->route_length; i++)
                    child1->route[i] = parent1->route[i];
                for (i = 0; i < parent2->route_length; i++)
                    child2->route[i] = parent2->route[i];
            }
        }
        free(parent1_n->route);
        free(parent2_n->route);
        free(child1_n->route);
        free(child2_n->route);
        free(parent1_n);
        free(parent2_n);
        free(child1_n);
        free(child2_n);

    }
    /*     else
    {
        printf("No crossover performed, copying xreal\n");
        for (i=0; i<nreal; i++)
        {
            child1->xreal[i] = parent1->xreal[i];
            child2->xreal[i] = parent2->xreal[i];
        }
    } */
    else
    {
        printf("No crossover performed, copying parent1 to child1 and parent2 to child2\n");
        child1->route_length = parent1->route_length;
        child2->route_length = parent2->route_length;
        for (i = 0; i < parent1->route_length; i++)
            child1->route[i] = parent1->route[i];
        for (i = 0; i < parent2->route_length; i++)
        child2->route[i] = parent2->route[i];
    }
/*     printf("Final child1 route: ");
    for (i=0;i < child1->route_length; i++)
    {
        printf("%d ", child1->route[i]);
    }
    printf("\nChild 1 route length: %d\n", child1->route_length);

    printf("Final child2 route: ");
    for (i=0;i < child2->route_length; i++)
    {
        printf("%d ", child2->route[i]);
    }
    printf("\nChild 2 route length: %d\n", child2->route_length);
    printf("--- Sinusoidal Motion crossover completed ---\n"); */
    return;
}
/* Routine for real variable SBX crossover */
/* void realcross (individual *parent1, individual *parent2, individual *child1, individual *child2)
{
    int i;
    double rand;
    double y1, y2, yl, yu;
    double c1, c2;
    double alpha, beta, betaq;
    if (randomperc() <= pcross_real)
    {
        nrealcross++;
        for (i=0; i<nreal; i++)
        {
            if (randomperc()<=0.5 )
            {
                if (fabs(parent1->xreal[i]-parent2->xreal[i]) > EPS)
                {
                    if (parent1->xreal[i] < parent2->xreal[i])
                    {
                        y1 = parent1->xreal[i];
                        y2 = parent2->xreal[i];
                    }
                    else
                    {
                        y1 = parent2->xreal[i];
                        y2 = parent1->xreal[i];
                    }
                    yl = min_realvar[i];
                    yu = max_realvar[i];
                    rand = randomperc();
                    beta = 1.0 + (2.0*(y1-yl)/(y2-y1));
                    alpha = 2.0 - pow(beta,-(eta_c+1.0));
                    if (rand <= (1.0/alpha))
                    {
                        betaq = pow ((rand*alpha),(1.0/(eta_c+1.0)));
                    }
                    else
                    {
                        betaq = pow ((1.0/(2.0 - rand*alpha)),(1.0/(eta_c+1.0)));
                    }
                    c1 = 0.5*((y1+y2)-betaq*(y2-y1));
                    beta = 1.0 + (2.0*(yu-y2)/(y2-y1));
                    alpha = 2.0 - pow(beta,-(eta_c+1.0));
                    if (rand <= (1.0/alpha))
                    {
                        betaq = pow ((rand*alpha),(1.0/(eta_c+1.0)));
                    }
                    else
                    {
                        betaq = pow ((1.0/(2.0 - rand*alpha)),(1.0/(eta_c+1.0)));
                    }
                    c2 = 0.5*((y1+y2)+betaq*(y2-y1));
                    if (c1<yl)
                        c1=yl;
                    if (c2<yl)
                        c2=yl;
                    if (c1>yu)
                        c1=yu;
                    if (c2>yu)
                        c2=yu;
                    if (randomperc()<=0.5)
                    {
                        child1->xreal[i] = c2;
                        child2->xreal[i] = c1;
                    }
                    else
                    {
                        child1->xreal[i] = c1;
                        child2->xreal[i] = c2;
                    }
                }
                else
                {
                    child1->xreal[i] = parent1->xreal[i];
                    child2->xreal[i] = parent2->xreal[i];
                }
            }
            else
            {
                child1->xreal[i] = parent1->xreal[i];
                child2->xreal[i] = parent2->xreal[i];
            }
        }
    }
    else
    {
        for (i=0; i<nreal; i++)
        {
            child1->xreal[i] = parent1->xreal[i];
            child2->xreal[i] = parent2->xreal[i];
        }
    }
    return;
} */

/* Routine for two point binary crossover */
void bincross (individual *parent1, individual *parent2, individual *child1, individual *child2)
{
    int i, j;
    double rand;
    int temp, site1, site2;
    for (i=0; i<nbin; i++)
    {
        rand = randomperc();
        if (rand <= pcross_bin)
        {
            nbincross++;
            site1 = rnd(0,nbits[i]-1);
            site2 = rnd(0,nbits[i]-1);
            if (site1 > site2)
            {
                temp = site1;
                site1 = site2;
                site2 = temp;
            }
            for (j=0; j<site1; j++)
            {
                child1->gene[i][j] = parent1->gene[i][j];
                child2->gene[i][j] = parent2->gene[i][j];
            }
            for (j=site1; j<site2; j++)
            {
                child1->gene[i][j] = parent2->gene[i][j];
                child2->gene[i][j] = parent1->gene[i][j];
            }
            for (j=site2; j<nbits[i]; j++)
            {
                child1->gene[i][j] = parent1->gene[i][j];
                child2->gene[i][j] = parent2->gene[i][j];
            }
        }
        else
        {
            for (j=0; j<nbits[i]; j++)
            {
                child1->gene[i][j] = parent1->gene[i][j];
                child2->gene[i][j] = parent2->gene[i][j];
            }
        }
    }
    return;
}

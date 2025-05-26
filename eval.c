/* Routine for evaluating population members  */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"

/* Routine to evaluate objective function values and constraints for a population */
void evaluate_pop (population *pop)
{
    int i;
    for (i=0; i<popsize; i++)
    {
        evaluate_ind (&(pop->ind[i]));
    }
    return;
}

/* Routine to evaluate objective function values and constraints for an individual */
/*void evaluate_ind (individual *ind)
{
    int j;
    test_problem (ind->xreal, ind->xbin, ind->gene, ind->obj, ind->constr);
    if (ncon==0)
    {
        ind->constr_violation = 0.0;
    }
    else
    {
        ind->constr_violation = 0.0;
        for (j=0; j<ncon; j++)
        {
            if (ind->constr[j]<0.0)
            {
                ind->constr_violation += ind->constr[j];
            }
        }
    }
    return;
}*/


// /* Routine to evaluate objective function values and constraints for an individual */
// void evaluate_ind (individual *ind)
// {
//     /*Acá la evaluación completa. Deben setearse los valores de obj y constr_violation. */
//     /*
//     void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
//     {
//     obj[0] = pow(xreal[0],2.0);
//     obj[1] = pow((xreal[0]-2.0),2.0);
//     return;
//     }
//     */

//     int j;
//     /*test_problem (ind->xreal, ind->xbin, ind->gene, ind->obj, ind->constr);*/
//     if (ncon==0)
//     {
//         ind->constr_violation = 0.0;
//     }
//     else
//     {
//         ind->constr_violation = 0.0;
//         for (j=0; j<ncon; j++)
//         {
//             if (ind->constr[j]<0.0)
//             {
//                 ind->constr_violation += ind->constr[j];
//             }
//         }
//     }
//     return;
// }

void evaluate_ind(individual *ind)
{
    double total_distance = 0.0;
    double total_emissions = 0.0;
    double constr_viol = 0.0;

    int current_vehicle = 0;
    int current_capacity = 0;
    double current_risk = 0.0;

    int prev_node = 0; // partimos desde el depósito 0
    int current_node;

    for (int i = 0; i < ind->route_length; i++) {
        current_node = ind->route[i];

        // Si es separador, terminamos la ruta actual
        if (current_node < 0) {
            // volver al depósito
            total_distance += d[prev_node][0];
            total_emissions += compute_emission(prev_node, 0, current_capacity);
            if (current_capacity > b) constr_viol += current_capacity - b;
            if (current_risk > theta) constr_viol += current_risk - theta;

            // reset
            current_capacity = 0;
            current_risk = 0.0;
            prev_node = 0;
            current_vehicle++;
        } else {
            // visitar cliente
            int demand = dm[current_node];
            double dist = d[prev_node][current_node];
            double speed_kmh = v[prev_node][current_node];
            double emission = compute_emission(prev_node, current_node, current_capacity);

            total_distance += dist;
            total_emissions += emission;
            current_capacity += demand;
            current_risk += dist * demand;

            prev_node = current_node;
        }
    }

    // Cerrar la última ruta si no terminó en separador
    if (prev_node != 0) {
        total_distance += d[prev_node][0];
        total_emissions += compute_emission(prev_node, 0, current_capacity);
        if (current_capacity > b) constr_viol += current_capacity - b;
        if (current_risk > theta) constr_viol += current_risk - theta;
    }

    // Asignar objetivos
    ind->obj[0] = total_distance;
    ind->obj[1] = total_emissions;

    // Violación de restricciones
    ind->constr_violation = constr_viol;
}


double compute_emission(int i, int j, int carga)
{
    double s = (double)v[i][j]; // velocidad en km/h
    double w = (double)(peso_vacio + carga); // peso total

    double f = 0.0;
    for (int l = 0; l < 5; l++) {
        f += (alpha[l] * pow(w, 4 - l)) +
             (beta[l] * pow(s, 4 - l)) +
             (gamma[l] * pow(w, 4 - l) * pow(s, l)) +
             (delta[l] * pow(s, 4 - l) * pow(w, l)) +
             (epsilon[l] * pow(s, 4 - l) * pow(w, l)) +
             (zeta[l]) + (hta[l]);
    }
    return f;
}

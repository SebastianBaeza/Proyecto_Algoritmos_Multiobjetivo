/* Routine for evaluating population members  */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"

double compute_emission(int i, int j)
{
    double s = v[i][j];

    double f = 0.0;
    int l;
    for (l = 1; l < 5; l++) {
        /*
        // f += (alpha[l] * pow(w, 4 - l)) +
        //      (beta[l] * pow(s, 4 - l)) +
        //      (gamma_param[l] * pow(w, 4 - l) * pow(s, l)) +
        //      (delta_param[l] * pow(s, 4 - l) * pow(w, l)) +
        //      (epsilon[l] * pow(s, 4 - l) * pow(w, l)) +
        //      (zeta[l]) + (hta[l]);
        */
        f += ((alpha[l] * pow(s, 2)) + (beta[l] * s) + (gamma_param[l]) + (delta_param[l] / s))/((epsilon[l] * pow(s, 2)) + (zeta[l] * s) + (hta[l]));
    }
    return f;
}

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


/* Routine to evaluate objective function values and constraints for an individual */
/* void evaluate_ind (individual *ind)
{
    /*Acá la evaluación completa. Deben setearse los valores de obj y constr_violation. */
    /*
    void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
    {
    obj[0] = pow(xreal[0],2.0);
    obj[1] = pow((xreal[0]-2.0),2.0);
    return;
    }
    

    int j;
    /*test_problem (ind->xreal, ind->xbin, ind->gene, ind->obj, ind->constr);
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
} */

void evaluate_ind(individual *ind)
{
    double total_distance = 0.0;
    double total_emissions = 0.0;
    double constr_viol = 0.0;

    int current_vehicle = 0;
    int current_capacity = 0;
    double current_risk = 0.0;
    
    double dist;
    double emission;

    int prev_node = 0;
    int current_node;
    int depot_counter = 0;
    int current_depot = set_O[0]; 

    int i;
    for (i = 0; i < ind->route_length; i++) {
        current_node = ind->route[i];

        if (current_node < 0) {

            total_distance += d[prev_node][0];
            total_emissions += d[prev_node][0] * ((peso_vacio + current_capacity) + compute_emission(prev_node, current_depot));
            if (current_capacity > b) constr_viol += current_capacity - b;
            if (current_risk > theta) constr_viol += current_risk - theta;

            current_capacity = 0;
            current_risk = 0.0;
            prev_node = 0;
            current_vehicle++;
            if (current_vehicle >= n_vehicles) {
                current_depot = set_O[++depot_counter]; 
                current_vehicle = 0;
            }
        } else {
            demanda = dm[current_node];
            dist = d[prev_node][current_node];
            emission = dist * ((peso_vacio + current_capacity) + compute_emission(prev_node, current_node));

            total_distance += dist;
            total_emissions += emission;
            current_capacity += demanda;
            current_risk += dist * demanda;

            prev_node = current_node;
        }
    }

    if (prev_node != 0) {
        total_distance += d[prev_node][current_depot];
        total_emissions += d[prev_node][current_depot] * ((peso_vacio + current_capacity) + compute_emission(prev_node, current_depot));
        if (current_capacity > b) constr_viol += current_capacity - b;
        if (current_risk > theta) constr_viol += current_risk - theta;
    }

    ind->obj[0] = total_distance;
    ind->obj[1] = total_emissions;

    ind->constr_violation = constr_viol;
}


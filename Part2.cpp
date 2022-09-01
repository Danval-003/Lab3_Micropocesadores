/**
 *                              UNIVERSIDAD DEL VALLE DE GUATEMALA
 *              31 de agosto de 2022||PROGRAMACIÓN DE MICROPROCESADORES||Seccion 20
 *              Author: Daniel Armando Valdez Reyes                 carnet: 21240
 *              Description:
 *              Programa que mediante el uso de hilos evalua el valor de convergencia de una serie
 *              Docente: Roger Artemio Diaz Fuentes
 */

//Importar las librerias
#include <iostream>
#include <math.h> //Se hace uso de esta libreria para el uso de la funcion matematica de la potencia
#include <string> //Se utiliza para la conversion de string a double
using namespace std;
//Funcion de la serie A
void *A_Serie(void * arg){
    int *n=(int*) arg;//Se obtiene el numero que se encarga de evaluar el hilo
    double *z = static_cast<double *>(malloc(sizeof(double)));//Se expande el double para que no se conflictos cuando se convierta en puntero void
    *z = 3/pow(2,*n);//se realiza la operacion de la serie A
    pthread_exit(z);// se devuelve el resultado
}
//Funcion de la serie B
void *B_Serie(void * arg) {
    int *n = (int *) arg;//Se obtiene el numero que se encarga de evaluar el hilo
    int np=*n;
    double *z = static_cast<double *>(malloc(sizeof(double)));//Se expande el double para que no se conflictos cuando se convierta en puntero void
    *z = 1.0/(np*(np+1));//se realiza la operacion de la serie B
    pthread_exit(z);// se devuelve el resultado
}

int main() {
    //Se inicia el thread_id
    pthread_t thread_id;
    //Se declara el attr
    pthread_attr_t attr;

    //Se inicia el attr
    pthread_attr_init(&attr);
    //Se define como Joinable
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

    //Se definen variables que se utilizaran para la navegacion en el menu de texto del programa
    bool inUse= true;//Ciclo que indica que estas dentro del menu de navegacion
    char input='-';//Entrada de las opciones del menu
    string n_String;// entrada de los datos enteros en formato string
    int n;//Entero

    //Se definen las variables que almacenaran los datos provenientes de los hilos y el resultado.
    double result=0;
    void *returned;

    //Se da la
    cout<<endl;
    cout<<"---------------------------------------------------------------------------------------------------------------------"<<endl;
    cout<<"                                              BIENVENIDO AL PROGRAMA DE "<<endl;
    cout<<"                               EVALUACION DE VALOR DE CONVERGENCIA DE CIERTAS SERIES"<<endl;
    cout<<"---------------------------------------------------------------------------------------------------------------------"<<endl;
    cout<<endl;


    while (inUse){
        cout<<endl;
        cout<<"Se evaluaran las series: \n"
              "A) Σ 3/(2^n) ------------ n=0 -> ∞ \n"
              "B) Σ 1/n(n+1) ----------- n=1 -> ∞ \n"
              "(Cualquier otra opcion)-> Salir"<<endl;

        cout<<" -- ";
        cin>>input;

        switch (tolower(input)) {
            case 'a':
                while (inUse){
                    result=6;
                    cout<<endl;
                    cout<<"¿Cual es el valor maximo de n que va a evaluar?\n(Recordar que debe ser un entero mayor o igual que 0)\n -- ";
                    cin>>n_String;
                    try{
                        n= stoi(n_String);
                        if(n<0){
                            cout<<"                   INGRESO UN VALOR NUMERICO INCORRECTO! (MENOR QUE 0)"<<endl;
                        } else{
                            inUse=false;
                        }
                    }catch (exception e){
                        cout<<" INGRESO UN VALOR INCORRECTO! (NO NUMERICO)"<<endl;
                    }
                }
                for(int i=0; i<n; i++) {
                    pthread_create(&thread_id, &attr, A_Serie, (void *)&i );
                    pthread_join(thread_id, &returned);
                    result-=*(double*) returned;
                    free(returned);
                }
                inUse= true;
                cout<<"\n-------------------------------------------------------------------------------------------------"<<endl;
                cout<<"EL VALOR DE CONVERGENCIA PARA ESTA SERIE CON UN VALOR MAXIMO PARA n DE "<<n<<" ES: "<<result<<endl;
                cout<<"---------------------------------------------------------------------------------------------------"<<endl;
                break;
            case 'b':
                while (inUse){
                    result=1;
                    cout<<endl;
                    cout<<"¿Cual es el valor maximo de n que va a evaluar?\n(Recordar que debe ser un entero mayor o igual que 1)\n -- ";
                    cin>>n_String;
                    try{
                        n= stoi(n_String);
                        if(n<1){
                            cout<<" INGRESO UN VALOR NUMERICO INCORRECTO! (MENOR QUE 1)"<<endl;
                        } else{
                            inUse=false;
                        }
                    }catch (exception e){
                        cout<<"                          INGRESO UN VALOR INCORRECTO! (NO NUMERICO)"<<endl;
                    }
                }
                for(int i=1; i<n; i++) {
                    pthread_create(&thread_id, &attr, B_Serie, (void *)&i );
                    pthread_join(thread_id, &returned);
                    result-=*(double*) returned;
                    free(returned);
                }
                inUse= true;
                cout<<"\n-------------------------------------------------------------------------------------------------"<<endl;
                cout<<"EL VALOR DE CONVERGENCIA PARA ESTA SERIE CON UN VALOR MAXIMO PARA n DE "<<n<<" ES: "<<result<<endl;
                cout<<"---------------------------------------------------------------------------------------------------"<<endl;
                break;
            default:
                inUse= false;

        }
    }

    pthread_exit(NULL);


    return 0;
}

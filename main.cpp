/**
 *                              UNIVERSIDAD DEL VALLE DE GUATEMALA
 *              31 de agosto de 2022||PROGRAMACIÓN DE MICROPROCESADORES||Seccion 20
 *              Author: Daniel Armando Valdez Reyes                 carnet: 21240
 *              Description:
 *              Programa que mediante el uso de hilos devuelve el numero de numeros primos dentro de
 *              un intervalo determinado.
 *              Docente: Roger Artemio Diaz Fuentes
 */
//Se importan las librerias
#include <iostream>
using namespace std;

/**
 * Estructura de los parametros que se le mandan a los hilos
 */
struct  parameters{
    int id;//Identificador del hilo
    int limit;//Limite superior del intervalo utilizado por el hilo
    int vmin;//Limite inferior del intervalo utilizado por el hilo
    int sum;//Numero de primos que detecta el hilo
    bool terminate= false;//Bandera que determina si el hilo termino de contar los primos
};

/**
 * Funcion en la que los hilos se basan para funcionar
 */
void *primenum(void *arg){
    //Impresion de los datos basicos del hilo
    cout<<"------------------------------------"<<endl;
    parameters *pi = (parameters *)arg;
    cout<<"THREAD "<<pi->id<<endl;
    cout<<"Lim inf: "<<pi->vmin<<endl;
    cout<<"Lim sup: "<<pi->limit<<endl;
    int add=0;//Se declara la variable que contara el numero de primos que encuentre.
    //Ciclo que revisa todos los numeros dentro de dicho rango
    while ((pi->vmin)<=(pi->limit)){
        int test=1;//Variable de testeo de multiplo
        int multiples=0;//Cantidad de muntiplos del numero a evaluar
        /**
         * Ciclo que recorre los posibles multiplos del numero a evaluar
         */
        while (test<=(pi->vmin)){
            if(((pi->vmin)%test)==0 ){//Si el residuo es 0 entonces es un multiplo del numero
                multiples++;//Se suma la cantidad de multiplos
            }
            test++;//Se cambia el valor de testeo.
        }
        if (multiples ==2){//Si tiene solo 2 multiplos entonces el numero es primo
            if(add==0){//Si la adicion en total es 0 se le muestra al usuario el
                cout<<"Primos: "<<endl;
            }
            cout<<" -- "<<pi->vmin<<endl;// se muestra el numero primo
            add++;//Se suma a la cantidad de primos encontrados
        }
        pi->vmin++;//Se cambia el numero que se esta evaluando
    }
    pi->sum=add;//Se guarda en el parametro del hilo el valor de la suma
    pi->terminate=true;//Se cambia el estado del terminado a verdadero
    cout<<"Suma del hilo "<<pi->id<<": "<<add<<endl;//Se muestra la suma que tiene este hilo
    pthread_exit(NULL);
}

/**
 * Clase principal del programa
 * @return 0
 */
int main() {
    cout << "///////////BIENVENIDO AL PROGRAMA PARA ENCONTRAR LA CANTIDAD DE NUMEROS PRIMOS EXISTENTES" << endl;
    int vmax=0;//Se declara la variable en que se guardara el limite superior
    string answer;//Se declara la variable en que se guardaran las respuestas del usuario en string
    while (vmax<=0){
        cout << "Por favor ingrese el valor hasta el que quiere que encontremos el numero de enteros, recuerde que debe ser un entero mayor que 0"<< endl;
        cin>> answer;//Se guarda la respuesta del usuario

        try {//Se utiliza un Try en caso el usuario ingrese una letra
            vmax= stoi(answer);//Se convierte en int y se guarda con el limite superior de todo el programa
            if(vmax<=0){//Si el valor ingresado es o hubo un error este le indicara que tiene que repetir su accion para continuar
                cout<<"Ingreso un valor incorrecto porfavor repita su eleccion"<<endl;
                cout<<endl;
            }
        }catch(exception e){//Si ingreso una letra se le indica que corrija su eleccion
            cout<<"Ingreso una letra, un caracter o un enter lo cual es invalido. Por favor repita su eleccion"<<endl;
            cout<<endl;
        }

    }
    int numThread=0;//Se inicia el numero de hilos
    while (numThread<=0){
        cout << "Por favor ingrese el valor de numero de hilos a utilizar, recuerde que debe ser un entero mayor que 0 y menor que el valor maximo"<< endl;
        cin>>answer;
        try {//Se utiliza un Try en caso el usuario ingrese una letra
            numThread= stoi(answer);//Se convierte en int y se guarda como el numero de hilos a utilizar en todo el programa
            if(numThread<=0 or numThread>vmax ){//Si el numero de hilos es mayor a el intervalo daria error al igual si el numero de hilos es menor o igual a 0
                numThread=0;
                cout<<"Ingreso un valor incorrecto porfavor repita su eleccion"<<endl;
                cout<<endl;
            }
        }catch(exception e){//Si ingreso una letra se le indica que corrija su eleccion
            cout<<"Ingreso una letra, un caracter o un enter lo cual es invalido. Por favor repita su eleccion"<<endl;
            cout<<endl;
        }
    }
    //Se le repite al usuario los datos que ingreso para que observe su seleccion
    cout<<"------------------------------------------------------------------------------------------------------"<<endl;
    cout<<"Valor maximo: "<<vmax<<endl;
    cout<<"Numero de Thread: "<<numThread<<endl;
    int n = vmax/numThread;//El valor n se obtiene de dividir el limite superior entre el numero de hilos del programa
    cout<<"La cantidad de numeros a evaluar por thread es: "<<n<<endl;
    cout<<"------------------------------------------------------------------------------------------------------"<<endl;
    cout<<"/////////////////////EMPEZAMOS/////////////////////////"<<endl;
    //Se inicia el thread_id
    pthread_t thread_id;
    //Se declara el attr
    pthread_attr_t attr;

    //Se inicia el attr
    pthread_attr_init(&attr);
    //Se define como Joinable
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

    //Se crea una lista con los parametros individuales de cada uno de los hilos
    parameters param[numThread];
    int vmin=0;//Se define el limite inferior del programa
    int result=0;//Se define la variable que guarda el resultado

    for(int i=1; i<=numThread; i++){//Ciclo for que recorre el numero de hilos y los crea
        parameters *p =&param[i-1];//Se inicia un puntero que apunta a la direccion de memoria correspondiente a cierto hilo
        (*p).id=i;//Se guarda el identificador del hilo
        if(i==numThread){//Si es el ultimo hilo se le dan los valores que quedan
            (*p).limit=vmax;
        }else{//Si no es el ultimo hilo se le da un limite en base a n y su id
            (*p).limit=i*n;
        }
        // El limite inferior del hilo es el mismo que el que lleva el programa hasta entonces
        (*p).vmin=vmin;
        //Se crea el hilo
        pthread_create(&thread_id,&attr,primenum, (void *)p);
        pthread_join(thread_id,NULL);// se inicia el hilo, para que se impriman en orden y no se ve alterado

        vmin=(*p).limit+1;//Se establece que el siguiente hilo tenga un limite inferior igual a su superior pero mas una unidad.

    }
    bool sum= false;//Se establece un booleano sobre si los hilos han terminado
    while (!sum){//Mientras los hilos no terminen el ciclo no lo hara
        sum=true;//Se declara sum como true
        for(auto& par:param){//Se establece un for each que recorre la lista con todos los datos de los distintos hilos
            if(par.terminate){//Detecta si el hilo ha terminado sus operciones o no
                if(sum and par.terminate){//Si el hilo termino y los anteriores tambien se declara que hasta ese hilo los demas han terminado
                    sum= true;
                }
                result+=par.sum;//Se suma el resultado del hilo al resultado de todo el programa
                par.sum=0;//Se declara que el resultado del hilo es 0 para que no se sume mas de una vez
            } else{
                sum=false;//Si no ha terminado se declara que los hilos aun no han terminado
            }

        }
    }
    cout<<endl;
    cout<<endl;
    cout<<"----------------------------------------------------------------------------"<<endl;
    //Se le muestra al usuario el numero total de primos encontrados en el programa
    cout<<"El numero total de primos es: "<<result<<endl;
    cout<<"----------------------------------------------------------------------------"<<endl;

    //se destruyen el attr y se terminan los hilos.
    pthread_attr_destroy(&attr);

    return 0;
}

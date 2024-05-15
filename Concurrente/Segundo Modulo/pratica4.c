// CONSIDERACIONES PARA RESOLVER LOS EJERCICIOS DE PASAJE DE
// MENSAJES ASINCRÓNICO (PMA):
// • Los canales son compartidos por todos los procesos.
// • Cada canal es una cola de mensajes; por lo tanto, el primer mensaje encolado es el
// primero en ser atendido.
// • Por ser PMA, el send no bloquea al emisor.
// • Se puede usar la sentencia empty para saber si hay algún mensaje en el canal, pero no
// se puede consultar por la cantidad de mensajes encolados.
// • Se puede utilizar el if/do no determinístico donde cada opción es una condición
// boolena donde se puede preguntar por variables locales y/o por empty de canales.
//  if (cond 1) -> Acciones 1;
//  (cond 2) -> Acciones 2;
//  ….
//  (cond N) -> Acciones N;
//  end if
//  De todas las opciones cuya condición sea Verdadera elige una en forma no
// determinística y ejecuta las acciones correspondientes. Si ninguna es verdadera, sale
// del if/do sin ejecutar acción alguna.
// • Se debe evitar hacer busy waiting siempre que sea posible (sólo hacerlo si no hay
// otra opción).
// • En todos los ejercicios el tiempo debe representarse con la función delay.

// 1. Suponga que N clientes llegan a la cola de un banco y que serán atendidos por sus
// empleados. Analice el problema y defina qué procesos, recursos y comunicaciones serán
// necesarios/convenientes para resolver el problema. Luego, resuelva considerando las
// siguientes situaciones:
// a. Existe un único empleado, el cual atiende por orden de llegada.
// b. Ídem a) pero considerando que hay 2 empleados para atender, ¿qué debe
// modificarse en la solución anterior?
// c. Ídem b) pero considerando que, si no hay clientes para atender, los empleados
// realizan tareas administrativas durante 15 minutos. ¿Se puede resolver sin usar
// procesos adicionales? ¿Qué consecuencias implicaría?

// N clientes, 1 empleado.

chan pedido(int idC);

Process Cliente[i:1..N]{
    send pedido(i);
}

Process Empleado{
    while (true){
        receive pedido(i);
        atender(i);
    }
}   


chan pedido(int idC);

Process Cliente[i:1..N]{
    send pedido(i);
}

Process Empleado[id:1..2]{
    while (true){
        receive pedido(i);
        atender(i);
    }
}

//ta mal, puede haber la posibilidad de que ambos vean que no esta empty, pero solo haya 1 dato, por lo que
// ambos van a hacer el receive y uno va a poder seguir con el dato y el otro se va a quedar quieto,
//por lo que hay que utilizar un admin o coordinador que sea 1 solo, donde el pueda ver este pedido y coordinar todo lo demas.
chan pedido(int idC);

Process Cliente[i:1..N]{
    send pedido(i);
}

Process Empleado[id:1..2]{
    int i;
    while (true){
        if(!pedido.empty){
            receive pedido(i);
            atender(i);
        }
        else{
            delay(15)
            //hacer cosa
        }
    }
}

//-----------------------------------------------------------------------------------

chan pedido(int idC);
chan atender(int idC)

Process Cliente[i:1..N]{
    send pedido(i);
}

Process Coordinaro{
    int idC;
    int idE;
    while(true){
        if(!pedido.empty){
            receive pedido(idC)
            send atender(idC)
        }
        else{
            send atender(-1)
        }

    }
}

Process Empleado[id:1..2]{
    while (true){
        receive atender(i);
        if(i>0)
            atender(i);
        else
            delay(15)
    }
}






2. Se desea modelar el funcionamiento de un banco en el cual existen 5 cajas para realizar
pagos. Existen P clientes que desean hacer un pago. Para esto, cada una selecciona la caja
donde hay menos personas esperando; una vez seleccionada, espera a ser atendido. En cada
caja, los clientes son atendidos por orden de llegada por los cajeros. Luego del pago, se les
entrega un comprobante. Nota: maximizando la concurrencia.


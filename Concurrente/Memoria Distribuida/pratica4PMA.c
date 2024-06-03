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

chan pedido(int);
chan atender(int)

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


chan fila[5](int);//espera en la fila que le corresponde
chan buscarCaja(int);//con esto el cliente le avisa al admin que quiere la caja mas vacia
chan dejeCaja(int);//aviso que caja deje
chan hacerPago(int);//hacer el pago

chan aviso[5](int);//la caja le avisa al empleado que libero un espacio
chan aQueCaja[P](int)//por este canal le enviara a cada cliente a que caja ir

chan atender[P](int);//atiende al cliente 
chan comprobante[P](text);//le deja el comprobante

Process Cliente[i:1..P]{
    int caja;
    text comprobante;
    int pago;
    send buscarCaja(idC);//busco caja
    receive aQueCaja[i](caja);//recibo caja
    send fila[caja](i);//hago fila
    receive atender[i](pago);
    //cuentaBancaria-=pago;
    send hacerPago[caja](pago);
    receive comprobante[i](comprobante)
    send dejeCaja(caja);
}

Process Admin{
  int cajasContador[5]=([5]0);
  int idCaja=9999;
  int idCliente;
  while(true){
    if(!empty(buscarCaja) && empty(dejeCaja)){
      receive buscarCaja(idCliente);
      for(int i=1;i<5;i++){
        if(cajasContador[i].cantidad<idCaja){
          idCaja=cajasContador[i];//caja con menor gente esperando
        }
      }
      cajasContador[idCaja]++;//aumento la cantidad de gente esperando
      send aQueCaja[idCliente](idCaja);//le indico a que caja ir
    }
    if(!empty(dejeCaja)){
      receive dejeCaja(idCaja);
      cajasContador[idCaja]--;
    }
  }
}

Process Caja[i:1..5]{
  int idC;
  string comprobante;
  int pago;
  while(true){
    receive fila[i](idC);
    send atender[idC](precioAPagar);
    receive pago[i](pago);
    //generarComprobante(comprobante)
    send comprobante[idC](comprobante);
  }
}

3. Se debe modelar el funcionamiento de una casa de comida rápida, en la cual trabajan 2 
cocineros y 3 vendedores, y que debe atender a C clientes. El modelado debe considerar 
que: - - - 
Cada cliente realiza un pedido y luego espera a que se lo entreguen. 
Los pedidos que hacen los clientes son tomados por cualquiera de los vendedores y se 
lo pasan a los cocineros para que realicen el plato. Cuando no hay pedidos para atender, 
los vendedores aprovechan para reponer un pack de bebidas de la heladera (tardan entre 
1 y 3 minutos para hacer esto). 
Repetidamente cada cocinero toma un pedido pendiente dejado por los vendedores, lo 
cocina y se lo entrega directamente al cliente correspondiente. 
Nota: maximizar la concurrencia. 

//cliente
chan pedido(int);

//vendedor
chan atiendo[3];
chan pedirPedido(int);

//cocinero
chan entrega(pedido);

Process Cocinero[i:1..2]{
  int idC;
  pedido p;
  while(true){
    receive cocinero[i](idC);//se queda esperando y cocina para el id
    //cocino(p)
    send entrega[idC](p);//entrega el pedido
  }
}

Process Vendedor[i:1..3]{
  int idC;
  int id;
  int tiempo
  while(true){
      send pedirPedido(i);//pido atender
      receive atiendo[i](idC);//recibe id
    if(idC!=-1)){//si habia alguien a quien atender
      send cocinero(idC);//le deja el id cliente que realizo el pedido a los cocineros
    }
    else{//sino delay que representa el reponer
      random(60,180,tiempo);
      delay(tiempo);
    }
  }
}

Process Coordinar{
  int idC;
  int idV;
  bool comenzar;
  while(true){
    receive pedirPedido(idV);// esperar a que algun vendedor pida atender
    if(!empty(pedido)){//fijarse si hay alguien a quien atender
      receive pedido(idC);//si hay alguien a quien atender, conseguir el id 
      send atiendo[idV](idC);//mandarle el id del cliente
    }
    else{
      send atiendo[idV](-1);//sino enviarle -1 o algun tipo de flag, para que el vendedor no se quede esperando y haga lo otro que puede hacer.
    }
  }
}

Process Cliente[i:1..C]{
  pedido p;
  send pedido(i);
  receive entrega[i](p);
}

4. Simular la atención en un locutorio con 10 cabinas telefónicas, el cual tiene un empleado 
que se encarga de atender a N clientes. Al llegar, cada cliente espera hasta que el empleado 
le indique a qué cabina ir, la usa y luego se dirige al empleado para pagarle. El empleado 
atiende a los clientes en el orden en que hacen los pedidos, pero siempre dando prioridad a 
los que terminaron de usar la cabina. A cada cliente se le entrega un ticket factura. Nota: 
maximizar la concurrencia; suponga que hay una función Cobrar() llamada por el empleado 
que simula que el empleado le cobra al cliente.

//cliente
chan espera(int);//espera a que lo atiendan
chan dejarCabina(int);//avisa que cabina dejo

//empleado
chan asignar[N](int);//deja id de cabina
chan ticket[N](text);//deja factura

Process Cliente [i:1..N]{
  text t;
  send espera(i);
  receive asignar[i](cabina);
  //usar(cabina)
  send dejarCabina(cabina)
  send pagar(i);
  receive ticket(t);
}

Process Empleado{
  int idC;
  int cabina;
  int cabinas[10]=([10]0)
  text t;
  cola esperaCabina[N];

    while(true){    
    if(!empty(pagar)){//si hay alguien que quiere pagar se lo atiende
      receive pagar(idC);//manda id
      t=Cobrar(idC);//se le cobra
      send ticket[idC](t);//enviamos ticket
    }
    if(!empty(espera) && esperaCabina.isEmpty() && empty(dejarCabina)){//si hay alguien esperando, no habia alguien antes y nadie esta queriendo liberar
      receive espera (idC);//id de cliente esperando
      cabinas.libre(cabina);//devuelve numero de cabina libre o -1 si no hay alguna libre
      if(cabina!=-1){//si hay cabina
        send asignar[idC](cabina);//le dice a cual ir
      }
      else{
        esperaCabina.push(idC);//se lo encola para la espera de la proxima cabina liberada, lo que se chequea siempre antes de atender a alguien nuevo.
      }
      }
    }else{//sino es porque hay o cabina libre o alguien esperando.
      if(!empty(dejarCabina)){//si hay alguien queriendo dejar una cabina.
        receive dejarCabina(cabina);//agarra cabina
        if(esperaCabina.isEmpty()){//si no hay nadie esperando una cabina
        cabinas[cabina]=0;//la asigna como libre
        }
        else{//sino
          idC=esperaCabina.pop();//se consigue de id de quien estaba esperando una libre
          send asignar[idC](cabina);//se le asigna la cabina.
        }
      }
    }
  }





while true
  if(!empty(pagar))
      receive pagar(idCli, ...)


  - empty(pagar) && !empty(espera) && libres > 0
      receive espera(idC1 ) // B;C->S;(SINCRONICO)// if(cond)->Acc1.(ASINCRONICO)

5. Resolver la administración de las impresoras de una oficina. Hay 3 impresoras, N usuarios y 
1 director. Los usuarios y el director están continuamente trabajando y cada tanto envían 
documentos a imprimir. Cada impresora, cuando está libre, toma un documento y lo 
imprime, de acuerdo con el orden de llegada, pero siempre dando prioridad a los pedidos 
del director. Nota: los usuarios y el director no deben esperar a que se imprima el 
documento. 

//usuario 
chan imprimir(texto);//manda a imprimir
chan imprimirPrioridad(texto);//manda a imprimir con prioridad
chan hayDoc(bool);//si hay documentos

Process Usuario[i:1..N]{
  text t;
  while(true){
    trabajar(t);
    send imprimir(t);
    send hayDoc(true);
  }
}

Process Director{
  text t;
  while(true){
    trabajar(t);
    send imprimirPrioridad(t);
    send hayDoc(true);  
  }
}

Process Impresora[i:1..3]{
  text t;
  boolean ok;
  while(true){
    recieve hayDoc(ok);
    if(!empty(imprimarPrioridad)){
      receive imprimirPrioridad(t);
      imprimir(t);
    }
    else{
      if(!empty(imprimir) && empty(imprimirPrioridad)){
        receive imprimir(t);
        imprimir(t);
      }
    }
  }
}
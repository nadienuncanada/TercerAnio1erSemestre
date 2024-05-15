// 1. SEMÁFOROS. Existen 15 sensores de temperatura y 2 módulos centrales de procesamiento. 
// Un sensor mide la temperatura cada cierto tiempo (función medir()), la envía al módulo central para que le indique qué acción debe hacer (un número del 1 al 10) 
// (función determinar() para el módulo central) y la hace (función realizar()). 
// Los módulos atienden las mediciones por orden de llegada.

// 1 senson mide, envia al modulo para que le diga que accion hacer, y la realiza(el sensor?), orden de llegada.
int colaEspera[15]([15]0);
int respuesta[15]([15]0);
sem respuestaEnviada[15]([15]0);
sem pasar[15]([15]0);
cola hayMedicion=0;
mutex mutexEsp=1;
mutex mutexResp=1;//nose si sea necesario
Process Modulos[id:1..2]{
    while(true){
    int idSensor;
    int midio;
    int determinado;
    P(hayMedicion)
    P(mutexEsp)
    idSensor,midio=colaEspera.pop();//saca el id del sensor y su medicion
    V(mutexEsp)
    determinado=determinar(midio);
    P(mutexResp)
    respuesta[idSensor]=determinado
    V(mutexResp)
    signal(respuestaEnviada[idSensor]);
    //se podria esperar que a quien le envie me diga que lo termino
    }

}
Process Sensores[id:1..15]{
    while(true){
        int medicion;
        int accionRealizar;
        medicion=medir();
        P(mutexEsp)
        colaEspera.push(id,medicion);//encola su id y lo que midio
        V(mutexEsp)
        V(hayMedicion)
        P(respuestaEnviada[id]);
        P(mutexResp)
        accionRealizar=respuesta[id];
        V(mutexResp)
        realizar(accionRealizar);
        //se podria enviar un mensaje que lo termino supongo
    }
}

//Parcial MC 11-10-22.
 
// Resolver con SEMÁFOROS el siguiente problema. En una planta verificadora de vehículos, existen 7 estaciones donde 
// se dirigen 150 vehículos para ser verificados. Cuando un vehículo llega a la planta, el coordinador de la planta le 
// indica a qué estación debe dirigirse. El coordinador selecciona la estación que tenga menos vehículos asignados en ese 
// momento. Una vez que el vehículo sabe qué estación le fue asignada, se dirige a la misma y espera a que lo llamen 
// para verificar. Luego de la revisión, la estación le entrega un comprobante que indica si pasó la revisión o no. Más allá 
// del resultado, el vehículo se retira de la planta. Nota: maximizar la concurrencia

//7 estaciones 150 autos, 1 coordinador, se le asigna la estacion con menos autos asignados, el auto se dirige, y espera a ser llamado, luego le entrega comprobante se retira. 

// sem estacionesDisp[7]([7]0);//semaforo para controlar cuando cada estacion esta libre//no es necesaria?

cola entraAuto;//cola de fila general que atiende el coordinador.
sem hayAuto=0;//semaforo para que los autos indiquen que llegaron y el coordinador active
sem mutexC=1;//mutex para la cola que lee el coordinador, tiene cada id de auto que llego a la planta
cola filaPorEstacion[7]([7]0);//cola con los ids de autos que estan esperando por cada estacion, esta cuenta con una funcion que devuelve la posicion con menos cantidad de autos esperando.
sem mutexCE[7]([7]0);//mutex para la cola de cada estacion, que tambien utiliza el coordinador
sem autoEnvio[7]([7]0);//sem para avisar que el auto esta esperando.
int asignacion[150]([150]0);//donde el coordinador le deja a que estacion ir.
sem asignaron[150]([150]0);//semaforo para indicar que le dejaron el id de la estacion.
int resultado[150];//devolucion de resultado
sem atendido[150]([150]0)
sem retiro[7]([7]0);//para indicar que el auto se fue

Process estaciones[id:1..7]{
    while(true){
        int idAtender
        P(mutexCE[id]);
        if(filaPorEstacion[id].isEmpty()){
            V(mutexCE[id]);
            P(autoEnvio[id])
        }else{
            V(mutexCE[id])
        }
        P(mutexCE[id])
        idAtender=filaPorEstacion[id].pop();
        V(mutexCE[id])
        resultado[idAtender]=atender(idAtender);
        V(atendido[idAtender]);
        P(retiro[idAtender]);
    }
}

Process vehiculo[id:1..150]{
    int idFilaConMenorCantidad;
    int resultado;
    P(mutexC)
    entraAuto.push(id);
    V(mutexC)
    V(hayAuto);
    P(asignaron[id])
    idFilaConMenorCantidad=asignacion[id];
    P(mutexCE[idFilaConMenorCantidad]);
    filaPorEstacion[idFilaConMenorCantidad].push(id);
    V(mutexCE[idFilaConMenorCantidad]);
    V(autoEnvio[idFilaConMenorCantidad]);
    P(atendido[id]);
    resultado=resultado[id];
    V(retiro[id]);
}

Process coordinador{
    for(i=1;i<150;i++){
        int idAtender;
        int idFilaConMenorCantidad;
        P(mutexC)
        if(entraAuto.isEmpty()){
            V(mutexC)
            P(hayAuto);
        }
        else{
            V(mutexC)
        }
        P(mutexC)
        idAtender=entraAuto.pop();
        V(mutexC);
        idFilaConMenorCantidad=filaPorEstacion.getIdMenorCantidadEsperando();
        asignacion[idAtender]=idFilaConMenorCantidad;
        V(asignaron[idAtender]);
    }
}

//la resolucion es un poco diferente, utilzan una cola y un contado para cada cola, yo utiliza una array de colas, creo que funciona y todo el resto bien(digamos la logica es casi un calco, pero alguna que otra estrucura es distinta).


// Resolver con MONITORES el siguiente problema. En un sistema operativo se ejecutan 20 procesos que 
// periódicamente realizan cierto cómputo mediante la función Procesar(). Los resultados de dicha función son 
// persistidos en un archivo, para lo que se requiere de acceso al subsistema de E/S. Sólo un proceso a la vez puede hacer 
// uso del subsistema de E/S, y el acceso al mismo se define por la prioridad del proceso (menor valor indica mayor 
// prioridad).

Process proceso[id:1..20]{
    arch archivo;
    
    while(true){
        int prioridad=prioridad();//si la prioridad de un proceso nunca cambia, esta linea estaria afuera del while.
        archivo=Procesar();
        acceso.entrada(id,prioridad);
        Persistir(archivo);
        acceso.liberar();
    }
}
Monitor acceso{
    cola esperaPrioridad;//cola que cuenta con la funcion de encolar los id ordenando mediante prioridad 
    arch archivoDondeSePersiste;
    cond espera[20];
    boolean libre=true;
    int esperando=0;
    Procedure entrada(id:int in; prioridad:int in){
        if(!libre){
        esperaPrioridad.push(id,prioridad);
            esperando++;
            wait(espera[id]);
        }else{
            libre=false;
        }
    }
    Procedure liberar(){
      if(esperando>0){
        signal(espera[esperaPrioridad.pop(id)])//hago signal del primero en la cola(ordenado por prioridad), consiguiendo su id. 
        esperando--;
      }
      else{
        libre=true;
      }
    }
}

//Parcial MC 4-12-23

// SEM: Un sistema debe validar un conjunto de 10000 transacciones que se encuentran disponibles en una estructura de 
// datos. Para ello, el sistema dispone de 7 workers, los cuales trabajan colaborativamente validando de a 1 
// transacción por vez cada uno. Cada validación puede tomar un tiempo diferente y para realizarla los workers 
// disponen de la función Validar(t), la cual retorna como resultado un número entero entre 0 al 9. Al finalizar el 
// procesamiento, el último worker en terminar debe informar la cantidad de transacciones por cada resultado de la 
// función de validación. Nota: maximizar la concurrencia.

// 1 sitema, 10000 transacciones, 7 workers validan de a 1 a la vez, tiempos diferentes,(tienen la funcion Validar(t)), se debe guardar cuantas veces salio 0..9.

cola datos;//pre cargado con 10000 datos.
sem mutexD=1;
int cant=0;
int contador[10]([10]0);
sem cont[10]([10]0);
int termino=0;
sem terminoDisp=1;
sem espera=0;

Process worker[id:1..7]{
    int nValidacion;
    transaccion t;
    P(mutexD);
    while(!datos.isEmpty()){
        t=datos.pop();
        V(mutexD);
        nValidacion=Validar(t);
        P(cont[nValidacion]);
            contador[nValidacion]++;
        V(cont[nValidacion]);
    }
    V(mutexD);
    P(terminoDisp);
    termino++;
    if(termino==7){
        for (i=0;i<9;i++){
            print(cont[i]);//printeo la cantidad de validacion que salieron aca.
        }  
        for(i=0;i<7;i++){
            V(espera);//deja 7 "listos en espera" 1 para el que se va a dormir despues de esto y el resto para los que no fueron los ultimos.
        }
        V(terminoDisp)
        P(espera);
    }
    else{
        V(terminoDisp);
        P(espera);
    }
}

// En una empresa trabajan 20 vendedores ambulantes que forman 5 equipos de 4 personas cada uno (cada vendedor 
// conoce previamente a que equipo pertenece). Cada equipo se encarga de vender un producto diferente. Las 
// personas de un equipo se deben juntar antes de comenzar a trabajar. Luego cada integrante del equipo trabaja 
// independientemente del resto vendiendo ejemplares del producto correspondiente. Al terminar cada integrante del 
// grupo debe conocer la cantidad de ejemplares vendidos por el grupo. Nota: maximizar la concurrencia.

Process vendedor[id:1..20]{
    int equipo=Equipo();
    int cantidadVentas=0;
    int totalVentas=0;
    Equipo[equipo].juntarse();
    //realiza todas las ventas??//aumenta por cada venta cantidadVentas;//o venden a de a uno, y tienen que acceder al monitor por cada venta?// viendo la resolucion es como lo plantee, cargan todas las ventas que 1 realizo de una.
    Equipo[equipo].vendido(cantidadVentas,totalVentas);
}
Monitor Equipo[id:1..4]{
    int cant=0;
    cond espera;
    int ventasTotales=0;
    cond esperaTotal;
    Procedure juntarse(){
        cant++;
        if(cant==4){
            signal_all(espera);    
            cant=0;
        }
        else{
            wait(espera);
        }
    }

    Procedure vendido(cantV:int in;tV:int out){
        ventasTotales+=cantV;
        cant++;
        if(cant==4){
            signal_all(esperaTotal);
        }
        else{
            wait(esperaTotal);
        }
        tV=ventasTotales;
    }
}


//PARCIAL 18-12-23

// Resolver con SEMÁFOROS el siguiente problema. Se debe simular el uso de una máquina expendedora de gaseosas 
// con capacidad para 100 latas por parte de U usuarios. Además existe un repositor encargado de reponer las latas de 
// la máquina. Los usuarios usan la máquina según el orden de llegada. Cuando les toca usarla, sacan una lata y luego 
// se retiran. En el caso de que la máquina se quede sin latas, entonces le debe avisar al repositor para que cargue 
// nuevamente la máquina en forma completa. Luego de la recarga, saca una lata y se retira. Nota: maximizar la 
// concurrencia; mientras se reponen las latas se debe permitir que otros usuarios puedan agregarse a la fila.

cola ordenUsuarios;
sem mutexOrden=1;
int latas=100;
sem hayUsuario=0;
sem maquinaLibre=1;
sem agarraLata[U];
sem contLatas=1;
sem mefui=0;
Process Usuraio[id:1..U]{
    P(mutexOrden)
        ordenUsuarios.push(id)//pushea su id
    V(mutexOrden);
    V(hayUsuario)//avisa que dejo su id
    P(agarraLata[id])//espera que le confirmen que entre a agarrar la lata
    //agara lata
    P(contLatas)//pide el mutex del contador
    latas--;//resta
    V(contLatas)//devuelve
    V(mefui[id]);//avisa que ya se va
}
Process Maquina{
    int idAtender;
    while(true){
        wait(hayUsuario);//espera a que llegue alguien, si llegan mas de 1, quedan acumulados
        P(mutexOrden)//mutex de la cola ordenUsuario
        idAtender=ordenUsuarios.pop();//consigue el id(orden de llegada)
        V(mutexOrden)//devuelve el mutex
        V(agarraLata[idAtender]);//le da el permiso de que agarre una lata
        P(mefui[idAtender])//espera a que el usuario le diga que se fue.
        P(contLatas)//pide el mutex para con contador de latas
        if(latas==0){//si no hay mas hay que reponer
            V(contLatas)//devuelve el mutex que uso para ver la cantiad de latas
            Repositor.reponer();//llama al repositor
        }
        V(contLatas)//si todavia habia devuelve el mutex del contador, para que todo siga correcto.
    }
}
Process Repositor{
    Procedure reponer(){
        P(contLatas)//pide el mutex para modificar la cantidad de latas
        latas=100;//aumenta
        latas--//se toma una
        V(contLatas)//devuelve el mutex del contador de latas
    }
}
//CREO QUE ESTA MAL, O AL MENOS NO ESTA RESULTO CASI NI PARECIDO

//Resolver el siguiente problema con MONITORES. En una montaña hay 30 escaladores que en una parte de la subida 
// deben utilizar un único paso de a uno a la vez y de acuerdo al orden de llegada al mismo. Nota: sólo se pueden 
// utilizar procesos que representen a los escaladores; cada escalador usa sólo una vez el paso

Process escalador[id:1..30]{
    subida.utilizar(id);
    subida.salir();
}

Monitor subida{
    cond espera;
    boolean libre=true;
    int esperando=0;

    Procedure utilizar(id:int in){
        if(!libre){
            esperando++;
            await(espera);
        } ;
        else{
            libre=false;
        }
    }
    procedure salir(){
        if(esperando>0){
            signal(espera);
            esperando--;
        }
        else{
            libre=true;
        }
    }

}
// PRACTICA 2 Semaforos

// 2. Un sistema de control cuenta con 4 procesos que realizan chequeos en forma 
// colaborativa. Para ello, reciben el historial de fallos del día anterior (por simplicidad, de 
// tamaño N). De cada fallo, se conoce su número de identificación (ID) y su nivel de 
// gravedad (0=bajo, 1=intermedio, 2=alto, 3=crítico). Resuelva considerando las siguientes 
// situaciones:
// a) Se debe imprimir en pantalla los ID de todos los errores críticos (no importa el 
// orden).
// b) Se debe calcular la cantidad de fallos por nivel de gravedad, debiendo quedar los 
// resultados en un vector global.
// c) Ídem b) pero cada proceso debe ocuparse de contar los fallos de un nivel de 
// gravedad determinado.

cola historial;
sem mutexH=1;
int cant=N;
int termino=0;
sem terminaron=1;
sem espera;
int vector[4]([4]0);
sem vector;//esto es necesario en el b, en el c cada uno se ocupa de uno solo, por lo que 1 solo proceso accede a esa posicion del vector.
Process chequeos[id:1..4]{
    int id;
    int fallo;
    P(mutexH)
    while(cant>0){
        id,fallo=historial.pop();
        cant--;
        V(mutexH);
        if(fallo==3){
            print(id);
        }
        if(fallo==id){
            vector[id]++;
        }
        P(mutexH)
    }
    V(mutexH)
    P(terminaron)
    termino++;
    if(termino==4){
        V(terminaron)
        for i=1;i<4;i++{
            V(espera)
        }
        P(espera)
    }
    else{
        V(terminaron)
        P(espera)
    }
}

//4. Suponga que existe una BD que puede ser accedida por 6 usuarios como máximo al 
// mismo tiempo. Además, los usuarios se clasifican como usuarios de prioridad alta y 
// usuarios de prioridad baja. Por último, la BD tiene la siguiente restricción:
// • no puede haber más de 4 usuarios con prioridad alta al mismo tiempo usando la BD.
// • no puede haber más de 5 usuarios con prioridad baja al mismo tiempo usando la BD.
// Indique si la solución presentada es la más adecuada. Justifique la respuesta. 

sem limitedb=6;
sem limiteBaja=5;
sem limiteAlta=4;

Process usuarioAlta[id:1..n]{
    P(limiteAlta)
    P(limitedb)
    //accede
    V(limitedb)
    V(limiteAlta)

}
Process usuarioBaja[id:1..N]{
    P(limiteBaja)
    P(limitedb)
    //accede
    V(limitedb)
    V(limiteBaja)
}

// En una estación de trenes, asisten P personas que deben realizar una carga de su tarjeta SUBE en la terminal 
// disponible. La terminal es utilizada en forma exclusiva por cada persona de acuerdo con el orden de llegada. 
// Implemente una solución utilizando sólo emplee procesos Persona. Nota: la función UsarTerminal() le permite cargar 
// la SUBE en la terminal disponible.

sem accessTerminal[P];
cola llegada;
sem mutexC=1;
boolean libre=true;
int esperando=0;
sem espera=1;

Process persona[id:1..P]{
    if(!libre){
        P(espera)
        esperando++;
        V(espera)
        P(mutexC)
        llegada.push(id);
        V(mutexC)
        P(accessTerminal[id]);
    }
    else{
        libre=false;
    }
    UsarTerminal();
    P(espera)
    if(esperando>0){
        P(mutexC)
        V(accessTerminal[llegada.pop()]);
        V(mutexC)
        esperando--;
    }
    else{
        libre=true;
    }
    V(espera)
}
//8min

// Resuelva el mismo problema anterior pero ahora considerando que hay T terminales disponibles. Las personas 
// realizan una única fila y la carga la realizan en la primera terminal que se libera. Recuerde que sólo debe emplear 
// procesos Persona. Nota: la función UsarTerminal(t) le permite cargar la SUBE en la terminal t

cola terminales;//tiene las T terminales ya cargadas
sem accessTe=1;
int terminales=T;
sem contT=1;
sem espera[P]([P]0);
cola esperando;
sem mutexEsp=1;

Process persona[id:1..P]{
    int t;
    P(contT)
    if(terminales==0){
        V(contT)
        P(mutexEsp)
        esperando.push(id);
        V(mutexEsp)
        P(espera[id]);
    }
    else{
        P(contT)
        terminales--;
        V(contT)
    }
    P(accessTe)
    t=terminales.push()
    V(accessTe)
    UsarTerminal(t);
    P(accessTe)
    terminales.pop(t)
    V(accessTe)
    P(mutexEsp)
    if(!esperando.isEmpty()){
        V(espera[esperando.push()])
        V(mutexEsp)
    }
    else{
        V(mutexEsp)
        P(contT)
        terminales++;
        V(contT)
    }
}
//12min

// Resolver con MONITORES el siguiente problema: En una elección estudiantil, se utiliza una máquina para voto 
// electrónico. Existen N Personas que votan y una Autoridad de Mesa que les da acceso a la máquina de acuerdo con el 
// orden de llegada, aunque ancianos y embarazadas tienen prioridad sobre el resto. La máquina de voto sólo puede ser 
// usada por una persona a la vez. Nota: la función Votar() permite usar la máquina

Process Persona[id:1..N]{
    int prioridad=Prioridad();
    maquina.entrada(id,prioridad);
    Votar();
    maquina.salida()
}
Process Autoridad{
    while(true){
        maquina.siguiente();
    }
}
Monitor maquina{
    cola espera;//cola que depende de la prioridad se ordena la misma
    cond esperando[N];
    cond salio;
    cond entro;
    Procedure entrada(id:int in;prioridad: int in){
        espera.push(id,prioridad);
        signal(entro);
        await(esperando[id]);
    }
    Procedure salida(){
        signal(salio);

    }
    Procedure siguiente(){
        if(!espera.isEmpty())-> await(entro);
        signal(esperando[espera.pop()]);
        await(salio);
    }
}
//12mins


// Resolver con SEMÁFOROS el siguiente problema. En una planta verificadora de vehículos, existen 7 estaciones donde 
// se dirigen 150 vehículos para ser verificados. Cuando un vehículo llega a la planta, el coordinador de la planta le 
// indica a qué estación debe dirigirse. El coordinador selecciona la estación que tenga menos vehículos asignados en ese 
// momento. Una vez que el vehículo sabe qué estación le fue asignada, se dirige a la misma y espera a que lo llamen 
// para verificar. Luego de la revisión, la estación le entrega un comprobante que indica si pasó la revisión o no. Más allá 
// del resultado, el vehículo se retira de la planta. Nota: maximizar la concurrencia. 

sem contadorEstaciones[7]([7]1);
int estaciones[7]([7]0);//array que tiene la cantidad de autos de cada estacion, cuenta con la funcion de conseguir el indice del que menor cantidad tiene.
cola autosEsperando;
sem mutexC=1;
sem hayAuto=0;
int estacionDesignada[150]([150]0);
sem estacionAsignada[150]([150]0);
sem estacionAtendiendo[7]([7]1);
int atendeme[7]([7]0);
sem podesDejarTuId[7]=([7]1);
int respuesta[150]([150]0);
sem respuestaEnviada[150]([150]0);


Process vehiculo[id:1..150]{
    int idEstacion;
    int r;
    P(mutexC)
    autosEsperando.push(id);
    V(mutexC)
    V(hayAuto);
    P(estacionAsignada[id]);
    idEstacion=estacionDesignada[id];

    V(podesDejarTuId[idEstacion]);
    atendeme[idEstacion]=id;
    V(estacionAtendiendo[idEstacion]);
    P(respuestaEnviada[id]);
    r=respuestaEnviada[id];
    P(contadorEstaciones[idEstacion])
    estaciones[idEstacion]--;
    V(contadorEstaciones[idEstacion]);
}

Process coordinador{
    int idAtender;
    while(true){
        P(mutexC)
        if(autosEsperando.isEmpty()){
            V(mutexC)
            P(hayAuto);
        } 
        idAtender=autosEsperando.pop();
        V(mutexC)
        estacionDesignada[idAtender]=estaciones.menorCantidadEsperando();
        V(estacionAsignada[idAtender]);
    }
}
Process estacion[id:1..7]{
    int idAtender;
    int r;
    while(true){
        P(estacionAtendiendo[id]);
        idAtender=atendeme[id];
        r=atender(idAtender);
        respuestaEnviada[idAtender]=r;
        V(respuestaEnviada[idAtender]);
        V(podesDejarTuId[id]);
    }
}
//22mins aprox.

// 2. MONITORES. Una boletería vende E entradas para un partido, y hay P personas (P>E) que quieren comprar.
// Se las atiende por orden de llegada y la función vender() simula la venta. 
// La boletería debe informarle a la persona que no hay más entradas disponibles o devolverle el número de entrada si pudo hacer la compra.

// 3. MONITORES. Por un puente turístico puede pasar sólo un auto a la vez. 
// Hay N autos que quieren pasar (función pasar()) y lo hacen por orden de llegada.

// Resolver con MONITORES el siguiente problema. En un sistema operativo se ejecutan 20 procesos que 
// periódicamente realizan cierto cómputo mediante la función Procesar(). Los resultados de dicha función son 
// persistidos en un archivo, para lo que se requiere de acceso al subsistema de E/S. Sólo un proceso a la vez puede hacer 
// uso del subsistema de E/S, y el acceso al mismo se define por la prioridad del proceso (menor valor indica mayor 
// prioridad).

// Monitor: En una empresa trabajan 20 vendedores ambulantes que forman 5 equipos de 4 personas cada uno (cada vendedor 
// conoce previamente a que equipo pertenece). Cada equipo se encarga de vender un producto diferente. Las 
// personas de un equipo se deben juntar antes de comenzar a trabajar. Luego cada integrante del equipo trabaja 
// independientemente del resto vendiendo ejemplares del producto correspondiente. Al terminar cada integrante del 
// grupo debe conocer la cantidad de ejemplares vendidos por el grupo. Nota: maximizar la concurrencia.
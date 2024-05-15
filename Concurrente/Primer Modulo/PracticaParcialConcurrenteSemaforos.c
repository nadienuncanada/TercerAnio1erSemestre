// a) En una estación de trenes, asisten P personas que deben realizar una carga de su tarjeta SUBE en la terminal 
// disponible. La terminal es utilizada en forma exclusiva por cada persona de acuerdo con el orden de llegada. 
// Implemente una solución utilizando sólo emplee procesos Persona. Nota: la función UsarTerminal() le permite cargar 
// la SUBE en la terminal disponible. 

sem mutexC=1;
bool libre=true;
cola esperando;
sem espera[P]([P]0);

Process Persona[id:1..P]{

  int idSig;
    P(mutexC)
  if(!libre){
    esperando.push(id);
    V(mutexC)
    P(espera[id]);
  }
  else{ 
    libre=false;
    V(mutexC)
  }
  UsarTerminal()
  P(mutexC)
  if(!esperando.isEmpty()){
    idSig=esperando.pop();
    V(espera[idSig]);
  } 
  else{
    libre=true;
  }
    V(mutexC)
}

//Resuelva el mismo problema anterior pero ahora considerando que hay T terminales disponibles. Las personas 
// realizan una única fila y la carga la realizan en la primera terminal que se libera. Recuerde que sólo debe emplear 
// procesos Persona. Nota: la función UsarTerminal(t) le permite cargar la SUBE en la terminal t

cola esperando;
sem espera[P];
sem listo[P];
cola libres;//tiene las T terminales disponibles.
sem mutexC=1;
sem mutex=1;
int libres=T;//esto me faltaba, antes me fijaba si habia en la cola, pero si hago eso, se puede meter alguien que no tendria que entrar.
//ya que si habia alguien en la cola de espera, lo llamo pero al devolver a la cola la terminal alguien que recien entra lo podria tomar, haciendo bastante lio. encambio manteniendo la int de libres puedo mantener mejor el control ya que solo aumento libres cuando verdaderamente pueden usar la terminal que deje.


Process Persona[id:1..P]{
  int terminalAUsar;
  P(mutexC)
  if(libres==0){
      cola.push(id);
      V(mutexC)
      P(esperando[id]);
  }
  else{
    libres--;
    V(mutexC);
  }
  P(mutex)
  terminalAUsar=libres.pop();
  UsarTerminal(terminalAUsar);
  V(mutex);
  P(mutex);
  libres.push(terminalAUsar);
  V(mutex);
  P(mutexC);
  if(!esperando.isEmpty()){
      V(espera[esperando.pop()]);
  }
  else{
    libres++;
  }
  V(mutexC);
}



1. SEMÁFOROS. Existen 15 sensores de temperatura y 2 módulos centrales de procesamiento. Un sensor mide la temperatura cada cierto tiempo (función medir()), 
la envía al módulo central para que le indique qué acción debe hacer (un número del 1 al 10) (función determinar() para el módulo central) y la hace (función realizar()).
 Los módulos atienden las mediciones por orden de llegada.

cola Mediciones;//el id y la medicion//respetar el orden
int Devoluciones[15]//para saber quien lo toma
sem semDev[15]{[15]0};
sem mutexC=1;
sem hayDato=0;

Process Sensor[id:1..15]{
  while(true){
    int medicion;
    int respuesta;
    medicion=Medir();
    P(mutexC)
    Mediciones.push(id,medicion);
    V(hayDato)
    V(mutexC)
    P(semDev[id]);
    respuesta=semDev[id];
    realizan(respuesta);
  }
}

Process Central[id:1..2]{
  while(true){
    int idSensor;
    int medicion;
    P(hayDato)
    P(mutexC)
    idSensor,medicion=Mediciones.pop();
    V(mutexC)
    Devoluciones[idSensor]=determinar(medicion);
    V(semDev[idSensor])
  }
}
// 2. MONITORES. Una boletería vende E entradas para un partido, y hay P personas (P>E) que quieren comprar. 
// Se las atiende por orden de llegada y la función vender() simula la venta. La boletería debe informarle a la persona que no hay más entradas disponibles o 
// devolverle el número de entrada si pudo hacer la compra.

Process Persona[id:1..P]{
  int entrada;
  AccesoBoleteria.pedirEntrada(entrada);

}
Process Boleteria{
  while(true){
    VenderEntrada();
  }
}

Monitor AccesoBoleteria{
  int cantidaEntrada=e;
  cond espera;
  int esperando=0;
  int entradaActual;
  cond hayAlguien;

  procedure pedirEntrada(entrada:out int){
    esperando++;
    signal(hayAlguien);
    wait(espera);
    entrada=entradaActual;
  }

  procedure VenderEntrada(){
    if(esperando=0)->wait(hayAlguien)
    if(cantidaEntrada>0){
      entradaActual=vender();
      cantidaEntrada--;
    }
    else{
      entradaActual=-1;
    }
    esperando--;
    signal(espera);
  }
}

// 3. MONITORES. Por un puente turístico puede pasar sólo un auto a la vez. Hay N autos que quieren pasar (función pasar()) y lo hacen por orden de llegada.

Process Auto[id:1..N]{
  Puente.pedirPasar();
}
Monitor Puente{
  cond espera;
  int cantEsperando=0;
  boolean libre=true;

  Procedure pedirPasar(){
    if(!libre){
      cantEsperando++;
      wait(espera)
    }
    else{
      libre=false;
    }
    pasar();
    if(cantEsperando>0){
      cantEsperando--;
      signal(espera)
    }
    else{
      libre=true;
    }
  }
}

// Resolver con MONITORES el siguiente problema: En una elección estudiantil, se utiliza una máquina para voto 
// electrónico. Existen N Personas que votan y una Autoridad de Mesa que les da acceso a la máquina de acuerdo con el 
// orden de llegada, aunque ancianos y embarazadas tienen prioridad sobre el resto. La máquina de voto sólo puede ser 
// usada por una persona a la vez. Nota: la función Votar() permite usar la máquina.

Process Personas[id:1..N]{
  int prioridad;//prioridad de cada persona, dependiendo de si es ancina, embarazada, etc.
  Maquina.hacerFila(id,prioridad);
  Votar();
  Maquina.retirarse();
}
Process Autoridad{
  for(int i=1;i<N;i++){
    Maquina.dejarPasar();
  }
}

Monitor Maquina{
Cola fila;//esta cola puede encolar dependiendo del nivel de prioridad que se le ingresa, guardando en orden por prioridad los id de las personas
cond llamadoAutoridad;
cond espera[N];
boolean libre=true;
cond listo;

Procedure hacerFila(id:in int; prio:in int){
  fila.push(id,prio);
  signal(llamadoAutoridad);
  wait(espera[id])
  libre=false;
}
procedure dejarPasar(){
  if(fila.isEmpty())->wait(llamadoAutoridad);
  if(!libre){
    await(listo);
  }
  signal(espera[fila.pop(id)]);
}
Procedure retirarse{
  if(!fila.isEmpty()){
    signal(listo);
  }
  else{
    libre=true;
  }
}
}//creo que lo resolvi parecido a la resolucion, no igual, pero creo que respeto todo je.
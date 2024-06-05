// PMA-> declara chan(chan nombrechan(datos))/ guardas if(cond1)-> acciones1; (cond2)->acciones2...end if;
//PMS-> canales implicitos !(enviar) ?(recibir)/ son bloqueantes y sincronicos. if y do guardas, B;C(Ejemplo?)->S;
// ADA-> tasks y entrys, quien declara la entry puede hacer accept sobre esta, el resto solo entry calls a ella.





// SOLUCIONES MD - 2 (FabianMartinez->Facundo Tomatis practicas-> examenes practicos concu)

Resolver con PMA (Pasaje de Mensajes ASINCRÓNICOS) el siguiente problema. Simular la atención en un locutorio con 10 cabinas telefónicas, 
que tiene un empleado que se encarga de atender a los clientes. Hay N clientes que al llegar esperan hasta que el empleado les indica a que cabina ir, 
la usan y luego se dirigen al empleado para pagarle. El empleado atiende a los clientes en el orden en que hacen los pedidos, 
pero siempre dando prioridad a los que terminaron de usar la cabina. Nota: maximizar la concurrencia; suponga que hay una función Cobrar() 
llamada por el empleado que simula que el empleado le cobra al cliente.

chan Atencion(int);  
chan Atendido[N](int);//aca enviamos a que cabina tiene que ir
chan pagar(int,int,int);
chan Pagado[N]();

Process Clientes[id:1..N]{
  int cabina,precio;
  send Atencion(id);//espera a que le indiquen a que cabina ir
  receive Atendido[id](cabina);//le mandan a que cabina ir
  usarCabina(cabina,precio);
  send Pagar(id,precio,cabina);//envia lo que le costo usar la cabina su id y que cabina deja.
  receive Pagado[id]();//espera a que le cobren??
}

Process Empleado{
  int idAtender;
  cola libres;
  while(true){
    if(empty(pagar) && !libres.isEmpty() && !empty(Atencion))=>
      receive Atencion(idAtender);
      send Atendido[idAtender](libres.pop());//le doy una de las cabinas Libres.
    [] (!empty(pagar))=>//no tiene que haber nadie queriendo pagar y tiene que haber cabinas.
        receive Pagar(id,precio,cabina);
        Cobrar(precio);
        libres.push(cabina);
        send Pagado[id]();
    end if
  }
}
// SOLUCIONES MD - 2 (FabianMartinez->Facundo Tomatis practicas-> examenes practicos concu)

Resolver con ADA la siguiente situación. 
En una oficina hay un empleado y P personas que van para ser atendidas para realizar un trámite.
 Cuando una persona llega espera a lo sumo 20 minutos a que comience a atenderla el empleado para resolver el trámite que va a hacer, y luego se va; 
 si pasó el tiempo se retira sin realizar el trámite. El empleado atienden las solicitudes en orden de llegada. 
 Cuando las P personas se han retirado el empleado también se retira. Nota: cada persona hace sólo un pedido y termina; 
 suponga que existe una función Atender() llamada por el empleado que simula que el empleado está resolviendo el trámite del cliente; todas las tareas deben terminar.

Process parcial is

  task empleado is
    Entry atencion(tramite: in text, res: out text);
    Entry retirarse();
  end empleado;

  task type persona is;

  task contador is
    Entry retira();
  end contador;

  task body persona is 
    text tramite,res;
  begin
    tramite=conseguirTramite();
    SELECT
      empleado.atencion(tramite,res);
        contador.retira();//esto lo hace si o si, puede ir despues del select y lo puedo sacar de este y del or delay.
    OR DEALY(20mins)
      contador.retira();
    end select;
  end persona;

  task body contador is
  int contador=0;
  bool ya=true;
  begin
    while(contador<>P) loop
      accept retira() do
        contador++;
      end retira;
    end loop;
    empleado.retirarse(ya);// en cambio de mandar una boolean
  end contador;

  task body empleado is
  bool irse=false;
  begin
    while(!irse) loop
      SELECT 
       accept atencion(tramite,res) do 
          res:=Atender(tramite);
        end atencion;
      OR
        accept retirarse(ya) do//se puede hacer que al recibir el mensaje irse:=true; y hace el mismo efecto.
          irse:=ya;
        end retirarse;
      end select;
    end loop;
  end empelado;
begin
  null;//bueno si no es por pequenos detalles todo bien, igual bastante simple 15mins tarde, con 20 podia pulir mas.
end parcial;

//agusrnfr Practica->SegundoParcial-> Parcial 2 soluciones.

Resolver con Pasaje de Mensajes Sincrónicos (PMS) el siguiente problema. 
En un torneo de programación hay 1 organizador, N competidores y S supervisores. 
El organizador comunica el desafío a resolver a cada competidor. Cuando un competidor cuenta con el desafío a resolver, lo hace y lo entrega para ser evaluado. 
A continuación, espera a que alguno de los supervisores lo corrija y le indique si está bien. En caso de tener errores, el competidor debe corregirlo y volver a entregar, 
repitiendo la misma metodología hasta que llegue a la solución esperada. Los supervisores corrigen las entregas respetando el orden en que los competidores van entregando. 
Nota: maximizar la concurrencia y no generar demora innecesaria.

Process coordinador{
  int id,idS;
  text respuesta;
  cola esperando;
  while(true){
    if competidor[*]?resuelto(id,respuesta)=>//si hay alguien para dejar la respuesta, la acepta
      respuesta.push(id,respuesta);//la encola para no perder el orden.
    [] !respuesta.isEmpty();supervisor[*]?disponible(idS)=>//si hay alguien para correguir y hay un supervisor disponible
        supervisor[idS]!aCorreguir(respuesta.pop());//el pop da el id y la respuesta de un competidor;
  }
}

Process organizador{
  text desafío=generarDesafio();
  for i=1..N{
    competidor[*]?pedirDesafio(id)=>//recibe el id de quien pide el desafio
    competidor[id]!resolver(desafio);//le da el desafio a quien lo pidio
  }
}

Process competidores[i:1..N]{
  text desafio;
  bool correcto=false;
  organizador!pedirDesafio(i);//pide desafio
  organizador?resolver(desafio);//lo recibe
  while(!correcto){
      desafio=Resolver(desafio);
      coordinador!resuelto(id,desafio);//envia su id y el desafio resuelto
      supervisor[*]?respuesta(correcto);//recibe la correcion, si esta mal loop de nuevo;
  }
}

Process supervisor[i:1..N]{
  text respuesta;
  bool correcion=false;
  int idC;
  while(true){
    coordinador!disponible(i);//dice que esta disponible haciendo que le puedan dar para correguir
    coordinador?aCorreguir(idC,respuesta);//recibe id y la respuesta
    correcion=correguir(respuesta);//corrigue y obvtiene la correcion
    competidor[idC]!respuesta(correcion);//manda la correcion a quien es debido.
  }
}

//agusrnfr Practica->SegundoParcial-> Parcial 2 soluciones.

Resolver con ADA el siguiente problema. 
Una empresa de venta de calzado cuenta con S sedes. En la oficina central de la empresa se utiliza un sistema que permite controlar el stock de los diferentes modelos, 
ya que cada sede tiene una base de datos propia. El sistema de control de stock funciona de la siguiente manera: 
dado un modelo determinado, lo envía a las sedes para que cada una le devuelva la cantidad disponible en ellas; al final del procesamiento, 
el sistema informa el total de calzados disponibles de dicho modelo. Una vez que se completó el procesamiento de un modelo, se procede a realizar lo mismo con el siguiente modelo. Nota: suponga que existe una función DevolverStock(modelo,cantidad) que utiliza cada sede donde recibe como parámetro de entrada el modelo de calzado y retorna como parámetro de salida la cantidad de pares disponibles. 
Maximizar la concurrencia y no generar demora innecesaria.
//que esta pasando aca?-> necesitamos no generar demora innecesaria, por lo que el for que "le manda el dato" tiene que ser a peticiones, es decir quien necesita el modelo lo va a pedir, no podemos enviarle y esperar que lo agarre eso esta mal(genera demora), por lo que primero que hacemos es hacer un for aceptando peticiones, una vez que termina el for a medidad que los procesos van pidiendo el modelo se va terminando el for, una vez que fueron todos enviados, entramos a otro for donde recibimos los resultados, de la misma manera, no pidiendole a cada uno sino esperando a que cada uno envie la respuesta(evitando otra demora innecesaria), y ahi es donde contabilizamos el total de ese modelo.
// por otro lado tenemos la sede quien va a hacer la peticion del modelo, para luego conseguir la cantidad y enviar el resultado.  
//la idea principal es evitar la demora inncesaria mediante la peticion de los recursos y no el "forzamiento" de recibir estos. por esto se hace que cada uno envie cuando este listo para recibir o dar y no forzarlo a recibir el mensaje o pedirle la peticion. Sino logramos un programa secuencial.(este ejercicio se basa en el 7 de la practica de ADA).
Process parcial is

  task type sede is;
  Sedes:array(1..S) of sede;

  task sistema is 
    Entry pedido(modelo: out data);
    Entry devolucion(cantidad: in int);
  end sistema;
  task body sede is
    data modelo; 
    int cant=-1;
  begin
    while(true) loop
      sistema.pedido(modelo);
      DevolverStock(modelo,cant);
      sistema.devolucion(cant);
    end loop;
  end sede;
  task body sistema is
    data modelo;
    int cant,cantTotal; 
  begin
    while(true) loop
      cant=0;
      cantTotal=0;
      modeloAct=generarModelo();
      for i in 1..S loop
        accept pedido(modelo:out data) do
          modelo:=modeloAct;
        end pedido;
      end loop;
      for i int 1..S loop
        accept devolucion(cant: in int) do 
          cantTotal+=cant;  
        end devolucion;
      end loop;
      print('Cantidad total del modelo:',modelo,'=',cantTotal);
    end loop;
  end sistema;
begin
  null;
end parcial;

//(agusrnfr) Practicas/Segundo Parcial/PConcu-2022---Soluciones-parcial-MD.pdf
Resolver con PMA el siguiente problema.
 Se debe modelar el funcionamiento de una casa de venta de repuestos automotores, en la que trabajan V vendedores y que debe atender a C clientes. 
 El modelado debe considerar que: (a) cada cliente realiza un pedido y luego espera a que se lo entreguen;y (b) los pedidos que hacen los clientes son tomados por cualquiera de los vendedores. 
Cuando no hay pedidos para atender, los vendedores aprovechan para controlar el stock de los repuestos (tardan entre 2 y 4 minutos para hacer esto). 
 Nota: maximizar la concurrencia.

chan pedido(int,listado);//idC
chan entrega[C](type);//respuesto
chan atencion(int)//idV
chan atenderA[V](int)//idC

Process vendedor[i:1..V]{
  int idC;
  text listado;
  type repuesto;
  while(true){
    send atencion(i)
    receive atenderA[i](idC,listado);
    if(idC==-1){
      delay(random(2,4));//controla stock de 2 a 4 mins.
    }
    else{//le envia el repuesto;
        repuesto=conseguir(listado);
        send entrega[idC](respuesto);
    }
  }
}

Process cliente[i:1..C]{
  type repuesto;
  text listado;
  send pedido(i,listado);
  receive entrega[i](repuesto);
}
Process coordinador{
  buffer cola;
  int idC,idV;
  while(true){//que tan mal esta mi enfoque? de guardar los clientes en una cola? en la resolucion hacer un receive de que pueden atender y apartir de ahi atienden o lo mandar a rebisar stock/ lo cual veo mas eficiente.
    if(!empty(pedido))->//si hay pedido lo acepto
     receive pedido(id,listado);
     cola.push(id,listado);
    if(empty(pedido) && !empty(atencion))->//si hay un empleado que pide
      receive atencion(idV);
      if(!cola.isEmpty()){
      send atenderA[idV](cola.pop());
      }
      else{
        send atenderA[idV](-1,null);
      }
    //de esta manera, la anterior sirve? si-> es mas limpia este y creo que es un mejor manejo de guardas en general.
    receive atencion(idV)
    if(!empty(pedido)){
      receive pedido(idC,listado);
      send atenderA[idV](idc,listado);
    }
    else{
      send atenderA[idV](-1,null);
    }
  }
}


SITIO WEB IDENTIFICACION GENETICA
Process parcial is 
  task type servidor;//encargado de recibir las muestras y darle la respuesta a los clientes
  Servidores: array(1..5) of servor;//son 5 servidores

  task Admin is//encargado de aceptar secuencias y pasarlos a los servidores
    Entry entregaSecuencia(secuencia:in string, idC:in int);//recibe el id y la secuencia de los clientes
    Entry disponible(idC:out int secuencia:out string);//recibe que servidor esta disponible, y se le va a dar el idC y secuencia de ese cliente.
  end admin;

  task type cliente is//encargado de generar la muestra enviarla y esperar la respuesta.
    Entry iden(id:in int);//recibe su id
    Entry respuesta(resultado: in int);//recibe la respuesta
  end cliente;

  Clientes: array(1..C) of cliente;

  task body cliente is
    string secuencia;
    int idC,resultadoC;
  begin 
    accept ident(i) do
      idC:=i;
    while(true) loop
      secuencia:=generar();
      Admin.entregaSecuencia(secuencia,idC)
      accept respuesta(id)(resultado) do 
        resultadoC:=resultado;
      end respuesta;
    end loop;
  end cliente;

  task body servidor is
    string secuencia;
    int id,idC,resultado;
  begin
    accept ident(i) do 
      id=i;
    while(true)loop
      Admin.disponible(idC,secuencia);
      resultado=ResolverAnalisis(secuencia);
      cliente(idC).respuesta(resultado);
    end loop
  end servidor

  task body Admin is
    int idS;
  begin
    while(true) loop
      accept disponible(idC,secuencia) do
        accept entregaSecuencia(secuencia,idC) do
        end entregaSecuencia;
      end disponible;
    end loop;
  end Admin;
begin
  for i in 1..C loop
    cliente(i).ident(i);
  end loop;
end parcial;

Resolver con Pasaje de Mensajes Sincrónicos (PMS) el siguiente problema. 
En un torneo de programación hay ( 1 ) organizadores, ( n ) competidores y ( s ) supervisores. 
El organizador comunica el desafío a resolver a cada competidor. Cuando un competidor cuenta con la solución a resolver, lo hace y entrega para ser evaluado. 
A continuación espera a que alguno de los supervisores lo corrija y le indique si está bien. En caso de tener errores, el competidor debe corregirlo y volver a entregar, repitiendo la misma metodología hasta llegar a la solución esperada. Los supervisores corrigen las entregas respetando el orden en que los competidores van entregando. 
Nota: maximizar la concurrencia y no generar demora innecesaria.

Process organizador{
  int id;
  string desafio;
  desafio=conseguirDesafio();
  for i=1 to N{
    competidor[*]?dame(id);
    competidor[id]?consigo(desafio)
  }
}
Process admin{
  int idS,idC;
  string desafio;
  buffer cola;
  while(true){
    supervisor[*]?disponible(idS)//de esta manera no se respeta el orden de competidores(creo).
    competidor[*]?entregar(desafio,idC);
    supervisor[idS]!correguir(desafio,idC);
    //esta es la manera correcta
    if(competidor[*]?entregar(desafio,idC))->
      cola.push(desafio,idC);
    [](!cola.isEmpty()); supervisor[*]?disponible(idS)->
      supervisor[idS]!correguir(cola.pop());//dandole el id y el desafio 
    end if;
  }
}
Process competidor[id:1..N]{
  string desafio;
  bool correcto=false;
  organizador!dame();
  organizador?consigo(desafio);
  while(!correcto){
    desafio=HacerDesafio();
    admin!entregar(desafio,id);
    supervisor[*]?resolucion(correcto);
  }
}
Process supervisor[id:1..S]{
  bool correcion;
  int idC;
  while(true){
      admin!disponible(id);
      admin?correguir(idC,desafio);
      correcion:=correguir(desafio);
      competidor[idC]!resolucion(correcion);
  }
} 

// ADA-> banco

//clientes realizan pago y se llevan comprobante. regualres y premiun. 1 unico empleado que atiende en orden de llegada,dando prioridad a los premiuns,los regulares esperan hasta  un maximo de 30mins.

Process parcial is
  task type clienteR;//va a pedir pagar y espera recibir el comprobante.
  ClientesR:array(1..R) of clienteR;
  task type clienteP;
  ClientesP:array(1..P) of clienteP;

  task empleado is
    entry solicitudR(pago:in int;comprobante: out string);
    entry solicitudP(pago:in int;comprobante: out string);
  end empelado;

  task body clienteR is
  begin
    SELECT
      admin.solicitudR(pago,comprobrante);
    OR DELAY(180)
      null;
  end clienteR;
  task body clienteP is

  begin
    admin.solicitudP(pago,comprobante);
  end clienteP;
  task empleado is

  begin
    while(true){

      SELECT
        WHEN(solicitudP`count==0)-> accept solicitudR(pago:in int;comprobante: out string) do
          comprobrante:=HacerPago(pago);
        end solicitudR;
      OR
        accept solicitudP(pago:in int;comprobante: out string) do
          comprobrante:=HacerPago(pago);
        end solicitudP;
    }
  end;  
begin
  null;
end;  





// PMS-> 20 turistas 1 tirolesa. 1 guia 1 empleado. empleado espera a todos los turistas.

Process Turista[i:1..20]{
  Empleado!llegue();//avisa que llego
  Empelado?terminoCharla();//espera que termine la charla
  Guia!pedirTirolesa(i);//pide tirolesa
  Guia?usarTirolesa();//espera el uso
  usarTirolesa();//la usa
  Guia!dejeTirolesa();//avisa que la dejo.
}
Process Empleado{
  for i=1 to 20{//espera los 20 sin ningun orden(no busy waiting)
    Turista[*]?llegue();
  }
  darCharla();//da la charla
  for i=1 to 20{//le dice a los 20 que termino la charla
    Turista[i]!terminoCharla();
  }
}

Process Guia{
    int total=0;
    bool libre=true;
    cola buffer;
    while(total!=20){
      if(libre);Turista[*]?pedirTirolesa(id)->//si esta libre usala y lo pongo en false.
        libre=false;
        Turista[id]!usarTirolesa();
      []Turista[*]dejeTirolesa()->//si quiere dejarla siempre lo atiendo.
        if(cola.isEmpty()){//si no hay nadie que la quiera usar dejo libre
          libre=true;
        }
        else{//sino se la doy a la siguiente persona en orden
          Turista[cola.pop()]!usarTirolesa
        }
      [](!libre);Turista[*]?pedirTirolesa(id)->//si no esta libre te guardo en la cola.
        cola.push(id);
    }
  }



// ADA-> 1 empleado, pedidos de N clientes, ancionos/embarazadas, atiende los pedidos, primero embarazadas si no las atendes al toke se van, ancianos bancan 5, el resto esperan si o si. atiende en orden.
Process parcial is 
  task type embarazadas
  task type ancianos;
  task type cliente;
  task empleado is
    Entry emb(pedido: in pedido);
    Entry anc(pedido: in pedido);
    Entry cli(pedido: in pedido);  
  end empleado;

task body embarazadas is
  pedido p;
begin 
  p=generarPedido();
  SELECT
    Empleado.emb(p);
  else
    null;
  emd select;
end embarazadas;

task body ancianos is
  pedido p;
begin 
  p=generarPedido();
  SELECT
    Empleado.anc(p);
  OR DELAY(300)//5mins;
    null;
  end select
end ancianos;

task body cliente is
  pedido p;
begin 
  p=generarPedido();
  Empleado.cli(p);
end cliente;

task body empleado is
begin
  SELECT
    accept emb(p) do   
      AtenderPedido(p)
    end emb;
  OR
    WHEN(emb`count==0); accept anc(p) do
      AtenderPedido(p);
    end anc;
  OR 
    WHEN(emb`count==0 and anc`count==0); accept cli(p) do
      AtenderPedido(p);
    end cli;
end empelado;

begin
  null;
end parcial;//12 mins-> todo bien??

//PMA-> 100 empleados 5 impresoras, orden de llegada, asignan a impresora libre.
chan Impresora(int,documento);
chan impreso[100](fotocopia);

Process Empleado[i:1..100]{
  documento=getDocumneto();
  fotocopia f;
  send impresora(i,documento);//manda para imprimir el documento y su id.
  receive impreso[i](f);//recibe la fotocopia, se la envia exclisivamente a el

}

Process Impresora[id:1..5]{
  int i;
  doc documento;
  fotocopia f;
  while(true){
    recieve impresora(i,documento);//recibe
    f=Imprimir(documento);//fotocopia
    send impreso[i](f);//devuelve
  }
}
//PMS-> 
Process Empleado[id:1..100]{
  documento d=getDocumneto();
  fotocopia f;
  Admin!pedido(i,d);
  Impresora[*]?impreso(f);
}
Process Admin{
  int idE,idF;
  buffer cola;

  while(true){
    if Empleado[*]?pedido(idE,d)->
      cola.push(idE,d);
    if(!cola.isEmpty); Impresora[*]?lista(idF)->
    Impresora[idF]!fotocopia(cola.pop());//popea el idE,d(documento/s a fotocopiar);
  }
}
Process Impresora[i:1..5]{
  int idE;
  documento d;
  fotocopia f;
  while(true){
  Admin!lista(i);
  Admin?fotocopia(idE,d);
  fotocopiar(d);
  Empleado[idE]!impreso(f);
  }
}//19mins-> bien?? creeria que si y si no ya es tarde :)(23:57)-> rindo manana :) ;):(()()())

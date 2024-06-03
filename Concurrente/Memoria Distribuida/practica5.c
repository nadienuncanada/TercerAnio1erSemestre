1. Se requiere modelar un puente de un único sentido que soporta hasta 5 unidades de peso. 
El peso de los vehículos depende del tipo: cada auto pesa 1 unidad, cada camioneta pesa 2 
unidades y cada camión 3 unidades. Suponga que hay una cantidad innumerable de 
vehículos (A autos, B camionetas y C camiones). Analice el problema y defina qué tareas, 
recursos y sincronizaciones serán necesarios/convenientes para resolver el problema. 
a. Realice la solución suponiendo que todos los vehículos tienen la misma prioridad. 
b. Modifique la solución para que tengan mayor prioridad los camiones que el resto de los 
vehículos. 
a- 
Process ejercicio1 is
  Task puente is
    Entry entrada_auto;
    Entry entrada_camion;
    Entry entrada_camioneta;
    Entry salida_auto;
    Entry salida_camion;
    Entry salida_camioneta;
  end puente;
  Task type vehículo

  arrayAuto: array (1..A) of vehículo;
  arrayCamion: array(1..A) of vehículo;
  arrayCamionetas: array (1..a) of vehículo;

  Task body vehículo is
    begin 
      if("Auto")// si es un auto/camion/camioneta hace su debido llamado a ingreso, y quedara encolado.
        puente.entrada_auto;
      else if("Camion")
        puente.entrada_camion;
      else
        puente.entrada_camioneta;
      end if;

      if("Auto")//luego de haber ingresado, hace su llamado a salida, y se va.
        puente.salida_auto;
      else if("Camion")
        puente.salida_camion;
      else
        puente.salida_camioneta;
      end if;
  end vehículo;

  Task body puente is
  int peso=0;
  begin
    SELECT 
    WHEN(peso+1<=5)=>
      ACCEPT entrada_auto do
        peso=peso+1;
      end entrada_auto;
    OR
    WHEN(peso+2<=5)=>
      ACCEPT entrada_camioneta do
        peso=peso+2;
      end entrada_camioneta;
    OR 
    WHEN(peso+3<=5)=>
      ACCEPT entrada_camion do
        peso=peso+3;
      end entrada_camion;
    OR
    ACCEPT salida_auto do
      peso=peso-1;
    end salida_auto;
    OR 
    ACCEPT salida_camioneta do
      peso=peso-2;
      end salida_camioneta;
    OR
    ACCEPT salida_camion do
      peso=peso-3;
    end salida_camion;
    END SELECT;
  end puente;
begin
  null;
end ejercicio1;

b-
Process ejercicio1b is
  task puente is
    entry entrada_auto;
    entry entrada_camion;
    entry entrada_camioneta;
    entry salida_auto;
    entry salida_camion;
    entry salida_camioneta;
  end puente;
  task type vehiculo is;
  arrayAuto: array (1..A) of vehiculo; 
  arrayCamion: array (1..A) of vehiculo;
  arrayCamioneta: array (1..A) of vehiculo;
  
  task body vehiculo is
  begin
    if("Auto")  
      puente.entrada_auto;
    else if("Camioneta")
      puente.entrada_camioneta;
    else 
      puente.entrada_camion;
    fi
    if("Auto")
      puente.salida_auto;
    else if("Camioneta")
      puente.salida_camioneta;
    else
      puente.salida_camion;
    fi
  end vehiculo;

  task body puente is
  int peso=0;
  begin
    SELECT
      WHEN(entrada_camioneta`count=0 && peso+1<=5)=>
        ACCEPT entrada_auto do
          peso+=1;
        end entrada_auto;
      OR
      WHEN(entrada_camioneta`count=0 && peso+3<=5)=>
        ACCEPT entrada_camion do
          peso+=3;
        entrada_camion
      OR
      WHEN (peso+2<=5)=>
        ACCEPT entrada_camioneta do
          peso+=2;
        entrada_camioneta;
      OR
        ACCEPT salida_auto do
          peso-=1;
        end salida_auto
      OR
        ACCEPT salida_camioneta do
        peso-=2;
        end salida_camioneta;
      OR
        ACCEPT salida_camion do
          peso-=3;
        end salida_camion
    END SELECT
  end puente; 
begin
  null;
end ejercicio1b;

2. Se quiere modelar el funcionamiento de un banco, al cual llegan clientes que deben realizar 
un pago y retirar un comprobante. Existe un único empleado en el banco, el cual atiende de 
acuerdo con el orden de llegada. Los clientes llegan y si esperan más de 10 minutos se 
retiran sin realizar el pago. 

Process ejercicio2 is
task type cliente is;
arrayCliente: array (1..A) of cliente;
task empleado is
    entry pago(int in precio; text out comprobrante);
end empleado;

task body cliente is
int precio;
text comprobante;
begin
  precio=getPrecio();
  SELECT 
   empleado.pago(precio,comprobante);
  OR 
    delay(6000)
    null;
  END SELECT;
end cliente;
task body empelado is
begin
  loop
    accept pago(precio,comprobante)do 
      r=generarComprobante(precio);
      comprobante=r;
    end pago;
  end loop
end empelado;
begin
  null;
end ejercicio2

3. Se dispone de un sistema compuesto por 1 central y 2 procesos periféricos, que se 
comunican continuamente. Se requiere modelar su funcionamiento considerando las 
siguientes condiciones: - - 
La central siempre comienza su ejecución tomando una señal del proceso 1; luego 
toma aleatoriamente señales de cualquiera de los dos indefinidamente. Al recibir una 
señal de proceso 2, recibe señales del mismo proceso durante 3 minutos. 
Los procesos periféricos envían señales continuamente a la central. La señal del 
proceso 1 será considerada vieja (se deshecha) si en 2 minutos no fue recibida. Si la 
señal del proceso 2 no puede ser recibida inmediatamente, entonces espera 1 minuto y 
vuelve a mandarla (no se deshecha). 

3-
process ejercicio3 is

  task central is 
    Entry entrada_periferico1(text in senal);
    Entry entrada_periferico2(text in senal);
    Entry timer(in boolean seguir);
  end central;

  task timer is;

  task periférico_1 is;

  task periférico_2 is:

  task body central is 
  bool seguir;
  begin
    accept entrada_periferico1;//arranca solo si le envia p1
    loop
      SELECT//luego puede o aceptar p1 o p2
        accept entrada_periferico1(senal) do 

        end entrada_periferico1;
      OR
        accept entrada_periferico2(senal) do//si acepta p2, acepta al timer quien es el encargado de hacer la espera de 3 mins.
          accept timer(seguir)
          seguir=seguir;
          while(seguir) do//solo se espera p2 o timer(quien va a dar false y cerrar el while.)
            SELECT
              accept entrada_periferico2(senal) do
            OR 
              accept timer(seguir)
            END SELECT
          od
        end entrada_periferico2;
      end select;
    end loop 
  end central;

  task body timer is
  begin
    central.time(true);//cuando le llamen envia true
    delay 3mins;//espera 3 minutos
    central.time(false);// envia false, cuando ya estan dentro del while
  end timer;

  task body periférico_1 is
    text senal;
  begin
    loop
      SELECT
        senal=generarSenal();
        central.entrada_periferico1(senal);
        
      OR
        delay(2mins);
        null;
      end select;
    end loop;
  end periférico_2;

task body periférico_2 is
  text senal;
  bool nueva=true;
  begin
    loop
      senal=generarSenal();
      while(nueva) do
        SELECT
          central.entrada_periferico2(senal);
          nueva=false;
        ELSE
          delay(1min);
          nueva=true;
        end select;
      od
    end loop;
  end periférico_2;
begin
  null; 
end ejercicio3

4. En una clínica existe un médico de guardia que recibe continuamente peticiones de 
atención de las E enfermeras que trabajan en su piso y de las P personas que llegan a la 
clínica ser atendidos.  
Cuando una persona necesita que la atiendan espera a lo sumo 5 minutos a que el médico lo 
haga, si pasado ese tiempo no lo hace, espera 10 minutos y vuelve a requerir la atención del 
médico. Si no es atendida tres veces, se enoja y se retira de la clínica. 
Cuando una enfermera requiere la atención del médico, si este no lo atiende inmediatamente 
le hace una nota y se la deja en el consultorio para que esta resuelva su pedido en el 
momento que pueda (el pedido puede ser que el médico le firme algún papel). Cuando la 
petición ha sido recibida por el médico o la nota ha sido dejada en el escritorio, continúa 
trabajando y haciendo más peticiones. 
El médico atiende los pedidos dándole prioridad a los enfermos que llegan para ser atendidos. 
Cuando atiende un pedido, recibe la solicitud y la procesa durante un cierto tiempo. Cuando 
está libre aprovecha a procesar las notas dejadas por las enfermeras. 

Process ejercicio4 is 
  Task medico is
    Entry pedidoPersona;
    Entry pedidoEnfermera;
  end medico;

  task escritorio is
    Entry pedidoTardio(text in papel)
    Entry pedidoEscritorio(text out papel);

  end escritorio;

  task type enfermera;
  task type persona;

arrayPersonas: array (1..P) of persona;
arrayEnfermera: array (1..E) of enfermera;

task body Persona is
int intentos=0;
bool enojado=true;
int tiempo;
begin
  tiempo=getTiempo();
  while(intentos<3) and (enojado) do
    SELECT
      medico.pedidoPersona(tiempo) do 
        enojado=false;
    OR DELAY 5mins
      intentos++;
      if(intentos<3)=>delay(10mins)
    end select;
  od
  if(enojado)=>retirarseEnojadamente();
end persona;

task body enfermera is
text papel;
int tiempo;
begin
  LOOP
    SELECT
      tiempo=getTiempo();
      medico.pedidoEnfermera(tiempo);
    ELSE
      papel=generarPapel();
      escritorio.pedidoTardio(papel);
    end select;
  end loop;
end enfermera;

task body escritorio is
 buffer cola; 
begin
  loop
    SELECT 
      accept pedidoTardio(papel)do
        cola.push(papel);
      end pedidoTardio;
    OR
      accept pedidoEscritorio(papel)do
        if(!cola.isEmpty())=>papel=cola.pop();
        else=> papel="";
  end loop
end escritorio;

task body medico is
text p;
begin
  loop
    SELECT 
      WHEN(pedidoEnfermera`count=0)=> accept pedidoPersona(tiempo) do
        delay(tiempo)
      end pedidoPersona;
    OR
      accept pedidoEnfermera(tiempo) do
        delay(tiempo);
      end pedidoEnfermera;
    OR
      WHEN(pedidoEnfermera`count=0) and (pedidoPersona`count=0) escritorio.pedidoEscritorio(papel) do
        p=papel;
        if(p!="")
        firmar(p)
      end pedidoEscritorio;
  end loop;
end medico;
begin 
null;
end ejercicio4;


5. En un sistema para acreditar carreras universitarias, hay UN Servidor que atiende pedidos 
de U Usuarios de a uno a la vez y de acuerdo con el orden en que se hacen los pedidos.  
Cada usuario trabaja en el documento a presentar, y luego lo envía al servidor; espera la 
respuesta de este que le indica si está todo bien o hay algún error. Mientras haya algún error, 
vuelve a trabajar con el documento y a enviarlo al servidor. Cuando el servidor le responde 
que está todo bien, el usuario se retira. Cuando un usuario envía un pedido espera a lo sumo 
2 minutos a que sea recibido por el servidor, pasado ese tiempo espera un minuto y vuelve a 
intentarlo (usando el mismo documento). 

5-
Process ejercicio5 is

  task servidor is
    Entry entrega(documento: in text, correcto: out boolean);//envia documento y una boolean donde se le dice si es correcto.
  end servidor;

  task type usuario;
  arrayUsuario: array(1..U) of usuario;

  task body usuario is
    text documento;
    boolean correcto=false;
    boolean hacerDocumento=true;
    begin
      while (!correcto) do// mientras lo entregado no este correcto, se sigue repitiendo la logica.
            if(hacerDocumento)
              documento=Trabajar(documento);
        SELECT
          Servidor.entrega(documento,correcto)
          if(!correcto)=> hacerDocumento=true;//si lo entrego y esta mal tiene que volver a trabajarlo.
        OR DELAY(120) do //a lo sumo espera 2 minutos, con el entry
            delay(60);
            hacerDocumento=false;//si espero y no lo atendieron, no tiene que hacer de nuevo el documento, tiene que entregar el mismo.
          end delay
        end select;
      od
  end usuario;
  task body servidor is
  begin
    loop //constantemente recibe corrigue y deja el resultado en la variable out correcto.
      accept entrega(documento,correcto) do
        correcto=correguir(documento);
      end entrega;
    end loop;
  end servidor;
begin
  null;
end ejercicio5;

Process Testeo{
  while(true){
    trabaja(error);
    Mantenimiento!(error);
    Mantenimiento?(respuesta);.//se comentaria este para el 2
  }
}
// esta solucion para el 2(de la explicacion practica, hace que testeo espere a que mantenimiento termine la respuesta, lo que esta mal)
Process Mantenimiento{
  while(true){
    error = Testeo?(error);
    respuesta=resolver(error);
    Testeo!(respuesta);//se comentaria este para el 2
  }
}

Process Testeo{
  while(true){
    trabaja(error);
    Admin!(error);
  }
}

Process Admin{
  cola buffer;
  while(true){
    do Testeo?(error)-> buffer.push(error);//te mando un error testeo? guardalo,
      !buffer.isEmpty(); Mantenimiento?(){//hay un error en el buffer que enviar? mantienimiento te pidio un error?
         Mantenimiento!(buffer.pop());//es porque esta libre, mandale el error
      }
    od  
  }
}
Process Mantenimiento{
  while(true){
    Admin!();//le avisa que puede recibir un error
    Admin?(error);//lo recibe
    resolver(error);//lo resuelve
  }
}
1. Suponga que existe un antivirus distribuido que se compone de R procesos robots 
Examinadores y 1 proceso Analizador. Los procesos Examinadores están buscando 
continuamente posibles sitios web infectados; cada vez que encuentran uno avisan la
dirección y luego continúan buscando. El proceso Analizador se encarga de hacer todas las 
pruebas necesarias con cada uno de los sitios encontrados por los robots para determinar si 
están o no infectados.  
a) Analice el problema y defina qué procesos, recursos y comunicaciones serán 
necesarios/convenientes para resolver el problema. 
b) Implemente una solución con PMS. 

Process Examinador[i=1 to R]{
  while(true){
    sitioWeb = buscarSitioWeb();
    Analizador!(sitioWeb);
  }
}
Process Admin{
  cola buffer;
  while(true){
    do Examinador?(sitioWeb)-> buffer.push(sitioWeb);
      !buffer.isEmpty(); Analizador?(){//hay un sitio web en el buffer que enviar? analizador te pidio un sitio web?
         Analizador!(buffer.pop());//es porque esta libre, mandale el sitio web
      }
    od  
  }
}
Process Analizador{
  while(true){
    Admin!();//le avisa que puede recibir un sitio web
    Admin?(sitioWeb);//lo recibe
    resolver(sitioWeb);//lo resuelve
  }
}
2. En un laboratorio de genética veterinaria hay 3 empleados. El primero de ellos 
continuamente prepara las muestras de ADN; cada vez que termina, se la envía al segundo 
empleado y vuelve a su trabajo. El segundo empleado toma cada muestra de ADN 
preparada, arma el set de análisis que se deben realizar con ella y espera el resultado para 
archivarlo. Por último, el tercer empleado se encarga de realizar el análisis y devolverle el 
resultado al segundo empleado. 

Process Empleado1{
  text muestraADN;
  while(true){
    muestraADN = prepararMuestraADN();
    Intermediador!(muestraADN);
  }
}

Process Intermediador{
  cola buffer;
  text muestraADN;
  while(true){
    do Empleado1?(muestraADN)-> buffer.push(muestraADN);
      !buffer.isEmpty(); Empleado2?(){//hay una muestra de ADN en el buffer que enviar? empleado2 te pidio una muestra de ADN?
         Empleado2!(buffer.pop());//es porque esta libre, mandale la muestra de ADN
      }
    od  
  }
}
Process Empleado2{
  text resultado;
  text setPruebas;
  text muestraADN;
  while(true){
    Intermediador!();//le avisa que puede recibir una muestra de ADN
    Intermediador?(muestraADN);//lo recibe
    setPruebas=armar(muestraADN);//armar el set de pruebas  
    Empleado3!(setPruebas);
    Empleado3?(resultado);
  }
}
Process Empleado3{
  text setPruebas;
  text resultado; 
  while(true){
    Empleado2?(setPruebas);// recibe set de pruebas
    resultado=resolver(setPruebas);//lo resuelve
    Empleado2!(resultado);// se los envia al empleado 2
  }
}


3. En un examen final hay N alumnos y P profesores. Cada alumno resuelve su examen, lo 
entrega y espera a que alguno de los profesores lo corrija y le indique la nota. Los 
profesores corrigen los exámenes respetando el orden en que los alumnos van entregando.  
a) Considerando que P=1. 
b) Considerando que P>1. 
c) Ídem b) pero considerando que los alumnos no comienzan a realizar su examen hasta 
que todos hayan llegado al aula. 
Nota: maximizar la concurrencia y no generar demora innecesaria. 

a-
Process Alumno[i=1 to N]{
  text examen;
  int nota;
  while(true){
    examen = resolverExamen();
    Admin!(examen,i);
    Profesor?(nota);
    if(nota<6){
      print("llorar :(")
    }
    else{
      print("sonreir :)")
    }
  }
}
Process Admin{
  cola buffer;
  text examen;
  int i;
  while(true){
    do Alumno[*]?(examen,i)-> buffer.push(examen,i);
      !buffer.isEmpty(); Profesor?(){//hay un examen en el buffer que enviar? profesor te pidio un examen?
         Profesor!(buffer.pop());//es porque esta libre, mandale el examen con id.
      }
    od
  }
}
Process Profesor{
  text examen;
  int idA;
  int nota;
  while(true){
    Admin!();//le avisa que puede recibir un examen
    Admin?(examen,iA);//lo recibe
    nota=corregir(examen);//corrige 
    Alumno[iA]!(nota);//envia la nota
  }
}

b-
  Process Alumno[i=1 to N]{
    while(true){
      text examen;
      int nota;
      examen = resolverExamen();
      Admin!(examen,i);
      Profesor[*]?(nota);
    }
  }
  Process Admin{
    cola buffer;
    text examen;
    int idA;
    int idP;
    while(true){
      do Alumno[*]?(examen,idA)-> buffer.push(examen,idA);
        !buffer.isEmpty(); Profesor[*]?(idP){//hay un examen en el buffer que enviar? profesor te pidio un examen?
           Profesor[idP]!(buffer.pop());//es porque esta libre, mandale el examen con id.
        }
      od
    }
  }
  Process Profesor[i=1 to P]{
    text examen;
    int idA;
    int nota;
    while(true){
      Admin!(i);//le avisa quien de los P profesores pidio.
      Admin?(examen,idA);//lo recibe
      nota=corregir(examen);//corrige
      Alumno[idA]!(nota);//envia la nota
    }
  }
  
c- 
Process Alumno[i=1 to N]{
    while(true){
      text examen;
      int nota;
      Admin!();//le avisa al admin que esta listo para hacer el examen
      Admin?();//recibe la respuesta del admin
      examen = resolverExamen();
      Admin!examen(examen,i);
      Profesor[*]?(nota);
    }
  }
  Process Admin{
    cola buffer;
    text examen;
    int idA;
    int idP;
    for i=1 to N{
      Alumno[*]?();//recibe a los N mensaje sde los alumnos confirmando su llegada
    }
    for i=1 to N{
      Alumno[*]!();//les responde que ya pueden comenzar.
    }
    while(true){
      do Alumno[*]?examen(examen,idA)-> buffer.push(examen,idA);
        !buffer.isEmpty(); Profesor[*]?(idP){//hay un examen en el buffer que enviar? profesor te pidio un examen?
           Profesor[idP]!(buffer.pop());//es porque esta libre, mandale el examen con id.
        }
      od
    }
  }
  Process Profesor[i=1 to P]{
    text examen;
    int idA;
    int nota;
    while(true){
      Admin!(i);//le avisa quien de los P profesores pidio.
      Admin?(examen,idA);//lo recibe
      nota=corregir(examen);//corrige
      Alumno[idA]!(nota);//envia la nota
    }
  }


  4. En una exposición aeronáutica hay un simulador de vuelo (que debe ser usado con 
exclusión mutua) y un empleado encargado de administrar su uso. Hay P personas que 
esperan a que el empleado lo deje acceder al simulador, lo usa por un rato y se retira. El 
empleado deja usar el simulador a las personas respetando el orden de llegada. Nota: cada 
persona usa sólo una vez el simulador.  
//es valida esta solucion? porque cumplir yo creo que cumple.
//pero puede ser que no quieran la clase admin junto a empelado, sino una sola clase.
//no dejan nada sobre la cantidad de procesos y en tema de eficiencia a ciencia cierta no sabria.
Process Persona[i:1..P]{
    Admin!espera(i);//avisa que llego
    Empleado?();//espera a que lo atiendan
    //usarSimulador()
    Empleado!();//avisa que se retira
}
Process Admin{
  cola buffer;
  int idP;
  while(true){
    do
      Persona[*]?espera(idP)-> buffer.push(idP);//si una persona llega espera en la fila
      !buffer.isEmpty();Empleado?sig();{//si el empleado se desocupo y hay personas esperando
        Empleado!(buffer.pop());//le pasamos el id de la persona que atender
      }
    od
      }
  }
Process Emepleado{
    int idP;
  while(true){
    Admin!sig();//avisa que puede atender
    Admin?(idP);//recibe id que atender
    Persona[idP]!();//le avisa que puede pasar a la persona
    Persona[*]?();//espera que la persona le avisa que se fue.
  }
}//persona!puerto1 // person!puerto2

5. En un estadio de fútbol hay una máquina expendedora de gaseosas que debe ser usada por 
E Espectadores de acuerdo al orden de llegada. Cuando el espectador accede a la máquina 
en su turno usa la máquina y luego se retira para dejar al siguiente. Nota: cada Espectador 
una sólo una vez la máquina.

Process Espectador[i:1..E]{
    Maquina!espera(i);//se encola
    Maquina?respuesta();//espera a que lo llamen
    //usarMaquina();//utiliza maquina
    Maquina!libre();//avisa liberacion de maquian
}
Process Maquina{
  bool libre=true;
  cola buffer;
  int id;
  while(true){
    do
      !libre; Espectador[*]?espera(id)->buffer.push(id);// si no esta libre y hay mensaje de espera, se lo encola
      libre; Espectador[*]?espera(id)->{Espectador[id]!respuesta(); libre=false}//si esta libre y hay mensaje de espera, se le asigna permiso y libre se marca en false
      buffer.isEmpty(); Espectador[*]?libre()->libre=true;// si no hay nadie esperando y liberaron, se deja en libre
      !buffer.isEmpty(); Espectador[*]?libre()->Espectador[buffer.pop()]!respuesta();//si hay alguien esperando y liberaron, se le asigna a quien estaba esperando el uso.
    od
  }
}
//libre o buffer (B), Espectador[*]recepcion(C), ->acciones(S)// B;C->S;
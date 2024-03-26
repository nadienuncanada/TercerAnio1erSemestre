package ar.edu.info.unlp.ejercicioDemo;

public class RealizadorDeJuego {

	public RealizadorDeJuego() {
		
	}
	public String realizarJugada(Jugada uno,Jugada dos) {
		return dos.resolverJugada(uno);
	}
}

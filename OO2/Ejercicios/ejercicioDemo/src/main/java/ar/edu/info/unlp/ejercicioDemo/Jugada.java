package ar.edu.info.unlp.ejercicioDemo;

public interface Jugada {

	public String resolverJugada(Jugada uno);
	
	public String jugarContraPiedra();
	
	public String jugarContraPapel();
	
	public String jugarContraTijera();
	
	public String jugarContraSpock();
	
	public String jugarContraLagarto();
}

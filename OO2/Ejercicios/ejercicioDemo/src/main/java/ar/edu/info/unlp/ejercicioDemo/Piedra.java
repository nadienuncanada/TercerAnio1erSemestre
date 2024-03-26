package ar.edu.info.unlp.ejercicioDemo;

public class Piedra implements Jugada{

	@Override
	public String resolverJugada(Jugada uno) {
		return uno.jugarContraPiedra();
	}

	@Override
	public String jugarContraPiedra() {
		return "Empate";
	}

	@Override
	public String jugarContraPapel() {
		return "Gana Papel";
	}

	@Override
	public String jugarContraTijera() {
	return "Gana Piedra";
	}

	@Override
	public String jugarContraSpock() {
		// TODO Auto-generated method stub
		return "Gana Spock";
	}

	@Override
	public String jugarContraLagarto() {
		// TODO Auto-generated method stub
		return "Gana Piedra";
	}

}

package ar.edu.info.unlp.ejercicioDemo;

public class Tijera implements Jugada{

	@Override
	public String resolverJugada(Jugada uno) {
		// TODO Auto-generated method stub
		return uno.jugarContraTijera();
	}

	@Override
	public String jugarContraPiedra() {
		// TODO Auto-generated method stub
		return "Gana Piedra";
	}

	@Override
	public String jugarContraPapel() {
		// TODO Auto-generated method stub
		return "Gana Tijera";
	}

	@Override
	public String jugarContraTijera() {
		// TODO Auto-generated method stub
		return "Empate";
	}

	@Override
	public String jugarContraSpock() {
		// TODO Auto-generated method stub
		return "Gana Spock";
	}

	@Override
	public String jugarContraLagarto() {
		// TODO Auto-generated method stub
		return "Gana Tijera";
	}

}

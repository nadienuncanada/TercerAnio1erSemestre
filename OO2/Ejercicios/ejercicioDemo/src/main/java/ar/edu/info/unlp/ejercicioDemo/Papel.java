package ar.edu.info.unlp.ejercicioDemo;

public class Papel implements Jugada{

	@Override
	public String resolverJugada(Jugada uno) {
		
		return uno.jugarContraPapel();
	}

	@Override
	public String jugarContraPiedra() {
		// TODO Auto-generated method stub
		return "Gana Papel";
	}

	@Override
	public String jugarContraPapel() {
		// TODO Auto-generated method stub
		return "Empate";
	}

	@Override
	public String jugarContraTijera() {
		// TODO Auto-generated method stub
		return "Gana Tijera";
	}

	@Override
	public String jugarContraSpock() {
		// TODO Auto-generated method stub
		return "Gana Papel";
	}

	@Override
	public String jugarContraLagarto() {
		// TODO Auto-generated method stub
		return "Gana Lagarto";
	}

}

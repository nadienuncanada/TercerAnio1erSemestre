package ar.edu.info.unlp.ejercicioDemo;

public class Tierra extends Topografia{

	@Override
	public double proporcionDeAgua() {
		return 0;
	}

	@Override
	public boolean igual(Agua agua) {
		return false;
	}

	@Override
	public boolean igual(Tierra tierra) {
		return true;
	}

	@Override
	public boolean igual(Topografia otraTopografia) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean igual(Mixta topografiaMixta) {
		// TODO Auto-generated method stub
		return false;
	}

}

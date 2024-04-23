package ar.edu.info.unlp.ejercicioDemo;

public class Agua extends Topografia{
	
	public boolean igual(Agua agua) {
		return true;
	}
	public boolean igual(Tierra tierra) {
		return false;
	}
	@Override
	public double proporcionDeAgua() {
		return 1;
	}
	@Override
	public boolean igual(Topografia otraTopografia) {
		return otraTopografia.igual(this);
	}
	@Override
	public boolean igual(Mixta topografiaMixta) {
		return false;
	}
	
}

package ar.edu.info.unlp.ejercicioDemo;

public abstract class Topografia {
	
	public abstract double proporcionDeAgua();
	public abstract boolean igual(Agua agua);
	public abstract boolean igual(Tierra tierra);
	public abstract boolean igual(Topografia otraTopografia);
	public abstract boolean igual(Mixta topografiaMixta);
	

	
	
}

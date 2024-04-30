package ar.edu.info.unlp.ejercicioDemo;

import java.time.LocalDate;
import java.util.List;

public class Archivo extends FileSystem{

	private int tamanoBytes;
	
	public Archivo(String n, int tB) {
		super(n);
		this.tamanoBytes=tB;
		// TODO Auto-generated constructor stub
	}

	@Override
	public int tamanoTotalOcupado() {
		return this.tamanoBytes;
	}

	@Override
	public Archivo achivoMasGrande() {
		// TODO Auto-generated method stub
		return this;
	}

	@Override
	public Archivo archivoMasNuevo() {
		// TODO Auto-generated method stub
		return this;
	}

	@Override
	public FileSystem buscar(String nombre) {
		if(this.getNombre().equals(nombre))
			return this;
		return null;
	}

	@Override
	public void buscarTodos(String nombre, List<FileSystem>elementos) {
		if(this.getNombre()==nombre)
			elementos.add(this);
	}

	@Override
	public String listadoDeContenido(String nombre) {
		return nombre + this.getNombre()+ "\n";
	}
}

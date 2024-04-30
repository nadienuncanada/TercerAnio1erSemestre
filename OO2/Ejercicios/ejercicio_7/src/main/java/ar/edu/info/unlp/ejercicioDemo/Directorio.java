package ar.edu.info.unlp.ejercicioDemo;

import java.time.LocalDate;
import java.util.List;
import java.util.ArrayList;

public class Directorio extends FileSystem {
	
	private List<FileSystem> contenido;
	public Directorio(String n) {
		super(n);
		this.contenido=new ArrayList<FileSystem>();
		
	}

	@Override
	public int tamanoTotalOcupado() {
		// TODO Auto-generated method stub
		return contenido.stream().mapToInt(arch->arch.tamanoTotalOcupado()).sum()+32;//32kb es el tamano base
	}

	@Override
	public Archivo achivoMasGrande() {
		// TODO Auto-generated method stub
		return contenido.stream().map(c1->c1.achivoMasGrande())
				  .max((c1, c2)-> Integer.compare(
						  c1.tamanoTotalOcupado(), c2.tamanoTotalOcupado()))
						    .orElse(null);
	}

	@Override
	public Archivo archivoMasNuevo() {
		// TODO Auto-generated method stub
		return  contenido.stream().map(c1->c1.archivoMasNuevo())
				  .max((c1, c2)-> Integer.compare(
						  c1.tamanoTotalOcupado(), c2.tamanoTotalOcupado()))
						    .orElse(null);
	}

	@Override
	public FileSystem buscar(String nombre) {
		if(this.getNombre().equals(nombre))
			return this;
		return contenido.stream()
		  .filter(cont->cont.buscar(nombre)!=null)
		  .findFirst().orElse(null);

	}

	@Override
	public void buscarTodos(String nombre, List<FileSystem> elementos) {
		if(this.getNombre().equals(nombre))
			elementos.add(this);
		contenido.stream().forEach(c->c.buscarTodos(nombre,elementos));
	}

	public String listadoDeContenido() {
		StringBuilder listado = new StringBuilder();
		listado.append(this.getNombre()).append("\n");
		contenido.stream()
		.forEach(componente -> listado.append(componente.listadoDeContenido(this.getNombre())));
		String resultado = listado.toString();
		return resultado;
	}
	
	@Override
	public String listadoDeContenido(String nombre) {
		String pathActual = nombre + this.getNombre();
		StringBuilder listado = new StringBuilder();
		listado.append(pathActual).append("\n");
		contenido.stream()
		.forEach(componente -> listado.append(componente.listadoDeContenido(pathActual)));
		String resultado = listado.toString();
		return resultado;
	}

	public void addComponente(FileSystem contenido) {
		this.contenido.add(contenido);
	}
}

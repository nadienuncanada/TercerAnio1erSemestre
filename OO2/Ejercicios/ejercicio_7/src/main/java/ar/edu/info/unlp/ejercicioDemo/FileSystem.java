package ar.edu.info.unlp.ejercicioDemo;

import java.time.LocalDate;
import java.util.List;
import java.util.ArrayList;


public abstract class FileSystem {
	private String nombre;
	private LocalDate fechaCreacion;
	public FileSystem (String n) {
		this.nombre="/"+n;
		this.fechaCreacion=LocalDate.now();
	}
	
	public abstract int tamanoTotalOcupado();
	public abstract Archivo achivoMasGrande();
	public abstract Archivo archivoMasNuevo();
	public abstract FileSystem buscar(String nombre);
	public abstract void buscarTodos(String nombre, List<FileSystem> elementos);
	public abstract String listadoDeContenido(String nombre);
	
	public List<FileSystem> buscarTodos(String nombre) {
		List<FileSystem> elementos=new ArrayList<FileSystem>();
		this.buscarTodos(nombre, elementos);
		return elementos;
	}
	public String getNombre() {
		return this.nombre;
	}
	public LocalDate getFechaCreacion() {
		return this.fechaCreacion;
	}
	
}

package ar.edu.info.unlp.ejercicioDemo;

public abstract class Empleado {
	private double sueldoBase;
	
	public Empleado(double sBase) {
		this.sueldoBase= sBase;
	}
	public double getDescuento() {
		return (0.13*this.sueldoBase)+(0.05*getAdicional());
	}
	public double getSueldoBase() {
		return sueldoBase;
	}	
	public abstract double getAdicional();
	
	public double sueldo() {
		return getSueldoBase()+getAdicional()-getDescuento();
	}
}

package ar.edu.info.unlp.ejercicioDemo;

public class Planta extends Empleado {
	
	private boolean esCasado;
	private Integer cantHijos;
	private Integer antiguedad;

	public Planta(double sB,boolean esC, Integer cantH, Integer ant) {
		super(sB);
		this.esCasado=esC;
		this.cantHijos=cantH;
		this.antiguedad=ant;
		// TODO Auto-generated constructor stub
	}
	@Override
	public double getAdicional() {
		double adicional = 0;
		if(getEsCasado())
			adicional+=5000;
		adicional+=(getCantHijos()*2000);
		adicional+=(getAntiguedad()*2000);
		return adicional;
	}
	public boolean getEsCasado() {
		return esCasado;
	}

	public Integer getCantHijos() {
		return cantHijos;
	}

	public Integer getAntiguedad() {
		return antiguedad;
	}
	public double getSueldoBase() {
		return super.getSueldoBase();
	}
	
	
	
}

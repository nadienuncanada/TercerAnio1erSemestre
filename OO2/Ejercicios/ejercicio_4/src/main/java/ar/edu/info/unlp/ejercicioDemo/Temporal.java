package ar.edu.info.unlp.ejercicioDemo;

public class Temporal extends Empleado{

	private Integer hrsTrabajo;
	private Integer canthijos;
	private boolean esCasado;

	public Temporal(double sBase,Integer hrsT, Integer cantHijos, boolean casado) {
		super(sBase+(hrsT*300));
		// TODO Auto-generated constructor stub
		this.hrsTrabajo=hrsT;
		this.canthijos=cantHijos;
		this.esCasado=casado;
	}
	
	public Integer getHrsTrabajo() {
		return hrsTrabajo;
	}
	public Integer getCanthijos() {
		return canthijos;
	}
	public boolean getEsCasado() {
		return esCasado;
	}

	@Override
	public double getAdicional() {
		double adicional=0;
		if(getEsCasado())
			adicional+=5000;
		adicional+=(getCanthijos()*2000);
		return adicional;
	}
	public double getSueldoBase() {
		return super.getSueldoBase();
	}
	
	
	
}

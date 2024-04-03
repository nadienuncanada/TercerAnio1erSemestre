package ar.edu.info.unlp.ejercicioDemo;

public class Pasante extends Empleado {

	private Integer cantExamenes;
	 
	public Pasante(double sBase,Integer cantExamenes) {
		super(sBase);
		// TODO Auto-generated constructor stub
		this.cantExamenes=cantExamenes;
	}

	public Integer getCantExamenes() {
		return cantExamenes;
	}

	@Override
	public double getAdicional() {
		double adicional=0;
		adicional+=(getCantExamenes()*2000);
		return adicional;
	}

}

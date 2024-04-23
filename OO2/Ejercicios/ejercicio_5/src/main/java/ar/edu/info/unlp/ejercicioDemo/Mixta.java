package ar.edu.info.unlp.ejercicioDemo;

import java.util.List;
//import java.util.ArrayList;


public class Mixta extends Topografia{
	private List<Topografia> partes;
	
	public Mixta(List<Topografia> newPartes) {
		this.partes=newPartes;
	}

	@Override
	public boolean igual(Topografia otraTopografia) {
		return otraTopografia.igual(this);
	}

	@Override
	public double proporcionDeAgua() {
		return partes.stream().mapToDouble(parte->parte.proporcionDeAgua()).sum()/4;
	}

	@Override
	public boolean igual(Agua agua) {
		return false;
	}

	@Override
	public boolean igual(Tierra tierra) {
		return false;
	}
	public Topografia elemento(int i) {
		return this.partes.get(i);
	}

	@Override
	public boolean igual(Mixta topografiaMixta) {
		for (int i=0;i<partes.size();i++) {
			if(!(partes.get(i).igual(topografiaMixta.elemento(i))))
				return false;
		}
		return true;
	}
	
}

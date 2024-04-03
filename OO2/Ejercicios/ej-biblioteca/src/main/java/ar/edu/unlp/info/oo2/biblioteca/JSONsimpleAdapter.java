package ar.edu.unlp.info.oo2.biblioteca;

import java.util.List;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

public class JSONsimpleAdapter extends VoorheesExporter {

	public String exportar(List<Socio> socios) {
		JSONArray lista= new JSONArray();
		JSONObject o =new JSONObject();
		if(socios.isEmpty())
			return "[]";
		socios.forEach(socio -> {
			o.put("nombre", socio.getNombre());
			o.put("email", socio.getEmail());
			o.put("legajo", socio.getLegajo());
			lista.add(o);
		});
		return lista.toJSONString();
		}
		
	}


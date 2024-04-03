package ar.edu.unlp.info.oo2.biblioteca;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import ar.edu.unlp.info.oo2.biblioteca.*;
import org.json.simple.parser.*;

public class BibliotecaTest {
	Biblioteca biblio; 
	Socio socio ;
	JSONsimpleAdapter adapter;
	@BeforeEach
	  void setUp() throws Exception {
		adapter= new JSONsimpleAdapter();
		socio = new Socio("Pepe","pepe@gmail.com","12345");
		biblio = new Biblioteca();
		biblio.setExporter(adapter);
		}
		
	  @Test
	  void testVacio() {
		  assertEquals("[]", biblio.exportarSocios());
	  }

	  @Test
	  void testAgregarSocio() throws ParseException {
		    JSONParser parser = new JSONParser();
		    biblio.agregarSocio(socio);
		    String s=("[\r\n" + //
		                "\t{\r\n" + 
		                "\t\t\"nombre\": \"Pepe\",\r\n" + //
		                "\t\t\"email\": \"pepe@gmail.com\",\r\n" + //
		                "\t\t\"legajo\": \"12345\"\r\n" + //
		                "\t}\r\n" + //
		                "]");
		    assertEquals(parser.parse(s),parser.parse(biblio.exportarSocios()));
		  }
	  
		}




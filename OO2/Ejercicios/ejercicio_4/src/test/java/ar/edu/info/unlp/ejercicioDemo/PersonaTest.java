package ar.edu.info.unlp.ejercicioDemo;

import static org.junit.jupiter.api.Assertions.assertEquals;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import java.util.ArrayList;
import java.util.List;
/**
 * Aca escribiremos los test de unidad para cada clase 
 * 
 */
public class PersonaTest {
	
	Planta p1;
	Temporal t1;
	Pasante ps1;
	
	@BeforeEach
	void setUp() throws Exception {
		p1 = new Planta(50000,true,3,5);//+5000+6000+10000
		t1= new Temporal(20000,6,0,true);//+21800+0+5000
		ps1= new Pasante(20000,5);//+10000
	}
	
    @Test
    public void testAdicional() {
        assertEquals(21000,p1.getAdicional());
        assertEquals(5000,t1.getAdicional());
        assertEquals(10000,ps1.getAdicional());
    }
    
    @Test
    public void testDescuento() {
        assertEquals(7550,p1.getDescuento());
        assertEquals(3084,t1.getDescuento());
        assertEquals(3100,ps1.getDescuento());
    }
    public void testSueldo() {
    	assertEquals(63450,p1.sueldo());
    	assertEquals(23716,t1.sueldo());
    	assertEquals(26900,ps1.sueldo());
    	
    }
}

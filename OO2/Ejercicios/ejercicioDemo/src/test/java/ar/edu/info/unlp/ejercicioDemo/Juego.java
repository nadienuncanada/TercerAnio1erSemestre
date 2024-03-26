package ar.edu.info.unlp.ejercicioDemo;

import static org.junit.jupiter.api.Assertions.assertEquals;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

public class Juego {

	RealizadorDeJuego rdj= new RealizadorDeJuego();
	Piedra piedra = new Piedra();
	Papel papel = new Papel();
	Tijera tijera = new Tijera();
	
	@BeforeEach
	void setUp() {
		
	}
	
	@Test
	public void Prueba() {
		assertEquals("Gana Tijera",tijera.resolverJugada(papel));
	}
	
	@Test
	public void Tijera() {
		 assertEquals("Gana Tijera", rdj.realizarJugada(tijera, papel));
		 assertEquals("Empate", rdj.realizarJugada(tijera, tijera));
		 assertEquals("Gana Piedra", rdj.realizarJugada(tijera, piedra));

	}
	@Test
	public void Piedra() {
		 assertEquals("Gana Papel", rdj.realizarJugada(piedra, papel));
		 assertEquals("Empate", rdj.realizarJugada(piedra, piedra));
		 assertEquals("Gana Piedra", rdj.realizarJugada(tijera, piedra));

	}
	@Test
	public void Papel() {
		 assertEquals("Gana Tijera", rdj.realizarJugada(tijera, papel));
		 assertEquals("Empate", rdj.realizarJugada(papel, papel));
		 assertEquals("Gana Papel", rdj.realizarJugada(papel, piedra));

	}
	
	
}

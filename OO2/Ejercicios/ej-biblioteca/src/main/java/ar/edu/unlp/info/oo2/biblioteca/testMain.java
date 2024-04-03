package ar.edu.unlp.info.oo2.biblioteca;

public class testMain {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Biblioteca biblioteca = new Biblioteca();
		biblioteca.agregarSocio(new Socio("Arya Stark", "needle@stark.com", "5234-5"));
		biblioteca.agregarSocio(new Socio("Tyron Lannister", "tyron@thelannisters.com",
		"2345-2"));
		System.out.println(biblioteca.exportarSocios());
	}

}

int main() {
	
	if (setuid(0) == -1) {
		perror("Error en setuid");
		return 1;
	}
	return 0;


}

/*¿Cuál es la funcionalidad de la llamada setuid?*/
// La llamada setuid establece el id del usuario efectivo del proceso que la llama.
// Si el id del usuario efectivo es 0, el proceso tiene permisos de superusuario.

/*¿Cómo podemos comprobar, desde la linea de comandos (el shell), el valor devuelto tras la ejecución del programa?*/
// Se puede comprobar el valor devuelto tras la ejecución del programa con el comando echo $?

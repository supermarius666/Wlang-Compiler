//una riga
/*
*
*  più righe 
*
*/

fun a() { b()!! }
fun b() { c()!! }
fun c() { 
	c("uno", "due")!! 
}

a()!!
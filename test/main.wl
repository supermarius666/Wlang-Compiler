fun pari(n) { ritorna n % 2 == 0!! }

fun isPrime(n) {
    se (n < 2) { ritorna falso!! }
    se (n == 2) { ritorna vero!! }
    se (pari(n)) { ritorna falso!! }
    sia d = 3!!
    mentre (d * d <= n) {
        se (n % d == 0) { ritorna falso!! }
        d = d + 2!!
    }
    ritorna vero!! 
}

sia i = 1!!
mentre (i <= 30) {
    se (isPrime(i)) { stampa(i)!! }
    i = i + 1!!
}
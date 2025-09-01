# Wlang Compiler 
### Introduzione
Wlang è un piccolo linguaggio di programmazione che viene compilato in bytecode e il risultato viene eseguito su una macchina virtuale stack-based. E' stato realizzato prima il linguaggio e poi il compilatore che ha lo stesso nome. Questo progetto nasce con finalità didattiche e pratiche: imparare come si costruisce un compilatore molto semplice  e una VM portabile ed estendibile.

## Compilatore Wlang
Il compilatore Wlang serve a compilare il linguaggio Wlang.
Questo compilatore genera un bytecode che poi viene eseguito da una macchina virtuale. 

#### Funzionamento 
Per poter utilizzare il compilatore bisogna prima creare la cartella build e poi installarlo nel sistema:
```bash
mkdir build
./buid.sh 
```
Chiderà la password dell'utente per poter salvare l'eseguibile in `/usr/local/bin/`. In questo modo si potrà utilizzare l'eseguibile direttamente scrivendo il nome nel terminale, senza `./wlang`.

Una volta installato in compilatore ci sono due modi in cui può essere usato:
1. compilare ed eseguire direttamente il codice passato come sorgente:
```bash
wlang file.wl 
```
Questo genera direttamente il risultato del programma nel terminale.

2. compilare il programma senza eseguirlo:
```bash
wlang -name output file.wl
```
In questo caso verrà generato un file binario nel formato `wlb`(wlang binary). Questo formato codifica in bianrio tutto il programma e può essere eseguito dalla macchina virtuale. 

Per eseguire il codice binario sulla macchina virtuale si può fare:
```bash
wlang -b file.wlb
```
In questo modo si può vedere il risultato dell'esecuzione del programma.

Infie, si può decidere dove salvare il file di output quando viene generato in questo modo:
```bash
wlang -name output file.wl -path <path>
```

## Linguaggio Wlang 
Wlang è un linguaggio di alto livello orientato a scrivere piccoli script nonostante sia un linguaggio compilato. La sintassi è molto semplice ed è basata sulla lingua italiana.
E' un linguaggio imperativo con una tipizzazione dinamica.

#### Tipo supporati da Wlang:
Il linguaggio Wlang supporta quattro tipo nativi:
| Tipo      | Esempio          |
|-----------|------------------|
| NIL       |   nulla          |
| BOOL      |   vero, falso    |
|NUMBER     |   1, 2.0, 3.1415 |
|STRING     |   "Wlang", "a"   |

###### Operatori 
In Wlang si possono usare questi operatori:
| Operatore | Significato                          |
|-----------|--------------------------------------|
| `+`       | Somma                                |
| `-`       | Sottrazione                          |
|`*`        | Moltiplicazione                      |
|`/`        | Divisione                            |
|`%`        | Modulo                               |
|`e`        | And                                  |
|`o`        | Or                                   |
| `<`       | Minore di                            |
| `>`       | Maggiore di                          |
| `<=`      | Minore o uguale a                    |
| `>=`      | Maggiore o uguale a                  |
| `==`      | Uguale a                             |
| `!=`      | Diverso da                           |
| `!!`      | Fine istruzione                      |
|`non`      | Negazione                            |
| `vero`    | True                                 |
|`falso`    | False                                |

#### Sintassi 
La sintassi è molto simile a quella del linguaggio C. 
###### Commenti
Si possono creare commenti su una riga sola con `//`:
```wlang
// questo è un commento su una riga sola 
```
Si possono creare commenti su righe multiple con `/**/`: 
```wlang
/* questo è un commento su riga */

/*
*   Questo è
*   un commento su 
*   più righe 
*/ 
```
###### Dichiarazione variabili
Le variabili si possono dichiarare con la keyword `sia`. Ogni riga di istruzione deve finire con `!!`, simile a `;` nei linguaggi C, C++, JAVA, etc. 
```wlang
// dichiarazione 
sia somma!!

// dichiarazione e assegnazione 
sia media = 0.0!!
sia ok = vero!!
sia nome = "Luca"!!
```

###### Stampa 
Il linguaggio Wlang presenta una funzione built-in per la stampa. La funzione in questione è `stampa()`. E' molto facile da utilizzare:
```wlang
sia a = 3!!
stampa(a)!!
/* output: 3 */

sia nome = "Francesco"!!
stampa(nome)!!
/* output: Francesco */

sia b!!
stampa(b)!!
/* output: nulla */

stampa (vero e non vero)!!
/* output: falso */

stampa(12 + 3)!!
/* output: 15 */

```
In Wlang si può ottenere la concatenazione delle stringhe attraverso l'operatore `+`:
```wlang
sia nome = "Mario"!!
sia cognome = "Rossi"!
stampa("Il nome completo è " + nome + " " + cognome)!!
/* output: Il nome completo è Mario Rossi */
```


Infine, se non viene specificato nella stringa lo `\n`, due o più stampe successive compaiono sulla stessa riga:
```wlang 
stampa("uno ")!!
stampa("due\n")!!
/* output: uno due */

stampa("uno\n")!!
stampa("due\t")!!
stampa("tre\n")!!
/* output: 
*   uno
*   due     tre 
*/

```

###### Escape
Caratteri speciali:
| Escape    | Significato      |
|-----------|------------------|
| `\n`      |  new line        |
| `\t`      |   tab            |
|  `\s`     |   space          |
| `\"`      |   per usare gli apici nelle stringhe|
| `\\`      |   per usare \ nelle stringhe |



###### Condizioni 
Per eseguire un pezzo di codice al verificarsi di una certa condizione si usa `se` e `altrimenti`:
```wlang 
sia a = 10!!
se (a % 2 == 0)
    stampa("pari")!!
altrimenti
    stampa("dispari")!!
/* output: pari */
```

###### Cicli
Il linguaggio Wlang mette a disposizione il ciclo `mentre`, simile al ciclo `while` di altri linguaggi:
```wlang
sia i = 10!!
mentre (i >= 0)
{
    se (i % 2 == 0)
    {
        stampa ("pari\n")!!
    }
    altrimenti 
    {
        sia c = i * 2!!
        stampa(c)!!
        stampa("\n)!!
    }
    i = i - 1!!
}
stampa("fine\n")!!

/*
*   output: 
*   pari
*   18
*   pari
*   14
*   pari
*   10
*   pari
*   6
*   pari
*   2
*   pari
*   fine
*/
```
###### Funzioni 
Le funzioni si dichiarano con la keyword `fun`:
```wlang
fun somma(a, b)
{
    stampa(a + b)!!
}

somma(2, 3)!!
/* output: 5 */
```
Per far restituire qualcosa da una funzione si utilizza la keyword `ritorna`:
```wlang 
fun somma(a, b)
{
    ritorna (a + b)!!
}

sia a = somma(2, 3)!!
sia c = somma(a, 4)!!
stampa(c)!!
/* output: 9 */
```


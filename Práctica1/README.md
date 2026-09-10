# Practica 1: De regex a NFA

Flores Doniz Daniel

## Requisitos

- Debian o cualquier distribución Linux.
- GCC.
- Una terminal.

Para comprobar que GCC está instalado:

```bash
gcc --version
```

## Compilación

Desde la raíz del proyecto, ejecutar:

```bash
gcc -std=c17 -Wall -Wextra -pedantic \
    src/main.c src/regex.c src/nfa.c \
    -o regex_to_nfa
```

Si la compilación termina sin mensajes de error, se generará el ejecutable `regex_to_nfa`.

## Ejecución

El programa recibe la expresión regular desde la entrada estándar.

### Convertir a notación postfija

Usar la opción `-r`:

```bash
printf 'ab\n' | ./regex_to_nfa -r
```

Salida esperada:

```text
ab.
```

Otros ejemplos:

```bash
printf 'a|b\n' | ./regex_to_nfa -r
# ab|

printf '(ab)*\n' | ./regex_to_nfa -r
# ab.*

printf 'a+\n' | ./regex_to_nfa -r
# a+
```

La concatenación se representa con `.` en la salida postfija.

### Probar cadenas contra una expresión regular

Usar la opción `-t`. La primera línea es la expresión regular y las siguientes líneas son las cadenas que se probarán:

```bash
printf 'ab\nab\na\nabc\n' | ./regex_to_nfa -t
```

Salida esperada:

```text
100
```

En este caso:

- `ab` es aceptada.
- `a` es rechazada.
- `abc` es rechazada.

Otro ejemplo con cero o más repeticiones:

```bash
printf 'a*\n\na\naa\nb\n' | ./regex_to_nfa -t
```

Salida esperada:

```text
1110
```

Cada `1` indica que la cadena fue aceptada y cada `0` que fue rechazada. Los resultados se imprimen en el mismo orden que las cadenas de entrada.

## Operadores soportados

| Operador | Significado |
| --- | --- |
| `\|` | Unión |
| `*` | Cero o más repeticiones |
| `+` | Una o más repeticiones |
| `?` | Cero o una repetición |
| `()` | Agrupación |
| `\` | Escapa el carácter siguiente para tratarlo como literal |

La concatenación es implícita. Por ejemplo, `ab` significa que debe aparecer primero `a` y después `b`.

## Uso incorrecto

Si se ejecuta el programa sin una opción, muestra el uso correcto:

```bash
./regex_to_nfa
```

Salida esperada:

```text
Usage: ./regex_to_nfa -r | -t
```

# Practica 1: De regex a NFA

Flores Doniz Daniel

## Estructura del proyecto

```text
compilers_lab/regex_to_nfa/
├── Dockerfile
├── CMakeLists.txt
├── validator
└── src/
    ├── main.c
    ├── regex.c
    ├── regex.h
    ├── nfa.c
    └── nfa.h
```

## Evaluación oficial con Docker



### Requisitos

- Debian o cualquier distribución Linux.
- Docker instalado y en ejecución.
- Una terminal.

Para comprobar Docker:

```bash
docker --version
```

### Construir la imagen

Ejecutar dentro de `compilers_lab/regex_to_nfa`:
```bash
sudo docker build -t regex_to_nfa_validator .
```


### Ejecutar el validator

Después de construir la imagen y en la misma ruta:

```bash
sudo docker run --rm regex_to_nfa_validator
```


Se debe correr la evaluación y mostrar el resultado de la misma.
## Opción alternativa: Compilación manual

Esta opción no usa Docker.

### Requisitos

- Debian o cualquier distribución Linux.
- GCC.
- Una terminal.

Para comprobar que GCC está instalado:

```bash
gcc --version
```

### Compilar

Ejecutar dentro de `compilers_lab/regex_to_nfa`:
```bash
gcc -std=c17 -Wall -Wextra -pedantic \
    src/main.c src/regex.c src/nfa.c \
    -o regex_to_nfa
```

Si la compilación termina sin mensajes de error, se generará el ejecutable `regex_to_nfa` en la carpeta actual.

## Ejecución manual

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




Cada `1` indica que la cadena fue aceptada y cada `0` que fue rechazada.

## Operadores soportados

| Operador | Significado |
| --- | --- |
| `\|` | Unión |
| `*` | Cero o más repeticiones |
| `+` | Una o más repeticiones |
| `?` | Cero o una repetición |
| `()` | Agrupación |
| `\` | Escapa el carácter siguiente para tratarlo como literal |

La concatenación es implícita.
## Uso incorrecto

Si se ejecuta el programa sin una opción, muestra el uso correcto:

```bash
./regex_to_nfa
```

Salida esperada:

```text
Usage: ./regex_to_nfa -r | -t
```

# Practica 2: De NFA a DFA

Flores Doniz Daniel

## Estructura del proyecto

```text
compilers_lab/regex_to_nfa/
├── run_tests.sh
├── src/
│   ├── main.cpp
│   ├── nfa.hpp
│   ├── nfa.cpp
│   ├── dfa.hpp
│   ├── dfa.cpp
│   ├── subset_construction.hpp
│   └── subset_construction.cpp
└── tests/
    ├── test_epsilon_closure.cpp
    ├── test_move.cpp
    └── test_subset_construction.cpp
```

### Requisitos

- Debian o cualquier distribución Linux.
- Compilador de C++ 
- Una terminal.

Para comprobar que GCC/G++ est'a instalado:

```bash
g++ --version
```

## Compilación y ejecución

Navegar a `compilers_lab/nfa_to_dfa` y otorgar permisos de ejecución:

```bash
cd src/
chmod +x run_tests.sh
```

Ejecutar el script directamente:

```bash
./run_tests.sh
```
# Usa la imagen base de Alpine Linux
FROM alpine:latest

# Instala las dependencias necesarias para compilar el código C
RUN apk add --no-cache gcc musl-dev make cmake

# Establece el directorio de trabajo dentro del contenedor
WORKDIR /app

# Copia los archivos del proyecto al contenedor
COPY . .

# Compila el proyecto y ejecuta el binario resultante
CMD ["sh", "-c", "cmake . && make && ./validator"]

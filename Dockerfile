# Imagem base a ser utilizada
FROM ubuntu:latest

# Instala as dependências necessárias
RUN apt-get update && \
    apt-get install -y g++ make

# Diretório de trabalho
WORKDIR /app

# Copia todos os arquivos e diretórios dentro de "scripts/" para "/app" no contêiner
COPY scripts/ /app/

# Copia o arquivo "entrypoint.sh" para "/app" no contêiner
COPY entrypoint.sh /app/

# Compila os programas
RUN g++ -o upload upload.cpp
RUN g++ -o findrec findrec.cpp
RUN g++ -o seek1 seek1.cpp
RUN g++ -o seek2 seek2.cpp

# Define o arquivo "entrypoint.sh" como executável
RUN chmod +x /app/entrypoint.sh

# Volume para salvar a saída do terminal em um arquivo txt
VOLUME /app/output

# Comando padrão a ser executado quando um contêiner for iniciado
CMD ["bash", "/app/entrypoint.sh"]
# TP2-BD

- O objetivo deste trabalho é desenvolver programas para armazenamento e consulta de dados em memória secundária, utilizando as estruturas de arquivo de dados e índice estudadas nas aulas. Os programas devem oferecer suporte para inserção e diferentes formas de busca, baseando-se nas técnicas de organização e indexação de arquivos apresentadas durante o curso. A implementação será realizada na linguagem C++, utilizando as bibliotecas padrão de chamadas de sistema disponíveis no sistema operacional Linux.

- Dados de entrada foram fornecidos pelo professor da disciplina, o arquivo de dados possui id, título, ano, autores, citações, atualizações e um breve resumo de artigos científicos. 

## Nota

Obtemos nota 9.97 neste trabalho.

## Documentação

Na pasta [documentation](documentation/) conterá o arquivo:

- [TP2_BD_DOCUMENTAÇÃO](documentation/TP2_BD_DOCUMENTAÇÃO.pdf): Contém a documentação do trabalho solicitada no item 4(c), com as informações das estruturas de dados utilizadas, as funções implementadas e as decisões de projeto.

## Sobre os Scripts

Na pasta [scripts](scripts/) conterá todos os scripts utilizados para a elaboração do trabalho, segue abaixo uma breve descrição sobre cada script:

- [upload.cpp](scripts/upload.cpp): realiza a leitura do arquivo de entrada e armazena os dados em um arquivo binário organizado por hash, também constrói um arquivo de índice primário e secundário, que serão utilizado para a realização das consultas. A execução deste script é realizada através do comando: `g++ upload.cpp -o upload && ./upload <arquivo de entrada>`. Os arquivos gerados serão: `arquivo_de_dados.bin`, `indice_primario.bin` e `indice_secundario.bin`, que estarão na pasta [Arquivos](scripts/Arquivos/).

- [findrec.cpp](scripts/findrec.cpp): realiza a busca por um registro através do id de um artigo no arquivo organizado por hash  [arquivo_de_dados.bin](scripts/Arquivos/), a execução deste script é realizada através do comando: `g++ findrec.cpp -o findrec && ./findrec <id>`. O resultado da busca será impresso no terminal.

- [seek1.cpp](scripts/seek1.cpp): realiza a busca por um registro através do id de um artigo no arquivo [indice_primario.bin](scripts/Arquivos/) que é uma árvore B+, a execução deste script é realizada através do comando: `g++ seek1.cpp -o seek1 && ./seek1 <id>`. O resultado da busca será impresso no terminal.

- [seek2.cpp](scripts/seek2.cpp): realiza a busca por um registro através do título de um artigo no arquivo [indice_secundario.bin](scripts/Arquivos/) que é uma árvore B+, a execução deste script é realizada através do comando: `g++ seek2.cpp -o seek2 && ./seek2 "<título>"`. O resultado da busca será impresso no terminal.

## Avisos Importantes

- No script [upload.cpp](scripts/upload.cpp), certifique-se de que está passando o nome do arquivo de entrada corretamente e se o arquivo está na pasta [scripts](scripts/), caso contrário o script não irá funcionar.

```sh
    filename = 'artigos.csv'
```

Caso não possua o arquivo de entrada, baixe-o [aqui](https://drive.google.com/file/d/1EVoP0d9Wwzj1O6eoFIkel9I3cpe43Gbv/view?usp=sharing).

- No script [seek2.cpp](scripts/seek2.cpp), certifique-se de que está passando o título do artigo corretamente. O título deve estar entre aspas duplas, caso contrário o script não irá funcionar.

```sh
    string title = "A new approach to the maximum flow problem";
```

Também foi notado uma diferença quando se captura o campo título no excel ou num editor de texto qualquer a respeito dos caracteres unicode, por exemplo, o título do seguinte artigo foi extraído do excel:

```
    0.18 ��m CMOS power amplifier for ultra-wideband (UWB) system
```

Note que os caracteres unicode viraram "�", porém quando o título é extraído de um editor de texto qualquer, o título fica da seguinte forma:

```
    0.18 μm CMOS power amplifier for ultra-wideband (UWB) system
```

Note que o caractere unicode foi mantido. Portanto, para que o script funcione corretamente, certifique-se de que o título do artigo esteja no formato unicode.

## Execução com o Docker

Para executar o trabalho, foi criado um dockerfile que contém todas as dependências necessárias para a execução do trabalho, para executar o dockerfile, basta executar os seguintes comandos:

```sh
    docker build -t tp2_bd .
    docker run --rm -v /home/matheus/Documentos/TP2-BD:/app/output tp2-bd
```

Onde `/home/matheus/Documentos/TP2-BD` é o caminho para a pasta do trabalho no seu computador, e `tp2-bd` é o nome da imagem docker que será criada. Para saber onde está a pasta do trabalho no seu computador, basta executar o comando `pwd` dentro da pasta do trabalho e substituir o caminho `/home/matheus/Documentos/TP2-BD` pelo caminho que foi retornado.

 Certifique-se de que o arquivo de entrada `artigos.csv` está na pasta [scripts](scripts/), caso contrário o script do dockerfile não irá funcionar!

O parâmetro `-t` é utilizado para dar um nome a imagem docker. O parâmetro `--rm` é utilizado para que o container seja removido após a execução do trabalho. O parâmetro `-v` é utilizado para montar um volume, ou seja, para que o docker tenha acesso aos arquivos do seu computador. 

Após a execução do dockerfile, a saída do terminal ficará salva em um arquivo chamado `output.txt`, que estará na pasta do trabalho no seu computador. O arquivo `output.txt` conterá o resultado da execução de todos os scripts, ou seja, o resultado da execução do script [upload.cpp](scripts/upload.cpp), [findrec.cpp](scripts/findrec.cpp), [seek1.cpp](scripts/seek1.cpp) e [seek2.cpp](scripts/seek2.cpp).

## Execução normal

Caso não queira executar o trabalho com o docker, basta executar os seguintes comandos:

```sh
    g++ upload.cpp -o upload && ./upload artigos.csv
    g++ findrec.cpp -o findrec && ./findrec <id>
    g++ seek1.cpp -o seek1 && ./seek1 <id>
    g++ seek2.cpp -o seek2 && ./seek2 "<título>"
```

Onde `<id>` é o id do artigo que deseja buscar e `"<título>"` é o título do artigo que deseja buscar. Note que o título deve estar entre aspas duplas.

## Autores 

- [Matheus Santos](https://github.com/mtss-dev) --- Email para contato: matheus.silva@icomp.ufam.edu.br

- [Vinicius Luiz](https://github.com/VinnyHardth) --- Email para contato: vinicius.fonseca@icomp.ufam.edu.br


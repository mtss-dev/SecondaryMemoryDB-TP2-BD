#!/bin/bash

export TERM=xterm

clear
echo -e "Executando o programa ./upload para criar os três arquivos (arquivo de dados, índice primário e secundário)...\n" | tee /app/output/output.txt
./upload artigo.csv 2>&1 | tee -a /app/output/output.txt
echo -e "\nPrograma ./upload finalizado com sucesso!" | tee -a /app/output/output.txt

echo -e "Executando o programa ./findrec com testes na busca por IDs de registros...\n" | tee -a /app/output/output.txt
./findrec 1000 2>&1 | tee -a /app/output/output.txt
./findrec 2000 2>&1 | tee -a /app/output/output.txt
./findrec -1 2>&1 | tee -a /app/output/output.txt
echo -e "\nPrograma ./findrec finalizado com sucesso!" | tee -a /app/output/output.txt

echo -e "\nExecutando o programa ./seek1 com testes na busca por IDs de registros...\n" | tee -a /app/output/output.txt
./seek1 3000 2>&1 | tee -a /app/output/output.txt
./seek1 4000 2>&1 | tee -a /app/output/output.txt
./seek1 -1 2>&1 | tee -a /app/output/output.txt
echo -e "\nPrograma ./seek1 finalizado com sucesso!" | tee -a /app/output/output.txt

echo -e "\nExecutando o programa ./seek2 com testes na busca por títulos de registros...\n" | tee -a /app/output/output.txt
./seek2 "DFT studies on the quantitative structure-activity relationship of N-(2-chloroethyl)-N′-cyclohexyl-N-nitrosoureas as anticancer agents" 2>&1 | tee -a /app/output/output.txt
./seek2 "Optimal approximation for submodular and supermodular optimization with bounded curvature." 2>&1 | tee -a /app/output/output.txt
./seek2 "A new approach to the design of a class of robust controllers for uncertain systems with time-varying delay" 2>&1 | tee -a /app/output/output.txt
echo -e "\nPrograma ./seek2 finalizado com sucesso!\n" | tee -a /app/output/output.txt

echo "Todos os testes foram executados com sucesso!" | tee -a /app/output/output.txt
echo "Fim da execução do script entrypoint.sh" | tee -a /app/output/output.txt
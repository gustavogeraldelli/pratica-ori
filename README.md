# Sistema de Indexação de Postagens
Como parte de uma atividade acadêmica, esse projeto usa uma tabela hash para indexar as palavras das postagens, permitindo buscas rápidas e eficientes.

## Uso
1. Compilando:
```bash
make
```

2. Executando:
```bash
./main
```

## Buscando postagens
Pode-se procurar de três formas:

- Por uma palavra: `exemplo`
- Por duas palavras com "AND": `exemplo1 AND exemplo2` (postagens que contém as duas palavras)
- Por duas palavras com "OR": `exemplo1 OR exemplo2` (postagens que contém pelo menos uma das palavras)

O sistema padroniza as palvras da entrada quanto a letras maiúsculas, espaços e pontuações
### парсер .csv файлов

#### Инструкция по сборке:

```
    $ PATH_TO_CLANG++/clang++ -fcolor-diagnostics -fansi-escape-codes -g -std=c++20 PATH_TO_FOLDER/csv-parser/*.cpp PATH_TO_FOLDER/csv-parser/sources/*.cpp -o PATH_TO_FOLDER/csv-parser/csv_parser

```

#### Инструкция по запуску 

```
    $ ./csv_parser FILENAME.csv
```
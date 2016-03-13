# hashtable
## Как запускать
gcc hashtable.c hashfunction.c test.c
## Протестировать логгер
gcc hashtable.c hashfunction.c test.c logger/logger.c -DFLAG
где FLAG:
  - INFO: вывод названий запускаемых функций
  - DEBUG: INFO + вывод подаваемых аргументов и всего возможного.
  - ERROR: вывод ошибок 
  - ECHO: вывод всех сообщений в stdout
  
  
Лог файлы сохраняются в logger/log/

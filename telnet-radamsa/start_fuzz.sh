#!/bin/bash

# Создание директории для логов
mkdir -p "logs"

# Компиляция программы
g++ -o radamsaFuzz radamsaFuzz.cpp -lboost_system -lpthread

# Запрос пути к директории у пользователя
read -p "Введите путь до проекта с Embox: " directory_path

# Проверка существования директории
if [ ! -d "$directory_path" ]; then
    echo "Указанная директория не существует."
    exit 1
fi

# Установка времени ожидания
qemu_start_delay=12
fuzzer_run_time=3

# Обработка сигналов для корректного завершения работы
trap 'echo "Завершение работы..."; kill -9 $qemu_pid $xterm_pid; exit' SIGINT SIGTERM

# Основной цикл
while true; do
    # Запуск QEMU в новом терминале
    xterm -e "script output.log -c 'bash -c \"cd $directory_path && ./scripts/qemu/auto_qemu\"'" &

    xterm_pid=$!

    # Ожидание запуска QEMU
    sleep $qemu_start_delay

    # Запуск фаззера
    ./radamsaFuzz

    # Ожидание завершения работы фуззера
    sleep $fuzzer_run_time

    # Формирование имени файла лога
    current_time=$(date "+%Y-%m-%d_%H-%M")

    # Перемещение и переименование логов
    mv "log.txt" "logs/Fuzzer-$current_time"
    tail -n +94 "output.log" > "logs/Embox-$current_time"

    # Получение PID процесса QEMU
    qemu_pid=$(pgrep qemu-system-arm)

    # Ожидание перед завершением процессов
    sleep 1

    # Принудительное завершение процессов
    kill -9 $qemu_pid
    kill -9 $xterm_pid

done


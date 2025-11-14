#!/usr/bin/env python3
import sys
import subprocess

class Colors:
    GREEN = '\033[92m'
    RED = '\033[91m'
    ORANGE = '\033[93m'
    RESET = '\033[0m'

def run_test(program_path, test_file, expected_file):
    try:
        # Читаем тестовые данные
        with open(test_file, 'r') as f:
            test_input = f.read().strip()
        
        # Читаем ожидаемый результат
        with open(expected_file, 'r') as f:
            expected_output = f.read().strip()
        
        # Запускаем программу
        result = subprocess.run(
            [program_path],
            input=test_input,
            text=True,
            capture_output=True,
            timeout=10
        )
        
        actual_output = result.stdout.strip()
        
        # Разбиваем на слова/токены для сравнения
        expected_tokens = expected_output.split()
        actual_tokens = actual_output.split()
        
        # Создаем цветной вывод
        colored_output = []
        extra_tokens = []
        
        # Сравниваем токены
        for i, expected_token in enumerate(expected_tokens):
            if expected_token in actual_tokens:
                colored_output.append(f"{Colors.GREEN}{expected_token}{Colors.RESET}")
            else:
                colored_output.append(f"{Colors.RED}{expected_token}{Colors.RESET}")
        
        # Проверяем на лишние токены в выводе программы
        if len(actual_tokens) > len(expected_tokens):
            extra_tokens = actual_tokens[len(expected_tokens):]
        
        # Выводим результат
        print("Результат: " + " ".join(colored_output))
        
        # Выводим лишние токены оранжевым
        if extra_tokens:
            print(f"{Colors.ORANGE}Лишние: {' '.join(extra_tokens)}{Colors.RESET}")
        
        return actual_output == expected_output and not extra_tokens
            
    except Exception as e:
        print(f"{Colors.RED}Ошибка: {e}{Colors.RESET}")
        return False

def main():
    if len(sys.argv) != 4:
        print(f"Использование: {sys.argv[0]} <программа> <тестовый_файл> <файл_ответов>")
        sys.exit(1)
    
    program_path, test_file, expected_file = sys.argv[1], sys.argv[2], sys.argv[3]
    
    success = run_test(program_path, test_file, expected_file)
    sys.exit(0 if success else 1)

if __name__ == "__main__":
    main()

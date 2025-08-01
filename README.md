# Image Tool

Утилита командной строки для обработки изображений с поддержкой изменения размера, сравнения и оценки качества изображений.


## Возможности

- **Изменение размера изображений** с различными алгоритмами
- **Сравнение изображений** с метриками качества
- **Оценка качества изображений**


## Установка

### Требования

**Обязательные**
- C++17 или новее
- CMake 3.12+
- OpenCV
- CLI11

**Для ИИ-Методов (Real-ESRGAN)**
- Vulkan SDK (для Real-ESRGAN)
- Совместимая с Vulkan видеокарта (для Real-ESRGAN)

> **Примечание:** Методы realesrgan будут недоступны без правильной настройки Vulkan.

### Установка зависимостей

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install cmake build-essential libopencv-dev

# Для Real-ESRGAN (Vulkan)
sudo apt install vulkan-tools libvulkan-dev vulkan-validationlayers-dev
```

**macOS:**
```bash
brew install cmake opencv

# Для Real-ESRGAN (Vulkan SDK нужно установить отдельно)
# Скачайте Vulkan SDK с https://vulkan.lunarg.com/
```

**Windows (vcpkg):**
```bash
vcpkg install opencv

# Установите Vulkan SDK с https://vulkan.lunarg.com/
```

> **Примечание:** CLI11 автоматически загружается через CMake FetchContent во время сборки. Интернет-соединение требуется только при первой сборке.

### Сборка

```bash
git clone https://github.com/kahniel/image_tool.git
cd image_tool
mkdir build && cd build
cmake ..
make
```

**Опции сборки:**

- BUILD_BENCHMARKS=ON/OFF - собирать ли утилиты бенчмарков (по умолчанию: ON)
- BUILD_APPS=ON/OFF - собирать ли основное приложение (по умолчанию: ON)

### Внешние модели и исполняемые файлы

**Проект использует:**
- EDSR модели (в директории models/) - для нейронного апскейлинга
- BRISQUE модели (в директории models/) - для оценки качества
- realesrgan-ncnn-vulkan (в директории external/) - исполняемый файл Real-ESRGAN

Убедитесь, что все файлы на месте после клонирования репозитория.

## Быстрый запуск

```bash
# Сборка проекта
./scripts/build.sh

# Простые примеры использования
./scripts/example.sh

# Полный бенчмарк всех методов на изображениях из data/
./scripts/run_benchmark.sh
```

## Использование

### Команда resize - Изменение размера изображений

```bash
./build/bin/image_tool resize -i <input> -o <output> -s <scale> -m <method>
```

**Параметры:**
- `-i, --input` - Путь к входному изображению (обязательно)
- `-o, --output` - Путь к выходному изображению (обязательно)
- `-s, --scale` - Коэффициент масштабирования (положительное целое число, по умолчанию: 1)
- `-m, --method` - Метод изменения размера (по умолчанию: "my-up")

**Доступные методы:**
- `my-up` - Наивный алгоритм увеличения
- `my-down` - Наивный алгоритм уменьшения
- `linear` - Билинейная интерполяция
- `cubic` - Бикубическая интерполяция
- `lanczos` - Фильтр Lanczos
- `edsr` - Enhanced Deep Super-Resolution (до x4)
- `realesrgan` - Real-ESRGAN (до x4)

> Производительность: ИИ-методы (EDSR, Real-ESRGAN) дают лучшее качество, но работают значительно медленнее традиционных методов. Real-ESRGAN особенно эффективен для аниме-изображений.

**Примеры:**
```bash
# Увеличение изображения в 4 раза бикубической интерполяцией
./build/bin/image_tool resize -i data/ponyo.png -o large_ponyo.png -s 2 -m cubic

# Уменьшение изображения в 2 раза наивным методом
./build/bin/image_tool resize -i data/ponyo.png -o small_ponyo.jpg -s 4 -m my-down

# Восстановление уменьшенного изображения методом Real-ESRGAN
./build/bin/image_tool resize -i small_ponyo.jpg -o processed_ponyo.png -s 4 -m realesrgan
```

### Команда compare - Сравнение изображений

```bash
./build/bin/image_tool compare -1 <first_image> -2 <second_image> -m <method>
```

**Параметры:**
- `-1, --first` - Путь к первому изображению (обязательно)
- `-2, --second` - Путь ко второму изображению (обязательно)
- `-m, --method` - Метод сравнения (по умолчанию: "mse")

**Доступные методы сравнения:**
- `mse` - Mean Squared Error (среднеквадратичная ошибка)
- `ssim` - Structural Similarity Index (индекс структурного сходства)

**Примеры:**
```bash
# Сравнение с методом по умолчанию (MSE)
./build/bin/image_tool compare -1 data/ponyo.png -2 processed_ponyo.png

# Сравнение изображений по SSIM
./build/bin/image_tool compare -1 data/ponyo.png -2 processed_ponyo.png -m ssim
```

**Пример вывода**
```
Structural Similarity Index (SSIM) between
        data/ponyo.png & processed_ponyo.png:
        0.8948
```

**Интерпретация результатов:**
- **MSE**: Чем меньше значение, тем лучше (0 = идентичные изображения)
- **SSIM**: Чем больше значение, тем лучше (1 = идентичные изображения, 0 = полностью разные)

### Команда quality - Оценка качества изображения без референса (BRISQUE)

```bash
./build/bin/image_tool quality -i <input>
```

**Параметры:**
- `-i, --input` - Путь к изображению для анализа (обязательно)

**Пример:**
```bash
# Оценка качества изображений
./build/bin/image_tool quality -i large_ponyo.png
./build/bin/image_tool quality -i data/kyle.png
```

**Пример вывода**
```
BRISQUE score for large_ponyo.png:
        40.4695
BRISQUE score for data/kyle.png:
        12.1241
```
> **Примечание:** Для BRISQUE меньшие значения указывают на лучшее качество изображения. 

## Утилиты для бенчмарков

Утилиты позволяют протестировать различные методы восстановления и замерить их результаты для конкретного референсного изображения или директории с изображениями. 

### 1. Подготовка тестовых данных

```bash
./build/bin/prepare_data [--dir] --orig <originals> --down <downsized_directory>
```

Создает сжатые версии изображений с коэффициентами x2, x3, x4 для последующего восстановления.

**Параметры**
- `--dir` - Флаг, добавляемый если в `--orig` передается директория с изображениями
- `-o, --orig, --original` - Референсное изображение либо директория с референсными изображениями
- `--down, --downsized` - Директория, в которую будут помещены сжатые изображения

### 2. Бенчмарк производительности

```bash
./build/bin/resize_performance --downsized <downsized_directory> --upsized <upsized_directory> --results <results_directory>
```

Восстанавливает изображения всеми доступными методами и измеряет время выполнения.

**Параметры**
- `-d, --down, --downsized` - Директория, в которой находятся сжатые изображения
- `-u, --up, --upsized` - Директория, в которую будут помещены восстановленные изображения
- `-r, --res, --results` - Директория, в которую будет записан файл с замером времени выполнения методов

### 3. Оценка качества

```bash
./build/bin/quality_evaluation [--dir] --upsized <upsized_directory> --orig <originals> --results <results_directory>
```

Оценивает качество восстановленных изображений с помощью различных метрик.

**Параметры**
- `--dir` - Флаг, добавляемый если в `--orig` передается директория с изображениями
- `-u, --up, --upsized` - Директория, в которой находятся восстановленные изображения
- `-o, --orig, --original` - Референсное изображение либо директория с референсными изображениями
- `-r, --res, --results` - Директория, в которую будет записан файл с результатами метрик

**Пример полного бенчмарка:**
```bash
# Подготовка данных
./build/bin/prepare_data --dir --orig data/ --downsized test/downsized/

# Восстановление изображений
./build/bin/resize_performance --downsized test/downsized/ --upsized test/upsized/ --results test/results/

# Оценка качества
./build/bin/quality_evaluation --dir --upsized test/upsized/ --orig data/ --results test/results/
```

**Итоговая структура test/**

```
test/
└── downsized/
│   ├── ...
├── upsized
│   ├── ...
└── results
    └── results_time.csv
    └── results.csv — Результаты метрик
```

## Устранение неполадок

### Real-ESRGAN не работает

```bash
# Проверьте поддержку Vulkan
vulkaninfo

# Убедитесь, что файл существует и исполняем
ls -la external/realesrgan-ncnn-vulkan
chmod +x external/realesrgan-ncnn-vulkan
```

### Ошибки при загрузке моделей

```bash
# Убедитесь, что все модели на месте
ls -la models/
```

### Проблемы с OpenCV

```bash
# Убедитесь, что все модели на месте
pkg-config --modversion opencv4
```

### Проблемы со сборкой

```bash
# Очистка и пересборка
rm -rf build/
mkdir build && cd build
cmake ..
make
```

## Форматы файлов

Поддерживаются все форматы изображений, которые поддерживает OpenCV:
- PNG
- JPEG/JPG
- BMP
- TIFF
- WEBP
- И другие

## Справка

Для получения справки по любой команде используйте флаг `--help`:

```bash
./build/bin/image_tool --help                    # Общая справка
./build/bin/image_tool resize --help             # Справка по команде resize
./build/bin/resize_performance --help            # Справка по бенчмарку производительности
```

## Структура проекта

```
image_tool/
├── src/                   # Исходный код библиотеки
│   ├── resizers/               # Алгоритмы изменения размера
│   ├── metrics/                # Метрики качества и сравнения
│   ├── rgb/                    # Работа с изображениями
│   └── utils/                  # Вспомогательные утилиты
├── benchmarks/            # Утилиты бенчмарков
├── models/                # Модели нейронных сетей
├── external/              # Внешние исполняемые файлы
├── data/                  # Тестовые изображения
└── include/               # Заголовочные файлы
```

## Коды возврата

- `0` - Успешное выполнение
- `1` - Ошибка выполнения (неверные параметры, проблемы с файлами и т.д.)

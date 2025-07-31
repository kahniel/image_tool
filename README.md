# Image Tool

Утилита командной строки для обработки изображений с поддержкой изменения размера, сравнения и оценки качества изображений.

## Возможности

- **Изменение размера изображений** с различными алгоритмами
- **Сравнение изображений** с метриками качества
- **Оценка качества изображений**

## Установка

### Требования

- C++17 или новее
- CMake 3.12+
- OpenCV
- CLI11

### Сборка

```bash
git clone https://github.com/kahniel/image_tool.git
cd image_tool
mkdir build && cd build
cmake ..
make
```

### Установка зависимостей

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install cmake build-essential libopencv-dev
```

**macOS:**
```bash
brew install cmake opencv
```

**Windows (vcpkg):**
```bash
vcpkg install opencv cli11
```

## Использование

### Команда resize - Изменение размера изображений

```bash
./build/bin/image_tool resize -i <input> -o <output> -s <scale> -m <method>
```

**Параметры:**
- `-i, --input` - Путь к входному изображению (обязательно)
- `-o, --output` - Путь к выходному изображению (обязательно)
- `-s, --scale` - Коэффициент масштабирования (положительное число, по умолчанию: 1)
- `-m, --method` - Метод изменения размера (по умолчанию: "my-up")

**Доступные методы:**
- `my-up` - Собственный алгоритм увеличения
- `my-down` - Собственный алгоритм уменьшения
- `linear` - Билинейная интерполяция
- `cubic` - Бикубическая интерполяция
- `lanczos` - Фильтр Lanczos
- `edsr` - Enhanced Deep Super-Resolution
- `realesrgan` - Real-ESRGAN

**Примеры:**
```bash
# Увеличение изображения в 4 раза методом Real-ESRGAN
./build/bin/image_tool resize -i data/ponyo.png -o output/large_ponyo.png -s 4 -m realesrgan

# Уменьшение изображения в 2 раза наивным методом
./build/bin/image_tool resize -i data/image.jpg -o small_image.jpg -s 2 -m my-down

# Изменение размера с бикубической интерполяцией
./build/bin/image_tool resize -i input.png -o output.png -s 3 -m cubic
```

### Команда compare - Сравнение изображений

```bash
./image_tool compare -1 <first_image> -2 <second_image> -m <method>
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
# Сравнение изображений по MSE
./image_tool compare -1 original.png -2 processed.png -m mse

# Сравнение изображений по SSIM
./image_tool compare -1 img1.jpg -2 img2.jpg -m ssim

# Сравнение с методом по умолчанию (MSE)
./image_tool compare -1 before.png -2 after.png
```

### Команда quality - Оценка качества изображения

```bash
./image_tool quality -i <input>
```

**Параметры:**
- `-i, --input` - Путь к изображению для анализа (обязательно)

**Пример:**
```bash
# Оценка качества изображения
./image_tool quality -i data/image.png
```

## Примеры использования

### Пакетная обработка изображений

```bash
# Уменьшение всех изображений в папке
for img in data/*.png; do
    filename=$(basename "$img" .png)
    ./image_tool resize -i "$img" -o "small_${filename}.png" -s 4 -m my-down
done

# Последующее увеличение с помощью Real-ESRGAN
for img in small_*.png; do
    filename=$(basename "$img" .png)
    ./image_tool resize -i "$img" -o "restored_${filename}.png" -s 3 -m realesrgan
done
```

### Сравнение результатов разных методов

```bash
# Создание версий с разными методами
./build/bin/image_tool resize -i original.png -o nearest.png -s 2 -m linear
./build/bin/image_tool resize -i original.png -o cubic.png -s 2 -m cubic
./build/bin/image_tool resize -i original.png -o lanczos.png -s 2 -m lanczos

# Сравнение результатов с оригиналом
./build/bin/image_tool compare -1 original.png -2 nearest.png -m ssim
./build/bin/image_tool compare -1 original.png -2 cubic.png -m ssim  
./build/bin/image_tool compare -1 original.png -2 lanczos.png -m ssim
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
./image_tool --help                    # Общая справка
./image_tool resize --help             # Справка по команде resize
./image_tool compare --help            # Справка по команде compare
./image_tool quality --help            # Справка по команде quality
```

## Коды возврата

- `0` - Успешное выполнение
- `1` - Ошибка выполнения (неверные параметры, проблемы с файлами и т.д.)

## Известные ограничения

- Размер изображений ограничен доступной памятью
- Некоторые методы (EDSR, Real-ESRGAN) требуют дополнительных вычислительных ресурсов
- Коэффициент масштабирования должен быть положительным числом

# Класс CCrfLossLayer

<!-- TOC -->

- [Класс CCrfLossLayer](#класс-ccrflosslayer)
    - [Настройки](#настройки)
        - [Вес функции ошибки](#вес-функции-ошибки)
        - [Урезание градиентов](#урезание-градиентов)
    - [Обучаемые параметры](#обучаемые-параметры)
    - [Входы](#входы)
    - [Выходы](#выходы)
        - [Значение функции потерь](#значение-функции-потерь)

<!-- /TOC -->

Класс реализует слой, вычисляющий функцию потерь использующуюся при обучении CRF. Функция ошибки равна `-log(вероятность эталонной последовательности классов)`.

## Настройки

### Вес функции ошибки

```c++
void SetLossWeight( float lossWeight );
```

Установка коэффициента, на который будут домножаться градиенты этой функции потерь при обучении. По умолчанию `1`. Полезен при использовании нескольких функций потерь в одной сети.

### Урезание градиентов

```c++
void SetMaxGradientValue( float maxValue );
```

Установка максимального значения элемента в градиенте. Все значения градиента, по модулю превосходящие `GetMaxGradientValue()`, будут приведены к значениям, равным по модулю `GetMaxGradientValue()`.

## Обучаемые параметры

Слой не имеет обучаемых параметров.

## Входы

Слой имеет три обязательных входа и один необязательный.

1. На первый вход подаётся блоб с данными типа `int`, с оптимальными последовательностями классов. Блоб имеет размеры:

    - `BatchLength` равен `BatchLength` входов;
    - `BatchWidth` равен `BatchWidth` входов;
    - `Channels` равен числу классов;
    - остальные размерности равны `1`.

2. На второй вход подаётся блоб с данными типа `float`, содержащий ненормализованные логарифмы вероятностей оптимальных последовательностей классов. Блоб имеет те же размеры, что и у первого входа.

3. На третий вход подаётся блоб с ненормализованными логарифмами вероятностей эталонного класса. Блоб имеет размеры:

    - `BatchLength` равен `BatchLength` входов;
    - `BatchWidth` равен `BatchWidth` входов;
    - остальные размерности равны `1`.

4. *[Опционально]* На четвертый вход подаётся блоб, содержащий веса последовательностей в батче, размера:
    - `BatchWidth` должен быть равен `BatchWidth` блоба из первого входа
    - остальные размерности равны `1`

## Выходы

Слой не имеет выходов.

### Значение функции потерь

```c++
float GetLastLoss() const;
```

Получение значения функции потерь на последнем запуске сети.

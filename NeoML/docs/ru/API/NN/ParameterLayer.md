# Класс CParameterLayer

<!-- TOC -->

- [Класс CParameterLayer](#класс-CParameterLayer)
    - [Настройки](#настройки)
        - [Инициализация матрицы весовв](#Инициализация-матрицы-весов)
    - [Обучаемые параметры](#обучаемые-параметры)
        - [Матрица весов](#матрица-весов)
    - [Входы](#входы)
    - [Выходы](#выходы)

<!-- /TOC -->

Класс реализует слой, содержащий блоб обучаемых параметров.

## Настройки

### Инициализация матрицы весов

```c++
void SetBlob(CDnnBlob* _blob);
```
Установка блоба начальных весов слоя.

## Обучаемые параметры

### Матрица весов

```c++
const CPtr<CDnnBlob>& GetBlob() const;
```
Блоб с обучаемыми весами слоя.

## Входы

У слоя нет входов.

## Выходы

Выходом слоя является блоб весов ( той же размерности, что был установлен, соответственно).
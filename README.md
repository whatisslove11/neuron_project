# Нейронная сеть по распознаванию цифр
Для выполнения этой задачи я выбрал модель однослойного перцептрона

Проект написан на С++ с использованием только стандартных библиотек, за исключением "stb_image.h" - используется для перевода картинок в читаемый для компьютера вид

Нейронная сеть обучалась на 60.000 тренировочных изображений размером 28х28 пикселей и проходила валидацию на 10.000 изображениях, показав итоговую точность порядка 90% 

Рассмотрим файлы и их роль:

1. "my_functions.h" - заголовочный файл, обьявляющий доступ для 2 функций - sigmoid_or_ReLU() и char_to_int()
2. sigmoid_or_ReLU.cpp - функция активации, используется для расчетов в hidden & final слоях
3. "stb_image.h" - найденное на просторах интернета, обеспечивает перевод изображения в читаемый для машины вид (в виде массива 28х28)
4. char_to_int.cpp - файл для расшифорвки правильного ответа из названия изображения (по сути, просто преобразует символ char в int)

Рассмотрим функции, описанные в main.cpp:

1. getRandom - функция, инициализирующая вихрь Мерссена для генерации случайных значений весов нейронной сети. 
2. getIndex - функция, обеспечивающая доступ к связям определенного нейрона. В связи с тем, что все коэф. связи между нейронами записывались не от нейрона n слоя ко всем нейронами слоя n+1, а именно от одного нейрона слоя n+1 ко всем нейронами слоя n ( hidden <-> start, final <-> hidden), то для доступа ко всем связям определенного нейрона используется данная функция
3. validation - функция, которая принимает количество изображений для тренировки и последующего измерения точности работы нейронной сети. Кроме приема данных, отвечает за првоерку их правильности. Т.к. датасет ограничен только 60.000 изображений вида train и 10.000 изображениями вида test, то при вводе большего значения необходимо попросить пользователя ввести значения заново.

Кратко рассмотрим алгоритм работы нейронной сети:
Изначально пользователю предлагается 2 варианта работы - загрузить уже имеющиеся веса нейронной сети или создать новые
При выборе create:
1. при помощи функции getRandom() инициализируются случайные веса между нейронами 1 и 2 слоев (start & hidden) и нейронами 2 и 3 слоев (hidden & final)

После выбора create/load будет выбор между train/test. Второй вариант просто принимает путь до файла и выносит вердикт, какую именно цифру предложили "отгадать" сети. Рассмотрим более подробно вариант train:

2. Далее происходит получение доступа к папке train, в которой хранится 60.000 изображений для обучения сети + хранится файл с названиями этих изображений. Пользователю предлагается ввести количество изображений для тренировки и для валидации
3. Далее сеть просто суммирает сначала для одного (первого) нейрона hidden слоя веса всех нейронов 1 слоя, после чего применяет к этому нейрону функцию активации sigmoid_or_ReLU(). Такой процесс происходит с каждым нейроном hidden слоя
4. После завершения процесса для hidden слоя аналогичный процесс запускается для всех нейронов final layer (cуммирование, после чего применене функции активации)
5. После завершения всех процессов происходит "вынос вердикта" - циклом проверяется, в каком из нейронов "лежит" значение больше; номер нейрона означает вердикт по цифре (в нейроне под номером 0 оказалось значение больше, то загруженаня цифра - 0)
6. После выноса вердкта идет проверка на правильность - нейронная сеть "смотрит" на правильный ответ (он зашифрован в названии файла), потом, в случае правильности, увеличивает счетчик правильных ответов на 1; в случае отрицательного ответа увеличивается, соответственно, счетчик неправильных ответов
7. После "прогонки" заданного пользователем количества изображений для обучения начинается процесс "подгонки" коэффициентов, используя Back propagation. Процесс происходит следующим образом: 

Изначально для всех нейронов, которые внесли большой вклад (больше, чем заданный коэффициент), происходит увеличения ценности нейрона.
После чего для всех связей между нейронами, давших неверный ответ (допустим, была загадна цифра 5, тогда под всеми связями имеются в виду связи hidden layer между всеми нейронами final layer, кроме нейрона под номером 5) идет уменьшение их "стоимости"

8. После применения Back propagation идет период валидации - тут уже сеть не будет корректировать собственные веса, а просто будет проверяться на правильность и точность ее обучения в период test.
9. После окончания периода валидации выводится итоговая статистика в формате (всего_угаданных_ответов)/(всего_ответов); также выводится промежуточная статистика для периода обучения и для периода валидации (выводится после прохождения каждых новых 5% от датасета)

После окончания обучения пользователю будет предложено обновить уже имеющиеся веса при помощи введения одной из букв: y/n

Будущие апдейты для данного проекта:
1. Написать вывод графиков точности обучения нейронной сети
2. Дописать обучение для более чем одной эпохи (сейчас происходит только одна эпоха обучения)
3. Вывод времени обучения нейронной сети (по возможности, попробовать ускорить процесс)



# Данный пользователь впервые описывал готовый проект и впервые пользовался github'ом. Прошу не судить строго за не самый лучший вид репозитория и за не самую лучшую записку.

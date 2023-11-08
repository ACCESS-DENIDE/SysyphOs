
[BITS 16]
[ORG 0x7c00]
start:
mov ax, cs ; Сохранение адреса сегмента кода в ax
mov ds, ax ; Сохранение этого адреса как начало сегмента данных
mov ss, ax ; И сегмента стека
mov sp, start ; Сохранение адреса стека как адрес первой инструкции этого кода. Стек будет расти вверх и не перекроет код.
mov ah, 0x0e ; В ah номер функции BIOS: 0x0e - вывод символа на активную видео страницу (эмуляция телетайпа)
call clear

read:
mov ax,0x1000
mov es,ax
mov bx,0x00 ; es:bx(0х1000:0х0000) - адрес буфера, в который считываются данные из ядра
mov ah,0x02 ; Функция 0x02 прерывания 0х13 (считывание заданного количества секторов с диска в память)
mov dl,1 ; Номер диска (носителя)
mov dh,0x00 ; Номер головки
mov cl,0x01 ; Номер сектора
mov ch,0x00 ; Номер цилиндр
mov al, 40 ; количество секторов
int 0x13 ; Прерывание на дисковый ввод/вывод
mov ebx, 0x00
mov ecx, 0x00
mov cl, 1
;cl - текущий выбранный цвет
;ch - текущая строка цвета для печати
;edx - счетчик строк
menu:
call clear
mov edi, 0xb8000 ;Начало области памяти, выделенной под видеоадаптер
mov edx, 0x00 ;Счетчик \n
mov esi, loading_string
call choice_color
mov ch, 1 ;Счетчик печатаемого цвета
call new_line
mov esi, gray
call choice_color
call new_line
mov esi, white
call choice_color
call new_line
mov esi, yellow
call choice_color
call new_line
mov esi, blue
call choice_color
call new_line
mov esi, red
call choice_color
call new_line
mov esi, green
call choice_color

input:
mov ah, 0x00 ;ожидание нажатия и считывание нажатой пользователем клавиши
int 0x16
cmp ah, 0x48 ;если это клавиша вверх, то выполняется следующая функция
je move_up
cmp ah, 0x50 ;если это клавиша вниз, то выполняется следующая функция
je move_down
cmp ah, 0x1C ;если это enter, то происходит загрузка ядра
je load_kernel
jmp input ;если была нажата другая клавиша, то снова считываем нажатие клавиши

overflow_above:
mov cl, 6
jmp menu
overflow_below:
mov cl, 1
jmp menu
move_up:
cmp cl, 1 ;Если текущий цвет начальный(т.е.серый = 1), то следующий цвет будет последним(т.е. зеленый = 6)
je overflow_above
dec cl ;Иначе декрементируем текущий цвет
jmp menu ;Выводим меню
move_down:
cmp cl, 6 ;Если текущий цвет последний(т.е. зеленый = 6), то следующий цвет будет начальным(т.е. серый = 1)
je overflow_below
inc cl ;Иначе инкрементируем текущий цвет
jmp menu
clear: ;Очистка экрана
mov ax, 3
int 0x10
ret

new_line:
inc edx ;Номер следующей строки
mov ebx, edx ;Копируем
imul ebx, 160 ;Умножаем на 160 (80 символов в строке, по 2 атрибута)
mov edi, 0xb8000
add edi, ebx ;Прибавляем к началу памяти
ret

loading_string:
db "Select the console color...", 0
gray:
db "gy", 0
hlt
white:
db "white", 0
yellow:
db "yellow", 0
blue:
db "blue", 0
red:
db "red", 0
green:
db "green", 0
;Функции, записывающие цвет текущего выбора
gray_color:
mov ah, 0x07
jmp puts
white_color:
mov ah, 0x0f
jmp puts
yellow_color:
mov ah, 0x0e
jmp puts
blue_color:
mov ah, 0x01
jmp puts
red_color:
mov ah, 0x04
jmp puts
green_color:
mov ah, 0x02
jmp puts

choice_color:
mov ah, 0x08 ;Цвет всего меню
cmp ch,cl ;Если текущий цвет и текущий цвет для печати не равны, то выводим его цветом обычного меню
jne puts
cmp cl, 1 ;Иначе выбираем для него цвет
je gray_color
cmp cl, 2
je white_color
cmp cl, 3
je yellow_color
cmp cl, 4
je blue_color
cmp cl, 5
je red_color
cmp cl, 6
je green_color
jmp puts


puts:
; Функция выводит в буфер видеопамяти (передается в edi) строку, оканчивающуюся 0 (передается в esi)
; После завершения edi содержит адрес по которому можно продолжать вывод следующих строк
mov al, [esi]
test al, al
jz return
mov [edi], al
mov [edi+1], ah
add edi, 2
add esi, 1
jmp puts
return:
inc ch ;Инкремент цвета для печати
ret

load_kernel:
call clear
mov [0x600], cl
cli ; Отключение прерываний
lgdt [gdt_info] ; Загрузка размера и адреса таблицы дескрипторов
in al, 0x92
or al, 2 ; Включение адресной линии А20
out 0x92, al
mov eax, cr0
or al, 1 ; Установка бита PE регистра CR0 - процессор перейдет в защищенный режим
mov cr0, eax
jmp 0x8:protected_mode

gdt:
; Нулевой дескриптор
db 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
; Сегмент кода: base=0, size=4Gb, P=1, DPL=0, S=1(user),
; Type=1(code), Access=00A, G=1, B=32bit
db 0xff, 0xff, 0x00, 0x00, 0x00, 0x9A, 0xCF, 0x00
; Сегмент данных: base=0, size=4Gb, P=1, DPL=0, S=1(user),
; Type=0(data), Access=0W0, G=1, B=32bit
db 0xff, 0xff, 0x00, 0x00, 0x00, 0x92, 0xCF, 0x00

gdt_info: ; Данные о таблице GDT (размер, положение в памяти)
dw gdt_info - gdt ; Размер таблицы (2 байта)
dw gdt, 0 ; 32-битный физический адрес таблицы.

[BITS 32]
protected_mode: ; Загрузка селекторов сегментов для стека и данных в регистры
mov ax, 0x10 ; Используется дескриптор с номером 2 в GDT
mov es, ax
mov ds, ax
mov ss, ax
; Передача управления загруженному ядру
call 0x10000 ; Адрес равен адресу загрузки в случае если ядро скомпилировано в"плоский" код
; Внимание! Сектор будет считаться загрузочным, если содержит в конце своих 512 байтов два следующих байта: 0x55 и 0xAA
times (512 - ($ - start) - 2) db 0 ; Заполнение нулями до границы 512 - 2 текущей точки
db 0x55, 0xAA ; 2 необходимых байта чтобы сектор считался загрузочным
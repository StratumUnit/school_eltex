MODULE_NAME = netlink
# Имя файла пользовательской программы (без расширения .c)
USER_APP = user

# Настройка сборки для Kbuild (система сборки ядра Linux)
obj-m += $(MODULE_NAME).o

# Путь к текущим исходникам ядра
KDIR = /lib/modules/$(shell uname -r)/build 
PWD = $(shell pwd)

# Цель по умолчанию: собирает и модуль, и приложение
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	gcc $(USER_APP).c -o $(USER_APP)

# Очистка всех собранных файлов
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f $(USER_APP)
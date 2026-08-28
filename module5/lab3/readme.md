#Мигание светодидов клавиатуры
Сборка помощью Makefile
make
![сборка](images/make.png)

Загрузка файла модуля
sudo insmod kbd_led_sysfs.ko
![сборка](images/insmod.png)

После успешной загрузки в виртуальном каталоге /sys/kernel появится файл kbd_leds/state
![sysfs файл](state.png)
Управление светодиодами происходит записью значений в файл state
Значение это двоичная маска 1(001), 2(010), 7(111)
Если записать значение 1 горит scroll lock, 2 — горит num lock, 3 — caps lock 
echo "7" | sudo tee /sys/kernel/kbd_leds/state

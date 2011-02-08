SpamTeacher v0.8 win32
Программа для рассылки сообщений с произвольного сервера smtp,
Создана с помощью qt (http://qt.nokia.com/) и minGW (http://www.mingw.org/).
Создатель: Евгений Лежнин <z_lezhnin@mail2000.ru>
В ней используется sendEmail v1.56 (http://caspian.dotconf.net).
Инструкция:
Архив включает необходимые библиотеки. Положите все файлы из архива (MassDelivery.exe, icon.xpm, en.qm, ru.qm, sendEmail.exe, QtCore4.dll, QtGui4.dll, readme.txt) в одну папку.
1. Введите всю необходимую информацию в окна настроек("Настройки отправителя", "Настройки получателя", "Настройки вложений").
2. Введите заголовок письма и сообщение. Вы можете вставлять шаблоны %name и %ms|%fs в сообщение с помощью кнопок "Имя" и "Пол" соответственно, эти шаблоны будут заменены текстом для каждого получателя индивидуально: шаблон %name будет заменён на имя получателя, а шаблон пола будет заменён на одну из указанных вами строк в зависимости от пола.
3. Отправляйте сообщение! Не забудьте ввести перед этим пароль и необходимую информация об отправителе и получателях.
...........................
Program for mass delivery of email through smtp.
Created by: Yevgeny Lezhnin: <z_lezhnin@mail2000.ru>
Program uses sendEmail v1.56 (http://caspian.dotconf.net/)
Program have all libraries in archive. Copy all necessary files (MassDelivery.exe, icon.xpm, en.qm, ru.qm, sendEmail.exe, QtCore4.dll, QtGui4.dll, readme.txt) in one directory.
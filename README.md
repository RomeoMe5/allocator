# allocator
Задача: реализовать демон, снимающий статистику с клавиатуры и записывающий ее в файл. Например, частоту нажатий клавиш за период в 5 минут.
события с клавиатуры можно брать тут: /dev/input/eventX (или удобнее: /dev/input/by-path/… - там будет понятно, что из этого клавиатура)
Описание событий: https://www.kernel.org/doc/Documentation/input/input.txt
демон реализовать двумя способами: самостоятельно через fork() и через systemd
про первый способ посмотреть можно тут:http://www.netzmafia.de/skripten/unix/linux-daemon-howto.html
про второй: https://www.freedesktop.org/software/systemd/man/systemd.service.html

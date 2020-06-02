## Blinking LED
Проверочный проект мигания светодиода с использованием CMake и системы сборки Jenkins.

### Установка компилятора GCC и отладчика OpenOCD
* Компилятор:
```sh
sudo apt-get install gcc-arm-none-eabi
sudo apt-get install gdb-arm-none-eabi
```
* Отладчик:
```sh
sudo apt-get install openocd
```

### Сборка с использованием CMake
Для сборки выполнить скрипт `compile.sh` :

```sh
./compile
```
Сборка будет установлена в папку `build`.

При сборке задействуется тулчейнфайл `arm-gcc-toolchain.cmake` в папке `arm-cmake-toolchains`.
Для выполнения команды без скрипта выполнить команды:
```sh
cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=./arm-cmake-toolchains/arm-gcc-toolchain.cmake ..
cmake --build .
```

### Настройка Visual Studio Code
##### Настройка для сборки проекта.
* В папке настройки среды `.vscode` создать файл `cmake-kits.json`, прописать в нем путь к тулчейнфайлу:
```javascript
[
    {
        "name": "GCC ARM NONE EABI",
        "toolchainFile": "arm-cmake-toolchains/arm-gcc-toolchain.cmake"
    }
]
```

##### Настройка для отладки.
* В папке настройки среды `.vscode` в файл настроек vscode `settings.json,` добавить пути к отладчику и ARM тулчейну:
```javascript
    "cortex-debug.armToolchainPath": "/usr/bin",
    "cortex-debug.openocdPath": "/usr/bin/openocd"
```

* В папке настройки среды `.vscode` создать файл `launch.json` и добавить настройки для отладки:
```javascript
{
    // Use IntelliSense to learn about possible attributes.
    // Hover to view descriptions of existing attributes.
    // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "name": "OpenOCD",
            "cwd": "${workspaceRoot}",
            "executable": "./build/blinking_led.elf", // Путь к исполняемому файлу
            "request": "launch", // Вид запуска
            "type": "cortex-debug",
            // Путь к нативному компилятору GCC
            "armToolchainPath": "/opt/st/stm32cubeide_1.3.0/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-arm-embedded.7-2018-q2-update.linux64_1.0.0.201904120901/tools/bin/",
            "serverpath": "/usr/bin/openocd", // Исполняемый файл отладчика
            "servertype": "openocd", // Тип отладчика
            "serialNumber": "", // Серийный номер отладчика(если не один)
            "interface": "swd", // Интерфейс отладчка jtag/swd
            "runToMain": true, // Старт с функции main
            "device": "STM32F103RC", // МК
            "svdFile": "/home/orfanidi/SVD_Files/ST/STM32F103.svd", // Файл карты памяти МК для отладки
            "configFiles": [
                "STM32F103RC.cfg" // Файл конфигурации отладчика OpenOCD
            ]
        }
    ]
}
```

* Файл конфигурации отладчика `STM32F103RC.cfg`
```cmd
source [find interface/jlink.cfg]

transport select "swd"

source [find target/stm32f1x.cfg]
```
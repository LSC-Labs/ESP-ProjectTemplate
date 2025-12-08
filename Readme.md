# ESP-Project Template

This template should ease the development of smart device applications that are running on small devices like the Espressiff family. In most samples you write a sketch to build a functionality on this devices and you find out, that it is very easy to implement these functions. Next you try to get this device up and running, then you find out, that you need a lot of modules, like a WiFi connection, an message broker or a possibility to udpate this devices.
In principle, these functions are also easy to implement and very time consumint, cause you have to implement and learn a lot of things.

This is, where this template comes in place. The template is using the ESP-Runtime PLibESPV1 which offers a application functionality and organize the development into "modules" to ease the implementation.
You can use existing modules (runtime) and write your own modules with new functions or to replace an existing one.

As soon as a modules are defined, they can be reused and you don't have to worry about how to display and configure them in your frontend. Also a simple to use security module is available, so you don't have to think a lot about authentication or authorization for user and administrator functionalities.

What you have to do to get a smart device with a Web GUI:
- write your own module with the functionality
- select the modules you want to use in your application (standard and your own modules)
- compile and test

See the WiKi at  https://github.com/LSC-Labs/ESP-ProjectTemplate/wiki

## Setup your project...
After cloning this template as base of your project, follow the "initialze" script in the project root.
1. registers the submodules - the PLibESPV1 libarary and the wiki.
2. load/update the needed npm modules like gulp
3. decide if you want to get updates from the runtime (default is detached).
4. customize the project for your needs.

### Customizing your project

Files that are used to support your project.

| file | to do |
|-- |-- |
|package.json | Edit the version number and the description of your project. The version number will also be used at compile time.
|pages.json | Modules you want to use in your project. see ([Scripts](Scripts)) in the WiKi how to use.
|src/web/js/settings.js| Contains the defaults for your Web GUI, like icons, menu structures and other settings you need at runtime.


After setting up this files, use the "sync-Files" and the "build-Pages" npm functions. If you are using Visual Code as editor, you can start these scripts in the section "NPM SCRIPTS". These script can be started again, if you changed some modules and you need an update.




from os import path
from platformio.package.manager.library import LibraryPackageManager

Import('env')

source = path.join('main', 'tft_espi_config.h')
target = path.join('$PROJECT_LIBDEPS_DIR', '$PIOENV', 'TFT_eSPI', 'User_Setup_Select.h')

env.Execute(Copy(target, source))

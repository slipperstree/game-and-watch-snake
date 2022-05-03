# Snake game for Nintendo Game &amp; Watch
This is a port of the [EmbSnake](https://gitee.com/slipperstree/EmbSnake) that runs on the Nintendo Game &amp; Watch: Super Mario Bros / Zelda game.

## Build
You need backup your G&W first. See [game-and-watch-backup](https://github.com/ghidraninja/game-and-watch-backup)
- You need STM32 Drivers in the `game-and-watch-snake` directory for the `Makefile`. 
You can simply create a symbolic link to [game-and-watch-retro-go/Drivers](https://github.com/kbeckmann/game-and-watch-retro-go) using the following command: `ln -s ../some-where/game-and-watch-retro-go/Drivers Drivers`
- Build and flash by `make flash`
- Or you can just download [pre builded ELF file](/Release/gw_snake.elf) for test.

## Features
This game contains the following pages.
- Home
- Demo mode (which can play game it self just like G&W mario or zelda TIME MODE does)
- Game mode (which you can play by yourself)
- Game over page (Hiscore cannot be saved in flash by now)
- Enter sleep mode by press power button. (not save state)
- etc.

## Controls
- Press any button at Home page will start a new game.
- Wait a while at Home page will start demo.
- Swich Game / Demo mode by press GAME / TIME button.
- Other operation will show at the bottom of screen.

## Pictures
- ![Home](/Pics/Home.jpg)
- ![Demo](/Pics/Demo.jpg)
- ![Game](/Pics/Game.jpg)
- ![GameOver](/Pics/GameOver.jpg)

## Tasks
- Some simple sound
- Save hi-score
- Screen bright
- Show a DigitClock at demo mode (snake will eat digit for update time)
- [Done]Sleep when Power button pushed

## Thanks
- This project is based on [game-and-watch-base](https://github.com/ghidraninja/game-and-watch-base).
- This project use Drivers by [game-and-watch-retro-go](https://github.com/kbeckmann/game-and-watch-retro-go) downloaded.

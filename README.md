# Snake game for Nintendo Game &amp; Watch
This is a port of the [EmbSnake](https://gitee.com/slipperstree/EmbSnake) that runs on the Nintendo Game &amp; Watch: Super Mario Bros / Zelda game.

## Build
You need backup your G&W first. See [game-and-watch-backup](https://github.com/ghidraninja/game-and-watch-backup)
- Get Drivers
You need STM32 Drivers in the `game-and-watch-snake` directory for the `Makefile`. 
You can simply create a symbolic link to [game-and-watch-retro-go/Drivers](https://github.com/kbeckmann/game-and-watch-retro-go) using the following command
`ln -s ../some-where/game-and-watch-retro-go/Drivers Drivers`
- Build and flash
`make flash`

## Features
This game contains the following pages.
- Home
- Demo mode (which can play game it self just like G&W mario or zelda TIME MODE does)
- Game mode (which you can play by yourself)
- Game over page (Hiscore cannot be save in flash by now)
- etc.

## Controls
- Press any botton at Home page will start a new game.
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
- Sleep when Power button pushed
- Screen bright
- Show a DigitClock at demo mode (snake will eat digit for update time)

## Thanks
- This project is based on [game-and-watch-base](https://github.com/ghidraninja/game-and-watch-base).
- This project use Drivers by [game-and-watch-retro-go](https://github.com/kbeckmann/game-and-watch-retro-go) downloaded.

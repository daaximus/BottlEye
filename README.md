<img src="./bottleye_logo.png" width="500">

# Overview
BottlEye is a usermode emulator for the commercial anti-cheat [BattlEye](battleye.com) that works by recreating the anti-cheat module and responding to requests sent by the game server. The write-up of this project can be found on our [website](https://secret.club/2020/07/06/bottleye.html)


# How to use

First you need to set up a man-in-the-middle environment that allows you to capture the responses sent by the real BEClient. When those are logged, implement them into BottlEye and compile it. Replace the BEClient_x64.dll module on disk with your BottlEye build and launch the game.

If you are having issues figuring out the ip address and port to monitor with Wireshark (or whatever tool you choose to use), build BottlEye without the responses and launch the game with it. BottlEye will print the ip and port, which you can later use to filter traffic with.

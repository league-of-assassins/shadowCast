# shadowCast
 Shadow Casting using SFML

It has its own class so you can implement it to your own project without any worries. And it uses vectors so you are free to add as many shadows as you want inside one instance.

<img src="/assets/sc.gif" width="400" height="400">

Usage:

    Define shadows' casting limits (borders) with its init func

    Add shadows by passing two positions

    Change any shadow's fixed positions

    Remove any shadow

    Need to call updateShadows inside window loop as base pos keeps changing

    Need to draw shadows



    Refer to App.cpp / App.hpp for more info about their usages


Requires SFML 2.6.0 or later versions

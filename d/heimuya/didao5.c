//room: didao5.c
inherit ROOM;

void create()
{
  set("short","秘道");
        set("long", @LONG
走在里面，你不觉想到这似乎在每个角落都藏着无限杀机，你感到近了近了，
越来越近了。
LONG );

  set("exits",([
      "east" : "/d/heimuya/didao3",
      "west" : "/d/heimuya/didao3",
      "north" : "/d/heimuya/didao2",
      "south" : "/d/heimuya/didao1",
  ]));
  set("objects",([
      "/clone/beast/dushe" : 4,
  ]));
  setup();
  replace_program(ROOM);
}

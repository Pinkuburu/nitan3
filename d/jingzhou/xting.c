inherit ROOM;

void create()
{
	set("short", "西厅");
	set("long", @LONG
这里是万府内的西厅，房间里摆着一张桌子，以及一个书
架，书架上放着很多书籍，是万门弟子读书的地方。
LONG);
	set("exits", ([
		"east" : __DIR__"dayuan",
		
	]));

	set("coor/x", -480);
	set("coor/y", -180);
	set("coor/z", 10);
	setup();
	replace_program(ROOM);
}


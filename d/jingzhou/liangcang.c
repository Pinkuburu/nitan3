inherit ROOM;

void create()
{
	set("short", "粮仓");
	set("long", @LONG
这里就是荆州粮仓了，地上堆着许多麻袋，大概装的是包
谷大米之类的粮食。
LONG);
	set("exits", ([
		"west" : __DIR__"lcmen",
	]));

	set("coor/x", -430);
	set("coor/y", -230);
	set("coor/z", 20);
	setup();
	replace_program(ROOM);
}


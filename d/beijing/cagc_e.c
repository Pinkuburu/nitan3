#include <room.h>
inherit ROOM;

void create()
{
	set("short", "东长安广场");
        set("long", @LONG
这里是天安门右边的东长安街广场，宽阔的长安大街向东西两头延伸，这里
京城里最热闹繁华的地段，既是交通要道，也是京城百姓们的聚点, 广场上车水
马龙，热闹非凡。小贩们沿街叫卖着自己的货物。一些游手好闲的人常常在这里
溜溜达达。这里可以望见西边高大厚实的古城墙，那里便是天安门广场了。东边
则是东长安大街。
LONG );
       set("exits", ([
		"east" : "/d/beijing/caroad_e1",
		"west" : "/d/beijing/tiananmen",
	]));
	set("objects", ([
		"/d/beijing/npc/shusheng1" : 2,
		"/d/beijing/npc/xianren" : 1,
	]));
	set("outdoors", "beijing");
	set("coor/x", -20);
	set("coor/y", 260);
	set("coor/z", 0);
	setup();
	replace_program(ROOM);
}


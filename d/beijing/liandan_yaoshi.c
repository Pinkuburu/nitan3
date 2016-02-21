#include <ansi.h>
inherit ROOM;

void create()
{
        set("short", "姚春内室");
        set("long", @LONG
这里是一间小小的房间，没什么特别的地方，你看见书桌旁边有一位头发花
白的老人正在阅读经书，那便是京城回春堂著名的『跌打名医』姚春了。姚春的
前方有一扇半开的铁门，不停的从里面冒出滚滚浓烈的药味.
LONG );
        set("no_fight", 1);
        set("objects", ([
                "/d/beijing/npc/liandan_yaochun"     : 1,
        ]));
        set("exits", ([
                "north" : "/d/beijing/liandan_fang",
                "south": "/d/beijing/liandan_neiting",
        ]));

	set("coor/x", -30);
	set("coor/y", 280);
	set("coor/z", 0);
	setup();
}

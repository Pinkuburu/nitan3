// /d/city/tianbaoge.c
// by Lonely

#include <ansi.h>

inherit ROOM;

void create()
{
        set("short", "天宝阁");
        set("long", @LONG
这里原是本城一家最大的书店，因该店孙老板经营不善而倒闭。在此
之后扬州外的长乐帮曾经租下过这个店面，但不知为何又搬走了。最近一
位书商将此店买下，装修了一番后又重新开业了。听说店里除了卖书之外，
还将提供一些新的服务。
LONG );

        set("exits", ([
                "south" : __DIR__"xidajie3",
        ]));
        
        set("objects", ([
                "/clone/npc/publisher" : 1,
        ]));

        set("no_fight", 1);
        set("no_sleep", 1);
        set("day_shop", 1);
        
	set("coor/x", -30);
	set("coor/y", 10);
	set("coor/z", 0);
	setup();
}

void reset()
{
        ::reset();
        set("no_clean_up", 1);
}

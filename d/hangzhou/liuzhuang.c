// liuzhuang.c
// Date: Nov.1997 by Venus
#include <room.h>
inherit ROOM;
int do_knock(string arg);

void create()
{
        set("short", "刘庄");
        set("long", @LONG
庄园的大匾上是『刘庄』两个大字。大门(gate)紧闭，里面静悄悄的一点声
音都没有。
LONG );
        set("exits", ([
            	"east"  : "/d/hangzhou/dadao3",
		"south" : "/d/hangzhou/liuzhuang1",
		"north" : "/d/hangzhou/liuzhuang2",
        ]));
        set("item_desc", ([
           	"gate" : "朱红的大门紧闭着，敲敲(knock)看吧。\n"
        ]));
        set("objects", ([
                "/d/hangzhou/npc/guanjia" : 1,
                "/d/hangzhou/npc/jiading" : 2,
        ]));
        setup();
}

void init()
{
        add_action("do_knock", "knock");
}

int do_knock(string arg)
{
        object me;

        me = this_player();
	message_vision("$N“咚，咚，咚，。。。”地敲着门，可里面却一"
		       "点动静都没有。\n", me);
        return 1;
}


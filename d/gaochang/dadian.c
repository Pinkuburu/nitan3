#include <ansi.h>

inherit ROOM;

void create()
{
        set("short", "高昌大殿");
        set("long",
"这里就是传说中埋藏着无数宝藏高昌迷宫的大殿，四壁供
的都是泥塑木雕的佛像.大殿之上有一个大大的龙椅，大堂之
上透着一股威严，有一块大匾，上书：

        " WHT "※※※※※※※※※※※※※※※※※※※
        ※                                  ※
        ※     " NOR + HIR "文     成      武     德" NOR + WHT "     ※
        ※                                  ※
        ※※※※※※※※※※※※※※※※※※※" NOR "

东西两面都是殿堂。\n");

        set("exits", ([
                "west" : __DIR__"wroom1",
                "east" : __DIR__"eroom1", 
        ]));
        setup();
        replace_program(ROOM);
}

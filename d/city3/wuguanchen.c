// Room: wuguanchen.c
// Date: Feb.14 1998 by Java

inherit ROOM;
void create()
{
	set("short", "金牛武馆一部");
        set("long", @LONG
这里是「金牛武馆」教授一部，师傅陈浒教授如下技能：基本轻功，基本内
功，基本招架，基本拳法，基本杖法，基本棒法。
LONG );
	set("objects", ([
		"/d/city3/npc/chenhu" : 1,
	]));
        set("no_fight",0);
        set("no_steal",1);
        set("no_beg",1);
	set("no_clean_up", 0);
	set("exits", ([
		"west" : "/d/city3/wuguan",
	]));
	set("coor/x", -400);
	set("coor/y", -150);
	set("coor/z", 0);
	setup();
	replace_program(ROOM);
}

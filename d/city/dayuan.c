// Room: /city/dayuan.c
// YZC 1995/12/04 

inherit ROOM;

void create()
{
	set("short", "财主大院");
	set("long", @LONG
你走进大院，迎面是一个假山水池，池上摆着一块奇形怪状的石头，据说是
当年花石纲的遗石。水池的两旁种满了花草。东边是一棵槐树，郁郁葱葱，遮盖
了大半个院子，显得阴森恐怖。一条长廊走道通往后院，有几个家丁把守。
LONG );
	set("exits", ([
		"north" : "/d/city/houyuan",
		"south" : "/d/city/caizhu",
	]));
	set("objects", ([
		"/d/city/npc/guanjia" : 1,
		"/d/city/npc/jiading" : 2,
	]));
//	set("no_clean_up", 0);
	set("coor/x", -20);
	set("coor/y", 20);
	set("coor/z", 0);
	setup();
	replace_program(ROOM);
}

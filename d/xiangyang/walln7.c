// Room: /d/xiangyang/walln7.c
// Date: Jan. 8 1999 by Winder

inherit ROOM;

void create()
{
	set("short", "北城头");
        set("long", @LONG
这里是襄阳的北城头。一些宋兵正在城头巡逻，一个牙将正在这儿负责接受
各种守城材料，一些弓箭手正拈弓搭箭紧盯着城下的动静。
LONG );
        set("outdoors", "xiangyang");

	set("exits", ([
		"southeast" : "/d/xiangyang/walle8",
		"westdown"  : "/d/xiangyang/walln5",
	]));
	set("objects", ([
		"/d/xiangyang/npc/ya"   : 1,
		"/d/xiangyang/npc/bing" : 2,
	]));
	set("coor/x", -260);
	set("coor/y", -40);
	set("coor/z", 40);
	setup();
	replace_program(ROOM);
}

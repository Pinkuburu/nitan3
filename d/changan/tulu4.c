// Room:/d/changan/tulu4.c

inherit ROOM;

void create()
{
	set("short", "土路");
        set("long", @LONG
这是一条黄土飞扬的路，不算太宽，路上的行人也不多，大概是地处偏远的
缘故，几乎没有什么人烟。
LONG );
	set("exits",  ([  /*  sizeof()  ==  2  */
		"northdown" : "/d/huanghe/yongdeng",
		"southwest" : "/d/changan/tulu3",
	]));
	set("outdoors", "changan");
	set("coor/x", -380);
	set("coor/y", 90);
	set("coor/z", 10);
	setup();
	replace_program(ROOM);
}

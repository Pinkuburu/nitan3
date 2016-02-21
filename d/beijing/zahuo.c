inherit ROOM;

void create()
{
	set("short", "杂货铺");
        set("long", @LONG
这里是北京城东长安街最东边的一家杂货铺，杂货铺不大，门前高高的挂着
一条横幅，上面写着『南北货』三个大字，横幅纸张已经泛黄，看来这家店铺在
这里已经有一段历史了。店铺里大箱小箱堆满了一地，都是一些普通的日常用品。
店铺掌柜懒洋洋地躺在一只躺椅上，招呼着过往行人。
LONG );
	set("exits", ([
		"west" : "/d/beijing/caroad_e2",
	]));
	set("objects", ([
		"/d/beijing/npc/hu": 1,
	]));

	setup();
	replace_program(ROOM);
}


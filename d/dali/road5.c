//Room: /d/dali/road5.c
//Date: June. 29 1998 by Java

inherit ROOM;
void create()
{
	set("short","黄土路");
        set("long", @LONG
这里是大理境内，你走在一条黄土大路上，行人多是本地的摆夷族人。也间
或有些中原打扮的人事走动，此路北面通向官道，向西北有路通往大理城的南门，
东行可达广东佛山。
LONG );
	set("objects", ([
	   "/d/dali/npc/yetu": 2,
	]));
	set("outdoors", "dali");
	set("exits",([ /* sizeof() == 1 */
	    "northwest"  : "/d/dali/shuangheqiao",
	    "southeast"  : "/d/foshan/road1",
	]));
	setup();
	replace_program(ROOM);
}


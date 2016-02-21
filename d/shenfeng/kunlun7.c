// Room: /d/gaochang/room63.c// This is a room made by roommaker. write by Fan. :)
inherit ROOM;
void create()
{
	set("short", "昆仑山");
	set("long", @LONG
这一带树木丛生，山石嶙峋，行走甚是不便，山上积雪更厚，道路崎
岖，行走自是费力。转过两个山坡，进了一座大松林。林中松树都是数百
年的老树，枝柯交横，一出松林，即到山峰脚下。仰望山峰，此时近观，
更觉惊心动魄。
LONG );
	set("no_new_clean_up", 0);
	set("outdoors", "kunlun");
	set("exits", ([ /* sizeof() == 2 */
                "southeast" : __DIR__"kunlun6",
                "northeast" : "/d/kunlun/klshanlu",

        ]));

	setup();
	replace_program(ROOM);
}

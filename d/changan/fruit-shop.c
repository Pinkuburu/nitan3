//Room: fruit-shop.c

inherit ROOM;

void create ()
{
        set ("short", "水果店");
        set("long", @LONG
长安城唯一的一家水果店。店里面摆满了一排排的竹筐，筐里有各式各样的
水果。这儿的水果为了保持新鲜，都是每天从城外的果园用快车运来的。水果店
女老板张兰香正热情的招呼着客人。
LONG );
        set("exits", ([
        	"north"   : "/d/changan/qixiang2",
        ]));
        set("objects", ([
        	"/d/changan/npc/lanxiang"   : 1,
        ]));

        set("coor/x", -420);
	set("coor/y", 30);
	set("coor/z", 0);
	setup();
        replace_program(ROOM);
}

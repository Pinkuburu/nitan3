// Room /d/heimuya/midao02.c
// By haiyan

#include <room.h>
inherit ROOM;

void create()
{
     set("short", "秘道");
        set("long", @LONG
这是日月神教极其秘密的通道。日月神教武功多为诡秘异功，为免外人打扰，
故教中弟子多在此地练功。
LONG );
     set("exits", ([
         "west"    : "/d/heimuya/midao01",
         "east"    : "/d/heimuya/lwchang1",
         "south"   : "/d/heimuya/lwchang2",
         "north"   : "/d/heimuya/lwchang3",
     ]));
     set("no_clean_up", 0);
     setup();
     replace_program(ROOM);
}


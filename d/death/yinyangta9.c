#include <ansi.h>
inherit ROOM;

void create()
{
        set("short", HIB "������" NOR);
        set("long", HIB @LONG
���ǵ�������ߵ�һ��������Լ������ߣ�����ʮ�˼����ഫ����
ס��������ȸ���������Ҳ��֪�����Ǽ١�
LONG NOR
        );

        set("exits", ([
              "up"   : __DIR__"yinyangta10",
              "down" : __DIR__"yinyangta8",
        ]));
        
        set("objects",([
        ]));
        
        setup();
        replace_program(ROOM);
}
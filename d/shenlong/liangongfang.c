// liangongfang.c ������
// by aln

#include "room.h"
inherit ROOM;

void create()
{
        set("short", "������");
        set("long", @LONG
���������������ܵ�����������������������ż������ţ���λ�����̳���
����ϥ���������������ı�����һЩ���ӣ������з����˸���ҩ�ơ������Ϸ���
���������̴Ӹ����Ի���ڹ��ķ����ӡ�
LONG );
        set("exits", ([
                "west" : __DIR__"wuchang2",
        ]));

        set("cost", 0);

        setup();
}

void init()
{
        add_action("do_practice", "practice");
        add_action("do_practice", "lian");
}

int do_practice(string arg)
{
        object me = this_player();
        string skill;
        int lvl;

        if( me->query("family/family_name") != "������") {
                message("vision","ֻ��һλ�����̳��ϰ�" + me->name() + "һ�����˳�ȥ��\n",environment(me), ({me}));
                me->move(__DIR__"zhulin1");
                message_vision("$N�������̳������˳�����\n", me);
                return 1;
        }             

        if( !arg ) return ("/cmds/skill/enable")->main(me, arg);

        if( arg == "strike" && me->query_skill_mapped(arg) == "huagu-mianzhang" )
                return 0;

        write("��������ϰ�������Ƶĳ�����\n");
        return 1;
}

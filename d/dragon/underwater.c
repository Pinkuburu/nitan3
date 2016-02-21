inherit ROOM; 
#include <ansi.h> 
void create()
{
        set("short", "̶��");
        set("long",
"[1;32m̶ˮ���������̶ˮ�������ͷ�����飬һֻ����һ���\n"
"����������ľһ��һ������ˮ�е�����һ��������ͷ����������������\n"
"΢�����㶼��֪������û�������Ļ��ᡭ�������������뷨��������(cli\n"
"mb)�ɡ�\n"
);
        set("no_magic", 1);
	setup();
}

void init()
{
   add_action("do_climb", "climb");
   if(interactive(this_player()))
   call_out("do_flush",60,this_player());
}

int do_climb(string arg)
{
   object me, room;

   me = this_player();
   if(me->is_busy())
        return notify_fail("������û�գ�\n");
   switch (random(3)) {
   case 0:
      message_vision("$N�ֽŲ��õ���������������һ�룬һʧ����ˤ��������\n", me);
      me->receive_damage("jing",20);
        me->start_busy(5);
      break;
   case 1:
      message_vision("$N�ֽŲ��õ����������ۿ��쵽ͷ�ˣ����һʧ����ˤ��������\n", me);
      me->receive_damage("jing",40);
        me->start_busy(5);
      break;
   case 2:
      if(!arg || arg != "��" && arg != "up" ) return 0;
     room = find_object("/d/dragon/spirit9");
     if(!objectp(room)) room=load_object("/d/dragon/spirit9");
      me->move(room);
      message_vision("$N����ʪ���ܵش�ˮ������������\n", me);
      break;
   }
   return 1;
}

void do_flush(object me)
{
   object room;

   if(!me || environment(me) != this_object())
   return;
   tell_object(me,"\n��ͻȻ����̶���ˮ��Ȼ��������\n\n");
   tell_object(me,"ˮ������������̶�׵�һ��������ӿ������\n");
   tell_object(me,"������һƬ��ȫ�ĺڰ�֮�У����г����˿־塭��\n\n");
    room = find_object("/d/dragon/lake");
    if(!objectp(room)) room= load_object("/d/dragon/lake");
   me->move(room);
   tell_object(me,"��֪�����˶�ã�����ǰһ������ˮ�������˺��档\n\n");
}


// chen.c

#include <ansi.h>

inherit BUNCHER;
inherit F_DEALER;

void create()
{
        set_name("���е�", ({ "chen youde", "chen" }));
        set("long", "���е���������ݹݳ���\n");
        set("gender", "����");
        set("age", 45);
        set_skill("dodge", 80);
        set_skill("force", 80);
        set_skill("parry", 80);
        set_skill("claw", 80);
        set_skill("hand", 80);
        set_skill("strike", 80);
        set_skill("finger", 80);
        set_skill("cuff", 80);
        set_skill("blade", 80);
        set_skill("club", 80);
        set_skill("staff", 80);
        set_skill("sword", 80);
        set_skill("throwing", 80);
        set_temp("apply/attack", 80);
        set_temp("apply/defense", 80);
        set_temp("apply/damage", 20);

        set("combat_exp", 400000);
                set("vendor_goods", ({ 
                       "/clone/book/basic-sword",  
                       "/clone/book/basic-blade",  
                       "/clone/book/basic-dagger",  
                       "/clone/book/basic-unarmed",  
                       "/clone/book/basic-dodge",  
                       "/clone/book/basic-parry",  
                       "/clone/book/basic-force",  
                       "/clone/book/basic-staff",  
                       "/clone/book/basic-whip",  
                       "/clone/book/basic-throwing",  
                       "/clone/book/basic-cuff",  
                       "/clone/book/basic-hand",  
                       "/clone/book/basic-strike",  
                       "/clone/book/basic-finger",  
                       "/clone/book/basic-claw",  
                       "/clone/book/basic-club",  
               }));
        set("shen_type", 1);
        setup();

/*
        carry_object("/clone/book/basic-sword");
        carry_object("/clone/book/basic-sword");
        carry_object("/clone/book/basic-sword");
        carry_object("/clone/book/basic-sword");
        carry_object("/clone/book/basic-sword");
        carry_object("/clone/book/basic-blade");
        carry_object("/clone/book/basic-blade");
        carry_object("/clone/book/basic-blade");
        carry_object("/clone/book/basic-blade");
        carry_object("/clone/book/basic-blade");
        carry_object("/clone/book/basic-dagger");
        carry_object("/clone/book/basic-dagger");
        carry_object("/clone/book/basic-dagger");
        carry_object("/clone/book/basic-dagger");
        carry_object("/clone/book/basic-dagger");
        carry_object("/clone/book/basic-unarmed");
        carry_object("/clone/book/basic-unarmed");
        carry_object("/clone/book/basic-unarmed");
        carry_object("/clone/book/basic-unarmed");
        carry_object("/clone/book/basic-unarmed");
        carry_object("/clone/book/basic-dodge");
        carry_object("/clone/book/basic-dodge");
        carry_object("/clone/book/basic-dodge");
        carry_object("/clone/book/basic-dodge");
        carry_object("/clone/book/basic-dodge");
        carry_object("/clone/book/basic-parry");
        carry_object("/clone/book/basic-parry");
        carry_object("/clone/book/basic-parry");
        carry_object("/clone/book/basic-parry");
        carry_object("/clone/book/basic-parry");
        carry_object("/clone/book/basic-force");
        carry_object("/clone/book/basic-force");
        carry_object("/clone/book/basic-force");
        carry_object("/clone/book/basic-force");
        carry_object("/clone/book/basic-force");
        carry_object("/clone/book/basic-staff");
        carry_object("/clone/book/basic-staff");
        carry_object("/clone/book/basic-staff");
        carry_object("/clone/book/basic-staff");
        carry_object("/clone/book/basic-staff");
        carry_object("/clone/book/basic-whip");
        carry_object("/clone/book/basic-whip");
        carry_object("/clone/book/basic-whip");
        carry_object("/clone/book/basic-whip");
        carry_object("/clone/book/basic-whip");
        carry_object("/clone/book/basic-throwing");
        carry_object("/clone/book/basic-throwing");
        carry_object("/clone/book/basic-throwing");
        carry_object("/clone/book/basic-throwing");
        carry_object("/clone/book/basic-throwing");
        carry_object("/clone/book/basic-cuff");
        carry_object("/clone/book/basic-cuff");
        carry_object("/clone/book/basic-cuff");
        carry_object("/clone/book/basic-cuff");
        carry_object("/clone/book/basic-cuff");
        carry_object("/clone/book/basic-hand");
        carry_object("/clone/book/basic-hand");
        carry_object("/clone/book/basic-hand");
        carry_object("/clone/book/basic-hand");
        carry_object("/clone/book/basic-hand");
        carry_object("/clone/book/basic-strike");
        carry_object("/clone/book/basic-strike");
        carry_object("/clone/book/basic-strike");
        carry_object("/clone/book/basic-strike");
        carry_object("/clone/book/basic-strike");
        carry_object("/clone/book/basic-finger");
        carry_object("/clone/book/basic-finger");
        carry_object("/clone/book/basic-finger");
        carry_object("/clone/book/basic-finger");
        carry_object("/clone/book/basic-finger");
        carry_object("/clone/book/basic-claw");
        carry_object("/clone/book/basic-claw");
        carry_object("/clone/book/basic-claw");
        carry_object("/clone/book/basic-claw");
        carry_object("/clone/book/basic-claw");
*/
}

void init()
{
        add_action("do_buy", "buy");
        add_action("do_list", "list");
}

int accept_object(object who, object ob)
{
        int r; 
        if (r = ::accept_object(who, ob)) 
                return r; 

        if (count_ge(who->query("combat_exp"), 3500))
        {
                message_vision("���е�����$N˵������书Ӧ��"
                               "�������������ٳ�����������û�������ˡ�\n", who);
                return 0;
        }

        if (ob->query("money_id") && ob->value() >= 500)
        {
                who->set_temp("marks/yangzhou_paied",1);
                message_vision("���е¶�$N˵���ã���λ" + RANK_D->query_respect(who) + 
                               "��ѧʲô�أ�\n" , who);
                return 1;
        } else
    if (ob->query("money_id") && ob->value() < 500)
    {
                message_vision("���е���ü��$N˵��Ǯ�Ҳ��ں�"
                               "������Ҳ����Ҳ̫���˵���ɣ�\n", who);
        return 0;
    }
}

int recognize_apprentice(object ob)
{
        if (! (int)ob->query_temp("marks/yangzhou_paied") == 1)
                return 0;

        return 1;
}

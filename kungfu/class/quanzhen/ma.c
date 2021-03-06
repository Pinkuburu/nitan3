// Copyright (C) 2003, by Lonely. All rights reserved.
// This software can not be used, copied, or modified 
// in any form without the written permission from authors.

#include <ansi.h>
#include "quanzhen.h"

inherit NPC;
inherit F_MASTER;
inherit F_COAGENT;
inherit F_QUESTER;

string ask_me();
string ask_miji();
mixed ask_skill1();

void create()
{
        set_name("马钰", ({"ma yu", "ma"}));
        set("gender", "男性");
        set("age", 42);
        set("class", "taoist");
        set("nickname",MAG"丹阳子"NOR);
        set("long",
                "他就是王重阳的大弟子，全真七子之首，丹阳子马钰马真人。\n"
                "他慈眉善目，和蔼可亲，正笑着看着你。\n");
        set("attitude", "peaceful");
        set("shen_type",1);
        set("str", 28);
        set("int", 32);
        set("con", 31);
        set("dex", 30);

        set("title","全真七子之首");

        set("chat_chance_combat", 60);
        set("chat_msg_combat", ({
                (: exert_function, "recover" :),
                (: exert_function, "recover" :),
                (: perform_action, "strike.ju" :),
                (: perform_action, "strike.ju" :),
                (: perform_action, "strike.ju" :),
                (: perform_action, "sword.ding" :),
                (: perform_action, "sword.ding" :),
                (: perform_action, "sword.ding" :),
        }));

        set("qi", 4500);
        set("max_qi", 4500);
        set("jing", 2200);
        set("max_jing", 2200);
        set("neili", 4700);
        set("max_neili", 4700);
        set("jiali", 100);
        set("level", 18);
        set("combat_exp", 1400000);

        set_skill("force", 180);
        set_skill("quanzhen-xinfa", 180);
        set_skill("sword", 170);
        set_skill("quanzhen-jian",170);
        set_skill("dodge", 180);
        set_skill("jinyan-gong", 180);
        set_skill("parry", 180);
        set_skill("literate",100);
        set_skill("strike", 180);
        set_skill("chongyang-shenzhang", 180);
        set_skill("haotian-zhang", 180);
        set_skill("finger",180);
        set_skill("zhongnan-zhi", 180);
        set_skill("taoism",150);
	set_skill("medical", 210);
	set_skill("liandan-shu", 210);
	set_skill("cuff",200);
	set_skill("whip",200);
	set_skill("chunyang-quan",200);
	set_skill("duanyun-bian",200);
        set_skill("array",150);
	set_skill("beidou-zhenfa", 150);
	
        map_skill("force", "quanzhen-xinfa");
        map_skill("sword", "quanzhen-jian");
        map_skill("dodge", "jinyan-gong");
        map_skill("parry", "quanzhen-jian");
        map_skill("finger", "zhongnan-zhi");
        map_skill("strike", "haotian-zhang");
        prepare_skill("finger", "zhongnan-zhi");
        prepare_skill("strike","haotian-zhang");

        create_family("全真教", 2, "弟子");

        set("book_count",1);
        set("miji_count",1);
        set("inquiry", ([
                "全真教" :  "我全真教是天下道家玄门正宗。\n",
		"秘籍" : (: ask_miji :),                
                "金雁功" : (: ask_me :),
                "金雁图谱" : (: ask_me :),
                "鹰飞雁舞" : (: ask_skill1 :),                
        ]) );

        set("master_ob",3);
	setup();

        carry_object("/clone/weapon/changjian")->wield();
        carry_object("/d/quanzhen/npc/obj/grayrobe")->wear();

}

void attempt_apprentice(object ob)
{
        if (! permit_recruit(ob))
                return;

        if ((int)ob->query_skill("quanzhen-xinfa",1) < 100 )
        {
                command("say 你的本门内功心法火候不足,难以领略更高深的武功。");
                return;
        }
        if ((int)ob->query("shen") < 10000)
        {
                command("say 行侠仗义我辈学武人需铭记在心，这点你做的可不够啊！\n");
                return;
        }
        command("pat " + ob->query("id"));
        command("say 好吧，我就收下你这个徒弟了。");
        command("recruit " + ob->query("id"));
}

string ask_me()
{
        mapping fam;
        object ob;

        if (!(fam = this_player()->query("family")) || fam["family_name"] != "全真教")
                return RANK_D->query_respect(this_player()) +
                "与本教毫无瓜葛，我教的武功典籍可不能交给你。";
        if (query("book_count") < 1)
                return "你来晚了，金雁图谱已经被人取走了。";
        add("book_count", -1);
        ob = new("/clone/book/jinyantu");
        ob->move(this_player());
        return "好吧，这本「金雁图谱」你拿回去好好研读。";

}

string ask_miji()
{
	mapping fam;
	object ob, *obs,ob2 ;

	if (!(fam = this_player()->query("family")) || fam["family_name"] != "全真教")
		return RANK_D->query_respect(this_player()) +
		"与本派素无来往，不知此话从何谈起？";

	if (query("miji_count") < 1)
		return "你来晚了，本派的鞭法秘籍不在此处。";

	obs = filter_array(children("/d/zhongnan/obj/whip_book"), (: clonep :));
		  if (sizeof(obs) > 5)
		return "你来晚了，本派的鞭法秘籍不在此处。";

	ob=new("/d/quanzhen/obj/whip_book");
	ob->move(this_player());
	add("miji_count", -1);
	// command ("rumor "+this_player()->query("name")+"弄到了一册丹阳鞭法谱。");
	return "好吧，这本「丹阳鞭法谱」你拿回去好好钻研。";
}

mixed ask_skill1()
{
        object me;

        me = this_player();

        if (me->query("can_perform/jinyan-gong/yanwu"))
                return "这招我不是已经教会你了吗？";

        if (me->query("family/family_name") != query("family/family_name"))
                return "阁下与贫道素不相识，不知此话从何说起？";

        if (me->query_skill("jinyan-gong", 1) < 1)
                return "你连金雁功都没学，何谈绝招可言？";

        if (me->query("family/gongji") < 50)
                return "你在我全真教内甚无作为，这招我暂时还不能传你。";

        if (me->query("shen") < 3000)
                return "你的侠义正事还做得不够，这招我暂时还不能传你。";

        if (me->query_skill("jinyan-gong", 1) < 60)
                return "你的金雁功不够娴熟，练高点再来吧。";

        if (me->query_skill("force") < 80)
                return "你的内功修为不够，修炼高后再来找我吧。";

        message_sort(HIY "\n$n" HIY "对$N" HIY "点了点头，当下更不答"
                     "话，但见身影时如鹰击长空、时如群雁飞舞，令人眼"
                     "花撩乱，煞是好看。\n\n" NOR, me,
                     this_object()); 

        command("nod");
        command("say 这招很简单，稍加练习便成。");
        tell_object(me, HIC "你学会了「鹰飞雁舞」。\n" NOR);
        if (me->can_improve_skill("dodge"))
                me->improve_skill("dodge", 1500000);
        if (me->can_improve_skill("jinyan-gong"))
                me->improve_skill("jinyan-gong", 1500000);
        me->improve_skill("martial-cognize", 1500000);
        me->set("can_perform/jinyan-gong/yanwu", 1);
        me->add("family/gongji", -50);

        return 1;
}

// Copyright (C) 2003, by Lonely. All rights reserved.
// This software can not be used, copied, or modified 
// in any form without the written permission from authors.
// yin.c 殷天正

#include <ansi.h>
#include "mingjiao.h"

inherit NPC;
inherit F_MASTER;
inherit F_QUESTER;

string ask_me();
string ask_book();

void create()
{
        set_name("殷天正",({"yin tianzheng","yin","tianzheng"}));
	set("title",HIG "明教" HIW "白眉鹰王" NOR);
        set("long", "他就是明教的四大护法之一，一个身材魁梧的秃顶老者。\n"+
                    "他长眉胜雪，垂下眼角，鼻子钩曲，有若鹰嘴。\n");                    
        set("age", 61);
        set("attitude", "peaceful");
        set("shen", -13000);
        set("str", 35);
        set("per", 20);
        set("int", 25);
        set("con", 28);
        set("dex", 25);        
        set("max_qi", 3000);
        set("max_jing", 2700);
        set("eff_jingli", 2500);
        set("jiali", 70);
        set("combat_exp", 1300000);
        set("level", 30);
        set("jian_count", 1);
        
        set_skill("dodge", 250);
        set_skill("force", 250);
        set_skill("claw", 290); 
        set_skill("parry", 250);         
        set_skill("literate", 210);
        set_skill("qingfu-shenfa", 250);
        set_skill("shenghuo-shengong", 240);
        set_skill("yingzhua-shou", 290);
        set_skill("martial-cognize", 200);      
               
        map_skill("force", "shenghuo-shengong");
        map_skill("dodge", "piaoyi-shenfa");
        map_skill("claw", "yingzhua-shou");
        map_skill("parry", "yingzhua-shou");
        
        prepare_skill("claw","yingzhua-shou");

        create_family("明教", 36, "护法法王");
        
        set("inquiry", ([
                 "宝剑" : (: ask_me :),
                 "白虹剑" : (:ask_me :),
                 "鹰爪手法" : (: ask_book :),
        ]));
        
	set("chat_chance_combat", 20);
	set("chat_msg_combat", ({
		(: command("perform claw.xue") :),
	}) );
	        
        set("master_ob",4);
	setup();
        carry_object("/d/mingjiao/obj/yel-cloth")->wear();
}

void attempt_apprentice(object ob)
{
        mapping fam = ob->query("family");

        if (! permit_recruit(ob))
                return;

        if ((int)ob->query("shen") < 5000) {
		command("say " + RANK_D->query_respect(ob) +
			"有所不知，我平生最恨奸诈之人，你正气太低，我不能让你用明教武功害人。");
		return;
	}
	
        if (! fam || fam["family_name"] != "明教"){
                command("say "+RANK_D->query_respect(ob) + "与本派素无来往，不知此话从何谈起？");
                return;
        }
        if ((int)ob->query_skill("shenghuo-shengong", 1) < 100) {
                command("say 要学更为高深的武功，必须有高深的内功修为。"); 
                command("say " + RANK_D->query_respect(ob)+"是否还应该在圣火神功上多下点功夫？");
                return;
        }     
        if ((int)ob->query_str() < 28) {
                command("say 想学我的鹰爪手法，必须要有足够的力量。");
                command("say 在臂力方面，"+RANK_D->query_respect(ob)+"是否还嫌差了点？");
                return;
        }   
        command("say 嗯，看你还是个学武的料，我就收下你吧！");
        command("say 多谢明尊敝佑，让我明教又得一良材，为康扶正义，把我明教发扬光大吧。");
        command("recruit " + ob->query("id"));
        ob->set("title", this_object()->query("title")+"属下教众");
}

string ask_me()
{
	object me = this_player();
        object ob;

        if (count_lt(me->query("combat_exp"), 100000))
                return RANK_D->query_respect(me)+"看来功力不够，恐怕不能保住这把宝剑。";
                
        if (me->query_temp("askbhj"))
                return "你刚才不是问过了么？";

        if (this_object()->query("jian_count") < 1) 
                return "你来晚了，我的白虹宝剑已给别人拿走了。";  
        		
        ob = new("/d/mingjiao/obj/baihong-jian");
        ob->move(me);
        this_object()->add("jian_count", -1);
	me->set_temp("askbhj", 1);
        return "好吧，这把白虹宝剑你就拿去吧，望你不要借它为恶，常为行侠仗义。";
}

string ask_book()
{
        mapping fam; 
        object ob;
        
        if (! (fam = this_player()->query("family")) || fam["family_name"] != "明教")
                return RANK_D->query_respect(this_player())+"与本派素无来往，不知此话从何谈起？"; 
                
        if (this_player()->query_skill("claw", 1) > 30)
                return RANK_D->query_respect(this_player())+"看来已经不需要这本书了，还是留给其他明教弟子用吧！";
                
        ob = new("/d/mingjiao/obj/hand_book");
        ob->move(this_player());
        
        return "好吧，这本「鹰爪手法」你拿回去好好钻研。";
}

int accept_ask(object me, string topic)
{
        switch (topic)
        {

        case "赤血爪" :
                return MASTER_D->teach_pfm(me, this_object(),
                         ([ "perform" : "can_perform/sougu/xue",
                           "name"    : "赤血爪",
                           "sk1"     : "sougu",
                           "lv1"     : 140,
                           "neili"   : 2000,
                           "gongxian": 800,
                           "shen"    : 100000, ]));
                break;

        default:
                return 0;
        }
}

void reset()
{
        set("jian_count", 1);
}

// Copyright (C) 2003, by Lonely. All rights reserved.
// This software can not be used, copied, or modified 
// in any form without the written permission from authors.
// wu.c ���߷�

inherit NPC;

string ask_me_2(string name);
void create()
{
	set_name("���߷�", ({ "wu mianfeng", "wu" }));
	set("long","
���ǻ�ҩʦ����������߷磬��������С����Ĭ���ԣ��ڽ���
���������죬��ȴ���û�ҩʦ���Σ��˶������ƹ��һ���ҩ��
�صء������һ��������Ⱦ�������\n");
	set("gender", "����");
	set("age", 40);
	set("attitude", "friendly");
	set("shen_type", -1);
	set("str", 25);
	set("int", 30);
	set("con", 25);
	set("dex", 25);

	set("max_qi", 1500);
	set("max_jing", 1000);
	set("neili", 1500);
	set("max_neili", 1500);
	set("jiali", 100);
	set("level", 10);
	set("combat_exp", 400000);

	set_skill("force", 120);
	set_skill("bibo-shengong", 120);
	set_skill("dodge", 120);
	set_skill("luoying-shenfa", 120);
	set_skill("strike", 120);
	set_skill("luoying-shenzhang", 120);
	set_skill("unarmed", 120);
	set_skill("luoying-shenjian", 120);
	set_skill("qimen-wuxing", 120);
	set_skill("xuanfeng-tui", 120);
	set_skill("hand", 120);
	set_skill("lanhua-shou", 120);
	set_skill("sword",120);
	set_skill("yuxiao-jianfa", 120);
	set_skill("literate", 80);

	map_skill("force", "bibo-shengong");
	map_skill("dodge", "luoying-shenfa");
	map_skill("strike", "luoying-shenzhang");
	map_skill("parry", "yuxiao-jianfa");
	map_skill("hand", "lanhua-shou");
	map_skill("unarmed", "xuanfeng-tui");
	map_skill("sword","yuxiao-jianfa");

	prepare_skill("strike", "luoying-shenjian");

	create_family("�һ���", 2, "����");
	setup();
        set("inquiry", ([
            	"�Ż���¶��" : (: ask_me_2, "yulu-wan" :)
        ]));

	set("drug_count", 3);
	set("yulu_count", 1);

        carry_object("/clone/cloth/cloth")->wear();
	carry_object("/d/taohua/obj/tiexiao")->wield();
}

string ask_me_2(string name)
{
	mapping fam; 
	object ob;
	
	if (!(fam = this_player()->query("family")) || fam["family_name"] != "�һ���")
		return RANK_D->query_respect(this_player()) + 
		"�뱾��������������֪�˻��Ӻ�̸��";

	if ( this_player()->query("family/master_name") != "��ҩʦ" )
		return "�㹦�򲻹������ǲ�ҪΪ�";		

	if (query("yulu_count") < 1)
		return "��Ǹ�������ò���ʱ���Ѿ������ˡ�";

	if (this_player()->query_condition("work") > 0)
                return "�Ҳ��ǲŸ�����ҩ����������Ҫ�ˣ�����̰�����ᣡ";

	ob = new("/d/taohua/obj/"+name);
        ob->move(this_player(), 1);
	this_player()->apply_condition("work", 45);

	add("yulu_count", -1);

	message_vision("���߷��$Nһ��" + ob->query("name") + "��\n", this_player());

	return "��ȥ�ɡ�С�ı�Ū���ˡ�";
}

int accept_kill(object me)
{
	if (me->query("family/family_name") == "�һ���"){
        command("say ���ұ���ͬ�ţ����Ҫ�Ӻ����ң�\n");
	me->set("th_killer", 1);
        kill_ob(me);
        return 1;
	}
	return 0;
}
void unconcious()
{
	object ob, me=this_object();

	if ( objectp(ob = me->query_temp("last_damage_from"))
	&& ob->query("family/family_name") == "�һ���")	
	ob->set("th_killer", 1);

	::unconcious();
}
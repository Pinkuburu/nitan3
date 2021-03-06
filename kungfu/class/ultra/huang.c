// huang 黄裳

#include <ansi.h>

inherit NPC;
inherit F_MASTER;
inherit F_NOCLONE;

#define ULTRA_OPINION   "宗师"
#define OPINION_TYPE    HIY "拳脚功夫" NOR 
#define MY_OPINION      "unarmed"

#define QUEST_HS_D(x)   ("/quest/quesths/quest/quesths" + x)
#define QUEST_WUGOOD_D  ("/quest/questsj/quest_sj")
#define HSMSG_D         ("/quest/quesths/quest/huangshang-msg")
#define ANTI_ROBOT      "/kungfu/class/ultra/robot"

mixed ask_opinion();
mixed ask_skill();
int give_quest();
int give_up();
void destory(object ob);
string time_period(int timep);

string  *my_opinions = ({ "unarmed", "finger",
                          "strike", "cuff", "hand", "claw" });

string* hs_msg = ({
        "黄裳头也不抬地说：“",
        "黄裳翻出一叠档案道：“",
        "黄裳低头想了想，说道：“",
        "黄裳拿着一份公文，说道：“",
        "黄裳微微笑了笑道：“",
});

// 该头文件必须在这里嵌入
#include <ultra.h>

void create()
{
        set_name("黄裳", ({ "huang shang", "huang" }) );
        set("title", HIY "大宗师" NOR);
        set("gender", "男性");
        set("age", 92);
        set("long", "此人气度闲雅，看不出武功高低。\n");
        set("attitude", "peaceful");
        set("str", 32);
        set("int", 39);
        set("con", 37);
        set("dex", 32);

        set("max_qi", 20000);
        set("max_jing", 20000);
        set("neili", 30000);
        set("max_neili", 30000);
        set("jiali", 100);

        set("combat_exp", 50000000);

        set("inquiry", ([
                "九阴真经" : "想不到当年我随手写的一本武学心得竟成为危害江湖的不祥之物。",
                "评价"     : (: ask_opinion :),
                "opinion"  : (: ask_opinion :),
                "武学修养" : "武学修养(martial-cognize)不到家，还谈什么武功呢？",                
                "摄心大法" : (: ask_skill :),
                "任务"     : "天性好，又是负神的话，可以在我这里领任务(quest)。\n",
        ]));

        set_skill("unarmed", 500);
        set_skill("finger", 500);
        set_skill("claw", 500);
        set_skill("strike", 500);
        set_skill("hand", 500);
        set_skill("cuff", 500);
        set_skill("parry", 500);
        set_skill("dodge", 500);
        set_skill("force", 500);
        set_skill("jiuyin-shengong", 500);
        set_skill("martial-cognize", 500);
        set_skill("literate", 200);

        map_skill("parry", "jiuyin-shengong");
        map_skill("force", "jiuyin-shengong");
        map_skill("dodge", "jiuyin-shengong");
        map_skill("unarmed", "jiuyin-shengong");
        map_skill("finger", "jiuyin-shengong");
        map_skill("strike", "jiuyin-shengong");
        map_skill("hand", "jiuyin-shengong");
        map_skill("cuff", "jiuyin-shengong");
        map_skill("claw", "jiuyin-shengong");

        prepare_skill("claw", "jiuyin-shengong");
        prepare_skill("cuff", "jiuyin-shengong");

        set("opinion/ultra",  1);
        set("opinion/master", 1);

        set("rank_info/self", "某家");

        setup();
        carry_object("/clone/cloth/cloth")->wear();

        set("startroom", "/d/dali/lushui");
        check_clone();
}

void init()
{
        object me;

        if (! interactive(me = this_player()))
                return;

        me->delete_temp("want_opinion");
        if (ultrap(me))
        {
                message_vision(CYN "$N" CYN "微微一笑，对$n"
                               CYN "点了点头。\n" NOR,
                               this_object(), me);
        }
        if( !is_fighting() &&random(5)==0)
        {
        	remove_call_out("greeting");
        	call_out("greeting", 1, me);
        }        
        add_action("give_quest", "quest");
        add_action("give_up","giveup");
}

void greeting(object ob)
{
        if( !ob || environment(ob) != environment() ) return;
        if(ob->query("quest_hs") && ob->query("shen") < 0)
                write("黄裳说道：" + ob->query("name")+ ",你的任务完成得怎么样了？\n");
}

mixed ask_opinion()
{
        object me;
        object ob;

        me = this_player();

        if (count_lt(me->query("combat_exp"), 200000))
                return "呵呵，你这点武功还谈什么评价？";

        if (me->query("opinion/" MY_OPINION) == ULTRA_OPINION)
                return "哈哈，你拳脚方面的修为已经称得上是宗师了！咱们要不要过过招？";

        if (is_fighting(me))
                return "哈哈，废什么话，打赢了我，你自己给自己评价吧！";

        if (me->is_fighting())
                return "你等会儿吧！待我赶走他们。";

        if (objectp(ob = query_temp("opinion")))
        {
                if (ob == me)
                {
                        command("say 你着什么急？不要烦我。");
                        return 1;
                }

                message_vision(CYN "$N" CYN "正在沉思，没有回答$n"
                               CYN "，只是摆了摆手。\n" NOR,
                               this_object(), me);
                return 1;
        }

        switch(me->query_temp("want_opinion/huang"))
        {
        case 0:
                command("look " + me->query("id"));
                command("say 好吧，你空手攻我一招，我看看你的水平如何。");
                break;
        case 1:
                command("say 不是说让你来一招我看看吗？");
                break;
        case 2:
                message_vision(CYN "$N" CYN "皱眉对$n" CYN
                               "道：“我看你这人是够罗嗦的。”\n" NOR,
                               this_object(), me);
                break;
        case 3:
                command("say 给我闭上你的嘴！");
                return 1;
        }

        me->add_temp("want_opinion/huang", 1);
        return 1;
}

int calc_level(object me, string skill)
{
        int lvl;
        string sp;
        mapping action;

        lvl = me->query_skill(skill, 1) / 2;
        sp = me->query_skill_mapped(skill);
        if (! sp) return lvl;

        lvl += me->query_skill(sp, 1);
        action = SKILL_D(sp)->query_action(me, 0);
        if (mapp(action))
                lvl += (action["force"] + action["attack"] +
                        action["parry"] + action["dodge"] - 150) / 15;
        return lvl;
}

void do_congration(object me)
{
        command("chat 恭喜这位" + RANK_D->query_respect(me) + "，不过武"
                "道永无止境，还望继续努力啊！");
}

int accept_hit(object me)
{
        if (me->query_temp("want_hit/huang") >= 2)
        {
                command("heng");
                command("say 事不过三，你要找死那就去死吧！");
                kill_ob(me);
                return 1;
        }

        command("shake");
        command("say 这位" + RANK_D->query_respect(me) + "，"
                "练武之人切忌贸然挑衅。");
        me->add_temp("want_hit/huang", 1);
        return 0;
}

int accept_fight(object me)
{
        object weapon;

        if (me->query("opinion/" MY_OPINION) == ULTRA_OPINION)
        {
                message_vision(CYN "$N" CYN "大喜，对$n" CYN
                               "道：“很好，近年我还没有和宗"
                               "师高手过过招，来吧！”\n" NOR,
                               this_object(), me);
                return 1;
        }

        if (! me->query_temp("want_opinion/huang"))
        {
                command("say 平白无故动什么手？");
                return 0;
        }

        if (objectp(weapon = me->query_temp("weapon")))
        {
                command("say 把你手中的" + weapon->name() + NOR +
                        CYN "给我放下！" NOR);
                return 0;
        }

        if (me->query("qi") * 100 / me->query("max_qi") < 80||
            me->query("jing") * 100 / me->query("max_jing") < 80)
        {
                command("say 你现在状态不佳，等休息好了再说吧！");
                return 0;
        }

        command("nod");
        COMBAT_D->do_attack(me, this_object(), 0);
        COMBAT_D->do_attack(this_object(), me, 0);

        if (me->query("qi") <= 0)
        {
                message_vision(CYN "\n$N" CYN "摇摇头对$n" CYN
                               "道：“你现在的武功还差得远，连"
                               "我一招都接不住！”\n" NOR,
                               this_object(), me);
                return notify_fail("看来要好好的修炼功夫了。\n");
        }

        call_out("check_opinion", 2, me);
        set_temp("opinion", me);
        me->delete_temp("want_opinion");
        message_vision(CYN "\n$N" CYN "点点头道：“好了，你先等"
                       "会儿，待我想想。”\n" NOR,
                       this_object(), me);
        return notify_fail("看来不能心急，还得等一会儿。\n");
}

int accept_kill(object me)
{
        if (count_lt(me->query("combat_exp"), 500000000))
        {
                message_vision(CYN "$N" CYN "大怒道：“无名小辈捣"
                               "什么乱？给我滚！”说罢一掌把$n" CYN
                               "劈倒在地。\n" NOR, this_object(), me);
                me->unconcious();
                return -1;
        }
        command("sigh");
        command("say 很好！近年还真没有遇到什么敢和我较量的高手！");
        return 1;
}

int recognize_apprentice(object me, string skill)
{
        if (me->query("character") != "光明磊落" &&
            me->query("character") != "国士无双")
        {
                command("say 大丈夫处世，不论善恶行事一定要光"
                        "明磊落，你生性不是这种人。");
                return -1;
        }

        if (skill == "martial-cognize")
                return 1;

        if (! ultrap(me))
        {
                command("say 想学我的九阴神功？你还不够资格，"
                        "等你成了大宗师再说吧！");
                return -1;
        }

        if (me->query_skill(skill, 1) >= 180)
        {
                command("say 够了！我就教你到这里吧，武功还是"
                        "要靠自己多研究才是！");
                return -1;
        }

        if (skill != "jiuyin-shengong")
                return 1;

        if (me->query_temp("can_learn/huang/jiuyin-shengong"))
                return 1;

        me->set_temp("can_learn/huang/jiuyin-shengong", 1);
        command("nod");
        command("say 很好，我就传你九阴神功吧，可不要忘记做人"
                "的根本！");
        return 1;
}

void unconcious()
{
        /*
        message("vision", "\n"HIR + name() + "脸色一变，吐了一口鲜"
                "血，身形一闪，消逝不见。\n\n" NOR, environment());
        command("chat 罢！罢！罢！老夫去也！");
        destruct(this_object());
        */
        recover();
        return;
}

mixed ask_skill()
{
        object me;
        
        me = this_player();

        if (me->query("can_perform/jiuyin-shengong/xin"))
        {
             command("say 你不是已经会了吗？");
             return 1;
        }

        if (! me->query("can_learn/jiuyin-shengong/xin"))
        {               
               message_sort(HIM "\n黄裳叹道：“当年我随手写了一本武学心得，名曰「九阴真经」，本欲"
                            "留与后人研究武学之用，不料竟成为危害江湖的不祥之物。如果阁下能将其上、"
                            "下两册收集齐交还与我，我便传你绝招「摄心大法」。\n", me);
               command("tan");

               return "阁下意下如何？";
        }

        
        if (me->query_skill("jiuyin-shengong", 1) < 240)
        {
               command("say 你九阴神功火候还不够，我还不能传你这招！");
               return 1;
        }
        if (me->query_skill("martial-cognize", 1) < 240)
        {
               command("say 你武学修养不足，还是下去多磨练一下吧！");
               return 1;
        }
        if (  ! me->query("can_perform/jiuyin-shengong/shou")
           || ! me->query("can_perform/jiuyin-shengong/zhi")
           || ! me->query("can_perform/jiuyin-shengong/zhua")
           || ! me->query("can_perform/jiuyin-shengong/zhang") )
        {
               command("shake");
               command("say 等你将九阴神功其他绝招领悟后再来找我吧！");
               return 1; 
        }

        command("nod");
        command("say 很好，很好！");
        command("say 既然这样我就传你摄心大法！");
        message_sort(HIC "\n黄裳将$N" HIC "呼自身旁，对着$N" HIC "耳边小声嘀咕了几句。"
                       "黄裳不时地做出各种奇怪的表情，$N" HIC "边听边看，良久，$N" HIC
                       "一声长啸，胸中疑虑顿然消失，再无困扰 ……\n", me);
        command("haha");
        
        if (me->can_improve_skill("force"))
                me->improve_skill("force", 1500000);
        if (me->can_improve_skill("jiuyin-shengong"))
                me->improve_skill("jiuyin-shengong", 1500000);
        if (me->can_improve_skill("martial-cognize"))
                me->improve_skill("martial-cognize", 1500000);

        tell_object(me, HIG "你学会了「摄心大法」！\n" NOR);
        me->set("can_perform/jiuyin-shengong/xin", 1);
        return 1;
}
void die()
{
        /*
        message("vision", "\n"HIR + name() + "大叫一声，狂吐了一口鲜"
                "血，身形一闪，就此不见。\n\n" NOR, environment());
        command("chat 老夫去也！");
        destruct(this_object());
        */
        recover();
        return;
}
/*
int accept_object(object me, object ob) 
{
        if (base_name(ob) != "/clone/lonely/book/zhenjing1"
          && base_name(ob) != "/clone/lonely/book/zhenjing2")
        {
               command("say 你给我这种东西干什么？");
               return 0;
        } 
      
        if (base_name(ob) == "/clone/lonely/book/zhenjing1")
        {
               command("nod");
               command("say 很好，很好！");
               if (me->query("give_zhenjing2"))
               {
                     command("haha");
                     command("say 既然这样我就传你绝招，你可以随时来问我！");
                     tell_object(me, HIG "黄裳决定传你「摄心大法」。\n" NOR); 
                     me->set("can_learn/jiuyin-shengong/xin", 1);
                     me->delete("give_zhenjing1");
                     me->delete("give_zhenjing2");
                     destruct(ob);
                     return 1;
               }
               me->set("give_zhenjing1", 1);
               command("say 还有一本「九阴真经下册」，你尽快找到交还与我！");
               destruct(ob);
               return 1;
        } 

        if (base_name(ob) == "/clone/lonely/book/zhenjing2")
        {
               command("nod");
               command("say 很好，很好！");
               if (me->query("give_zhenjing1"))
               {
                     command("haha");
                     command("say 既然这样我就传你绝招，你可以随时来问我！");
                     tell_object(me, HIG "黄裳决定传你「摄心大法」。\n" NOR); 
                     me->set("can_learn/jiuyin-shengong/xin", 1);
                     me->delete("give_zhenjing1");
                     me->delete("give_zhenjing2");
                     destruct(ob);
                     return 1;
               }
               me->set("give_zhenjing2", 1);
               command("say 还有一本「九阴真经上册」，你尽快找到交还与我！");
               destruct(ob);
               return 1;
        } 
 
}
*/

int give_quest()
{
   // mapping quest,qgood,skills,robot_quest;
   mapping quest,qgood,robot_quest;
   // object me,target,thing,npc;
   object me,thing,npc;
   // int i, giveup,tag,want,lev,lev1;
   int i, giveup,tag,want;
    mixed exp;
    int all_lvl,lvl;
    //string msg,*sname,place;
    string msg,place;

    me = this_player();
    exp = me->query("combat_exp");

    if( (me->query("character") != "光明磊落") &&
            (me->query("character") != "国士无双") &&
           (me->query("character") != "狡黠多变"))
    {
        write("黄裳问你：你这种性格的人，来这里干什么？\n");
        return 1;
    }
    if( !(me->query("shen") < 0))
    {
        write("黄裳看着你只是摇头：“行走江湖，不是光想着做大侠，我讲求的是以杀止杀，你杀性不够，还是算了吧！”\n");
        return 1;
    }
    if (count_lt(exp, 500000))
    {
        write("黄裳对你说道：以你现在的经验，还需要多多磨练啊。\n");
        return 1;
    }
 
    if ( quest = me->query("quest_hs") )
    {
        write("黄裳对你说：先把你手上的任务完成再说。\n");
        return 1;
    }
    if (mapp(me->query("quest_dg")))
    {
        write("黄裳对你说：独孤求败的任务你还没完成，莫非想找老夫讨打？\n");
        return 1;
    }

    giveup = me->query_temp("hs_giveup");
    if((time()-me->query("quesths_giveup_time")) < 20 + giveup*2)
    {
        write("黄裳对你哼了一声道：“老完不成任务，我对你的表现很不满意，先反省一会儿。\n");
        return 1;
    }

// 以下给任务
                if (count_le(exp, 600000))         tag = 0;
                else if (count_le(exp, 900000))    tag = 1;
                else if (count_le(exp, 1200000))   tag = 2;
                else if (count_le(exp, 1800000))   tag = 3;
                else if (count_le(exp, 2500000))   tag = 4-random(3);
                else if (count_le(exp, 3200000))   tag = 5-random(3);
                else if (count_le(exp, 4000000))   tag = 6-random(3);
                else
    {
                        if (random(15) == 0 || (count_gt(exp, 10000000) && random(10) == 0))
            tag=8;
        else tag=7-random(3);
    }

    i = me->query("quesths_times");
    if((i > 20 && random(20)==18) || wizardp(me))
    {
        qgood = QUEST_WUGOOD_D->query_good();
        msg = qgood["msg"+random(4)];
        want = random(qgood["max_num"] - qgood["min_num"] + 1) + qgood["min_num"];
	msg = YEL"黄裳微微笑着，赞许地说道：“办事能力不错！\n"+
	      msg+"\n"+
              "你想法弄一批"+qgood["name"]+"来，大约要 "+want+" "+qgood["unit"]+"，有多少可以先交多少。去罢！”\n"NOR;
        write(msg);
        quest = ([ 
                   "id"         : qgood["id"],
                   "name"       : qgood["name"], 
                   "msg"        : msg, 
                   "want"       : want,
                   "type"       : "收", 
                   "check_file" : qgood["check_file"], 
                   "level"      : tag,
                   "bonus"      : 0,
                   "all_bonus"  : 0,
                ]),
        me->set("quest_hs",quest);
        return 1;
    }

    if(tag==8)
    {
            npc = new(CLASS_D("generate") + "/player_npc.c");
                        NPC_D->place_npc(npc, count_lt(me->query("combat_exp"), 500000)  ? ({ "大理一带", "终南山", "关外", "西域" }) :
                                              count_lt(me->query("combat_exp"), 800000) ? ({ "大理一带", "终南山", "西域" }) : ({"大理一带", "西域"}));
            npc->set_from_me(me,1);
            npc->add_temp("apply/attack", npc->query_skill("force") / 3);
            npc->add_temp("apply/dodge", npc->query_skill("force") / 3);
            npc->add_temp("apply/parry", npc->query_skill("force") / 3);
            npc->add_temp("apply/damage", 200 + random(100));
            npc->add_temp("apply/unarmed_damage", 200 + random(100));
            npc->add_temp("apply/armor", 200 + random(300));
            npc->set_temp("quester", me->query("id"));
            npc->set("auto_perform",1);
            NPC_D->random_move(npc);
            NPC_D->random_move(npc);
            NPC_D->random_move(npc);
            place = npc->query("place");

            npc->set("shen", atoi(count_div(npc->query("combat_exp"), 2000)));
            msg="黄裳忧心忡忡的说：“"+npc->name(1)+
                "这个假仁假义的无耻之徒多次危害武林，听说他最近在" + 
                place + "，你去除掉他吧！”\n";

            quest = ([
                "name" : npc->name(1),
                "id"   : npc->query("id"),
                "bonus": (200+random(51)),
                "type" :"杀",
                "level":8,
                "chinese_w":place,
                "where":npc->query("startroom"),
                "msg":msg,
		"object":base_name(npc),
            ]);
            tell_object(me,quest["msg"]+"\n");
            me->set("quest_hs",quest);

            return 1;
    }
    else
    {
            if (count_gt(exp, 4000000))
            {
                all_lvl = 0;
                lvl = random(28) + 1;
                for (i = 1;i < 8;i ++)
                {
                    all_lvl += i;
                    if (lvl > all_lvl - i && lvl <= all_lvl)
                    {
                        lvl = i;
                        break;
                    }
                }
            } else lvl = tag;
            while(1)
            {
                quest = QUEST_HS_D(lvl)->query_quesths();
                                if (quest["type"] == "杀" || quest["type"] == "擒")
                                {
                                        if (! get_object(quest["object"])->is_master())
                                             break;
                                } else
                    break;
            }

/*
    if(undefinedp(quest["msg"]) || quest["msg"] == "")
*/
    switch(quest["type"])
    {
        case "杀":
                msg=HSMSG_D->query_hsmsg("杀",quest["name"]);
                break;
        case "寻":
                msg=HSMSG_D->query_hsmsg("寻",quest["name"]);
                break;
        case "征":
                npc = new(CLASS_D("generate") + "/hs_target.c");
                        NPC_D->place_npc(npc, count_lt(me->query("combat_exp"), 500000)  ? ({ "大理一带", "终南山", "关外", "西域" }) :
                                              count_lt(me->query("combat_exp"), 800000) ? ({ "大理一带", "终南山", "西域" }) : ({"大理一带", "西域"}));
                NPC_D->set_from_me(npc, me, 100);
                npc->set_temp("quester",me->query("id"));
                NPC_D->random_move(npc);
                NPC_D->random_move(npc);
                NPC_D->random_move(npc);
                place = npc->query("place");
                msg = hs_msg[random(5)]+npc->name(1)+"去年从武林同盟这里借走不少钱，地方分舵催讨多次不得，"
                        "听说他现在正在"+place+"，你去试试！\n如果也收不到钱，就收了他的"+quest["name"]+"。拿武林令要出示(chushi)一下。";
                thing = new("/quest/quesths/wulin-ling");
                thing->move(me);
                message_vision("黄裳将武林令交给$N。\n",me);
                quest["target"] = npc->query("id");
                break;
        case "送":
                npc = new(CLASS_D("generate") + "/hs_receiver.c");
                        NPC_D->place_npc(npc, count_lt(me->query("combat_exp"), 500000)  ? ({ "大理一带", "终南山", "关外", "西域" }) :
                                              count_lt(me->query("combat_exp"), 800000) ? ({ "大理一带", "终南山", "西域" }) : ({"大理一带", "西域"}));
                NPC_D->set_from_me(npc, me, 100);
                npc->set_temp("quester", me->query("id"));
                NPC_D->random_move(npc);
                NPC_D->random_move(npc);
                NPC_D->random_move(npc);
                place = npc->query("place");
                thing = new(quest["object"]);
                thing->set("send_to",npc->query("id"));
                thing->set("send_from",query("id"));
                thing->set("send_from_name",query("name"));
                msg = hs_msg[random(5)]+"你帮我将"+thing->name(1)+"送至"+npc->name(1)+"处，"
                                        "他现在正在"+place+"，误了时间他可能就离开了。";
                if (!thing->move(me))
                {
                    thing->move(environment());
                    message_vision("黄裳将$N扔在地上。\n",thing);
                } else message_vision("黄裳将$N交给$n。\n",thing,me);
                quest["thing"] = thing;
                quest["send_to"] = thing->query("send_to");
                break;
    }
    }
    quest["msg"] = msg;
    quest["level"] = tag;
    if (quest["type"] == "杀" && random(6) == 3)
    {
        robot_quest = ([]);
        robot_quest = ANTI_ROBOT->get_question();
        msg = "";
        msg += "黄裳郑重说道：“我刚将武林同盟的口令更改了一下，你去通知一下" + quest["name"] + NOR"，新口令是：\n";
        me->set_temp("test",robot_quest);
        msg += robot_quest["question"];
        msg += "你只需要将口令用"HIR"普通的数字形式"NOR"传送过去就行了，传送的指令是：whisper。\n";
        quest["type"]   = "传";
        quest["msg"]  = msg;
        quest["answer"] = robot_quest["answer"];
    }

    me->set("quest_hs", quest);
    write(msg + "\n");
    return 1;
}

int accept_object(object me, object ob)
{
   // int rank,exp,pot,score,times,i,tag,all_quest_hs,bonus;
   int exp,pot,score,times,i,tag,all_quest_hs,bonus;
    mapping quest;
    object gold,who,obj;
    string msg;
   // int mar,weiwang,shen,quest_count;
   int mar,weiwang,quest_count;
    mixed special = 0;
    mixed money = 0;
    
    who = this_object();
    
    if( (me->query("character") != "光明磊落") && (me->query("character") != "狡黠多变") && !(me->query("shen") <0))  
    {
        write("黄裳奇道：“你是何人？”\n");
        return 0;
    }
    else if(!(quest = me->query("quest_hs")))
    {
        write("黄裳说道：“无功不受禄，东西你收回。”\n");
        return 0;
    }

    if (ob->query("money_id"))
    {
                if (ob->value() < 500000)
           {
                   tell_object(me,"黄裳满脸不悦的对你说：这么一点点钱，能用来干什么？你自己留着买鸡腿用吧！\n");
                   return notify_fail("黄裳抬头望天，缓缓说道：如果给个100两黄金，或许我就消除了你的任务。\n");
           }
           me->delete("quest_hs");
           tell_object(me,"黄裳接过你的" + ob->name(1) + "，微微笑了笑，点头道：那你这个任务不做就算了吧。\n");
           destruct(ob);
           return 1;
    }

    if (quest["type"]!="寻"&&quest["type"]!="征"&&quest["type"]!="收"&&quest["type"]!="送")
    {
        write("黄裳说道：“我没有叫你去找东西，东西你收回。”\n");
        return 0;
    }
    
    if (userp(ob) || 
        ob->is_item_make() ||
        ob->query_save_file())
    {
        write("黄裳大怒道：“好你个小子，竟然想蒙骗本盟主！来人！把他给我抓起来！”\n");
        me->get_into_prison(this_object(), 0, 30);
        log_file( "static/QUEST_DIJIAO", sprintf("%s(%s)(黄裳任务)制造%s(%s)(%s)作弊\n", me->query("name"),
             me->query("id"),ob->name(),ob->query("id"),ctime(time()) ) );
        return 1;
    }

    if ( quest["type"] == "征" )
    {
        if ( (string)ob->query_temp("zheng") != me->query("id") )
        {
            write("黄裳接过来，一看怒道：“这哪是你拣来的，还想蒙骗本盟主！走，你的江湖威望降低了！”\n");
            me->add("weiwang",-weiwang*(random(5)+4)/4);
            if (me->query("weiwang") < 0) me->set("weiwang",0);
            return 0;
        }
    } else if ( quest["type"] == "送" )
    {
        if (!ob->is_receipt() || ob->query("reply_to") != who->query("id"))
        {
            write("黄裳接过来一看，皱眉道：“给我这玩艺干什么？”\n");
            return 0;
        }
        if ( ob->query("reply_by") != quest["send_to"] )
        {
            write("黄裳接过来一看，勃然大怒道：“这张回执是谁给你的？别想来糊弄我！”\n");
            return 0;
        }
        if ( ob->query("receive_from") != me->query("id") )
        {
            write("黄裳接过来一看，勃然大怒道：“这张回执是你自己得到的么？别想来糊弄我！”\n");
            return 0;
        }
    } else if ( quest["type"] == "收" )
    {
        switch( quest["id"] )
        {
            case "HEAD_OBJ":
            case "ARMOR_OBJ":
            case "CLOTH_OBJ":
                if ( !(bonus = quest["check_file"]->check_value(base_name(ob))) )
                {
                    write("黄裳接过来一看，皱眉道：“给我这玩艺干什么？”\n");
                    return 0;
                }
                
                if ( !mapp(ob->query("armor_prop")) ||
                     !stringp(ob->query("armor_type")) )
                {
                    write("黄裳接过来一看，皱眉道：“给我这玩艺干什么？”\n");
                    return 0;
                }
                quest["bonus"] = bonus;
                break;
            case "SWORD_OBJ":
            case "BLADE_OBJ":
            case "HAMMER_OBJ":
            case "STAFF_OBJ":
                if ( !(bonus = quest["check_file"]->check_value(base_name(ob))) )
                {
                    write("黄裳接过来一看，皱眉道：“给我这玩艺干什么？”\n");
                    return 0;
                }
                
                if ( !mapp(ob->query("weapon_prop")) )
                {
                    write("黄裳接过来一看，皱眉道：“给我这玩艺干什么？”\n");
                    return 0;
                }
                quest["bonus"] = bonus;
                break;
            default :
                if ( !(bonus = quest["check_file"]->check_value(base_name(ob))) )
                {
                    write("黄裳接过来一看，皱眉道：“给我这玩艺干什么？”\n");
                    return 0;
                }
                quest["bonus"] = bonus;
                break;
        }
    } else if ( ob->name(1) != quest["name"] )
    {
        write("黄裳说道：“这不是我想要的。”\n");
        return 0;
    }

//    message_vision("$N将$n交给黄裳。\n",me,ob);与give信息重复haiyan
    times = me->query("quesths_times"); //连续次数
    tag = me->query("quest_hs/level")+1;       //任务等级
    if((i=me->query("quesths_times")) < 15)   i = 15 - i;
    else i = 1;
    exp = tag*240 + quest["bonus"]*9;
    if (random(2))
    exp += random(times);
    exp = exp/i;
    
    if (tag > 4 && i > 5)
	exp /= 2;

    if (quest["type"]=="收")
    {
	exp = quest["bonus"]*(tag + 1);
	exp += random(exp/2);
    }

    pot = exp/(2+random(2));
    score = random(exp/20);
    weiwang = random(exp/15);

    if (times + 1 > 200)
	  mar = exp/3 + random(times);
    else mar = 100 + random(100);

    if (mar > 3000) mar = 3000;

    if(quest["type"]=="收")
    {
        if((int)quest["want"]>(quest["ok"]+1))
        {
            me->add("quest_hs/ok",1);
            write("黄裳接过来点点头说：“第 "+quest["ok"]+" 了，很好，还有 "+(quest["want"]-quest["ok"])+" 个，继续努力！”\n");
            exp /= 2;
            pot /= 2;
            mar /= 2;
            weiwang /= 2;
            score /= 3;
            QUEST_D->bonus(me, ([ "exp" : exp, "pot" : pot, "mar" : mar, 
                                  "weiwang" : weiwang, "score" : score]), 1);
            me->add("quest_hs/all_bonus", exp);
            destruct(ob);
            return 1;
        }
        exp += quest["all_bonus"]/2;
        pot = exp/(2+random(2));
        weiwang = random(exp/15);
        mar = mar*2;
        mar += mar/4 * quest["want"];
        score = score;
    }
    write("黄裳接过来说道：“很好！你这次任务完成得不错！”\n");
    destruct(ob);

    if (quest["type"]=="征" && objectp(obj = present("wulin ling", me)))
	destruct(obj);

    if(!random(10))
    {
        i=(exp/60+1);
        if(i>30) i=30;
        gold=new("/clone/money/gold");
        gold->set_amount(i);
        tell_object(me,"然后取出一锭黄金给你：“这次奖你些金子，下次继续努力！”\n"
                       HIW"\n你被奖励了 "+i+" 两黄金。\n");
        if(!gold->move(me))
        {
            message_vision("但是$N身上东西太重，黄金一下子从手中滑到了地上。\n",me);
            gold->move(environment());
        }
        QUEST_D->bonus(me, ([ "exp" : exp, "pot" : pot, "mar" : mar,
                            "weiwang" : weiwang, "score" : score, "percent" : 50 ]), 1);
    }
    else QUEST_D->bonus(me, ([ "exp" : exp, "pot" : pot, "mar" : mar,
                              "weiwang" : weiwang, "score" : score ]), 1);
    
    if ( times < 1 )
        me->set("quesths_times",1);
    else
        me->add("quesths_times",1);
        me->set_temp("hs_giveup",0);        

   me->add("total_hatred",-5);
   if (me->query("total_hatred") < 0) me->set("total_hatred",0);

   quest_count = me->query("quesths_times")%500;
   if (quest_count == 50)
   {
       msg = "$N呵呵笑了两声道：“你还可以，连着"+chinese_number(me->query("quesths_times"))+
       "次任务都完成的漂漂亮亮，继续努力！”\n";
	special = 1;
	   } else
       if (quest_count == 100)
   { 
       msg = "$N笑的合不拢嘴，看了你一眼道：“有志气！"+chinese_number(me->query("quesths_times"))+
       "次任务干得干净利索！”\n";
       special = "/d/shaolin/obj/puti-zi";
   } else
    if (quest_count == 150)
    {
        msg = "$N叹道：“真是长江后浪推前浪，想不到你接连"+chinese_number(me->query("quesths_times"))+
               "次任务都无一疏漏，不易，不易！”\n";
        money = 1;
    } else
    if (quest_count == 200)
    {
        msg = "$N叹道：“真是长江后浪推前浪，想不到你接连"+chinese_number(me->query("quesths_times"))+
               "次任务都无一疏漏，不易，不易！”\n";
        money = 1;
    } else
    if (quest_count == 250)
    {
        msg = "$N叹道：“真是长江后浪推前浪，想不到你接连"+chinese_number(me->query("quesths_times"))+
               "次任务都无一疏漏，不易，不易！”\n";
        money = 1;
    } else

    if (quest_count == 300)
    {
        msg = "$N叹道：“真是长江后浪推前浪，想不到你接连"+chinese_number(me->query("quesths_times"))+
               "次任务都无一疏漏，不易，不易！”\n";
	 special = "/clone/gift/tianxiang";
    } else
    if (quest_count == 350)
    {
        msg = "$N吓了一跳：“看来你真的很适合学武，竟接连"+chinese_number(me->query("quesths_times"))+
               "次任务都无一疏漏，不易，不易！”\n";
	 special = "/clone/gift/tianxiang";
    } else
    if (quest_count == 400)
    {
        msg = "$N叹道：“真是一个武学天才啊，想不到你接连"+chinese_number(me->query("quesths_times"))+
               "次任务都无一疏漏，不易，不易！”\n";
         special = "/clone/gift/jiuzhuan";
    } else
    if (quest_count == 450)
        {
        msg = "$N大声叫起来：“天才！真是天才，真有人连续"+chinese_number(me->query("quesths_times"))+
               "次任务都无一疏漏，不易，不易！”\n";
        special = "/clone/gift/jiuzhuan";
    } else
       if (quest_count == 0)
   {
       msg = "$N深深叹了口气，道：“没想到你连着"+chinese_number(me->query("quesths_times"))+
       "次任务无一失手，看来以后就是你们这些人挑大梁啦！”\n";
       special = "/clone/gift/xuanhuang";
   } else
       if ((quest_count % 10) == 0)
   {
       msg = "$N微微点头：“干的不错，居然连着" +
       chinese_number(me->query("quesths_times")) +
       "次都没有失手，接着来，别放松！”\n";
   }
   if (special) {
       // message_vision(msg, who, me);
       QUEST_D->special_bonus(who, me, special);
   }
    if (money) {
        // message_vision(msg, who, me);
        QUEST_D->money_bonus(who, me, money);
    }

    me->delete("quest_hs");

   //记录完成任务的次数（不要求连续），如果满1000次则奖励水晶残片
   all_quest_hs = (int)me->query("all_quest_hs");
   if (!all_quest_hs) all_quest_hs = 0;
   all_quest_hs += 1;
   if (all_quest_hs == 1000)
   {
       obj = new("/clone/enchase/GRNjewel.c");
       tell_object(me,this_object()->name() + "又对你点了点头：你已经从我这里接了一千个任务，我这里正好\n" +
                   "有个" + obj->query("name") + "，就奖给你了！\n");
       if (!obj->move(me))
               obj->move(environment());
       me->set("all_quest_hs",0);
   } else me->set("all_quest_hs",all_quest_hs);
   return 1;
}

int give_up()
{
    //object me,ob,obj;
    object me,ob;
    int giveup,rank;
    mapping quest;
    
    me = this_player();
    if( me->query("quest_hs")) 
    {
    if( (me->query("character") != "光明磊落") && (me->query("character") != "狡黠多变") && !(me->query("shen") <0))  
        return 1;
    }
    if((quest =  me->query("quest_hs")))
    {
        message_vision("$N战战兢兢地走到黄裳面前请罪：“小的该死，辜负了盟主的期望......”\n",me);
        if (quest["type"] == "传")
        {
                message_vision("$N皱着眉头对$n说：『不就是传一个口令么？有那么难？回去仔细想想，这个任务一定要完成！\n",this_object(),me);
                return 1;
        }
        if (quest["type"] == "征")
        {
               if (ob = present("wulin ling",me))
               {
                        ob->move("/d/register/prison");
                        destruct(ob);
               }
        }
        if (quest["type"] == "送")
        {
                if (objectp(ob = quest["thing"]))
                {
                        ob->move("/d/register/prison");
                        destruct(ob);
                }
        }
        message_vision("黄裳对$N叹了一声道: 怎么不动动脑筋？这么简单的事情都做不了。\n", me);
        
        if (quest["type"] == "收")
        {
                me->delete("quest_hs");
                return 1;
        }
        rank = me->query("quest_hs/level");
        me->add("weiwang",-(rank*6+random(rank+4)));
        if ( me->query("weiwang")<0 )
               me->set("weiwang",0);
        write("黄裳告诉你：“你的江湖威望降低了！”\n");

        me->set("quesths_times",0);

        giveup = me->query_temp("hs_giveup");
        if ( giveup < 1 )
            me->set_temp("hs_giveup",1);
        else if ( giveup < 5 )
            me->set_temp("hs_giveup",giveup+1);
        else me->set_temp("hs_giveup",5);

        me->set("quesths_giveup_time",time());
   
        me->delete("quest_hs");
    }
    else
        write("黄裳告诉你：你目前没有任何任务。\n");
    return 1;
}

string time_period(int timep)
{
    int t, d, h, m, s;
    string time;
    t = timep;
    s = t % 60;             t /= 60;
    m = t % 60;             t /= 60;
    h = t % 24;             t /= 24;
    d = t;
 
    if(d) time = chinese_number(d) + "天";
    else time = "";
 
    if(h) time += chinese_number(h) + "小时";
    if(m) time += chinese_number(m) + "分";
        time += chinese_number(s) + "秒";
    return time;
}

void destory(object ob)
{
    if(!ob) return;
    if(ob->is_character())
        message_vision("$N突然一溜烟地跑得没影了。\n",ob);
    ob->move("/d/wizard/wizard_room");
    destruct(ob);
    return ;
}
